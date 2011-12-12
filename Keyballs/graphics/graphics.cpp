#include "graphics.h"

Graphics::Graphics(HWND hWnd){
	this->hWnd = hWnd;
	this->width = 640;
	this->height = 480;
	this->fullscreen = false;
	this->pD3DDevice = NULL;
	this->pRenderTargetView = NULL;
	this->pSwapChain = NULL;
	this->pFont = NULL;
	this->InitGraphics();	
}

Graphics::Graphics(HWND hWnd, int width, int height, int fullscreen){
	this->hWnd = hWnd;
	this->width = width;
	this->height = height;
	this->fullscreen = fullscreen;
	if(!(this->InitGraphics()))
		true;//error handling here
}

Graphics::~Graphics(){
	this->pSwapChain->Release();
	this->pRenderTargetView->Release();
	this->pD3DDevice->Release();
}

int Graphics::InitGraphics(){
	// Create the clear the DXGI_SWAP_CHAIN_DESC structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Fill in the needed values
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = this->width;
	swapChainDesc.BufferDesc.Height = this->height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = this->hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !(this->fullscreen);

	//Create the D3D device and the swap chain
	HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL,
					D3D10_DRIVER_TYPE_HARDWARE,
					NULL,
					0,
					D3D10_SDK_VERSION,
					&swapChainDesc,
					&this->pSwapChain,
					&this->pD3DDevice);
	// Error checking. Make sure the device was created
	if(hr != S_OK)
	{
		return FALSE;
	}

	// Get the back buffer from the swapchain
	ID3D10Texture2D *pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*) &pBackBuffer);
	if(hr != S_OK)
	{
		return FALSE;
	}

	// Create the render target view
	hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	// Release the back buffer
	pBackBuffer -> Release();

	// Make sure the render target view was created successfully
	if(hr != S_OK)
	{
		return FALSE;
	}

	// Set the render target
	pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// Create and set the viewport
	D3D10_VIEWPORT viewPort;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pD3DDevice -> RSSetViewports(1, &viewPort);

	hr=D3DX10CreateFont(this->pD3DDevice,
							35,				//height
							0,				//width
							FW_MEDIUM,		//weight
							1,
							FALSE,
							DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,
							DEFAULT_QUALITY,
							DEFAULT_PITCH | FF_DONTCARE,
							L"Helvetica",
							&this->pFont);

	if(FAILED(hr))
	{
		return FALSE;
	}
	return TRUE;
}

void Graphics::Render(){
	if(this->pD3DDevice != NULL)
	{
		// Clear the target buffer
		this->pD3DDevice->ClearRenderTargetView(this->pRenderTargetView, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		// All drawing will go here

		// Display the next item in the swap chain
		this->pSwapChain -> Present(0, 0);
	}
}

void Graphics::Render(LPWSTR text){
	if(this->pD3DDevice != NULL)
	{
		// Clear the target buffer
		this->pD3DDevice->ClearRenderTargetView(this->pRenderTargetView, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		// All drawing will go here
		//this->ProcessFonts();
		if(true)
		{
			RECT rc;
			SetRectEmpty(&rc);
			this->GetFontRectangle(text, &rc);
			pFont->DrawText( NULL,
								text,
								-1,
								&rc,
								DT_LEFT,
								D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
		}

		// Display the next item in the swap chain
		this->pSwapChain -> Present(0, 0);
	}
}

void Graphics::Render(PrintableObject volume){
	if(this->pD3DDevice != NULL)
	{
		// Clear the target buffer
		this->pD3DDevice->ClearRenderTargetView(this->pRenderTargetView, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		// All drawing will go here
		//this->ProcessFonts();
		if(volume.GetDisplay())
		{
			RECT rc;
			SetRectEmpty(&rc);
			this->GetFontRectangle(volume.GetText(), &rc);
			pFont->DrawText( NULL,
								volume.GetText(),
								-1,
								&rc,
								DT_LEFT,
								D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
		}

		// Display the next item in the swap chain
		this->pSwapChain -> Present(0, 0);
	}
}

void Graphics::GetFontRectangle(LPCWSTR text, RECT *rect)
{
	this->pFont->DrawText(NULL,
							text,
							-1,
							rect,
							DT_CALCRECT | DT_LEFT,
							D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//#include "../main/includes.h"

//variable declarations
/*ID3D10Device* pD3DDevice = NULL;
IDXGISwapChain* pSwapChain = NULL;
ID3D10RenderTargetView* pRenderTargetView = NULL;

int InitDirect3D(HWND hWnd, int width, int height, int fullscreen)
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
	swapChainDesc.Windowed = !fullscreen;

	//Create the D3D device and the swap chain
	HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL,
					D3D10_DRIVER_TYPE_HARDWARE,
					NULL,
					0,
					D3D10_SDK_VERSION,
					&swapChainDesc,
					&pSwapChain,
					&pD3DDevice);
	// Error checking. Make sure the device was created
	if(hr != S_OK)
	{
		return FALSE;
	}

	// Get the back buffer from the swapchain
	ID3D10Texture2D *pBackBuffer;
	hr = pSwapChain ->  GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*) &pBackBuffer);
	if(hr != S_OK)
	{
		return FALSE;
	}

	// Create the render target view
	hr = pD3DDevice -> CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	// Release the back buffer
	pBackBuffer -> Release();

	// Make sure the render target view was created successfully
	if(hr != S_OK)
	{
		return FALSE;
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

	return TRUE;
}

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
*/
