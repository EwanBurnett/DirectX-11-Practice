#pragma once
#include "AppIncl.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <math.h>

class OrbitalCamera {
public:
	OrbitalCamera();
	~OrbitalCamera() = default;

	DirectX::XMMATRIX GetCameraView();
	void Orbit(float radius, float theta, float phi);
private:
	float mOrbitSpeed = 0.25f;	//Speed of the Camera 

	float mRadius;		//Camera distance from target
	float mPhi;			//Camera orbital pitch
	float mTheta;		//Camera orbital roll

	float mRoll;		//Camera rotation Roll
	float mPitch;		//Camera rotation Pitch
	float mYaw;			//Camera rotation Yaw

	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mTarget;
	DirectX::XMFLOAT3 mWorldUp;

	DirectX::XMFLOAT4X4 mViewMatrix;
};