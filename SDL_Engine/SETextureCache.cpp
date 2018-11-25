#include "SETextureCache.h"
#include "SETexture.h"
#include "SEDirector.h"
#include "SEFileUtils.h"
#include "SEScheduler.h"
#include "SESurface.h"

NS_SDL_BEGIN

struct TextureCache::AsyncStruct
{
public:
    std::string filename;
	std::string fullpath;
    std::function<void(Texture*)> callback;
    //Texture*texture;
	Surface* surface;
    bool loadSuccess;
public:
	AsyncStruct(const std::string& fn,const std::function<void(Texture*)>& callback)
		:filename(fn)
		,callback(callback)
		,surface(nullptr)
		,loadSuccess(false)
	{}
	virtual ~AsyncStruct()
	{
		SDL_SAFE_RELEASE_NULL(surface);
	}
};
//------------------------------------TextureCache--------------------------------------------------
TextureCache::TextureCache()
	:_loadingThread(nullptr)
	,_needQuit(false)
	,_asyncRefCount(0)
{
}

TextureCache::~TextureCache()
{
	this->waitForQuit();

	SDL_SAFE_DELETE(_loadingThread);

	this->removeAllTextures();
}

Texture* TextureCache::addImage(const std::string& filename)
{
	return TextureCache::addImage(filename,filename);
}

Texture* TextureCache::addImage(const std::string& filename,const std::string& key)
{
	//查找是否存在对应资源
	auto iter = _textures.find(key);

	if(iter != _textures.end())
		return iter->second;
	//未找到，加载图片
	auto renderer = Director::getInstance()->getRenderer();
	//找到对应路径的文件
	auto filepath = FileUtils::getInstance()->fullPathForFilename(filename);
	//如果文件未找到，直接返回nullptr
	if(filepath.empty())
		return nullptr;

	auto texture = Texture::create(renderer, filepath.c_str());
	//加载成功
	if(texture != nullptr)
	{
		//保持对texture的引用
		texture->retain();

		_textures.insert(std::make_pair(key,texture));
		return texture;
	}
	LOG("%s\n",IMG_GetError());

	return nullptr;
}

void TextureCache::addImageAsync(const std::string& filename,const std::function<void(Texture*)>& callback)
{
	Texture* texture = nullptr;
	//如果对应的纹理已经加载成功，则直接回调函数
	std::string fullpath = FileUtils::getInstance()->fullPathForFilename(filename);

	auto it = _textures.find(fullpath);
	//已经加载成功，尝试回调函数
	if(it != _textures.end())
	{
		if(callback)
			callback(texture);
		return ;
	}
	//文件名为空或者文件不存在
	if(fullpath.empty())
		return;
	//加载线程滞后赋值
	if(_loadingThread == nullptr)
	{
		_loadingThread = new std::thread(&TextureCache::loadImage,this);
		_needQuit = false;
	}
	//开启定时器
	if(0 == _asyncRefCount)
	{
		Director::getInstance()->getScheduler()->schedule(schedule_selector(TextureCache::addImageAsyncCallback),this,0.f,-1,0.f,false);
	}
	++_asyncRefCount;
	//添加异步结构体
	AsyncStruct* asyncStruct = new AsyncStruct(filename,callback);
	asyncStruct->fullpath = fullpath;

	_asyncStructQueue.push_back(asyncStruct);

	_requestMutex.lock();
	_requestQueue.push_back(asyncStruct);
	_requestMutex.unlock();
	//唤醒等待进程
	_sleepCondition.notify_one();
}

Texture* TextureCache::addTexture(Texture* texture,const std::string& key)
{
	if(_textures.find(key) != _textures.end() || texture == nullptr)
		return nullptr;
	//添加到全局map中
	texture->retain();
	_textures.insert(std::make_pair(key,texture));

	return texture;
}

