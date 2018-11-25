#include "SEChunk.h"
NS_SDL_BEGIN
const std::string Chunk::TYPE = "Chunk";

Chunk::Chunk()
	:_chunk(nullptr)
{
}
Chunk::~Chunk()
{
	if(_chunk)
		Mix_FreeChunk(_chunk);
}
Chunk*Chunk::create(const std::string&file)
{
	auto chunk = new Chunk();
	if(chunk && chunk->init(file))
		chunk->autorelease();
	else
		SDL_SAFE_DELETE(chunk);
	return chunk;
}
bool Chunk::init(const std::string&file)
{
	_chunk = Mix_LoadWAV(file.c_str());
	return _chunk != nullptr;
}
int Chunk::playChannel(int channel,int loops)
{
	return Mix_PlayChannel(channel,_chunk,loops);
}
int Chunk::volumeChunk(int volume)
{
	return Mix_VolumeChunk(_chunk,volume);
}
void Chunk::pause(int channel)
{
	Mix_Pause(channel);
}
void Chunk::resume(int channel)
{
	Mix_Resume(channel);
}
int Chunk::haltChannel(int channel)
{
	return Mix_HaltChannel(channel);
}
int Chunk::playing(int channel)
{
	return Mix_Playing(channel);
}
int Chunk::volume(int channel,int volume)
{
	return Mix_Volume(channel,volume);
}
NS_SDL_END
