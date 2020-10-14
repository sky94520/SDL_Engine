#include "SEAnimationCache.h"
#include "SEAnimation.h"
#include "SEFileUtils.h"
#include "SEDirector.h"
#include "SESpriteFrameCache.h"
#include "SEAnimationFrame.h"

#include "SETexture.h"
#include "SESurface.h"
#include "SESpriteFrame.h"
#include "extensions/SDL_anigif.h"

NS_SDL_BEGIN
AnimationCache*AnimationCache::m_pInstance = nullptr;

AnimationCache::AnimationCache()
{
}
AnimationCache::~AnimationCache()
{
	for(auto iter = _animations.begin();iter != _animations.end();)
	{
		auto animation = iter->second;
		animation->release();

		iter = _animations.erase(iter);
	}
}
AnimationCache*AnimationCache::getInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new AnimationCache();
		m_pInstance->init();
	}
	return m_pInstance;
}
void AnimationCache::purge()
{
	if(m_pInstance)
		SDL_SAFE_RELEASE_NULL(m_pInstance);
}
bool AnimationCache::init()
{
	return true;
}
void AnimationCache::addAnimation(Animation*animation,const std::string&name)
{
	_animations.insert(std::make_pair(name,animation));
	animation->retain();
}
void AnimationCache::removeAnimation(const std::string&name)
{
	auto iter = _animations.find(name);
	if(iter == _animations.end())
		return ;
	_animations.erase(iter);
	
	auto animation = iter->second;
	animation->release();
}
Animation*AnimationCache::getAnimation(const std::string&name)
{
	auto it = _animations.find(name);
	Animation* animation = nullptr;

	if (it != _animations.end())
	{
		animation = it->second;
	}
	return animation;
}
void AnimationCache::addAnimationsWithFile(const std::string&filename)
{
	if(filename.empty())
	{
		LOG("the filename is empty,AnimationCache::addAnimationsWithFile\n");
		return ;
	}
	auto valueMap = FileUtils::getInstance()->getValueMapFromFile(filename);

	this->addAnimationsWithValueMap(valueMap,filename);
}

void AnimationCache::addAnimationWithGIF(const std::string& gif, const std::string& name, bool restoreOriginalFrame, int loops)
{
	std::string fullname = FileUtils::getInstance()->fullPathForFilename(gif);
	if(fullname.empty())
	{
		LOG("the filename is empty,AnimationCache::addAnimationsWithGIF\n");
		return ;
	}
	//先获取gif的长度
	int length = AG_LoadGIF(fullname.c_str(), nullptr, 0);
	AG_Frame* ag_frames = new AG_Frame[length];
	AG_LoadGIF(fullname.c_str(), ag_frames, length);
	//构建帧
	std::vector<AnimationFrame*> frames(length);
	Renderer* renderer = Director::getInstance()->getRenderer();
	Size originalSize;
	for (int i = 0; i < length; i++)
	{
		//渲染
		Surface* surface = Surface::create(ag_frames[i].surface);
		Texture* texture = Texture::createWithSurface(renderer, surface);
		Size size(ag_frames[i].surface->w, ag_frames[i].surface->h);
		if (i == 0)
			originalSize = size;
		Point offset(ag_frames[i].x, ag_frames[i].y);
		texture = SpriteFrameCache::createTexture(texture, Rect(Point::ZERO, size), offset, originalSize, false);

		SpriteFrame* frame = SpriteFrame::createWithTexture(texture, Rect(Point::ZERO, originalSize));

		float delayUnits = ag_frames[i].delay / 1000.f;
		AnimationFrame* animationFrame = AnimationFrame::create(frame, delayUnits);
		frames[i] = animationFrame;
	}
	//创建动画
	Animation* animation = Animation::createWithAnimationFrames(frames, loops);
	animation->setRestoreOriginalFrame(restoreOriginalFrame);
	this->addAnimation(animation, name);
	delete[] ag_frames;
}

void AnimationCache::addAnimationsWithValueMap(const ValueMap&valueMap,const std::string&filename)
{
	//确保valueMap内有值
	if(valueMap.find("animations") == valueMap.end())
	{
		LOG("AnimationCache: No animations were found in provided dictionary\n");
		return;
	}
	const ValueMap&animations = valueMap.at("animations").asValueMap();
	//版本号 默认是1
	unsigned int version = 1;
	//获取版本号 并且尝试加载图片
	if(valueMap.find("properties") != valueMap.end())
	{
		const ValueMap&properties = valueMap.at("properties").asValueMap();
		version = properties.at("format").asInt();
		const ValueVector&spritesheets = properties.at("spritesheets").asValueVector();
		for(const auto&value:spritesheets)
		{
			std::string path = value.asString();
			Director::getInstance()->getSpriteFrameCache()->addSpriteFramesWithFile(path);
		}
	}
	switch(version)
	{
	case 1:this->parseVersion1(animations);break;
	case 2:this->parseVersion2(animations);break;
	}
}
void AnimationCache::parseVersion1(const ValueMap&animations)
{
	auto frameCache = Director::getInstance()->getSpriteFrameCache();
	for(auto iter = animations.cbegin();iter != animations.cend();++iter)
	{
		//是否已经加载过了，是则不重复加载
		auto it = _animations.find(iter->first);
		if (it != _animations.end())
			continue;
		//必有
		const ValueMap&animationDict = iter->second.asValueMap();
		const ValueVector&animationNames = animationDict.at("frames").asValueVector();
		float delay = animationDict.at("delay").asFloat();
		//可选
		unsigned int loops = 1;
		bool restoreOriginalFrame = false;

		if(animationDict.find("loops") != animationDict.end())
			loops = animationDict.at("loops").asInt();
		if(animationDict.find("restoreOriginalFrame") != animationDict.end())
			restoreOriginalFrame = animationDict.at("restoreOriginalFrame").asBool();

		Animation*animation = nullptr;
		//animationNames不能为空
		if(animationNames.empty())
		{
			LOG("AnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.\n",iter->first.c_str());
			continue;
		}
		//开始创建Animation
		std::vector<AnimationFrame*> frames;
		for(auto&frameName : animationNames)
		{
			SpriteFrame*spriteFrame = frameCache->getSpriteFrameByName(frameName.asString());
			AnimationFrame*animFrame = AnimationFrame::create(spriteFrame,delay);

			frames.push_back(animFrame);
		}
		animation = Animation::createWithAnimationFrames(frames,loops);
		animation->setRestoreOriginalFrame(restoreOriginalFrame);

		this->addAnimation(animation,iter->first);
	}
}
void AnimationCache::parseVersion2(const ValueMap&animations)
{
}
NS_SDL_END
