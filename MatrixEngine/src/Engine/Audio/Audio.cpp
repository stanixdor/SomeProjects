// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AudioManager.cpp
// Author			:	Thomas Komair
// Creation Date	:	2012/02/15
// Purpose			:	main implementation file for the AudioManager
// History			:
// - 2012/03/29		:	- initial implementation
// ---------------------------------------------------------------------------
#include "Audio.h"

#define MAX_VOICE_COUNT 100	// ! CANNOT BE ZERO

// ----------------------------------------------------------------------------
#pragma region// Voice

Voice::Voice(): pChannel(NULL), isLooping(false), isPaused(true), volume(1.0f)
{}
Voice::~Voice()
{}
void Voice::SetVolume(float vol)
{
	if(pChannel == NULL)
		return;
	
	volume = vol;

	if(volume < 0.0f)
		volume = 0.0f;
	else if (volume > 1.0f)
		volume = 1.0f;

	pChannel->setVolume(volume);
}
float  Voice::GetVolume()
{
	return volume;
}
void Voice::SetPause(bool paused)
{
	if(pChannel == NULL)
		return;
	isPaused = paused;
	pChannel->setPaused(paused);
}
bool Voice::IsPaused()
{
	return isPaused;
}
void Voice::SetLoop(bool isLoop)
{
	if(pChannel == NULL)
		return;

	isLooping = isLoop;
	if(isLooping)
		pChannel->setLoopCount(-1);
	else
		pChannel->setLoopCount(0);
}
bool Voice::IsValid()
{
	if(pChannel == NULL)
		return false;

	bool pl;
	pChannel->isPlaying(&pl);
	return pl;
} 
bool Voice::IsLooping()
{
	return isLooping;
}
#pragma endregion

// ----------------------------------------------------------------------------
#pragma region// Sound

Sound::Sound() : pSound(NULL)
{}
#pragma endregion

// ----------------------------------------------------------------------------
#pragma region// Audio Manager

// Global 
AudioManager gAudioMgr;

// ----------------------------------------------------------------------------
AudioManager::AudioManager()
	: pFMOD(NULL)
	, soundCount(0)
	, voiceCount(MAX_VOICE_COUNT)
	, voices(0)
{
}
AudioManager::~AudioManager()
{
	Shutdown();
}
bool	AudioManager::Initialize()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&pFMOD);
	if (result!= FMOD_OK)
	{
		//AE_ASSERT_MESG(result == FMOD_OK, "AUDIO: Error creating FMOD system! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	// Initialize FMOD
	result = pFMOD->init(MAX_VOICE_COUNT, FMOD_INIT_NORMAL, 0);
	if (result  != FMOD_OK)
	{
		pFMOD->release();
		pFMOD = NULL;
		return false;
	}

	AllocVoices();

	return true;
}
void	AudioManager::Shutdown()
{
	if(NULL == pFMOD)
		return;

	pFMOD->release();
	pFMOD = NULL;
	FreeVoices();
}
void	AudioManager::Update()
{
	if(NULL == pFMOD)
		return;

	// Update FMOD -> play audio
	pFMOD->update();
	
	// play the voices that were created this frame. 
	PlayScheduledVoices();

	// loop through the voices
	for (Voice::PTR_LIST::iterator it = usedVoices.begin(); it != usedVoices.end(); ++it)
	{
		// this voice is no longer playing
		if (!(*it)->IsValid())
		{
			// erase and push to free list
			freeVoices.push_back(*it);
			it = usedVoices.erase(it);
			if (it == usedVoices.end())
				break;
		}
	}

}
FMOD::System * AudioManager::GetFMOD()
{
	return this->pFMOD;
}
// ----------------------------------------------------------------------------

void AudioManager::FreeThisVoice(Voice *pVoice)
{
	if(NULL == pFMOD)
		return;
	if(pVoice == NULL)
		return;

	pVoice->pChannel->stop();
	usedVoices.remove(pVoice);
	freeVoices.push_back(pVoice);
}
Voice *	AudioManager::GetFreeVoice()
{
	if(NULL == pFMOD)
		return NULL;
	if(freeVoices.empty())
		return NULL;
	
	Voice * pv = freeVoices.back();
	freeVoices.pop_back();
	usedVoices.push_back(pv);

	return pv;
}
void	AudioManager::AllocVoices()
{
	if(NULL == pFMOD)
		return;

	// free the voices if necessary
	FreeVoices();

	// allocate new array of voices
	voiceCount = MAX_VOICE_COUNT;
	voices = new Voice[voiceCount];

	// push all onto the free voices
	for(unsigned int i = 0;i < voiceCount; ++i)
		freeVoices.push_back(voices +i);
}
void	AudioManager::FreeVoices()
{
	if(NULL == pFMOD)
		return;
	if(voices)
	{
		delete [] voices;
		voiceCount = 0;
		freeVoices.clear();
		usedVoices.clear();
	}
}

// ----------------------------------------------------------------------------
Sound *	AudioManager::CreateSound(const char * filename)
{
	if(NULL == pFMOD)
		return NULL;

	// Allocate new memory for the sound
	Sound * pSound = new Sound();
	pFMOD->createSound(filename, FMOD_LOOP_NORMAL | FMOD_2D, 0, &pSound->pSound);

	// save the name of the 
	pSound->filename = filename;

	// error check
	if(pSound->pSound == NULL)
	{
		// make sure to delete the sound pointer
		delete pSound;
		return NULL;
	}

	// All is good
	++soundCount;	// Stats update
	return pSound;
}
void		AudioManager::FreeSound(Sound * pSound)
{
	if(NULL == pFMOD)
		return;
	if(!pSound)
		return;
	
	if(pSound->pSound)
	{
		pSound->pSound->release();
		pSound->pSound = 0;
	}

	// Stats update
	--soundCount;
	delete pSound;
}
Voice *	AudioManager::Play(Sound * pSound, bool paused)
{
	// make sure we can actually play the sound
	if( pFMOD == NULL || pSound == NULL)
		return NULL;
	
	// look for a free voice
	Voice * pVoice = GetFreeVoice();

	// this voice is valid
	if(pVoice)
	{
		// we found an available voice
		pFMOD->playSound(pSound->pSound, 0, paused, &pVoice->pChannel);
		pVoice->SetPause(paused);
		pVoice->SetLoop(false);
		//toPlayVoices.push_back(pVoice);
	}
	
	// Return the voice (either NULL or correct)
	return pVoice;
}
Voice *	AudioManager::Loop(Sound * pSound, bool paused)
{
	// make sure we can actually play the sound
	if( pFMOD == NULL || pSound == NULL)
		return NULL;

	
	// look for a free voice
	Voice * pVoice = GetFreeVoice();

	// this voice is valid
	if(pVoice)
	{
		pFMOD->playSound(pSound->pSound, 0, paused, &pVoice->pChannel);
		pVoice->SetPause(paused);
		pVoice->SetLoop(true);
		//toPlayVoices.push_back(pVoice);
	}
	
	// Return the voice (either NULL or correct)
	return pVoice;
}
void		AudioManager::StopAll()
{
	if(NULL == pFMOD)
		return;

	// loop through the voices
	while(!usedVoices.empty())
	{
		// erase and push to free list
		usedVoices.back()->pChannel->stop();
		freeVoices.push_back(usedVoices.back());
		usedVoices.pop_back();
	}
}

void AudioManager::PlayScheduledVoices()
{
	while (toPlayVoices.empty() == false)
	{
		//toPlayVoices.back()->SetPause(false);
		toPlayVoices.pop_back();
	}
}
#pragma endregion