Texture* TextureCache::getTextureForKey(const std::string& key) const
{
	auto iter = _textures.find(key);

	if(iter != _textures.cend())
		return iter->second;

	return nullptr;
}

std::string TextureCache::getTextureFilePath(Texture* texture) const
{
	for(auto iter:_textures)
	{
		if(texture == iter.second)
			return iter.first;
	}
	return "";
}

void TextureCache::removeUnusedTextures()
{
	//遍历，找到引用为1的Texture，若有，则删除
	for(auto iter = _textures.begin();iter != _textures.end();)
	{
		auto texture = iter->second;
		//仅有一个引用的纹理为未使用
		if(texture->getReferenceCount() == 1)
		{
			texture->release();

			iter = _textures.erase(iter);
		}
		else
			++iter;
	}
}

void TextureCache::removeTextureForKey(const std::string& key)
{
	auto iter = _textures.find(key);

	if(iter != _textures.end())
	{
		auto texture = iter->second;
		SDL_SAFE_DELETE(texture);

		_textures.erase(iter);
	}
}

void TextureCache::removeAllTextures()
{
	for(auto iter = _textures.begin();iter != _textures.end();)
	{
		auto texture = iter->second;
		SDL_SAFE_DELETE(texture);
		
		iter = _textures.erase(iter);
	}
}

void TextureCache::waitForQuit()
{
    // notify sub thread to quick
    _needQuit = true;
    _sleepCondition.notify_one();
    if (_loadingThread)
	{
		_loadingThread->join();
	}
}

void TextureCache::loadImage()
{
	AsyncStruct* asyncStruct = nullptr;
	std::mutex signalMutex;
	std::unique_lock<std::mutex> signal(signalMutex);

	while(!_needQuit)
	{
		_requestMutex.lock();

		if(!_requestQueue.empty())
		{
			asyncStruct = _requestQueue.front();
			_requestQueue.pop_front();
		}
		else
			asyncStruct = nullptr;

		_requestMutex.unlock();
		//如果当前的请求队列为空，就阻塞此线程
		if(asyncStruct == nullptr)
		{
			_sleepCondition.wait(signal);
			continue;
		}
		//线程安全地加载图片资源
		Surface* surface = new Surface();
		surface->init(asyncStruct->fullpath.c_str());

		asyncStruct->surface = surface;
		asyncStruct->loadSuccess = surface ? true : false;

		_responseMutex.lock();
		_responseQueue.push_back(asyncStruct);
		_responseMutex.unlock();
	}
}

void TextureCache::addImageAsyncCallback(float dt)
{
	Texture* texture = nullptr;
	AsyncStruct* asyncStruct = nullptr;

	while(true)
	{
		_responseMutex.lock();

		if(!_responseQueue.empty())
		{
			asyncStruct = _responseQueue.front();
			_responseQueue.pop_front();
			SDL_assert(asyncStruct == _asyncStructQueue.front());
			_asyncStructQueue.pop_front();
		}
		else
			asyncStruct = nullptr;

		_responseMutex.unlock();

		if (nullptr == asyncStruct)
		{
			break;
		}
		//异步加载完成，则进行一些后续操作
		auto it = _textures.find(asyncStruct->filename);
		if(it != _textures.end())
		{
			texture = it->second;
		}
		else if(asyncStruct->loadSuccess)
		{
			Renderer* renderer = Director::getInstance()->getRenderer();

			Texture* texture = new Texture();
			texture->initWithSurface(renderer, asyncStruct->surface);

			_textures.insert(std::make_pair(asyncStruct->filename,texture));
		}
		//函数回调
		if(asyncStruct->callback)
			asyncStruct->callback(texture);
		//进行内存的回收
		delete asyncStruct;

		--_asyncRefCount;

		if(0 == _asyncRefCount)
			Director::getInstance()->getScheduler()->unschedule(this,schedule_selector(TextureCache::addImageAsyncCallback));
	}
}

NS_SDL_END
