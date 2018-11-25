#include "SESpriteFrameCache.h"
#include "SESpriteFrame.h"
#include "SEDirector.h"
#include "SETexture.h"
#include "SESurface.h"
#include "SEStringUtils.h"
#include "SETextureCache.h"
#include "SEFileUtils.h"
#include "SERenderer.h"

NS_SDL_BEGIN
SpriteFrameCache::SpriteFrameCache()
{
}

SpriteFrameCache::~SpriteFrameCache()
{
	for(auto it = _spriteFrames.begin();it!=_spriteFrames.end();)
	{
		auto spriteFrame = it->second;
		++it;
		SDL_SAFE_DELETE(spriteFrame);
	}
	_spriteFrames.clear();
}

bool SpriteFrameCache::addSpriteFramesWithFile(const std::string&xml,const std::string&textureFile)
{
	bool bRet = false;

	std::string texturePath;
	auto fullPath = FileUtils::getInstance()->fullPathForFilename(xml);
	if(fullPath.empty())
		return false;
	std::unique_ptr<char> points = std::move(FileUtils::getInstance()->getUniqueDataFromFile(fullPath));
	auto text = points.get();
	//读取xml文件
	rapidxml::xml_document<> doc;
	//先获取资源文件名称
	doc.parse<0>(text);
	rapidxml::xml_node<>*root = doc.first_node();
	//textureFile为空，尝试解析xml
	if(textureFile.empty())
	{
		texturePath = root->first_attribute("imagePath")->value();
		//尝试读取texture
		texturePath = FileUtils::getInstance()->fullPathForFilename(texturePath);
		//读取失败，尝试更换xml扩展名
		if(texturePath.empty())
		{
			texturePath = xml;
			size_t startPos =  texturePath.find_last_of('.');
			texturePath.erase(startPos);
			texturePath.append(".png");
		}
	}
	else
		texturePath = textureFile;

	auto texture = Director::getInstance()->getTextureCache()->addImage(texturePath);
	if(texture == nullptr)
		bRet = false;
	else
	{
		bRet = this->addSpriteFramesWithFile(root,texture);
	}
	
	points.reset();

	return bRet;
}

bool SpriteFrameCache::addSpriteFramesWithFile(const std::string&xml,Texture*texture)
{
	bool bRet = false;

	auto fullPath = FileUtils::getInstance()->fullPathForFilename(xml);
	if(fullPath.empty())
		return false;
	std::unique_ptr<char> points = std::move(FileUtils::getInstance()->getUniqueDataFromFile(fullPath));
	auto text = points.get();
	//读取xml文件
	rapidxml::xml_document<> doc;
	//先获取资源文件名称
	doc.parse<0>(text);
	rapidxml::xml_node<>*root = doc.first_node();

	bRet = this->addSpriteFramesWithFile(root,texture);
	
	points.reset();

	return bRet;
}

