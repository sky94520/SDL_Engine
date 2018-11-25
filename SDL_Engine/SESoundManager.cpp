#include "SESoundManager.h"
#include "SEMusic.h"
#include "SEChunk.h"
#include "SEFileUtils.h"
#include "SEDirector.h"
#include "SEScheduler.h"

NS_SDL_BEGIN

SoundManager*SoundManager::m_pInstance = nullptr;

struct SoundManager::AsyncStruct
{
	std::string filename;
	std::string fullpath;
	std::string type;
	std::function<void (Object*data,const std::string&type)> callback;
	Object*sound;
	bool loadSuccess;
public:
	AsyncStruct(const std::string&fn,const std::function<void(Object*data,const std::string&type)>& callback)
		:filename(fn),callback(callback),sound(nullptr),loadSuccess(false)
	{}
	~AsyncStruct()
	{
		SDL_SAFE_RELEASE_NULL(sound);
	}
};

SoundManager::SoundManager()
	:_effectVolume(128)
	,_loadingThread(nullptr)
	,_needQuit(false)
	,_asyncRefCount(0)
	,_playingMusic(true)
	,_playingChunk(true)
	,_playingMusicLoops(0)
{
	int musicType = MUS_WAV | MUS_MP3 | MUS_OGG;
	Mix_Init(musicType);
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,4096);
}
SoundManager::~SoundManager()
{
	this->waitForQuit();
	SDL_SAFE_DELETE(_loadingThread);

	for(auto iter = _musics.begin();iter != _musics.end();)
	{
		auto music = iter->second;

		iter = _musics.erase(iter);
		music->release();
	}
	for(auto iter = _effects.begin();iter != _effects.end();)
	{
		auto effect = iter->second;

		iter = _effects.erase(iter);
		effect->release();
	}
	Mix_CloseAudio();
	Mix_Quit();
}
SoundManager*SoundManager::getInstance()
{
	if(m_pInstance == nullptr)
		m_pInstance = new SoundManager();

	return m_pInstance;
}
void SoundManager::purge()
{
	SDL_SAFE_DELETE(m_pInstance);
}
Music*SoundManager::preloadBackgroundMusic(const std::string&file)
{
	//已经加载过，不加载
	auto iter = _musics.find(file);
	if(iter != _musics.end())
		return iter->second;
	auto filepath = FileUtils::getInstance()->fullPathForFilename(file);
	if(filepath.empty())
	{
		Mix_SetError("the file:%s not found!",file.c_str());
		return nullptr;
	}
	Music*music = Music::create(filepath);
	if(music)
	{
		music->retain();
		_musics.insert(std::make_pair(file,music));
		return music;
	}
	return nullptr;
}
int SoundManager::playBackgroundMusic(const std::string&file,int loops)
{
	_playingMusicName = file;
	_playingMusicLoops = loops;

	if (!_playingMusic)
	{
		return 0;
	}
	auto music = preloadBackgroundMusic(file);
	
	return music == nullptr ? -1 : music->playMusic(loops);
}

int SoundManager::stopBackgroundMusic()const
{
	return Music::haltMusic();
}
int SoundManager::pauseBackgroundMusic()const
{
	return Music::pausedMusic();
}
void SoundManager::resumeBackgroundMusic()
{
	Music::resumeMusic();
}
void SoundManager::rewindBackgroundMusic()
{
	Music::rewindMusic();
}
int SoundManager::isBackgroundPlaying()const
{
	return Music::playingMusic();
}
int SoundManager::getBackgroundMusicVolume()const
{
	return Music::volumeMusic(-1);
}
int SoundManager::setBackgroundMusicVolume(int volume)const
{
	return Music::volumeMusic(volume);
}
//------------------------------------音效------------------------------------------
Chunk* SoundManager::preloadEffect(const std::string&file)
{
	auto iter = _effects.find(file);
	if(iter != _effects.end())
		return iter->second;
	auto filepath = FileUtils::getInstance()->fullPathForFilename(file);
	//没找到资源
	if(filepath.empty())
	{
		Mix_SetError("the file:%s not found!",file.c_str());
		return nullptr;
	}
	//加载资源
	auto effect = Chunk::create(filepath);
	if(effect)
	{
		effect->retain();
		_effects.insert(std::make_pair(file,effect));
		return effect;
	}
	return nullptr;
}
void SoundManager::unloadEffect(const std::string&file)
{
	auto iter = _effects.find(file);
	if(iter != _effects.end())
	{
		auto effect = iter->second;
		_effects.erase(iter);
		effect->release();
	}
}
int SoundManager::playEffect(const std::string&file,int loops)
{
	if (!_playingChunk)
		return 0;

	auto effect = preloadEffect(file);

	return effect == nullptr ? -1 : effect->playChannel(-1,loops);
}
void SoundManager::pauseEffect(int channel)
{
	Chunk::pause(channel);
}
void SoundManager::pauseAllEffect()
{
	Chunk::pause(-1);
}
void SoundManager::resumeEffect(int channel)
{
	Chunk::resume(channel);
}
void SoundManager::resumeAllEffect()
{
	Chunk::resume(-1);
}
int SoundManager::stopEffect(int channel)
{
	return Chunk::haltChannel(channel);
}
int SoundManager::stopAllEffect()
{
	return Chunk::haltChannel(-1);
}
int SoundManager::isEffectPlaying(int channel)const
{
	return Chunk::playing(channel);
}
int SoundManager::getEffectVolume()const
{
	return _effectVolume;
}
void SoundManager::setEffectVolume(int volume)
{
	_effectVolume = volume;
	Chunk::volume(-1,volume);
}

