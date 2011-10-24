#include "../main/includes.h"

//variable declarations
ID3D10Device* pD3DDevice = NULL;
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