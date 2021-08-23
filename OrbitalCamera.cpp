#include "OrbitalCamera.h"

OrbitalCamera::OrbitalCamera()
{
	//Initialize
	mRadius = 0.0f;
	mPhi = 0.0f;
	mTheta = 0.0f;

	XMStoreFloat3(&mPosition, DirectX::XMVectorZero());
	XMStoreFloat3(&mTarget, DirectX::XMVectorZero());
	XMStoreFloat3(&mWorldUp, DirectX::XMVectorSet(0, 1, 0, 1));
}

DirectX::XMMATRIX OrbitalCamera::GetCameraView()
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&mPosition);
	DirectX::XMVECTOR tgt = DirectX::XMLoadFloat3(&mTarget);
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&mWorldUp);

	DirectX::XMStoreFloat4x4(&mViewMatrix, DirectX::XMMatrixLookAtLH(pos, tgt, up));
	return DirectX::XMLoadFloat4x4(&mViewMatrix);
}

void OrbitalCamera::Orbit(float radius, float theta, float phi)
{
	mRadius = radius;
	mPhi = phi;
	mTheta = theta;

	mPosition.x = mRadius * sinf(mPhi) * cosf(mTheta);
	mPosition.y = mRadius * cosf(mPhi);
	mPosition.z = mRadius * sinf(mPhi) * cosf(mTheta);

}
