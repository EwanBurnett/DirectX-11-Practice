#include "OrbitalCamera.h"

OrbitalCamera::OrbitalCamera()
{
	//Initialize
	mRadius = 0.0f;
	mPhi = 0.0f;
	mTheta = 0.0f;

	XMStoreFloat3(&mPosition, DirectX::XMVectorSet(0, 0, 5, 1));
	XMStoreFloat3(&mTarget, DirectX::XMVectorZero());
	XMStoreFloat3(&mWorldUp, DirectX::XMVectorSet(0, 1, 0, 1));
}

DirectX::XMMATRIX OrbitalCamera::GetCameraView()
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&mPosition);
	DirectX::XMVECTOR tgt = DirectX::XMLoadFloat3(&mTarget);
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&mWorldUp);

	DirectX::XMMATRIX v = DirectX::XMMatrixLookAtLH(pos, tgt, up);
	DirectX::XMMATRIX y = DirectX::XMMatrixRotationRollPitchYaw(mPitch, -mYaw, mRoll);
	DirectX::XMStoreFloat4x4(&mViewMatrix, 
		v * 
		y);

	return DirectX::XMLoadFloat4x4(&mViewMatrix);
}

void OrbitalCamera::Orbit(float radius, float theta, float phi)
{
	if (radius <= 0) { radius = 0.1f; }
	mRadius = radius;
	mPhi = phi;
	mTheta = theta;

	//mPosition.x = mRadius * sinf(mPhi) * cosf(mTheta);
	//mPosition.y = mRadius * cosf(mPhi);
	//mPosition.z = mRadius * sinf(mPhi) * cosf(mTheta);

	
	DirectX::XMVECTOR pos =
		DirectX::XMVector3Transform(
			DirectX::XMVectorSet(0.0f, 0.0f, -mRadius, 1.0f),
			DirectX::XMMatrixRotationRollPitchYaw(mPhi, -mTheta, 0.0f)
		);

	DirectX::XMStoreFloat3(&mPosition, pos);
}
