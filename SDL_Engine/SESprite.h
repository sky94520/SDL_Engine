#ifndef __Sprite_H__
#define __Sprite_H__
#include<string>

#include "SEProtocols.h"
#include "SENode.h"
#include "SERenderCommand.h"
NS_SDL_BEGIN

class Surface;
class SpriteFrame;

class Sprite : public Node, public TextureProtocol
{
protected:
	Texture* _texture;
	//src rect 表示该精灵绘画区域，一般不会改变，在大图片中显示的位置 TODO
	Rect _rect;
	//在TexturePacker中的包围盒
	Rect _spriteFrameRect;
	//配合 _rect ，为了以后的TexturePacker扩展
	bool _rectRotated;
	//偏移位置
	Point _offsetPosition;
	//图片原本大小
	Size _originalSize;
	//是否进行绘制
	bool _insideBounds;
	//纹理绘制命令
	TextureCommand _textureCommand;
public:
	Sprite();
	virtual ~Sprite();
	CREATE_FUNC(Sprite);
	//name加载图片，图片首先后保存在SpriteFrameCache中
	static Sprite* create(const std::string& filename);
	static Sprite* create(const std::string& fileName,const Rect&rect);
	//
	static Sprite* createWithTexture(Texture* texture);
	static Sprite* createWithTexture(Texture* texture,const Rect& rect);
	static Sprite* createWithTexture(Texture* texture,const Rect& rect,bool rotated,const Point& offset,const Size& originalSize);
	//用SpriteFrame创建
	static Sprite* createWithSpriteFrame(SpriteFrame* spriteFrame);
	static Sprite* createWithSpriteFrameName(const std::string& name);
	//使用Surface创建Sprite
	static Sprite* createWithSurface(Surface* surface);
	static Sprite* createWithTexture(Uint32 format,int access,int w,int h);

	bool init();

	bool initWithFile(const std::string& filename);
	bool initWithFile(const std::string& filename,const Rect& rect);

	bool initWithTexture(Texture* texture);
	bool initWithTexture(Texture* texture,const Rect& rect);
	bool initWithTexture(Texture* texture,const Rect& rect,bool rotated,const Point& offset,const Size& originalSize);
	
	bool initWithSpriteFrame(SpriteFrame* spriteFrame);
	bool initWithSpriteFrameName(const std::string& name);
	//
	bool initWithSurface(Surface* surface);
	bool initWithTexture(Uint32 format,int access,int w,int h);
	//获得Sprite的Texture
	virtual Texture* getTexture() const;
	virtual void setTexture(Texture* texture);
	void setTexture(const std::string& filename);
	//重写父类Node
	virtual void draw();
	//------------------------------SpriteFrame相关------------------------
	//获取该Sprite生成的SpriteFrame
	SpriteFrame* getSpriteFrame() const;
	void setSpriteFrame(const std::string& spriteFrameName);
	void setSpriteFrame(SpriteFrame* spriteFrame);
	//测试函数，主要为了精准碰撞 谨慎使用
	Rect getSpriteFrameRect()const;
	//-------------------------------设置rect--------------------------------
	Rect getTextureRect()const;
	void setTextureRect(const Rect&rect);
	void setTextureRect(const Rect&rect,bool rotated,Size untrimmedSize);

	Sprite*clone()const;
};
NS_SDL_END
#endif
