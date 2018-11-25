#include "SEDirector.h"
#include "SETexture.h"
#include "SESurface.h"
#include "SEPoint.h"
#include "SEScene.h"
#include "SEPoolManager.h"
#include "SEInputHandler.h"
#include "SEEventDispatcher.h"
#include "SEActionManager.h"
#include "SEFileUtils.h"
#include "SETextureCache.h"
#include "SESpriteFrameCache.h"
#include "SELabelAtlas.h"
#include "SEStringUtils.h"
#include "SENotificationCenter.h"
#include "SEScheduler.h"
#include "SEFileUtils.h"
#include "SESoundManager.h"
#include "ui/UIWidgetManager.h"
#include "SEUserDefault.h"
#include "SELabelDotChar.h"
#include "SEAnimationCache.h"
#include "SEFontCache.h"
#include "SEBMFontConfiguration.h"

NS_SDL_BEGIN
Director* Director::_pInstance = NULL;

Director::Director()
	:_window(nullptr)
	,_renderer(nullptr)
	,_running(true)
	,_runningScene(nullptr)
	,_nextScene(nullptr)
	,_deleteRunningScene(false)
	,_displayStates(true)
	,_secondsPerFrame(1/60.f)
	,_lastTick(0)
	,_nextTick(0)
	,_currentFrame(0)
	,_lastFrame(60)
	,_lastTime(0)
	,_totalFrames(0)
	,_renderCount(0)
	,_showFPSLabel(nullptr)
	,_inputHandler(nullptr)
	,_eventDispatcher(nullptr)
	,_actionManager(nullptr)
	,_textureCache(nullptr)
	,_spriteFrameCache(nullptr)
	,_scheduler(nullptr)
{
}

Director::~Director()
{
	//释放FPS Label
	SDL_SAFE_RELEASE(_showFPSLabel);
	//清除场景
	for(auto iter = _children.begin();iter != _children.end();)
	{
		auto scene = *iter;

		scene->onExit();
		scene->cleanup();
		scene->release();

		iter = _children.erase(iter);
	}
	_runningScene = nullptr;
	_nextScene = nullptr;
	//清除动画缓存池
	AnimationCache::purge();	
	//清除文件管理器 单例类的清除
	FileUtils::purge();
	//清除观察者
	NotificationCenter::purge();
	//清除音乐
	SoundManager::purge();
	//清除简单文件管理
	UserDefault::purge();
	//清除字体缓存
	BMFontConfiguration::removeFNTCache();
	FontCache::purge();
	//关闭ui模块
	ui::UIWidgetManager::purge();
	//释放事件分发器
	SDL_SAFE_RELEASE(_inputHandler);
	SDL_SAFE_RELEASE(_eventDispatcher);
	//释放动画管理器
	SDL_SAFE_RELEASE(_actionManager);
	//清除精灵帧池
	SDL_SAFE_RELEASE(_spriteFrameCache);
	//清除图片池 因为会调用removeAll,所以得放到最后
	SDL_SAFE_RELEASE(_textureCache);
	//清除计时器
	SDL_SAFE_RELEASE(_scheduler);
	//释放renderer和窗口
	SDL_SAFE_RELEASE(_renderer);
	SDL_SAFE_RELEASE(_window);

	SDL_Quit();
}

bool Director::init(const char* title,int x,int y,int width,int height,Uint32 flags)
{
	//初始化SDL
	if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)!=0)
	{
		LOG("%s",SDL_GetError());
		return false;
	}
	_window = Window::create(title,x,y,width,height,flags);
	_renderer = Renderer::create(_window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
		| SDL_RENDERER_PRESENTVSYNC);
	//保持引用
	_window->retain();
	_renderer->retain();
	//设置应用程序名称
	FileUtils::getInstance()->setAppString(title);
	//事件分发器
	_inputHandler = new InputHandler();
	//事件处理器
	_eventDispatcher =  new EventDispatcher();
	//动画管理器
	_actionManager = new ActionManager();
	//图片池
	_textureCache = new TextureCache();
	//精灵帧池
	_spriteFrameCache = new SpriteFrameCache();
	//计时器
	_scheduler = new Scheduler();
	//初始化随机数种子
	srand((unsigned int)time(NULL));
	
	printf("the window initlize successfully\n");

	return true;
}

