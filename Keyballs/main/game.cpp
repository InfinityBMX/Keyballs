#include "../main/includes.h"

//LPDIRECTSOUNDBUFFER dsbSample = NULL;
//LPDIRECTSOUNDBUFFER dsbRunLikeHell = NULL;
SoundObject* soSample = NULL;
SoundObject* soRunLikeHell = NULL;
Keyboard* keyboard = NULL;

int InitGame(HWND hWnd)
{
	if(!InitMouse(hWnd))
	{
		debug(L"Error initializing the mouse");
		return FALSE;
	}

	keyboard = new Keyboard(hWnd);

	//if(!InitKeyboard(hWnd))
	//{
	//	debug(L"Error initializing the keyboard");
	//	return FALSE;
	//}

	soSample = new SoundObject(L"../media/sample.wav");

	return TRUE;
}

void GameRun(HWND hWnd)
{
	PollMouse();
	keyboard->PollKeyboard();
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
	if(keyboard->KeyDown(DIK_DOWN))
		soSample->playLooping();
	if(keyboard->KeyDown(DIK_UP))
		soSample->stop();
	if(keyboard->KeyDown(DIK_RIGHT))
		soSample->fadeOut();
	if(keyboard->KeyDown(DIK_LEFT))
		soSample->fadeIn();
	if(keyboard->KeyDown(DIK_ESCAPE))
		PostMessage(hWnd, WM_DESTROY, 0, 0);
	if(MouseButton(0))
		PostMessage(hWnd, WM_DESTROY, 0, 0);

	soSample->process();
}

void GameEnd(HWND hWnd)
{
//	if(dsbSample != NULL)
//		dsbSample->Release();

//	if(dsbRunLikeHell != NULL)
//		dsbRunLikeHell->Release();

//	soSample->kill();
//	soRunLikeHell->kill();
}