void SoundManager::addSoundAsync(const std::string&filename,const std::string&type,std::function<void (Object*data,const std::string&)> callback)
{
	Object* sound = nullptr;
	//要加载的是音乐还是音效
	if (type == Music::TYPE)
	{
		auto iter = _musics.find(filename);
		
		if (iter != _musics.end())
			sound = iter->second;
	}
	else if (type == Chunk::TYPE)
	{
		auto iter = _effects.find(filename);

		if (iter != _effects.end())
			sound = iter->second;
	}
	//资源已经存在，直接回调函数
	if (sound != nullptr)
	{
		if (callback)
			callback(sound, type);
		return ;
	}
	//文件名为空，直接退出
	if (filename.empty())
		return ;

	auto fullpath = FileUtils::getInstance()->fullPathForFilename(filename);

	if (fullpath.empty())
		return;
	//加载线程滞后赋值
	if(_loadingThread == nullptr)
	{
		_loadingThread = new std::thread(&SoundManager::loadSound, this);
		_needQuit = false;
	}
	//开启定时器
	if (0 == _asyncRefCount)
	{
		Director::getInstance()->getScheduler()->schedule(schedule_selector(SoundManager::addSoundAsyncCallback),this,0.f,-1,0.f,false);
	}
	++_asyncRefCount;
	//添加异步结构体
	AsyncStruct* asyncStruct = new AsyncStruct(filename, callback);
	asyncStruct->fullpath = fullpath;
	asyncStruct->type = type;

	_asyncStructQueue.push_back(asyncStruct);
	_requestMutex.lock();
	_requestQueue.push_back(asyncStruct);
	_requestMutex.unlock();
	//唤醒工作进程
	_sleepCondition.notify_one();
}

void SoundManager::waitForQuit()
{
    // notify sub thread to quick
    _needQuit = true;
    _sleepCondition.notify_one();
    if (_loadingThread) _loadingThread->join();
}

bool SoundManager::isPlayingMusic() const
{
	return _playingMusic;
}

void SoundManager::setPlayingMusic(bool var)
{
	if (_playingMusic != var)
	{
		_playingMusic = var;
		//需要播放音乐
		if (_playingMusic && !_playingMusicName.empty())
		{
			this->playBackgroundMusic(_playingMusicName, _playingMusicLoops);
		}
		else
		{
			this->stopBackgroundMusic();
		}
	}
}

bool SoundManager::isPlayingChunk() const
{
	return _playingChunk;
}

void SoundManager::setPlayingChunk(bool var)
{
	if (_playingChunk != var)
	{
		_playingChunk = var;
	}
}

void SoundManager::loadSound()
{
	AsyncStruct*asyncStruct = nullptr;
	std::mutex signalMutex;
	std::unique_lock<std::mutex> signal(signalMutex);

	while (!_needQuit)
	{
		_requestMutex.lock();
		if (!_requestQueue.empty())
		{
			asyncStruct = _requestQueue.front();
			_requestQueue.pop_front();
		}
		else
		{
			asyncStruct = nullptr;
		}
		_requestMutex.unlock();
		//如果当前的请求队列为空，就阻塞此线程
		if (asyncStruct == nullptr)
		{
			_sleepCondition.wait(signal);
			continue;
		}
		//线程安全地加载资源
		if (asyncStruct->type == Music::TYPE)
		{
			Music* music = new Music();
			music->init(asyncStruct->fullpath);

			asyncStruct->sound = music;
		}
		else
		{
			Chunk* chunk = new Chunk();
			chunk->init(asyncStruct->fullpath);

			asyncStruct->sound = chunk;
		}
		asyncStruct->loadSuccess = asyncStruct->sound ? true : false;
		//加载完成
		_responseMutex.lock();
		_responseQueue.push_back(asyncStruct);
		_responseMutex.unlock();
	}
}

void SoundManager::addSoundAsyncCallback(float dt)
{
	Object*sound = nullptr;
	AsyncStruct*asyncStruct = nullptr;

	while (true)
	{
		_responseMutex.lock();

		if (_responseQueue.empty())
		{
			asyncStruct = nullptr;
		}
		else
		{
			asyncStruct = _responseQueue.front();

			SDL_assert(asyncStruct == _asyncStructQueue.front());
			_asyncStructQueue.pop_front();
			_responseQueue.pop_front();
		}
		_responseMutex.unlock();
		//当前没有加载完成的资源，直接退出这次循环
		if (asyncStruct == nullptr)
		{
			break;
		}
		//加载完成后的一些操作
		auto type = asyncStruct->type;
		auto filename = asyncStruct->filename;
		sound = asyncStruct->sound;

		if (type == Music::TYPE)
		{
			auto iter = _musics.find(filename);
		
			if (iter != _musics.end())
			{
				sound = iter->second;
			}
			else if(asyncStruct->loadSuccess)
			{
				_musics.insert(std::make_pair(filename,static_cast<Music*>(sound)));
				sound->retain();
			}
		}
		else if (type == Chunk::TYPE)
		{
			auto iter = _effects.find(filename);

			if (iter != _effects.end())
			{
				sound = iter->second;
			}
			else if (asyncStruct->loadSuccess)
			{
				_effects.insert(std::make_pair(filename,static_cast<Chunk*>(sound)));
				sound->retain();
			}
		}
		//尝试回调函数
		if(asyncStruct->callback)
			asyncStruct->callback(sound,type);
		//内存回收
		delete asyncStruct;

		--_asyncRefCount;

		if (0 == _asyncRefCount)
		{
			Director::getInstance()->getScheduler()->unschedule(this,schedule_selector(SoundManager::addSoundAsyncCallback));
		}
	}
}
NS_SDL_END
