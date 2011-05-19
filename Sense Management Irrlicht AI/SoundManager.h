#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H


#include <string>
#include "SDL_mixer.h"


class SoundManager
{
public:
	static SoundManager* Instance();

	~SoundManager();

	bool PlaySound(const std::string& filename, int loop, int channel,int volume);

	bool PlayMusic(const std::string& filename);

	void StopSound(int channel);

	void Update();

private:
	SoundManager();
	// No definition for these functions - Singleton
	SoundManager(const SoundManager&);
	SoundManager& operator=(const SoundManager&);
};

#endif
