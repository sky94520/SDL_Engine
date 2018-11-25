#ifndef __SDL_AnimationCache_H__
#define __SDL_AnimationCache_H__

#include<string>
#include<vector>
#include<unordered_map>
#include "SEObject.h"
#include "SEValue.h"

NS_SDL_BEGIN
class Animation;

class AnimationCache:public Object
{
private:
	static AnimationCache*m_pInstance;
	std::unordered_map<std::string,Animation*> _animations;
private:
	AnimationCache();
	~AnimationCache();
public:
	static AnimationCache*getInstance();
	static void purge();
	bool init();
	/**根据名字添加一个animation
	*@param animation 将要添加的动画
	*@param name 添加的动画的对应的键
	*/
	void addAnimation(Animation*animation,const std::string&name);
	/**从cache中删除一个Animation*/
	void removeAnimation(const std::string&name);
	/**根据name获取以前已经加载过的动画
	*@return 不存在放回 nullptr
	*/
	Animation*getAnimation(const std::string&name);
	/**根据外部文件添加动画
	@param plist plist的相对路径
	一定得确保frames已经由SpriteFrameCache加载
	*/
	void addAnimationsWithFile(const std::string&plist);
	/**根据键值对添加Animation
	@param valueMap 要添加的键值对
	@param plist 
	*/
	void addAnimationsWithValueMap(const ValueMap&valueMap,const std::string&plist);
private:
	/*详情请见 http://www.tuicool.com/articles/BNzYrq*/
	void parseVersion1(const ValueMap&animations);
	void parseVersion2(const ValueMap&animations);
};
NS_SDL_END
#endif
