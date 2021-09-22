#pragma once
#include "AppIncl.h"
#include <DirectXMath.h>
#include <string>
//#include "Geometry.h"

struct Transform {
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 translation;
};

class Entity {
public:
	Entity() = default;
	~Entity() = default;

	void SetID(UINT val);
	UINT GetID();
	void SetActive(bool state);
	bool IsActive();

	DirectX::XMMATRIX GetWorldMatrix();
	
public:
	virtual void Init();		//Called when the actor is spawned.
	virtual void Destroy();		//Called when the actor is despawned
	virtual void Awake();		//Called when the actor activates.
	virtual void Sleep();		//Called when the actor deactivates.
	virtual void Update(float dt);		//Called once per frame.

protected:
	std::string name;
	UINT id;
	UINT priority;
	bool bIsActive;

	Transform mTransform = {
			{1, 1, 1},	//Scaling
			{0, 0, 0},	//Rotation
			{0, 0, 0}, 	//Translation
	};
	
	DirectX::XMFLOAT4X4 mWorldMatrix;
};

class DebugEntity : public Entity {
public:
	void Init() override;
};