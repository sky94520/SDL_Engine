#ifndef __SDL_Music_H__
#define __SDL_Music_H__
#include<string>
#include "SDL_mixer.h"
#include "SEObject.h"
NS_SDL_BEGIN
class Music:public Object
{
private:
	Mix_Music*_music;
public:
	const static std::string TYPE;
public:
	Music();
	~Music();
	
	static Music*create(const std::string&file);
	bool init(const std::string&file);
	//获取音乐类型
	Mix_MusicType getMusicType()const;
	//播放音乐 -1为一直播放 0为播放一次，以此类推.返回0表示成功
	int playMusic(int loops)const;
public:
	//设置音乐大小 [0,128],-1返回当前设置大小
	static int volumeMusic(int volume);
	//中断当前播放音乐
	static int haltMusic();
	//暂停音乐
	static void pauseMusic();
	//恢复音乐播放
	static void resumeMusic();
	//回转音乐
	static void rewindMusic();
	//应该是 当前音乐是否暂停
	static int pausedMusic();
	//音乐是否正在播放
	static int playingMusic();
private:
	bool loadMUS(const std::string&file);
};
NS_SDL_END
#endif
