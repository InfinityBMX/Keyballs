#include "input.h"
//#include "../util/debug.h"
//#include "../main/includes.h"

LPDIRECTINPUT8 dInputObject;
LPDIRECTINPUTDEVICE8 diMouse;
//LPDIRECTINPUTDEVICE8 diKeyboard;
LPDIRECTINPUTDEVICE8 diJoystick;
DIMOUSESTATE mouseState;

//keyboard input
//char keys[256];

int InitDirectInput(HWND hWnd)
{
	//initialize DirectInput object
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**) &dInputObject,
		NULL);

	if(result != DI_OK)
		return 0;

	//intitialize the mouse
	result = dInputObject->CreateDevice(GUID_SysMouse, &diMouse, NULL);
	if(result != DI_OK)
		return 0;

	//initialize the keyboard
	//result = dInputObject->CreateDevice(GUID_SysKeyboard, &diKeyboard, NULL);
	//if(result != DI_OK)
	//	return 0;

	//clean return
	return 1;
}

int InitMouse(HWND hWnd)
{
	//set the data format for mouse input
	HRESULT result = diMouse->SetDataFormat(&c_dfDIMouse);
	if(result != DI_OK)
		return 0;

	//set the cooperation level
	//this will also hide the mouse pointer
	result = diMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if(result != DI_OK)
		return 0;

	//acquire the mouse
	result = diMouse->Acquire();
	if(result != DI_OK)
		return 0;

	//all okay
	return 1;
}

int MouseX()
{
	return mouseState.lX;
}

int MouseY()
{
	return mouseState.lY;
}

int MouseButton(int button)
{
	return BUTTON_DOWN(mouseState, button);
}

void PollMouse()
{
	diMouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);
}

void KillMouse()
{
	if(diMouse != NULL)
	{
		diMouse->Unacquire();
		diMouse->Release();
		diMouse = NULL;
	}
}

int Keyboard::InitKeyboard(HWND hWnd)
{
	HRESULT result = dInputObject->CreateDevice(GUID_SysKeyboard, &this->keyboard, NULL);
	if(result != DI_OK)
		return 0;

	//set the data format for keyboard input
	result = this->keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(result != DI_OK)
		return 0;

	//set the cooperative level
	result = this->keyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if(result != DI_OK)
		return 0;

	//acquire the keyboard
	result = this->keyboard->Acquire();
	if(result != DI_OK)
		return 0;

	//all good
	return 1;
}

void Keyboard::PollKeyboard()
{
	keyboard->GetDeviceState(sizeof(keys), (LPVOID)&this->keys);
}

int Keyboard::KeyDown(int key)
{
	return (this->keys[key] & 0x80);
}

void Keyboard::KillKeyboard()
{
	if(keyboard != NULL)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = NULL;
	}
}

Keyboard::Keyboard(HWND hWnd)
{
	if(dInputObject == NULL){
		// Do stuff when no DI object initialized
	}
	this->InitKeyboard(hWnd);
}