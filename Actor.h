#pragma once
#include "AppIncl.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

class Actor {
public:
	Actor();

	virtual void Init();			//Called when Actor is constucted
	virtual void Awake();			//Called when Actor is Activated
	virtual void Update(float dt);	//Called once per frame

	virtual void GetRenderable();

	bool IsActive();

private:
	bool bIsActive;		//Whether the Actor should be updated or not
	int ID;				//This actor's UID - unique per instance

	Transform transform;	//Represents the Actor's position, rotation and scaling within the world
	Geometry geometry;		//Class encapsulating the Actor's model data. Set data members in Init().
};

struct Transform {
	DirectX::XMFLOAT3 scale;		//Actor Scale
	DirectX::XMFLOAT3 rotation;		//Actor Rotation (rads)
	DirectX::XMFLOAT3 translation;	//Actor Translation
};

struct Material {
	//TODO: Add Lighting Values
	DirectX::XMFLOAT4 color;
};

struct Vertex {
	DirectX::XMFLOAT3 position;
	Material material;
};

class Geometry {
	std::vector<Vertex> verts;
	std::vector<int> indices;

	int indexCount;
};

