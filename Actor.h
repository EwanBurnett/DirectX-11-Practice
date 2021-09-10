#pragma once
#include "AppIncl.h"
#include <DirectXMath.h>
#include "Geometry.h"

struct Transform {
	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
};

class Actor {
public:
	Actor() = default;
	~Actor() = default;

	const char* name;
	Transform mTransform = {
			{0, 0, 0}, 	//Translation
			{0, 0, 0},	//Rotation
			{1, 1, 1}	//Scaling
	};
	GeometryData mGeometry;
};