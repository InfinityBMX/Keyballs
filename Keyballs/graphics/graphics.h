#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include <d3d10.h>
#include <d3dx10.h>
#include "printableObject.h"

//SoundObject Class
class Graphics
{
public:
	//Methods
	Graphics(HWND hWnd);
	Graphics(HWND hWnd, int width, int height, int fullscreen);
	~Graphics();
	int InitGraphics();
	void Render();
	void Render(PrintableObject volume);
	void Render(LPWSTR text);

private:
	//Data Members
	ID3D10Device* pD3DDevice;
	IDXGISwapChain* pSwapChain;
	ID3D10RenderTargetView* pRenderTargetView;
	ID3DX10Font* pFont;
	HWND hWnd;
	int width;
	int height;
	int fullscreen;

	//Private Methods
	void ProcessFonts();
	void GetFontRectangle(LPCWSTR text, RECT *rect);
};

#endif