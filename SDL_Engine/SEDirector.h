#ifndef __SDL_Director_H__
#define __SDL_Director_H__

#include<vector>
#include <algorithm>
#include<ctime>
#include<cstdlib>

#include "SDL.h"

#include "SEWindow.h"
#include "SERenderer.h"
#include "SESize.h"
#include "SERect.h"

NS_SDL_BEGIN
class Scene;
class InputHandler;
class EventDispatcher;
class ActionManager;
class TextureCache;
class SpriteFrameCache;
class LabelDotChar;
class Scheduler;

class Director : public Object
{
private:
	static Director* _pInstance;
private:
	Window* _window;
	Renderer* _renderer;
	//是否结束
	bool _running;
	/*保存各种场景*/
	std::vector<Scene*> _children;
	//当前场景
	Scene* _runningScene;
	//下一场景
	Scene* _nextScene;
	//是否删除运行场景
	bool _deleteRunningScene;
	//是否显示状态
	bool _displayStates;
	//每帧的持续时间
	float _secondsPerFrame;
	Uint32 _lastTick;
	Uint32 _nextTick;
	//每秒的帧数
	int _currentFrame;
	int _lastFrame;
	long _lastTime;
	unsigned int _totalFrames;
	//显示当前的渲染次数
	unsigned int _renderCount;
	//显示fps
	LabelDotChar* _showFPSLabel;
	//事件分发
	InputHandler* _inputHandler;
	EventDispatcher* _eventDispatcher;
	//动画管理器
	ActionManager* _actionManager;
	//添加图片池
	TextureCache* _textureCache;
	//精灵帧池
	SpriteFrameCache* _spriteFrameCache;
	//计时器
	Scheduler* _scheduler;
private:
	Director();
	~Director();
public:
	/*Director虽然继承自Object，但是没有调用autorelease，所以不会释放*/
	inline static Director* getInstance()
	{
		if(_pInstance == nullptr)
		{
			_pInstance = new Director();
		}
		return _pInstance;
	}
	static void purge();

	bool init(const char* title,int x,int y,int width,int height,Uint32 flags);
	void update();

	bool isRunning() const { return _running; }
	void setRunning(bool running) {  _running = running; }
	//获取当前平台
	std::string getPlatform() const;
	//--------------------------------场景相关-----------------------------------
	void runWithScene(Scene* pScene);
	void replaceScene(Scene* pScene);
	void pushScene(Scene* pScene);
	void popScene();
	//--------------------------------帧率相关-----------------------------------
	void setSecondsPerFrame(float secondPerFrame);
	void setDisplayStates(bool state);
	//--------------------------------分辨率适配相关-----------------------------------
	int renderSetScale(float scaleX,float scaleY);
	//是否开启屏幕适配
	int setResolutionScale();
	//开启逻辑分辨率
	int setLogicalSize();
	//获得当前运行的总帧数
	unsigned int getTotalFrames() const
	{
		return _totalFrames;
	}
	inline Window* getWindow() const
	{
		return _window;
	}
	inline Renderer* getRenderer() const
	{
		return _renderer;
	}
	inline EventDispatcher* getEventDispatcher() const
	{
		return _eventDispatcher;
	}
	inline ActionManager* getActionManager() const
	{
		return _actionManager;
	}
	inline TextureCache* getTextureCache() const
	{
		return _textureCache;
	}
	inline SpriteFrameCache* getSpriteFrameCache() const
	{
		return _spriteFrameCache;
	}
	inline Scheduler* getScheduler() const
	{
		return _scheduler;
	}
	Size getVisibleSize() const
	{
		return _window->getWindowSize();
	}
private:
	//处理场景
	void handleScene();
	//绘制场景
	void drawScene(float dt);
	//刷新状态
	void updateState();
};
NS_SDL_END
#endif
