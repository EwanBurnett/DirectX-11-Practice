#pragma once
#include "KnightWin.h"
#include <d3d11.h>

class Graphics {
public:
	Graphics(HWND hWnd, UINT width, UINT height);
	~Graphics();

	void EndFrame();
	void ClearBuffer(float r, float g, float b, float a = 1.0f);

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pImmContext = nullptr;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;

	UINT mClientWidth;
	UINT mClientHeight;

	BOOL mEnable4xMSAA;
};
