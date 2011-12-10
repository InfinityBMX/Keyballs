#ifndef GAME_H
#define GAME_H
#define APPTITLE L"KeyBalls"
#define FULLSCREEN 0
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#include "../audio/audio.h"
#include "../input/input.h"
#include "../util/debug.h"
#include "../graphics/graphics.h"

class Game
{
public:
	//Methods
	Game(HWND hWnd);
	void GameRun();
	void GameEnd();

private:
	//Data Members
	Keyboard* keyboard;
	SoundObject* soSample;
	HWND hWnd;
};

#endif