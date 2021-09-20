#pragma once
#include "AppIncl.h"
#include <DirectXMath.h>
#include "Geometry.h"

struct Transform {
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 translation;
};

class Actor {
public:
	Actor() = default;
	~Actor() = default;

	void SetID(UINT val);
	UINT GetID();
	void SetActive(bool state);
	bool GetActive();

	DirectX::XMMATRIX GetWorldMatrix();
	
public:
	virtual void OnSpawn();		//Called when the actor is spawned.
	virtual void OnDespawn();	//Called when the actor is despawned
	virtual void Awake();		//Called when the actor activates.
	virtual void Sleep();		//Called when the actor deactivates.
	virtual void Update();		//Called once per frame.

protected:
	const char* name;
	UINT id;
	bool isActive;

	Transform mTransform = {
			{1, 1, 1},	//Scaling
			{0, 0, 0},	//Rotation
			{0, 0, 0}, 	//Translation
	};
	
	DirectX::XMFLOAT4X4 mWorldMatrix;
};

class Dummy : Actor {
	void Update();
};