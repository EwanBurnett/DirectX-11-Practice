#include "Graphics.h"
#include <d3dcompiler.h>
#include <string>
#include "Entity.h"
#include <d3dx11effect.h>
//#include <d3dx11global.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace DirectX;

Graphics::Graphics()
{
	mFoV = 90.0f;
	mNearPlane = 0.1f;
	mFarPlane = 10.0f;
	mTimeScale = 1.0f;
	mTranslation.x = 1.0f;
	
	XMStoreFloat3(&mCameraPos, XMVectorSet(1.0f, 0, XMConvertToRadians(15.0f), 0));
	XMStoreFloat3(&mScale, XMVectorReplicate(1.0f));

	//Init matrices
	XMFLOAT4X4 I;
	XMStoreFloat4x4(&I, XMMatrixIdentity());
	mWorldMatrix = I;
	mViewMatrix = I;
	mProjMatrix = I;

	mClientHeight = 0;
	mClientWidth = 0;
}

bool Graphics::Init(HWND hWnd, UINT width, UINT height)
{
	//init window bounds
	mClientWidth = width;
	mClientHeight = height;
	mAspectRatio = static_cast<float>(width / height);

	

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

	//OutputDebugString(">> Creating Swapchain\n");
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
	pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer);


	//Create Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC mDSVdesc = {};
	mDSVdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	mDSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	mDSVdesc.Texture2D.MipSlice = 0;
	mDSVdesc.Flags = 0;

	pDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &mDSVdesc, &mDepthStencilView);


	//Setting the Viewport
	D3D11_VIEWPORT vp = { };
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(mClientWidth);
	vp.Height = static_cast<float>(mClientHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	pImmContext->RSSetViewports(1, &vp);

	//init GUI
	mGui = std::make_unique<GUI>();
	ImGui_ImplDX11_Init(pDevice.Get(), pImmContext.Get());

	//InitGeoBuffers();
	InitShaders();

	return true;
}

void Graphics::Update(float dt)
{
	InitGeoBuffers();
	
	static double angle;
	angle += 0.1 * mTimeScale * dt;
	if (angle >= 360) { 
		angle = 0.0f; 
	}

	if (bAutoMode) { mRotation.y = angle; };

	XMVECTOR scale = XMLoadFloat3(&mScale);
	XMVECTOR rotation = XMLoadFloat3(&mRotation);
	XMVECTOR translation = XMLoadFloat3(&mTranslation);

	XMStoreFloat4x4(&mWorldMatrix, 	//W = SRT
		XMMatrixScalingFromVector(scale)	*
		XMMatrixRotationRollPitchYawFromVector(rotation)	*
		XMMatrixTranslationFromVector(translation)
	);

	mArcBall.Orbit(mCameraPos.x, mCameraPos.y, mCameraPos.z);

	XMStoreFloat4x4(&mViewMatrix, mArcBall.GetCameraView());

	XMStoreFloat4x4(&mProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(mFoV), mAspectRatio, mNearPlane, mFarPlane));

	InitConstBuffers();
}

void Graphics::Draw()
{
	//Binding view to Output Merger
	pImmContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr); //mDepthStencilView.Get()

	pImmContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pImmContext->DrawIndexed(mIndexCount, 0, 0);

	CreateGUI();

	pSwapChain->Present(1u, 0);
}



