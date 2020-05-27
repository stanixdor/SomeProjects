// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEAudio.h
// Author			:	Thomas Komair
// Creation Date	:	2013/06/13
// Purpose			:	main header file for the AudioManager
// History			:
// - 2012/03/29		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_AUDIO_MANAGER_H_
#define AE_AUDIO_MANAGER_H_

#include <fmod.hpp>
#include <list>

struct Sound
{
	FMOD::Sound		*pSound;
	std::string		 filename;

	Sound();
};

class Voice
{
	// friends have access to privates
	friend class AudioManager;

public:

	Voice();
	~Voice();

	// Setters and Getters
	void SetVolume(float volume);
	void SetPause(bool paused);
	void SetLoop(bool isLoop);
	bool IsPaused();
	bool IsValid(); 
	bool IsLooping();
	float  GetVolume();
	
	// STL
	typedef std::list<Voice *> PTR_LIST;

private:
	
	FMOD::Channel * pChannel;
	float			volume;
	bool			isLooping;
	bool			isPaused;
};

class AudioManager
{
public:

	AudioManager();
	~AudioManager();

	bool Initialize();
	void Update();
	void Shutdown();

	// Sound Management
	Sound * CreateSound(const char * filename);
	void	FreeSound(Sound * pSound);
	Voice * Play(Sound * pSound, bool paused = false);
	Voice * Loop(Sound * pSound, bool paused = false);

	FMOD::System * GetFMOD();

	// Stop All
	void StopAll();
	void FreeThisVoice(Voice *pVoice);

private:
	
	// Helper methods
	void						AllocVoices();
	void						FreeVoices();

	// Voice manager... adds to free and removes from used
	Voice *						GetFreeVoice();

	// FMOD System
	FMOD::System*				pFMOD;

	// keep track of the sounds created 
	unsigned int				soundCount;
	unsigned int				voiceCount;
	// Voice Management List (equivalent to dead list and free list)
	Voice *						voices;
	Voice::PTR_LIST				freeVoices;
	Voice::PTR_LIST				usedVoices;
	Voice::PTR_LIST				toPlayVoices;

	// Actually play the voices that have been created this frame!
	// Note: this is done this way because a voice will play as soon
	// as it is created by FMOD (if it's not set to paused), which
	// means that any setting on the volume will not take effect until 
	// FMOD updates (which happens during the AudioManager update)
	void PlayScheduledVoices();

};

// GLOBAL Audio Manager
extern AudioManager gAudioMgr;

// ----------------------------------------------------------------------------
#endif