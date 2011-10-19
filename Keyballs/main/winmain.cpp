//Include the Windows header file, needed for all Windows applications
#include "../main/includes.h"

// DirectInput global vars
LPDIRECTINPUT8 directInputObject;

// forward declarations
ATOM RegWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
void ShutdownDirect3D();

// This is winmain, the main entry point for Windows applications
int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	MSG msg;
	HWND hWnd;

	//register the class
	RegWindow(hInstance);

	//set up the screen in windowed or fullscreen mode
	DWORD style;
	if(FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPED;

	// create the window
	hWnd = CreateWindow( 
			APPTITLE,				//window class
			APPTITLE,				//title bar
			WS_OVERLAPPEDWINDOW,	//window style
			CW_USEDEFAULT,			//x position of window
			CW_USEDEFAULT,			//y position of window
			SCREEN_WIDTH,			//width of the window
			SCREEN_HEIGHT,			//height of the window
			NULL,					//parent window
			NULL,					//menu
			hInstance,				//application instance
			NULL);					//window parameters

	if(!hWnd)
		return FALSE;

	// Display the window on the screen
	ShowWindow( hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// Initialize the Direct Input Device
	if FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInputObject, NULL)){debug(L"Failed");return false;}

	// Initialize the game
	if(!InitGame(hWnd))
	{
		MessageBox(hWnd, L"Error initializing the game", L"Error", MB_OK);
		return 0;
	}

	// main message loop:
	int done = 0;
	while(!done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//look for quit message
			if(msg.message == WM_QUIT)
				done = 1;

			//decode and pass messages to WndProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			//process game loop
			GameRun(hWnd);
	}
	return (int) msg.wParam;
}

/***********************************************************
* Reg Window
* Registers the window class
* Inputs - application instance - HINSTANCE
* Outputs - ATOM?
************************************************************/
ATOM RegWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	// Fill in the WNDCLASSEX structure. This describes how the window will look to the system
	wcex.cbSize = sizeof(WNDCLASSEX); // the size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW; // the class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; // the window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this instance
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = NULL; // icon to associate with the application
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW); // the default cursor to use
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName = NULL; // the resource name for the menu
	wcex.lpszClassName = APPTITLE; // the class name being created
	wcex.hIconSm = 0; // the handle to the small icon
	return RegisterClassEx(&wcex);
}

/*******************************************************
* WndProc
* The main window procedure for the application
* Inputs - application window handle - HWND
			message sent to the window - UINT
			wParam of the message being sent - WPARAM
			lParam of the message being sent - LPARAM
* Outputs - LRESULT
*********************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check for any available messages from the queue
	switch(message)
	{
		// Allow the user to press the Escape key to end the application
		case WM_KEYDOWN:
			switch(wParam)
			{
				// Check if the user hit the Escape key
				case VK_ESCAPE:
					PostQuitMessage(0);
				break;
			}
		break;

		// The user hit the close button, close the application
		case WM_DESTROY:
			ShutdownDirect3D();
			PostQuitMessage(0);
		break;
	}

	// Always return the message to the default window procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*******************************************************
* ShutdownDirect3D
* Closes down and releases the resources for Direct3D
* Inputs - void
* Outputs - void
********************************************************/
void ShutdownDirect3D()
{
	// Release the rendertarget
	if(pRenderTargetView)
	{
		pRenderTargetView -> Release();
	}

	// Release the swapchain
	if(pSwapChain)
	{
		pSwapChain -> Release();
	}

	// Release the D3D device
	if(pD3DDevice)
	{
		pD3DDevice -> Release();
	}
}