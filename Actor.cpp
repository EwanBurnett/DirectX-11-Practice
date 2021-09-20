#include "Actor.h"

using namespace DirectX;
void Actor::SetID(UINT val)
{
	id = val;
}
UINT Actor::GetID()
{
	return id;
}
void Actor::SetActive(bool state)
{
	isActive = state;
}
bool Actor::GetActive()
{
	return isActive;
}
DirectX::XMMATRIX Actor::GetWorldMatrix()
{
	XMVECTOR scale = XMLoadFloat3(&mTransform.scale);
	XMVECTOR rotation = XMLoadFloat3(&mTransform.rotation);
	XMVECTOR translation = XMLoadFloat3(&mTransform.translation);
	
	XMMATRIX world = (	//Calculate the world matrix
		XMMatrixScalingFromVector(scale) *
		XMMatrixRotationRollPitchYawFromVector(rotation) *
		XMMatrixTranslationFromVector(translation)
		);

	XMStoreFloat4x4(&mWorldMatrix, world);
	
	return world;
}

void Dummy::Update() {

}