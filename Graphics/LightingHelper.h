#pragma once
#include "../AppIncl.h"
#include <DirectXMath.h>

using namespace DirectX;

struct Material {
	Material() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Reflect;
};

//Directional Lights shine in parallel over the whole scene.
struct DirectionalLight {
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//How much ambient light is emitted
	XMFLOAT4 Diffuse;	//The Diffuse light emitted by this source. (w = for blending)
	XMFLOAT4 Specular;	//The amount of specular light that is emitted

	XMFLOAT3 Direction;	//Which direction the light is facing.

	float padding;	//Padding for arrays of lights
};

//Point Lights shine in a radius about their position.
struct PointLight {
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//How much ambient light is emitted
	XMFLOAT4 Diffuse;	//The Diffuse light emitted by this source. (w = for blending)
	XMFLOAT4 Specular;	//The amount of specular light that is emitted

	//Packing into a 4D vector - Position XYZ, Range W
	XMFLOAT3 Position;	//The Position of the light in world space.
	float Range;		//The radius in which the light emits.

	//Packing into a 4D vector - (A0, A1, A2, Pad)
	XMFLOAT3 Attenuation;
	float padding;

};

struct SpotLight {
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;	//How much ambient light is emitted
	XMFLOAT4 Diffuse;	//The Diffuse light emitted by this source. (w = for blending)
	XMFLOAT4 Specular;	//The amount of specular light that is emitted

	//Packing into a 4D vector - Position XYZ, Range W
	XMFLOAT3 Position;	//The Position of the light in world space.
	float Range;		//The radius in which the light emits.

	//Packing into a 4D vector - Direction XYZ, Spot W
	XMFLOAT3 Directon;
	float Spot;

	//Packing into a 4D vector - (A0, A1, A2, Pad)
	XMFLOAT3 Attenuation;
	float padding;
};