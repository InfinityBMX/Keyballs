//Include the Windows header file, needed for all Windows applications
#include "includes.h"

HINSTANCE hInst; // global handle to hold the application instance
HWND wndHandle; // global variable to hold the window handle
int width = 640;
int height = 480;
// Direct3D global vars
ID3D10Device* pD3DDevice = NULL;
IDXGISwapChain* pSwapChain = NULL;
ID3D10RenderTargetView* pRenderTargetView = NULL;
// DirectSound global vars
LPDIRECTSOUND8 directSoundDevice;
LPDIRECTSOUNDBUFFER directSoundBuffer = NULL;

// forward declarations
bool InitWindow( HINSTANCE hInstance, int width, int height );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();
bool InitDirect3D(HWND hWnd, int width, int height);

// This is winmain, the main entry point for Windows applications
int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// Initialize the window
	if(!InitWindow(hInstance, width, height)){return false;}

	// Initialize the D3D Device
	if(!InitDirect3D(wndHandle, width, height)){return false;}

	// Initialize the D3D Device
	HRESULT hr = DirectSoundCreate8(NULL, &directSoundDevice, NULL);
	if(directSoundDevice != NULL)
	{
		hr = directSoundDevice->SetCooperativeLevel(wndHandle, DSSCL_PRIORITY);
		if FAILED(hr){return false;}
	}

	WAVEFORMATEX wfx;
	DSBUFFERDESC dsbd;
	CWaveFile *waveFile = new CWaveFile();

	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));

	wfx.wFormatTag = (WORD) WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 44100;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
	wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);

	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = 0;
	dsbd.dwBufferBytes = 1330000;
	dsbd.guid3DAlgorithm = GUID_NULL;
	dsbd.lpwfxFormat = &wfx;

	hr = directSoundDevice->CreateSoundBuffer(&dsbd, &directSoundBuffer, NULL);
	if FAILED(hr){return false;}
	
	waveFile->Open(L"sample.wav", NULL, WAVEFILE_READ);
	if(waveFile->GetSize() == 0){return false;}

	VOID* buffer = NULL;
	DWORD bufferSize = 0;
	hr = directSoundBuffer->Lock(0,waveFile->GetSize(),&buffer,&bufferSize,NULL,NULL,DSBLOCK_ENTIREBUFFER);
	if FAILED(hr){return false;}

	DWORD bytesRead = 0;
	waveFile->ResetFile();
	hr = waveFile->Read((BYTE*) buffer, bufferSize, &bytesRead);
	if FAILED(hr){return false;}

	hr = directSoundBuffer->Unlock(&buffer,bufferSize,NULL,NULL);

	//TCHAR temp[20]={0};
	//wsprintf(temp,TEXT("Bytes = %u"),bytesRead);
	//MessageBox(NULL,temp, TEXT("message"),MB_OK);

	directSoundBuffer->Play(0,0,DSBPLAY_LOOPING);

	// main message loop:
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Call the render function
		Render();
	}
	return (int) msg.wParam;
}

/***********************************************************
* Init Window
* Inits and creates the main app window
* Inputs - application instance - HINSTANCE
			Window width - int
			Windo height - int
* Outputs - true if successful, false if failed - bool
************************************************************/
bool InitWindow( HINSTANCE hInstance, int width, int height )
{
	WNDCLASSEX wcex;

	// Fill in the WNDCLASSEX structure. This describes how the window will look to the system
	wcex.cbSize = sizeof(WNDCLASSEX); // the size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW; // the class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; // the window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this instance
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = 0; // icon to associate with the application
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW); // the default cursor to use
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName = NULL; // the resource name for the menu
	wcex.lpszClassName = TEXT( "DirectXExample" ); // the class name being created
	wcex.hIconSm = 0; // the handle to the small icon
	RegisterClassEx(&wcex);
	//Resize the window
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// create the window from the class above
	wndHandle = CreateWindow( TEXT("DirectXExample"),
					TEXT("DirectXExample"),
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					rect.right - rect.left,
					rect.bottom - rect.top,
					NULL,
					NULL,
					hInstance,
					NULL);

	if( !wndHandle )
	{
		return false;
	}

	// Display the window on the screen
	ShowWindow( wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);

	return true;
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
	switch( message)
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
			PostQuitMessage(0);
		break;
	}

	// Always reutnr the message to the default window procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/**********************************************************
* InitDirect3D
* Initializes Direct3D
* Inputs - Parent window handle - HWND,
			Window width - int
			Window height - int
*Outputs - true if successful, false if failed - bool
***********************************************************/
bool InitDirect3D(HWND hWnd, int width, int height)
{
	// Create the clear the DXGI_SWAP_CHAIN_DESC structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Fill in the needed values
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	//Create the D3D device and the swap chain
	HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL,
					D3D10_DRIVER_TYPE_REFERENCE,
					NULL,
					0,
					D3D10_SDK_VERSION,
					&swapChainDesc,
					&pSwapChain,
					&pD3DDevice);
	// Error checking. Make sure the device was created
	if(hr != S_OK)
	{
		return false;
	}

	// Get the back buffer from the swapchain
	ID3D10Texture2D *pBackBuffer;
	hr = pSwapChain ->  GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*) &pBackBuffer);
	if(hr != S_OK)
	{
		return false;
	}

	// Create the render target view
	hr = pD3DDevice -> CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	// Release the back buffer
	pBackBuffer -> Release();

	// Make sure the render target view was created successfully
	if(hr != S_OK)
	{
		return false;
	}

	// Set the render target
	pD3DDevice -> OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// Create and set the viewport
	D3D10_VIEWPORT viewPort;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pD3DDevice -> RSSetViewports(1, &viewPort);

	return true;
}

/*************************************************
* Render
* All drawing happens in the Render function
* Inputs - void
* Outputs - void
***************************************************/
void Render()
{
	if(pD3DDevice != NULL)
	{
		// Clear the target buffer
		pD3DDevice -> ClearRenderTargetView(pRenderTargetView, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		// All drawing will go here

		// Display the next item in the swap chain
		pSwapChain -> Present(0, 0);
	}
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