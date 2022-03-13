#pragma once
#include "AppIncl.h"
#include <DirectXMath.h>
#include <vector>

struct Vertex {
	DirectX::XMFLOAT3 position;
	//DirectX::XMFLOAT4 color;
	//FLOAT normal;
	//TODO: Add TexCoords etc.,
};



struct GeometryData {
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
};


namespace Geometry {

	GeometryData Quad{

		//Cube Verts
		{
			{DirectX::XMFLOAT3(-0.5,	-0.5,	0)},	//v0
			{DirectX::XMFLOAT3(-0.5,	0.5,	0)},	//v1
			{DirectX::XMFLOAT3(0.5,		0.5,	0)},	//v2
			{DirectX::XMFLOAT3(-0.5,	0.5,	0)},	//v3
		},

		//Quad Indices
		{
			0, 1, 2,	
			0, 2, 3
		}

	};

	GeometryData Cube{

		//Cube Verts
		{
			{DirectX::XMFLOAT3(-0.5,	-0.5,	-0.5)},	//v0
			{DirectX::XMFLOAT3(0.5,		-0.5,	-0.5)},	//v1
			{DirectX::XMFLOAT3(0.5,		-0.5,	0.5)},	//v2
			{DirectX::XMFLOAT3(-0.5,	-0.5,	0.5)},	//v3
			{DirectX::XMFLOAT3(-0.5,	0.5,	-0.5)},	//v4
			{DirectX::XMFLOAT3(0.5,		0.5,	-0.5)},	//v5
			{DirectX::XMFLOAT3(0.5,		0.5,	0.5)},	//v6
			{DirectX::XMFLOAT3(-0.5,	0.5,	0.5)}	//v7
		},

		//Cube Indices
		{
			0, 1, 2,	0, 2, 3, //Bottom Face
			0, 4, 5,	0, 5, 1, //Back Face
			1, 5, 6,	1, 6, 2, //Right Face
			2, 6, 7,	2, 7, 3, //Front Face
			3, 7, 4,	3, 4, 0, //Left Face
			4, 6, 5,	4, 7, 6, //Top Face
		}

	};
	
}






