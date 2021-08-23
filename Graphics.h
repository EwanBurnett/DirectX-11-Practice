#pragma once
#include "AppIncl.h"
#include "GfxErr.h"
#include "OrbitalCamera.h"
#include "AppGUI.h"
#include "imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <memory>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <effects.h>
#include <wrl.h>

namespace wrl = Microsoft::WRL;

class Graphics {
public:
	Graphics();
	~Graphics() = default;

	bool Init(HWND hWnd, UINT width, UINT height);
	void Update(float dt);
	void Draw();
	void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void SetWireframeMode(bool mode);

	wrl::ComPtr<ID3D11Buffer> mVertexBuffer;
	wrl::ComPtr<ID3D11Buffer> mIndexBuffer;

private:
	void InitGeoBuffers();
	void InitShaders();
	void InitConstBuffers();

	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

private:
	wrl::ComPtr<ID3D11Device> pDevice;
	wrl::ComPtr<IDXGISwapChain> pSwapChain;
	wrl::ComPtr<ID3D11DeviceContext> pImmContext;

	wrl::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

	OrbitalCamera mArcBall;
	std::unique_ptr<GUI> mGui;

	DirectX::XMFLOAT4X4 mWorldMatrix;
	DirectX::XMFLOAT4X4 mViewMatrix;
	DirectX::XMFLOAT4X4 mProjMatrix;

	UINT mIndexCount = 12;

	BOOL mIsWireframeView;

	UINT mClientWidth;
	UINT mClientHeight;
	FLOAT mAspectRatio;

	BOOL mEnable4xMSAA = true;
	UINT m4xMSAAQuality;
};

