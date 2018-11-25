#ifndef __SDL_SpriteFrame_H__
#define __SDL_SpriteFrame_H__
#include<string>

#include "SEObject.h"
#include "SEPoint.h"
#include "SESize.h"
#include "SERect.h"

NS_SDL_BEGIN
class Texture;

class SpriteFrame:public Object,public Clonable
{
protected:
	//左上角顶点位置
	Point _offsetPosition;
	//原来尺寸
	Size _originalSize;
	//是否旋转
	bool _rectRotated;
	//渲染专用TODO
	Rect _rect;
	//TexturePacker专用TODO
	Rect _texturePackerRect;
	//包含的tetxture
	Texture*_texture;
	//图片名称
	std::string _textureFilename;
	//锚点 不设置任何变量
	Point _anchorPoint;
public:
	SpriteFrame();
	virtual ~SpriteFrame();
	//以名字创建，图片会加入到TextureCache中
	static SpriteFrame*create(const std::string&filename,const Rect&rect);
	static SpriteFrame*create(const std::string&filename,const Rect&Rect
		,bool rotated,const Point&offset,const Size&originalSize, const Point& anchorPoint);
	//以图片创建，资源不会加载到TextureCache中
	static SpriteFrame*createWithTexture(Texture*texture,const Rect&rect);
	static SpriteFrame*createWithTexture(Texture*texture,const Rect&Rect
		,bool rotated,const Point&offset,const Size&originalSize, const Point& anchorPoint);

	bool initWithTextureFilename(const std::string&filename,const Rect&rect
		,bool rotated,const Point&offset,const Size&originalSize, const Point& anchorPoint);
	bool initWithTexture(Texture*texture,const Rect&Rect
		,bool rotated,const Point&offset,const Size&originalSize, const Point& anchorPoint);

	bool isRotated()const
	{
		return _rectRotated;
	}
	void setRotated(bool rotate)
	{
		_rectRotated = rotate;
	}
	Rect getRect()const
	{
		return _rect;
	}
	void setRect(const Rect&rect)
	{
		_rect = rect;
	}
	Point getOffsetPosition()const
	{
		return _offsetPosition;
	}
	void setOffsetPosition(const Point&offset)
	{
		_offsetPosition = offset;
	}
	Size getOriginalSize()const
	{
		return _originalSize;
	}
	void setOriginalSize(const Size&size)
	{
		_originalSize = size;
	}
	std::string getTextureFilename()const
	{
		return _textureFilename;
	}
	void setTextureFilename(const std::string&filename)
	{
		_textureFilename = filename;
	}
	void setTexturePackerRect(const Rect& rect)
	{
		_texturePackerRect = rect;
	}
	Rect getTexturePackerRect()const
	{
		return _texturePackerRect;
	}
	Point getAnchorPoint() const
	{
		return _anchorPoint;
	}
	
	void setAnchorPoint(const Point& anchorPoint)
	{
		_anchorPoint = anchorPoint;
	}
	//texture
	Texture*getTexture()const;
	void setTexture(Texture*texture); 

	SpriteFrame*clone()const;

};
NS_SDL_END
#endif
