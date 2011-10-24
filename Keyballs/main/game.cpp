#include "../main/includes.h"

LPDIRECTSOUNDBUFFER dsbSample = NULL;
LPDIRECTSOUNDBUFFER dsbRunLikeHell = NULL;

int InitGame(HWND hWnd)
{
	if(!InitMouse(hWnd))
	{
		debug(L"Error initializing the mouse");
		return FALSE;
	}

	if(!InitKeyboard(hWnd))
	{
		debug(L"Error initializing the keyboard");
		return FALSE;
	}

	if(loadFileIntoDefaultBuffer(L"../media/sample.wav", dsbSample, directSoundDevice) != AUDIO_SUCCESS)
	{
		debug(L"Error loading Sample.wav");
		return FALSE;
	}
	if(loadFileIntoDefaultBuffer(L"../media/Run Like Hell.wav", dsbRunLikeHell, directSoundDevice) != AUDIO_SUCCESS)
	{
		debug(L"Error loading Run Like Hell.wav");
		return FALSE;
	}
	
	return TRUE;
}

void GameRun(HWND hWnd)
{
	PollMouse();
	PollKeyboard();
	//When sample stops playing, play run like hell once
/*	if(!played)
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
	}*/
	Render();
	if(KeyDown(DIK_DOWN))
		dsbSample->Play(0,0,0);
	if(KeyDown(DIK_UP))
		dsbSample->Stop();
	if(KeyDown(DIK_RIGHT))
		dsbSample->Restore();
	if(KeyDown(DIK_ESCAPE))
		PostMessage(hWnd, WM_DESTROY, 0, 0);
	if(MouseButton(0))
		PostMessage(hWnd, WM_DESTROY, 0, 0);
}

void GameEnd(HWND hWnd)
{
	if(dsbSample != NULL)
		dsbSample->Release();

	if(dsbRunLikeHell != NULL)
		dsbRunLikeHell->Release();
}