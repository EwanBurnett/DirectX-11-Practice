#include "Graphics.h"
#include <d3dcompiler.h>
#include <string>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace DirectX;

Graphics::Graphics()
{

}

bool Graphics::Init(HWND hWnd, UINT width, UINT height)
{
	//init window bounds
	mClientWidth = width;
	mClientHeight = height;

	//Create D3D11 Device Interface
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
		nullptr,
		&pImmContext
	);

	if (FAILED(hr)) {
		MessageBox(0, "D3D11 Device Creation Failed", 0, 0);
		return false;
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
	dxgiFactory->CreateSwapChain(dxgiDevice.Get(), &sd, &pSwapChain);


	//Creating render target view
	wrl::ComPtr<ID3D11Texture2D> backBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
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

	//Setting the Viewport
	D3D11_VIEWPORT vp = { };
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(mClientWidth);
	vp.Height = static_cast<float>(mClientHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	pImmContext->RSSetViewports(1, &vp);

	//InitGeoBuffers();
	InitShaders();

	return true;
}

void Graphics::Update(float dt)
{
	InitGeoBuffers();

	//Update matrices (TODO)
	mWorldMatrix = mWorldMatrix;
	mViewMatrix = mViewMatrix;
	mProjMatrix = mProjMatrix;
}

void Graphics::Draw()
{
	//Binding view to Output Merger
	pImmContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr); //mDepthStencilView.Get()

	pImmContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pImmContext->DrawIndexed(mIndexCount, 0, 0);
	pSwapChain->Present(1u, 0);
}

void Graphics::Clear(float r, float g, float b, float a)
{
	pImmContext->ClearDepthStencilView(mDepthStencilView.Get(), 0, 1, 0);
	const float colour[] = { r, g, b, a };
	pImmContext->ClearRenderTargetView(mRenderTargetView.Get(), colour);
}

void Graphics::SetWireframeMode(bool mode)
{
	mIsWireframeView = mode;

	//Rasterizer Settings
	D3D11_RASTERIZER_DESC rdSolid;
	ZeroMemory(&rdSolid, sizeof(D3D11_RASTERIZER_DESC));
	rdSolid.FillMode = D3D11_FILL_SOLID;
	rdSolid.CullMode = D3D11_CULL_BACK;
	rdSolid.FrontCounterClockwise = false;
	rdSolid.DepthBias = 0;
	rdSolid.DepthBiasClamp = 0;
	rdSolid.SlopeScaledDepthBias = 0.0f;
	rdSolid.DepthClipEnable = true;
	rdSolid.ScissorEnable = false;
	rdSolid.MultisampleEnable = false;
	rdSolid.AntialiasedLineEnable = false;

	D3D11_RASTERIZER_DESC rdWire;
	ZeroMemory(&rdWire, sizeof(D3D11_RASTERIZER_DESC));
	rdWire.FillMode = D3D11_FILL_WIREFRAME;
	rdWire.CullMode = D3D11_CULL_BACK;
	rdWire.FrontCounterClockwise = false;
	rdWire.DepthBias = 0;
	rdWire.DepthBiasClamp = 0;
	rdWire.SlopeScaledDepthBias = 0.0f;
	rdWire.DepthClipEnable = true;
	rdWire.ScissorEnable = false;
	rdWire.MultisampleEnable = false;
	rdWire.AntialiasedLineEnable = false;

	wrl::ComPtr<ID3D11RasterizerState> mRS;
	if (mIsWireframeView) {
		pDevice->CreateRasterizerState(&rdWire, &mRS);
	}
	else {
		pDevice->CreateRasterizerState(&rdSolid, &mRS);
	}
	pImmContext->RSSetState(mRS.Get());
}

void Graphics::InitGeoBuffers()
{
	Vertex verts[] =
	{
		{XMFLOAT3(0, 0.5, 0), XMFLOAT4(1, 0, 0, 1)},
		{XMFLOAT3(0.5, -0.5, 0), XMFLOAT4(0, 1, 0, 1)},
		{XMFLOAT3(-0.5, -0.5, 0), XMFLOAT4(0, 0, 1, 1)}
	};


	//Fill Vertex buffer description
	D3D11_BUFFER_DESC vbd = { 0 };
	vbd.ByteWidth = sizeof(verts);
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = sizeof(Vertex);

	//Initialize the subresource with our verts
	D3D11_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = verts;

	//Create the vertex buffer
	wrl::ComPtr<ID3D11Buffer> mVB;
	pDevice->CreateBuffer(&vbd, &vInitData, &mVB);

	//Bind the vertex buffer to an input slot of the device
	UINT strides = sizeof(Vertex);
	UINT offset = 0;
	pImmContext->IASetVertexBuffers(0, 1, mVB.GetAddressOf(), &strides, &offset);

	OutputDebugString(">> Vertex Buffer Bound to pipeline\n");
	
	//Creating an Index Buffer
	
	OutputDebugString(">> Creating Index Buffer\n");

	//Index list for our Hexagon
	UINT indices[3] = {
		0, 1, 2
	};

	//Creating an index buffer Description
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * std::size(indices);
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

}

void Graphics::InitShaders()
{
	//Blob for our Shaders
	wrl::ComPtr<ID3DBlob> pBlob;

	//Loading our pixel shader, and binding it to the pipeline
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);

	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	pImmContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	//Loading our vertex shader, and binding it to the pipeline
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	
	//Fill out vertex desc with the semantics and formats used in our vertex struct
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//Set the input layout on the device
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	pDevice->CreateInputLayout(vertexDesc, (UINT)std::size(vertexDesc), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	pImmContext->IASetInputLayout(pInputLayout.Get());

	//Bind the vertex shader
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	pImmContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
}
