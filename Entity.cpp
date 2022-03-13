#include "Entity.h"

using namespace DirectX;
void Entity::SetID(UINT val)
{
	id = val;
}
UINT Entity::GetID()
{
	return id;
}
void Entity::SetActive(bool state)
{
	bIsActive = state;
}
bool Entity::IsActive()
{
	return bIsActive;
}
DirectX::XMMATRIX Entity::GetWorldMatrix()
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

void Entity::Init()
{
}

void Entity::Destroy()
{
}

void Entity::Awake()
{
}

void Entity::Sleep()
{
}

void Entity::Update(float dt)
{
}

void DebugEntity::Init() {
	name = "DEBUG";
	priority = 2;
	OutputDebugString("Debug Entity Init Complete!");
}
