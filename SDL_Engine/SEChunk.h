#ifndef __SDL_Chunk_H__
#define __SDL_Chunk_H__
#include<string>
#include "SDL_mixer.h"
#include "SEObject.h"
NS_SDL_BEGIN
class Chunk:public Object
{
private:
	Mix_Chunk*_chunk;
public:
	const static std::string TYPE;
public:
	Chunk();
	~Chunk();
	static Chunk*create(const std::string&file);
	bool init(const std::string&file);

	int playChannel(int channel,int loops);
	//获取该音量 -1返回该音量
	int volumeChunk(int vloume);
public:
	//暂停某一声道的音效 channel 为-1暂停所有音效
	static void pause(int channel);
	//恢复某一声道的音效
	static void resume(int channel);
	//中断某一声道的音效
	static int haltChannel(int channel);
	//判断某一声道是否播放
	static int playing(int channel);
	//设置音效 -1为设置所有声道数
	static int volume(int channel,int volume);
};
NS_SDL_END
#endif
