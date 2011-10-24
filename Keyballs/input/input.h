#ifndef _INPUT_H
#define _INPUT_H

#include <dinput.h>

//function prototypes
int InitDirectInput(HWND hWnd);
int InitKeyboard(HWND hWnd);
void PollKeyboard();
int KeyDown(int key);
void KillKeyboard();
int InitMouse(HWND hWnd);
void PollMouse();
int MouseButton(int button);
int MouseX();
int MouseY();
void KillMouse();

//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dInputObject;
extern LPDIRECTINPUTDEVICE8 diMouse;
extern LPDIRECTINPUTDEVICE8 diKeyboard;
extern DIMOUSESTATE mouseState;

#endif