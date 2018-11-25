#include "SEMusic.h"
NS_SDL_BEGIN

const std::string Music::TYPE = "Music";

Music::Music()
	:_music(nullptr)
{
}
Music::~Music()
{
	if(_music)
		Mix_FreeMusic(_music);
}
Music*Music::create(const std::string&file)
{
	auto music = new Music();
	if(music && music->init(file))
		music->autorelease();
	else
		SDL_SAFE_DELETE(music);
	return music;
}
bool Music::init(const std::string&file)
{
	_music = Mix_LoadMUS(file.c_str());
	return _music != nullptr;
}
Mix_MusicType Music::getMusicType()const
{
	return _music == nullptr? Mix_MusicType::MUS_NONE : Mix_GetMusicType(_music);
}
int Music::playMusic(int loops)const
{
	return _music == nullptr?-1:Mix_PlayMusic(_music,loops);
}
bool Music::loadMUS(const std::string&file)
{
	_music = Mix_LoadMUS(file.c_str());
	return _music != nullptr;
}
//---------------------------------static----------------------------
int Music::volumeMusic(int volume)
{
	return Mix_VolumeMusic(volume);
}
int Music::haltMusic()
{
	return Mix_HaltMusic();
}
void Music::pauseMusic()
{
	Mix_PauseMusic();
}
void Music::resumeMusic()
{
	Mix_ResumeMusic();
}
void Music::rewindMusic()
{
	Mix_RewindMusic();
}
int Music::pausedMusic()
{
	return Mix_PausedMusic();
}
int Music::playingMusic()
{
	return Mix_PlayingMusic();
}
NS_SDL_END
