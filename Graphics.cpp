#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace DirectX;


Graphics::Graphics(HWND hWnd, UINT width, UINT height)
{
	OutputDebugString(">> Initializing Graphics...\n");

	mClientWidth = width;
	mClientHeight = height;
	mEnable4xMSAA = true;

	//D3D Device Creation
	D3D_FEATURE_LEVEL featureLevel;

	OutputDebugString(">> Creating D3D11 Device\n");

	UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0,
		0,
		D3D11_SDK_VERSION,
		&pDevice,
		&featureLevel,
		&pImmContext
	);

	if (FAILED(hr)) {
		MessageBox(0, "D3D11 Device Creation Failed", 0, 0);
		return;
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, "DirectX 11 not supported by current hardware.", 0, 0);
		return;
	}


	//Checking 4x Multisampling Antialiasing support
	UINT m4xMSAAQuality;
	pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMSAAQuality);
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
	wrl::ComPtr<IDXGIDevice> dxgiDevice = 0;
	pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	wrl::ComPtr<IDXGIAdapter> dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	wrl::ComPtr<IDXGIFactory> dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	OutputDebugString(">> Creating Swapchain\n");
	dxgiFactory->CreateSwapChain(dxgiDevice.Get(), &sd, &pSwap);


	//Creating render target view
	wrl::ComPtr<ID3D11Texture2D> backBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
	pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &mRenderTargetView);


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

	wrl::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
	
	pDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	pDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), 0, &mDepthStencilView);
	
	//Binding views to Output Merger
	OutputDebugString(">> Binding to Output Merger\n");
	pImmContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	
	//Setting the Viewport
	D3D11_VIEWPORT vp = { 0 };
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(mClientWidth);
	vp.Height = static_cast<float>(mClientHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	pImmContext->RSSetViewports(1, &vp);
	
	//Run any initialization code
	Init();

	OutputDebugString(">> Graphics initialization complete\n");
}


void Graphics::Init()
{
	OutputDebugString(">> Creating Vertex Buffer\n");

	//Creating a Vertex Buffer
	//Vertex struct 
	struct Vertex1 {
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};
	
	//Raw vertex data (Hexagon)
	Vertex1 verts[] =
	{
		{XMFLOAT3(0, 0, 1), XMFLOAT4(0.8, 0.5, 0.5, 1)},
		{XMFLOAT3(0.5, 1, 1), XMFLOAT4(0.2, 0.7, 0.3, 1)},
		{XMFLOAT3(1, 0, 1), XMFLOAT4(0.4, 0.1, 0.9, 1)},
		{XMFLOAT3(0.5, -1, 1), XMFLOAT4(0.6, 0.3, 0.1, 1)},
		{XMFLOAT3(-0.5, -1, 1), XMFLOAT4(0.9, 0.4, 0.4, 1)},
		{XMFLOAT3(-1, 0, 1), XMFLOAT4(0.3, 0.5, 0.5, 1)},
		{XMFLOAT3(-0.5, 0, 1), XMFLOAT4(0.3, 0.8, 0.55, 1)}
	};

	//Fill out vertex desc with the semantics and formats used in our vertex struct
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//Fill Vertex buffer description
	D3D11_BUFFER_DESC vbd = { 0 };
	vbd.ByteWidth = sizeof(verts);
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = sizeof(Vertex1);

	//Initialize the subresource with our verts
	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = verts;

	//Create the vertex buffer
	wrl::ComPtr<ID3D11Buffer> mVB;
	pDevice->CreateBuffer(&vbd, &vInitData, &mVB);

	//Bind the vertex buffer to an input slot of the device
	UINT strides = sizeof(Vertex1);
	UINT offset = 0;
	pImmContext->IASetVertexBuffers(0, 1, mVB.GetAddressOf(), &strides, &offset);

	OutputDebugString(">> Vertex Buffer Bound to pipeline\n");
	
	//Creating an Index Buffer
	
	OutputDebugString(">> Creating Index Buffer\n");

	//Index list for our Hexagon
	UINT indices[18] = {
		0, 1, 2,	//Triangle 0
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 1		//Triangle 6
	};

	//Creating an index buffer Description
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * 18;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	//Initializse the index buffer with our data
	D3D11_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = indices;

	//Create the buffer
	wrl::ComPtr<ID3D11Buffer> mIB;
	pDevice->CreateBuffer(&ibd, &iInitData, &mIB);

	//Bind the buffer to the pipeline
	pImmContext->IASetIndexBuffer(mIB.Get(), DXGI_FORMAT_R32_UINT, 0);

	OutputDebugString(">> Index Buffer Bound to pipeline\n");
	//pImmContext->DrawIndexed(18, 0, 0);


	//Rasterizer Settings
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = false;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = false;
	rd.AntialiasedLineEnable = false;


	wrl::ComPtr<ID3D11RasterizerState> mRS;
	pDevice->CreateRasterizerState(&rd, &mRS);
	pImmContext->RSSetState(mRS.Get());

	//Shaders
	wrl::ComPtr<ID3DBlob> pVsBlob;
	wrl::ComPtr<ID3DBlob> pPsBlob;

	////Compile our shaders
	//D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, 0, "cs_5_0", D3DCOMPILE_DEBUG, 0, &pPsBlob, nullptr);
	//D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, 0, "cs_5_0", D3DCOMPILE_DEBUG, 0, &pVsBlob, nullptr);
		
	//Loading our pixel shader, and binding it to the pipeline
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pPsBlob);

	pDevice->CreatePixelShader(pPsBlob->GetBufferPointer(), pPsBlob->GetBufferSize(), nullptr, &pPixelShader);
	pImmContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	//Loading our vertex shader, and binding it to the pipeline
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pVsBlob);
	

	//Set the input layout on the device
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	pDevice->CreateInputLayout(vertexDesc, 2, pVsBlob->GetBufferPointer(), pVsBlob->GetBufferSize(), &pInputLayout);
	pImmContext->IASetInputLayout(pInputLayout.Get());

	//Bind the vertex shader
	pDevice->CreateVertexShader(pVsBlob->GetBufferPointer(), pVsBlob->GetBufferSize(), nullptr, &pVertexShader);
	pImmContext->VSSetShader(pVertexShader.Get(), nullptr, 0);


	
	//Set primitive topology 
	pImmContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	//pImmContext->Draw(18, 0);
}

void Graphics::DrawFrame()
{
	Init();
	pImmContext->Draw(6, 0);
	//pImmContext->DrawIndexed(18, 0, 0);
}

void Graphics::EndFrame()
{
	DrawFrame();
	pSwap->Present(1u, 0);
}

void Graphics::ClearBuffer(float r, float g, float b, float a)
{
	const float colour[] = { r, g, b, a };
	//const float colour[] = { 0, 0, 0 ,1 };
	pImmContext->ClearRenderTargetView(mRenderTargetView.Get(), colour);
}
