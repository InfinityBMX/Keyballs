#ifndef _INPUT_H
#define _INPUT_H

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

#define BUTTON_DOWN(obj, button) (obj.rgbButtons[button] & 0x80)

//function prototypes
int InitDirectInput(HWND hWnd);
//int InitKeyboard(HWND hWnd);
//void PollKeyboard();
//int KeyDown(int key);
//void KillKeyboard();
int InitMouse(HWND hWnd);
void PollMouse();
int MouseButton(int button);
int MouseX();
int MouseY();
void KillMouse();

//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dInputObject;
extern LPDIRECTINPUTDEVICE8 diMouse;
//extern LPDIRECTINPUTDEVICE8 diKeyboard;
extern DIMOUSESTATE mouseState;

//SoundObject Class
class Keyboard
{
public:
	//Methods
	Keyboard();
	Keyboard(HWND hWnd);
	void PollKeyboard();
	int KeyDown(int key);

private:
	//Data Members
	char keys[256];
	LPDIRECTINPUTDEVICE8 keyboard;

	//Private Methods
	int InitKeyboard(HWND hWnd);
	void KillKeyboard();
};

#endif