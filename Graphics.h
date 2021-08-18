#pragma once
#include "KnightWin.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <effects.h>
#include <wrl.h>

namespace wrl = Microsoft::WRL;

class Graphics {
public:
	Graphics(HWND hWnd, UINT width, UINT height);
	~Graphics() = default;

	void Init();
	void DrawFrame();
	void EndFrame();
	void ClearBuffer(float r, float g, float b, float a = 1.0f);

private:
	wrl::ComPtr<ID3D11Device> pDevice = nullptr;
	wrl::ComPtr<IDXGISwapChain> pSwap = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> pImmContext = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	//ID3D11RenderTargetView* mRenderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	UINT mClientWidth;
	UINT mClientHeight;

	BOOL mEnable4xMSAA;
};

