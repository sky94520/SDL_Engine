					SDL_Engine引擎注意事项 SDL 2.07
1.默认使用了一个窗口(也只有一个窗口)

3.Node删除时最好使用removeFromParent或者removeChild(node,true),因为这样才会清除干净

4.本引擎依赖于SDL SDL_image SDL_ttf SDL_mixer SDL_gfx Box2D zlib rapidxml

5.bug之一，目前对于双击没有较好的支持
已经修复，因为win下保证只会有一个触碰点

6.不建议使用SDL_gfx，如果要使用,最好先保存原来的渲染颜色和渲染模式

7.update返回的值一直是设置的帧率的时间，
无论实际上经过了多久，若要获得实际运行时间，使用SDL_GetTicks()

8.存在一部分警告，无伤大雅
.Debug下键盘事件，SDL_PollEvent会存在一定的延迟，原因未知

10.本引擎FPS显示使用LabelDotChar,不再使用lazyfont作为系统字体，不再依赖额外资源

11.本引擎使用了外置的PxString和SDL_Gfx，对里面的代码进行了很细微的改变以适应本引擎。
具体如下
修改texturedPolygon的参数值 SDL_Surface为SDL_Texture
内部使用SDL_QueryTexture()来获取大小，
并且对SDL_DestroyTexture和SDL_RenderPresent进行了注释

没有修改，SDL_gfx_mod.h 而没有修改 SDL_gfx

12.本引擎需要c++11以上支持

13.本引擎稍微修改了下 GB2ShapeCache-x.h ~.cpp，已经能支持Physics editor导出的文件

14.支持外部ui文件xml

15.渲染全部交给Sprite进行，不过目前并不好用，等待以后更新（目前已经更新）

16.关于和位置有关的动作，需要注意的是，SDL的使用的是Opengl坐标系,不是笛卡尔坐标系，这个需注意

17.contentSize只是逻辑大小，影响渲染，影响渲染的是scale anchorPoint flip等

18.如没有特殊需求，就不要修改texture的blend mode

19.SDL_Engine目前已经能很好的支持图片的锚点，旋转，动画等，代价就是会增加一些内存的使用

20.TextureCache的线程安全问题暂时没有解决，只是简单更新了下。
21.FileUtils isFileExist()是否存在该文件，会尝试打开文件，效率低下。

2018-11-12
22.当前所有的事件监听器优先级默认为0，注意，注意，注意！！！

2018-11-26
23.EventListenerTouchOneByOne类的onTouchBegan()返回值为true表示吞并事件，
并且该事件在onTouchMoved()、onTouchEnded()同样有用，不过如果想要真正地吞并事件，
还需要调用listener->setSwallowTouches(true)
