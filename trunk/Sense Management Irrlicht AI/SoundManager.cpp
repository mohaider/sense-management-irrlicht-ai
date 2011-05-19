#include "SoundManager.h"


#include <vector>
#include <utility>

namespace
{
	typedef std::pair<Mix_Chunk*, int>  SoundPair; 

	typedef std::vector<SoundPair> Sounds;

	Sounds mySounds;
}

SoundManager* SoundManager::Instance()
{
	static SoundManager sm;
	return &sm;
}

void SoundManager::StopSound(int channel)
{
	Mix_HaltChannel(channel);
}

bool SoundManager::PlaySound(const std::string& filename, int loop, int channelnumber,int volume)
{
	Mix_Chunk *sound = Mix_LoadWAV(filename.c_str());
	if(sound == NULL) 
	{
		fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
		return false;
	}

	int channel = Mix_PlayChannel(channelnumber, sound, loop);
	if(channel == -1)
	{
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
		return false;
	}

	Mix_Volume(channelnumber,volume);

	mySounds.push_back(SoundPair(sound, channel));

	return true;
}

bool SoundManager::PlayMusic(const std::string& filename)
{
	static Mix_Music *music = 0;

	if (music)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(filename.c_str());
	if(music == NULL) 
	{
		printf("Unable to load Ogg file: %s\n", Mix_GetError());
		return false;
	}

	if(Mix_PlayMusic(music, 0) == -1) 
	{
		printf("Unable to play Ogg file: %s\n", Mix_GetError());
		return false;
	}


	return true;
}

SoundManager::SoundManager()
{
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 2000;

	if(Mix_OpenAudio(
		audio_rate, audio_format, audio_channels, audio_buffers) != 0) 
	{
		fprintf(stderr, "Unable to initialize audio: %s\n", 
			Mix_GetError());
	}
}

SoundManager::~SoundManager()
{
	Mix_CloseAudio();
}

void SoundManager::Update()
{
	// 1st step: free up sounds which have finished
	for (unsigned int i = 0; i < mySounds.size(); i++)
	{
		if (Mix_Playing(mySounds[i].second) == 0)
		{
			// It's finished
			Mix_FreeChunk(mySounds[i].first);
			mySounds[i].first = 0;
		}
	}

	// 2nd step: delete finished sounds from the vector
}
