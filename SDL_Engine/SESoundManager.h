#ifndef __SDL_SoundManager_H__
#define __SDL_SoundManager_H__
#include<unordered_map>
#include<string>
#include<functional>
#include<thread>
#include<mutex>
#include<deque>
#include<condition_variable>

#include "SDL_mixer.h"
#include "SEObject.h"

NS_SDL_BEGIN
class Music;
class Chunk;

class SoundManager:public Object
{
protected:
	struct AsyncStruct;
private:
	static SoundManager*m_pInstance;
private:
	std::unordered_map<std::string,Music*> _musics;
	std::unordered_map<std::string,Chunk*> _effects;
	int _effectVolume;
	std::thread*_loadingThread;
	bool _needQuit;
	int _asyncRefCount;
	std::deque<AsyncStruct*> _asyncStructQueue;
	std::deque<AsyncStruct*> _requestQueue;
	std::deque<AsyncStruct*> _responseQueue;
	std::mutex _requestMutex;
	std::mutex _responseMutex;

	std::condition_variable _sleepCondition;
	//是否播放音乐和音效
	bool _playingMusic;
	bool _playingChunk;
	//保存当前播放音乐文件
	std::string _playingMusicName;
	int _playingMusicLoops;
public:
	SoundManager();
	~SoundManager();
public:
	static SoundManager*getInstance();
	static void purge();
	//------------------------音乐------------------------------------------
	//加载背景音乐
	Music*preloadBackgroundMusic(const std::string&file);
	//播放背景音乐
	int playBackgroundMusic(const std::string&file,int loops);
	//停止背景音乐的播放
	int stopBackgroundMusic()const;
	//暂停背景音乐
	int pauseBackgroundMusic()const;
	//恢复背景音乐的播放
	void resumeBackgroundMusic();
	//回放背景音乐
	void rewindBackgroundMusic();
	//是否正在播放
	int isBackgroundPlaying()const;
	//返回当前大小
	int getBackgroundMusicVolume()const;
	//设置音乐大小,[0,128]
	int setBackgroundMusicVolume(int volume)const;
	//------------------------------------音效------------------------------------------
	Chunk*preloadEffect(const std::string&file);
	//卸载资源
	void unloadEffect(const std::string&file);
	//播放音效，并返回音效所在的声道数
	int playEffect(const std::string&file,int loops);
	//暂停音效 channel 为声道位置
	void pauseEffect(int channel);
	//暂停所有音效
	void pauseAllEffect();
	//恢复音效
	void resumeEffect(int channel);
	//恢复所有音效
	void resumeAllEffect();
	//停止音效播放
	int stopEffect(int channel);
	//停止所有音效
	int stopAllEffect();
	//获取某一频道是否在播放
	int isEffectPlaying(int channel)const;
	//获取当前音效
	int getEffectVolume()const;
	void setEffectVolume(int volume);
	//异步加载资源
	void addSoundAsync(const std::string&filename,const std::string&type,std::function<void (Object*data,const std::string&)> callback);
	void waitForQuit();
	//播放音乐
	bool isPlayingMusic() const;
	void setPlayingMusic(bool var);
	//音效
	bool isPlayingChunk() const;
	void setPlayingChunk(bool var);
private:
	void loadSound();
	void addSoundAsyncCallback(float dt);
};
NS_SDL_END
#endif
