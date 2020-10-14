#ifndef __SDL_SpriteFrameCache_H__
#define __SDL_SpriteFrameCache_H__
#include<map>
#include<string>
#include<unordered_map>
#include<memory>
#include<cstdlib>
#include<vector>

#include "SEObject.h"
#include "SERect.h"
#include "SESize.h"
#include "SEPoint.h"

#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"
NS_SDL_BEGIN
class Texture;
class SpriteFrame;
/*保存着SpriteCache的引用*/
class SpriteFrameCache:public Object
{
private:
	//保存着SpriteFrame
	std::map<std::string,SpriteFrame*> _spriteFrames;
public:
	SpriteFrameCache();
	virtual ~SpriteFrameCache();
	//textureFile为空，从xml中读取文件，否则直接读取textureFile
	bool addSpriteFramesWithFile(const std::string&xml,const std::string&textureFile="");
	bool addSpriteFramesWithFile(const std::string&xml,Texture*texture);
	//获取SpriteFrame
	SpriteFrame*getSpriteFrameByName(const std::string&name)const;
	void removeSpriteFrameByName(const std::string&name);
	//释放某一文件中的全部资源
	void removeSpriteFramesByFileName(const std::string&filePath);

	/**创建一个texture的小图，并且该图片的blendmode必须为SDL_BLENDMODE_BLEND
	*brief texture 精灵图
	*brief srcRect 在精灵图中的包围盒
	*brief offset 原图的偏移位置
	*brief originalSize 原图大小
	*brief rotated 是否旋转
	*/
	static Texture* createTexture(Texture*texture,const Rect&srcRect,const Point&offset,const Size&originalSize,bool rotated);
private:
	bool addSpriteFramesWithFile(rapidxml::xml_node<>*root,Texture*texture);
};
NS_SDL_END
#endif
