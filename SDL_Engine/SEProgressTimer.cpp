#include "SEProgressTimer.h"
#include "SESprite.h"
#include "SEDirector.h"

NS_SDL_BEGIN
ProgressTimer::ProgressTimer()
	:_percentage(0.f)
	,_type(Type::BAR)
	,_sprite(nullptr)
	,_bg(nullptr)
	,_reverseDirection(false)
{
}
ProgressTimer::~ProgressTimer()
{
	SDL_SAFE_RELEASE(_sprite);
}
ProgressTimer*ProgressTimer::create(Sprite*sprite)
{
	auto progressTimer = new ProgressTimer();
	if(progressTimer && progressTimer->initWithSprite(sprite))
		progressTimer->autorelease();
	else
		SDL_SAFE_DELETE(progressTimer);
	return progressTimer;
}
bool ProgressTimer::initWithSprite(Sprite*sprite)
{
	setAnchorPoint(Point(0.5f,0.5f));

	_type = Type::RADIAL;
    _reverseDirection = false;
    setMidpoint(Point(0.5f,0.5f));
    setBarChangeRate(Point(0,1));

    setSprite(sprite);

    setPercentage(0.0f);

	return true;
}
void ProgressTimer::setPercentage(float percentage)
{
	_percentage = percentage;
	//边界控制
	if(_percentage < 0)
		_percentage = 0.f;
	if(_percentage > 100.f)
		_percentage = 100.f;

	switch(_type)
	{
	case Type::BAR:this->updateBar();break;
	case Type::RADIAL:this->updateRadial();break;
	}
}
void ProgressTimer::setType(Type type)
{
	_type = type;
}
void ProgressTimer::setSprite(Sprite*sprite)
{
	if(_sprite != sprite)
	{
		SDL_SAFE_RELEASE(_sprite);
		_sprite = sprite;
		SDL_SAFE_RETAIN(_sprite);
		//创建bg 精灵
		if(_bg)
			_bg->removeFromParent();
		//根据精灵大小创建对应的背景
		Size size = _sprite->getContentSize();
		_bg = Sprite::createWithTexture(SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,(int)size.width,(int)size.height);
		//设置锚点
		_bg->setPosition(_anchorPoint.x*size.width,_anchorPoint.y*size.height);
		//重新设置大小
		this->setContentSize(size);
		//设置混合模式
		_bg->getTexture()->setBlendMode(SDL_BLENDMODE_BLEND);
		this->addChild(_bg);
	}
}
void ProgressTimer::setMidpoint(const Point&midpoint)
{
	_midpoint = midpoint;
}
void ProgressTimer::setBarChangeRate(const Point&barChangeRate)
{
	_barChangeRate =barChangeRate;
}

void ProgressTimer::setRotation(float rotation)
{
	Node::setRotation(rotation);
	
	if(_bg)
		_bg->setRotation(rotation);
}