bool SpriteFrameCache::addSpriteFramesWithFile(rapidxml::xml_node<>*root,Texture*texture)
{
	std::string texturePath;
	
	texturePath = Director::getInstance()->getTextureCache()->getTextureFilePath(texture);
	//解析
	for(auto node = root->first_node();node;node = node->next_sibling())
	{
		std::string name=node->first_attribute("n")->value();
		int x = atoi(node->first_attribute("x")->value());
		int y = atoi(node->first_attribute("y")->value());
		int w = atoi(node->first_attribute("w")->value());
		int h = atoi(node->first_attribute("h")->value());
		//这里根据xml特性所判断的
		bool rotated = false;
		auto r = node->first_attribute("r");
		//该属性为nullptr，未找到该属性
		if(r)
		{
			rotated = true;
		}
		//以下项可能不存在
		int oX = node->first_attribute("oX")==nullptr?0:atoi(node->first_attribute("oX")->value());
		int oY = node->first_attribute("oY")==nullptr?0:atoi(node->first_attribute("oY")->value());
		//oW oH的值
		int oW = 0,oH = 0;
		if(node->first_attribute("oW"))
			oW = atoi(node->first_attribute("oW")->value());
		else if(r)
			oW = h;
		else
			oW = w;
		if(node->first_attribute("oH"))
			oH = atoi(node->first_attribute("oH")->value());
		else if(r)
			oH = w;
		else
			oH = h;
		//锚点
		Point anchorPoint(0.5f, 0.5f);
		if (node->first_attribute("pX") != nullptr)
			anchorPoint.x = atof(node->first_attribute("pX")->value());
		if (node->first_attribute("pY") != nullptr)
			anchorPoint.y = atof(node->first_attribute("pY")->value());
		//如果在map中没有对应的name，则加载
		if(_spriteFrames.find(name) == _spriteFrames.end())
		{
			auto realTexture = this->createTexture(texture,Rect((float)x,(float)y,(float)w,(float)h),Point((float)oX,(float)oY),Size((float)oW,(float)oH),rotated);
			//把该texture传递给TextureCache
			Director::getInstance()->getTextureCache()->addTexture(realTexture,name);
			//新建SpriteFrame 这里rect进行修改
			SpriteFrame*spriteFrame = SpriteFrame::createWithTexture(
				realTexture,
				Rect(0.f,0.f,(float)oW,(float)oH),
				rotated,
				Point((float)oX,(float)oY),
				Size((float)oW,(float)oH),
				anchorPoint);

			spriteFrame->setTextureFilename(texturePath);
			spriteFrame->setTexturePackerRect(Rect(oX,oY,w,h));
			//添加
			_spriteFrames.insert(std::make_pair(name,spriteFrame));
			//保持引用
			spriteFrame->retain();
		}
		else
		{
#ifdef SDL_ENGINE_DEBUG
			printf("该资源已经被加载:%s",name.c_str());
#endif
		}
	}
	return true;
}
SpriteFrame*SpriteFrameCache::getSpriteFrameByName(const std::string&name)const
{
	auto it = _spriteFrames.find(name);
	if(it != _spriteFrames.end())
		return it->second;
	return nullptr;
}
void SpriteFrameCache::removeSpriteFrameByName(const std::string&name)
{
	auto iter = _spriteFrames.find(name);
	if(iter == _spriteFrames.end())
		return;
	auto spriteFrame = iter->second;
	_spriteFrames.erase(iter);
	spriteFrame->release();
}
void SpriteFrameCache::removeSpriteFramesByFileName(const std::string&filePath)
{
	for(auto it = _spriteFrames.begin();it != _spriteFrames.end();++it)
	{
		auto spriteFrame = it->second;
		if(spriteFrame->getTextureFilename() == filePath)
		{
			it = _spriteFrames.erase(it);
			SDL_SAFE_RELEASE(spriteFrame);
			return;
		}
	}
}

Texture*SpriteFrameCache::createTexture(Texture*texture,const Rect&srcRect,const Point&offset,const Size&originalSize,bool rotated)
{
	Texture*target = texture;

	auto renderer = Director::getInstance()->getRenderer();
	Color4B oldColor = renderer->getDrawColor();

	Texture* realTexture = Texture::create(renderer, SDL_PIXELFORMAT_RGBA4444,SDL_TEXTUREACCESS_TARGET,(int)originalSize.width,(int)originalSize.height);
	realTexture->setBlendMode(SDL_BLENDMODE_BLEND);

	//设置渲染目标
	renderer->setTarget(realTexture);
	renderer->setDrawColor(Color4B(0,0,0,0));
	renderer->clear();
	//准备渲染
	Rect destRect(offset.x,offset.y,srcRect.size.width,srcRect.size.height);
	float rotation = 0.f;
	SDL_Point center = {int(srcRect.size.width/2),int(srcRect.size.height/2)};
	//暂不更新
	if(rotated)
	{
		center.x = center.y = (int)srcRect.size.width/2;
		
		rotation = -90.f;

		destRect.origin.x = offset.x;
		destRect.origin.y = offset.y;

		int n = int(srcRect.size.width);

		if(n%2 == 0)
			destRect.origin.y -= 1;
	}
	SDL_Rect src = srcRect.getSDL_Rect();
	SDL_Rect dest = destRect.getSDL_Rect();

	renderer->copyEx(target,&src,&dest,rotation,&center,SDL_FLIP_NONE);

	//还原
	renderer->setTarget(nullptr);
	renderer->setDrawColor(oldColor);

	return realTexture;
}
NS_SDL_END
