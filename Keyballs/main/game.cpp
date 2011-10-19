#include "../main/includes.h"

LPDIRECTSOUNDBUFFER dsbSample = NULL;
LPDIRECTSOUNDBUFFER dsbRunLikeHell = NULL;
bool played = false;

int InitGame(HWND hWnd)
{
	// Initialize the D3D Device
	if(!InitDirect3D(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN)){return FALSE;}
	if(InitSound(hWnd) != AUDIO_SUCCESS){return FALSE;}
	if(loadFileIntoDefaultBuffer(L"../media/sample.wav", dsbSample, directSoundDevice) != AUDIO_SUCCESS){return FALSE;}
	if(loadFileIntoDefaultBuffer(L"../media/Run Like Hell.wav", dsbRunLikeHell, directSoundDevice) != AUDIO_SUCCESS){return FALSE;}
	return TRUE;
}

void GameRun(HWND hWnd)
{
	//When sample stops playing, play run like hell once
	if(!played)
	{
		dsbSample->Play(0,0,0);
		played = true;
	}
	DWORD result;
	dsbSample->GetStatus(&result);
	if(result != DSBSTATUS_PLAYING)
	{
		dsbRunLikeHell->GetStatus(&result);
		if(result != DSBSTATUS_PLAYING)
			dsbRunLikeHell->Play(0,0,0);
	}
	Render();
}