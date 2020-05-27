// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	Level.cpp
// Author			:	Thomas Komair
// Creation Date	:	2013/04/26
// Purpose			:	Implementation of demo level game states functions.
// History			:
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// INCLUDES
/*#include "Level 1.h"
#include "../Audio/Audio.h"

// ---------------------------------------------------------------------------
// GLOBAL VARIABLES AND DEFINITIONS

// level 1;
namespace Level_1
{
	// Common
	Sound *			bgSound;
	Sound *			clapSound;
	Voice *			pBGVoice;
	Voice *			pClapVoice;
	f32				gVolume = 1.0f;
	bool			gMusicPaused = false;
}using namespace Level_1;

// ---------------------------------------------------------------------------
// HELPER FUNCTIONS DECLARATIONS

// ---------------------------------------------------------------------------
// GAME STATE FUNCTIONS
void Level_1_Load()
{
	// Load a couple of sounds
	bgSound = gAudioMgr.CreateSound("data\\Lost_Civilization.mp3");
	clapSound = gAudioMgr.CreateSound("data\\Clap.wav");
}
void Level_1_Init()
{
	// Set the background color to black.
	AEGfxSetClearColor(0xFF000000);
	
	// Create the bg sound and set it to loop
	pBGVoice = gAudioMgr.Loop(bgSound);

	// create the clap voice and set it paused
	pClapVoice = gAudioMgr.Play(clapSound, true);
}
void Level_1_Update()
{
	// play when the left mouse button is triggered
	if (AEInputMouseTriggered(AE_MOUSE_LEFT))
	{
		// if the clap voice is paused, then unpause it
		// this will happen at the beginning.
		if (pClapVoice->IsPaused())
			pClapVoice->SetPause(false);

		// if the clap voice is no longer valid (i.e. it stopped 
		// playing), then we play again with the audio manger. 
		else if (pClapVoice->IsValid() == false)
			pClapVoice = gAudioMgr.Play(clapSound);
	}

	// toggle music paused
	if (AEInputKeyTriggered(VK_SPACE))
	{
		gMusicPaused = !gMusicPaused;
		pClapVoice->SetPause(gMusicPaused);
		pBGVoice->SetPause(gMusicPaused);
	}
	
	// raise or lower the volume
	if (AEInputKeyPressed(VK_OEM_PLUS))
		gVolume = AEClamp(gVolume + 0.01f, 0.0f, 1.0f);
	if (AEInputKeyPressed(VK_OEM_MINUS))
		gVolume = AEClamp(gVolume - 0.01f, 0.0f, 1.0f);

		pBGVoice->SetVolume(gVolume);
}
void Level_1_Draw()
{
	AEGfxPrint(10, 5, 0xFFFFFFFF, "Click to play a clap sound\n'+' and '-' to control the music volume.");
}
void Level_1_Free()
{
	gAudioMgr.StopAll();
};
void Level_1_Unload()
{
	gAudioMgr.FreeSound(bgSound);
	gAudioMgr.FreeSound(clapSound);
}
*/