void ProgressTimer::updateBar()
{
	const int count = 4;
	Sint16 vx[count];
	Sint16 vy[count];
	//当前还剩多少
	Point hasRate = (Point(1,1) - _barChangeRate);
	//先确定当前显示比例
	Point percent = hasRate + Point(_percentage/100.f*_barChangeRate.x,_percentage/100.f*_barChangeRate.y);

	Size size = _sprite->getContentSize();
	Size destSize = Size(size.width*percent.x,size.height*percent.y);
	//获取渲染位置
	Point destPos = Point(_midpoint.x*size.width,_midpoint.y*size.height);
	//获取渲染面积
	SDL_Rect destRect = {int(destPos.x - destSize.width*_midpoint.x),int(destPos.y - destSize.height*_midpoint.y),int(destSize.width),int(destSize.height)};
	//转换成gfx使用的参数
	vx[0] = destRect.x;vy[0] = destRect.y;
	vx[1] = destRect.x + destRect.w;vy[1] = destRect.y;
	vx[2] = destRect.x + destRect.w;vy[3] = destRect.y + destRect.h;
	vx[3] = destRect.x;vy[2] = destRect.y + destRect.h;

	this->texturedPolygon(vx,vy,4);
}
void ProgressTimer::updateRadial()
{
	//当前渲染进度
	auto alpha = _percentage/100.f;
	//获取当前角度
	float angle = float(2*M_PI*(_reverseDirection? (1 - alpha) : alpha));
	//获取顶点中间位置
	Point topMid = Point(_midpoint.x,0.f);
	//将topMid绕_midpoint旋转angle度后，将结果保存
	Point percentagePt = topMid.rotateByAngle(_midpoint,angle);
	//定义索引值
	int index = 0;
	//定义一个点存储由旋转位置与精灵四边形边缘的碰撞位置
	Point hit = Point::ZERO;
	if(alpha == 0.f)
	{
		hit = topMid;
		index = 0;
	}
	else if(alpha == 1.f)
	{
		hit = topMid;
		index = 4;
	}
	else//使用循环进行判断碰撞点
	{
		float min_t = FLT_MAX;
		//顺时针遍历所有拐点坐标
		for(int i = 0;i < 4;i++)
		{
			//获取上一个拐点的索引
			int pIndex = (i + 3)%4;
			Point edgePtA = this->boundaryTexCoord(i % 4);
			Point edgePtB = this->boundaryTexCoord(pIndex);

			if(i == 0)
			{
				edgePtB = edgePtA.lerp(edgePtB,1 - _midpoint.x);
			}
			else if(i == 4)
			{
				edgePtA = edgePtB.lerp(edgePtA,1 - _midpoint.x);
			}
			            // 定义FLOAT临时变量s,t。  
            // s用来存放中心点按旋转角度方向的直线与边缘直线相交时交点与中心点的距离。  
            // t用来存放中心点按旋转角度方向的直线与边缘直线相交时交点与边缘段起点的距离。  
            float s = 0, t = 0;  
           //判断以中心点和旋转点percentagePt所在的直线与edgePtA，edgePtB所在的直线是否相交,如果相交，返回s和t。  
            if(Point::isLineIntersect(edgePtA, edgePtB, _midpoint, percentagePt, &s, &t))  
            {  
                //如果有交点。  
                if ((i == 0 || i == 4)) 
				{  
                    // 第一和最末次循环时s值的有效性判断。  
                    if (!(0.f <= s && s <= 1.f)) 
					{  
                        continue;  
                    }  
                }  
               //计录每次循环进行碰撞检测时，有效的最短t值和对应的循环索引。  
                if (t >= 0.f) 
				{  
                    if (t < min_t) 
					{  
                        min_t = t;  
                        index = i;  
                    }
                }  //endif

			}//end if
		}//end for
		//由min_t和方向计算出碰撞点存入hit。  
         hit = _midpoint+ ((percentagePt - _midpoint) * min_t);
	}//end else
	//确定顶点个数
	int count = 3 + index;

	//Sint16*vx = new Sint16[count];
	//Sint16*vy = new Sint16[count];
	Sint16 vx[8],vy[8];

	memset(vx,0,sizeof(vx));
	memset(vy,0,sizeof(vy));
	//前两个是确定的值
	vx[0] = Sint16(this->textureCoordFromAlphaPoint(_midpoint).x);
	vy[0] = Sint16(this->textureCoordFromAlphaPoint(_midpoint).y);

	vx[1] = Sint16(this->textureCoordFromAlphaPoint(topMid).x);
	vy[1] = Sint16(this->textureCoordFromAlphaPoint(topMid).y);
	//设置除了最后一个点后的其他点
	for(int i = 0;i < index;i++)
	{
		Point boundry = this->boundaryTexCoord(i);
		vx[i + 2] = (Sint16)this->textureCoordFromAlphaPoint(boundry).x;
		vy[i + 2] = (Sint16)this->textureCoordFromAlphaPoint(boundry).y;
	}
	vx[count - 1] = (Sint16)this->textureCoordFromAlphaPoint(hit).x;
	vy[count - 1] = (Sint16)this->textureCoordFromAlphaPoint(hit).y;

	this->texturedPolygon(vx,vy,count);

	//delete[] vx;
	//delete[] vy;
}
Point ProgressTimer::boundaryTexCoord(int index)
{
	if(index == 0)
		return Point(1,0);
	else if(index == 1)
		return Point(1,1);
	else if(index == 2)
		return Point(0,1);
	return Point(0,0);
}
void ProgressTimer::texturedPolygon(Sint16*vx,Sint16*vy,int n)
{
	auto renderer = Director::getInstance()->getRenderer();
	//设置bg为当前渲染目标
	Texture*bg = _bg->getTexture();

	SDL_BlendMode oldBlendMode = SDL_BLENDMODE_NONE;
	renderer->getDrawBlendMode(&oldBlendMode);

	renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);
	renderer->setTarget(bg);
	auto oldColor = renderer->getDrawColor();
	//这个是任何一个颜色都无所谓，反正是透明颜色
	renderer->setDrawColor(0,0,0,0);
	renderer->clear();
	//使用SDL_gfx渲染
	auto texture= _sprite->getTexture()->getSDL_Texture();
	auto render = renderer->getSDL_Renderer();

	::texturedPolygonMod(render,vx,vy,n,texture,0,0);
	//设置为默认渲染目标
	renderer->setTarget(nullptr);
	//恢复
	renderer->setDrawColor(oldColor.r,oldColor.g,oldColor.b,oldColor.a);
	renderer->setDrawBlendMode(oldBlendMode);
}
Point ProgressTimer::textureCoordFromAlphaPoint(Point alpha)
{
	Size size = this->getContentSize();
	return Point(size.width * alpha.x,size.height * alpha.y);
}
NS_SDL_END
