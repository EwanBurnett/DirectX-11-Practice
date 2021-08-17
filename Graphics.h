#pragma once
#include "KnightWin.h"
#include <d3d11.h>

class Graphics {
public:
	Graphics(HWND hWnd, UINT width, UINT height);
	void Update();

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;

	UINT mClientWidth;
	UINT mClientHeight;

	BOOL mEnable4xMSAA;
};

