#include "Graphics.h"
#pragma comment(lib,"d3d11.lib")

Graphics::Graphics(HWND hWnd, UINT width, UINT height)
{
	mClientWidth = width;
	mClientHeight = height;
	mEnable4xMSAA = true;

	//D3D Device Creation
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;

	/*
	HRESULT hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG,
		0,
		0,
		D3D11_SDK_VERSION,
		&md3dDevice,
		&featureLevel,
		&md3dImmediateContext
	);

	if (FAILED(hr)) {
		MessageBox(0, "D3D11 Device Creation Failed", 0, 0);
		// return false;
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, "DirectX 11 not supported by current hardware.", 0, 0);
		// return false;
	}
	*/

	D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG,
		0,
		0,
		D3D11_SDK_VERSION,
		&md3dDevice,
		&featureLevel,
		&md3dImmediateContext
	);

	//Checking 4x Multisampling Antialiasing support
	UINT m4xMSAAQuality;
	md3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMSAAQuality);
	if (m4xMSAAQuality <= 0) {
		MessageBox(0, "4x MSAA not supported.", 0, 0);
	}

	//Initialising the Swap Chain
	DXGI_MODE_DESC bd = { 0 }; //Buffer Desc 
	bd.Width = mClientWidth;
	bd.Height = mClientHeight;
	bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bd.RefreshRate.Numerator = 60;
	bd.RefreshRate.Denominator = 1;
	bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc = bd;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	if (mEnable4xMSAA) {
		sd.SampleDesc.Quality = m4xMSAAQuality - 1;
		sd.SampleDesc.Count = 4;
	}
	else {
		sd.SampleDesc.Quality = 1;
		sd.SampleDesc.Count = 0;
	}
	//Creating the Swapchain
	IDXGIDevice* dxgiDevice = 0;
	md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	//IDXGISwapChain* mSwapChain;
	dxgiFactory->CreateSwapChain(dxgiDevice, &sd, &pSwap);

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();


	//Creating render target view
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* backBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	
	backBuffer->Release();

	//Creating Depth / Stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (mEnable4xMSAA) {
		depthStencilDesc.SampleDesc.Quality = m4xMSAAQuality - 1;
		depthStencilDesc.SampleDesc.Count = 4;
	}
	else {
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.SampleDesc.Count = 1;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;

	md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	//Binding views to Output Merger
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	//Setting the Viewport
	D3D11_VIEWPORT vp = { 0 };
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(mClientWidth);
	vp.Height = static_cast<float>(mClientHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &vp);
	pSwap->Present(1,0);
}

void Graphics::Update()
{
	pSwap->Present(0, 0);
}