void Director::update()
{
	Uint32 currentTick = SDL_GetTicks();
	if(currentTick >= _nextTick)
	{
		++_totalFrames;
		++_currentFrame;
		//真实帧率
		float dt = _secondsPerFrame;//(_nextTick - _lastTick)/1000.f;
		_lastTick = currentTick;//_nextTick;
		_nextTick = _lastTick + (Uint32)(_secondsPerFrame * 1000);
		//进行绘制
		this->drawScene(dt);
	}
	//处理场景
	handleScene();
	//更新FPS
	updateState();
}

std::string Director::getPlatform() const
{
	return std::string(SDL_GetPlatform());
}

void Director::runWithScene(Scene* pScene)
{
	SDLASSERT(pScene != NULL,"the pScene should not null");
	pushScene(pScene);
}

void Director::replaceScene(Scene* pScene)
{
	SDLASSERT(pScene != NULL,"the pScene should not null");
	//当前_runningScene为空，相当于调用runWithScene
	if(_runningScene == nullptr)
	{
		runWithScene(pScene);
		_deleteRunningScene = false;

		return;
	}
	if(_runningScene == pScene)
		return;
	//保持对场景的引用
	pScene->retain();
	_deleteRunningScene = true;

	_children.pop_back();
	_children.push_back(pScene);
	_nextScene = pScene;
}

void Director::pushScene(Scene* pScene)
{
	SDLASSERT(pScene != NULL,"pScene should not null");

	pScene->retain();
	_children.push_back(pScene);
	_nextScene = pScene;
	_deleteRunningScene = false;
}

void Director::popScene()
{
	SDLASSERT(_runningScene != NULL, "the running scene should not null");
	int size = _children.size();
	//为1时，直接退出
	if(size == 1)
	{
		_running = false;
	}
	else 
	{
		_children.pop_back();

		_deleteRunningScene = true;
		_nextScene = _children.back();
	}
}

void Director::purge()
{
	//释放自身
	SDL_SAFE_RELEASE_NULL(_pInstance);	
}

void Director::setSecondsPerFrame(float secondPerFrame)
{
	_secondsPerFrame = secondPerFrame;
}

void Director::setDisplayStates(bool state)
{
	_displayStates = state;
}

int Director::renderSetScale(float scaleX, float scaleY)
{
	return _renderer->setScale(scaleX,scaleY);
}

int Director::setResolutionScale()
{
	Size originalSize = _window->getWindowSize();
	Size realSize = _window->getRealWindowSize();

	if(originalSize == realSize)
		return 0;

	Size scale = realSize / originalSize;

	return _renderer->setScale(scale.width,scale.height);
}

int Director::setLogicalSize()
{
	Size originalSize = _window->getWindowSize();
	Size realSize = _window->getRealWindowSize();

	if(originalSize == realSize)
		return 0;

	int w = (int)realSize.width;
	int h = (int)realSize.height;

	return _renderer->setLogicalSize(w,h);
}

void Director::handleScene()
{
	if(_nextScene)
	{
		if(_runningScene)
		{
			_runningScene->onExit();
			_runningScene->cleanup();
		}
		if(_deleteRunningScene)
		{
			_deleteRunningScene = false;
			_runningScene->release();
		}
		//对这个场景引用
		_runningScene = _nextScene;
		//对这个场景调用onEnter
		_runningScene->onEnter();
		
		_nextScene = nullptr;
	}
}

void Director::drawScene(float dt)
{
	//如果不空
	if(_runningScene)
	{
		//屏幕刷新
		_renderer->clear();
		//动作更新
		_actionManager->update(dt);
		//场景更新
		_runningScene->visit();
		//显示FPS
		if(_showFPSLabel && _displayStates)
			_showFPSLabel->draw();
		//绘制
		_renderCount = _renderer->render();
		//计时器更新
		_scheduler->update(dt);
		//事件分发
		_inputHandler->update();
		//刷新帧
		_renderer->present();
	}
}

void Director::updateState()
{
	if(SDL_GetTicks() - _lastTime >= 1000)
	{
		//由于是从0开始加，所以得加一
		_lastFrame = _currentFrame;
		_currentFrame = 0;
		_lastTime = SDL_GetTicks();
		//暂时输出在控制台
		if(_displayStates)
		{
			if(_showFPSLabel == nullptr)
			{
				//初始化fps label
				_showFPSLabel = LabelDotChar::create("FPS:60",3,Color3B(0,0,0));

				_showFPSLabel->setAnchorPoint(Point::ZERO);
				_showFPSLabel->retain();
			}
			std::string text = StringUtils::format("FPS:%d/%d",_lastFrame,_renderCount);
			_showFPSLabel->setString(text);
			//printf("FPS:%d/%d\n", _lastFrame, _renderCount);
		}
	}
}

NS_SDL_END
