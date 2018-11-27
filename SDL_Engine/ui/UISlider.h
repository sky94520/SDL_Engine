#ifndef __SDL_Slider_H__
#define __SDL_Slider_H__

#include <string>
#include "UIWidget.h"
#include "../SEPoint.h"

NS_SDL_BEGIN
class Scale9Sprite;
class Sprite;

namespace ui
{

class Slider : public Widget
{
public:
	enum class EventType
	{
		ON_PERCENTAGE_CHANGED,

		ON_SLIDEBALL_DOWN,
		ON_SLIDEBALL_UP,
		ON_SLIDEBALL_CANCEL,
	};
	typedef std::function<void (Slider*, EventType)> ccSliderCallback;

	Slider();
	virtual ~Slider();

	static Slider* create();
	/**
	 * 创建一个滑动条
	 * @param bar 滑动条背景
	 * @param slidBallNormal 滑动条
	 * @return 一个autoreleassed Slider object.
	 */
	static Slider* create(Scale9Sprite* bar, Sprite* slidBallNormal);

	bool init();
	bool init(Scale9Sprite* bar, Sprite* slidBallNormal);
	/**
	 * 加载滑动条纹理
	 * @param bar 纹理
	 */
	void loadBarSprite(Scale9Sprite* bar);
	/**
	 * 设置进度条纹理
	 * param progressBar 进度条点9精灵
	 */
	void loadProgressBarSprite(Scale9Sprite* progressBar);

	/**
	 * 加载滑动条正常精灵
	 * @slidBallNormalSprite 不能设置父亲
	 */
	void loadSlidBallSpriteNormal(Sprite* slidBallNormalSprite);
	/**
	 * 加载滑动条按下精灵
	 * @slidBallNormalSprite 不能设置父亲
	 */
	void loadSlidBallSpritePressed(Sprite* slidBallPressedSprite);
	/**
	 * 加载滑动条不可用精灵
	 * @slidBallNormalSprite 不能设置父亲
	 */
	void loadSlidBallSpriteDisabled(Sprite* slidBallDisabledSprite);
	/**
	 * 添加事件监听器
	 * @param callback 对应的回调函数
	 */
	void addEventListener(const ccSliderCallback& callback);
	/**
	 * 当前进度
	 */
	float getPercent() const;
	/**
	 * 设置进度
	 * @value 不能超过maxPercent
	 */
	void setPercent(float value);
	/**
	 * 最大进度
	 */
	float getMaxPercent() const;
	/**
	 * 设置最大进度
	 * @percent 最大进度
	 */
	void setMaxPercent(float percent);
protected:
	virtual bool touchBeganHook(Touch*touch,SDL_Event*event);
	virtual void touchMovedInHook(Touch*touch,SDL_Event*event);
	virtual void touchMovedOutHook(Touch*touch,SDL_Event*event);
	virtual void touchEndedHook(Touch*touch,SDL_Event*event);
	virtual void touchCancelledHook(Touch*touch,SDL_Event*event);
private:
	//更新显示
	void updatePercent();
protected:
	Scale9Sprite* _bar;
	Scale9Sprite* _progressBar;
	
	Sprite* _slidBallNormalSprite;
	Sprite* _slidBallPressedSprite;
	Sprite* _slidBallDisabledSprite;

	float _barLength;
	float _percent;
	//default is 100
	float _maxPercent;
	
	ccSliderCallback _callback;
	//touchBeganHook点击点
	Point _startPos;
};
}//end namespace ui
NS_SDL_END
#endif
