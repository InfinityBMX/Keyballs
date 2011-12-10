//#include "../main/includes.h"
#include "game.h"

Game::Game(HWND hWnd){
	this->keyboard = new Keyboard(hWnd);
	this->hWnd = hWnd;
	soSample = new SoundObject(L"../media/sample.wav");
	//What if something fails?
}

void Game::GameRun()
{
	this->keyboard->PollKeyboard();

	Render();
	if(this->keyboard->KeyDown(DIK_UP))
		this->soSample->playLooping();
	if(this->keyboard->KeyDown(DIK_DOWN))
		this->soSample->stop();
	if(this->keyboard->KeyDown(DIK_LEFT))
		this->soSample->fadeOut();
	if(this->keyboard->KeyDown(DIK_RIGHT))
		soSample->fadeIn();
	if(this->keyboard->KeyDown(DIK_M))
		soSample->mute();
	if(this->keyboard->KeyDown(DIK_U))
		soSample->unmute();
	if(this->keyboard->KeyDown(DIK_ESCAPE))
		PostMessage(this->hWnd, WM_DESTROY, 0, 0);

	soSample->process();
}

void Game::GameEnd()
{
	delete this->soSample;
	delete this->keyboard;
}