void Graphics::Clear(float r, float g, float b, float a){
	const float colour[] = { r, g, b, a };
	pImmContext->ClearRenderTargetView(mRenderTargetView.Get(), colour);
	pImmContext->ClearDepthStencilView(mDepthStencilView.Get(), 0, 1, 0);
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

void Graphics::CreateGUI()
{
	static float vScale[3] = { mScale.x, mScale.y, mScale.z };
	static float vRotation[3] = { mRotation.x, mRotation.y, mRotation.z };
	static float vTranslation[3] = { mTranslation.x, mTranslation.y, mTranslation.z };

	static float vWorldUp[3] = { 0, 1, 0 };
	static float vTarget[3] = { 0, 0, 0 };
	static float vCamPos[3] = { 0, 0, -2 };

	static float vColor[3] = { 0, 0, 0 };
	
	static float vNear = mNearPlane;
	static float vFar = mFarPlane;
	static int a = mIndexCount;

	if ((mFarPlane - mNearPlane) <= 1.0f) {
		mFarPlane += 1.0f;
	}
	ImGui::NewFrame();
	
	//Main Demo Panel
	ImGui::Begin("DX11 Box Demo");

	ImGui::Text("Box Demo by Ewan Burnett (2021)\nUse the controls to modify the position, orientation and scale of the box.\nYou can also adjust the other values to see how they affect the scene.");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::BulletText("Hold down any key to show a wireframe view.");

	ImGui::SliderFloat3("Object Scale", vScale, 0.1, 20);
	ImGui::SliderFloat3("Object Rotation", vRotation, 0, 360);
	ImGui::SliderFloat3("Object Translation", vTranslation, -20, 20);

	ImGui::SliderFloat("Near Plane", &mNearPlane, 0.1f, 99.0f);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("The Closest a rendered object can be to the Camera");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	
	ImGui::SliderFloat("Far Plane", &mFarPlane, 1.0f, 100.0f);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("The Furthest a rendered object can be from the Camera.");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::SliderFloat("Field of View", &mFoV, 30.0f, 110.0f);

	if (ImGui::Button("Toggle Auto Rotate")) {
		if (bAutoMode) { bAutoMode = false; }
		else { bAutoMode = true; }
	}
	ImGui::SliderFloat("Timescale", &mTimeScale, 0.1f, 100.0f);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("The \'speed\' of the application.");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::SliderInt("Indices", &a, 0, 36);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted("How many Indices are drawn. Modify this to see how DX11 constructs the box.");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	ImGui::End();

	//Camera Controls Panel
	ImGui::Begin("Camera Controls");
	ImGui::SliderFloat("Camera Distance", &mCameraPos.x, 0.1f, 20.0f);
	ImGui::SliderAngle("Camera Orbit", &mCameraPos.y, -180.0f, 180.0f);
	ImGui::SliderAngle("Camera Angle", &mCameraPos.z, -90.0f, 90.0f);

	ImGui::SliderFloat3("Camera Position", vCamPos, -10, 10);
	ImGui::SliderFloat3("Camera Target", vTarget, -10, 10);
	ImGui::SliderFloat3("Camera Up", vWorldUp, 0, 1);

	ImGui::ColorPicker3("Background Colour", vColor);
	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	mScale = { vScale[0], vScale[1], vScale[2] };
	mRotation = { XMConvertToRadians( vRotation[0]), XMConvertToRadians(vRotation[1]), XMConvertToRadians(vRotation[2]) };
	mTranslation = { vTranslation[0], vTranslation[1], vTranslation[2] };
	XMVECTOR tgt = XMVectorSet(vTarget[0], vTarget[1], vTarget[2], 1.0f);
	mArcBall.SetTargetPos(tgt);
	XMVECTOR up = XMVectorSet(vWorldUp[0], vWorldUp[1], vWorldUp[2], 1.0f);
	mArcBall.SetWorldUp(up);
	XMVECTOR pos = XMVectorSet(vCamPos[0], vCamPos[1], vCamPos[2], 1.0f);
	mArcBall.SetCameraPos(pos);
	mIndexCount = a;
	mClearColor = { vColor[0], vColor[1], vColor[2] };
}

void Graphics::InitGeoBuffers()
{
	/*Actor a;
	a.mGeometry = Geometry::Cube;
	Vertex *verts = &a.mGeometry.vertices[0];*/
	//Declare geometry vertices in local space
	Vertex verts[] =
	{
		//Cube
		{XMFLOAT3(-0.5, -0.75,	-0.5),		XMFLOAT4(1, 1, 1, 1)},		//White		0
		{XMFLOAT3(0.5,	-0.75,	-0.5),		XMFLOAT4(1, 1, 0, 1)},		//Yellow	1
		{XMFLOAT3(0.5,	-0.75,	0.5),		XMFLOAT4(1, 0, 1, 1)},		//Purple	2
		{XMFLOAT3(-0.5,	-0.75,	0.5),		XMFLOAT4(0, 1, 1, 1)},		//Turquoise	3
		{XMFLOAT3(-0.5, 0.75,	-0.5),		XMFLOAT4(1, 0, 0, 1)},		//Red		4
		{XMFLOAT3(0.5,	0.75,	-0.5),		XMFLOAT4(0, 1, 0, 1)},		//Green		5
		{XMFLOAT3(0.5,	0.75,	0.5),		XMFLOAT4(0, 0, 1, 1)},		//Blue		6
		{XMFLOAT3(-0.5,	0.75,	0.5),		XMFLOAT4(0, 0, 0, 1)},		//Black		7
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


	//Creating an Index Buffer

	//Index list for our shape
	UINT indices[] = {
		0, 1, 2,	0, 2, 3, //Bottom Face
		0, 4, 5,	0, 5, 1, //Back Face
		1, 5, 6,	1, 6, 2, //Right Face
		2, 6, 7,	2, 7, 3, //Front Face
		3, 7, 4,	3, 4, 0, //Left Face
		4, 6, 5,	4, 7, 6, //Top Face
	};

	
	//UINT *indices = &a.mGeometry.indices[0];
	

	//mIndexCount = sizeof(indices);

	//Creating an index buffer Description
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(indices) * 8;
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
	ID3DX11Effect* pEffect = nullptr;
	//Loading our pixel shader, and binding it to the pipeline
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	//D3DX11CompileEffectFromFile(L"Basic.fx", null, null, null, D3DCOMPILE_DEBUG, pDevice.GetAddressOf(), &pEffect, pBlob)
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

void Graphics::InitConstBuffers()
{
	struct ConstantBuffer {
		XMMATRIX mWorldView;
	};

	ConstantBuffer mCB = { };
	XMMATRIX w = XMLoadFloat4x4(&mWorldMatrix);
	XMMATRIX v = XMLoadFloat4x4(&mViewMatrix);
	XMMATRIX p = XMLoadFloat4x4(&mProjMatrix);

	mCB.mWorldView = XMMatrixTranspose(w * v * p);

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;

	D3D11_BUFFER_DESC mCBD = { 0 };
	mCBD.Usage = D3D11_USAGE_DYNAMIC;
	mCBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mCBD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mCBD.ByteWidth = sizeof(mCB);
	mCBD.MiscFlags = 0;
	mCBD.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA mCBData;
	mCBData.pSysMem = &mCB;

	pDevice->CreateBuffer(&mCBD, &mCBData, &pConstantBuffer);

	pImmContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

}
