#ifndef _GRAPHICS_H
#define _GRAPHICS_H

//function prototypes
int InitDirect3D(HWND, int, int, int);
void Render();

//variable declarations
extern ID3D10Device* pD3DDevice;
extern IDXGISwapChain* pSwapChain;
extern ID3D10RenderTargetView* pRenderTargetView;

#endif