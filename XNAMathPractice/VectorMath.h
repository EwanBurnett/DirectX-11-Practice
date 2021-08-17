#pragma once
#include <iostream>

//Vector Structs
struct Vector2 {
	float x, y;
	void zero() {
		x = 0;
		y = 0;
	}
};

struct Vector3
{
	float x, y, z;
	void zero() {
		x = 0;
		y = 0;
		z = 0;
	}
};

struct Vector4 {
	float x, y, z, w;
	void zero() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
};

//Operator Overloads
std::ostream& operator<< (std::ostream& os, Vector2 v);
std::ostream& operator<< (std::ostream& os, Vector3 v);
std::ostream& operator<< (std::ostream& os, Vector4 v);
Vector2& operator+ (Vector2& v, Vector2& b);
Vector3& operator+ (Vector3& v, Vector3& b);
Vector4& operator+ (Vector4& v, Vector4& b);
Vector2& operator- (Vector2& v, Vector2& b);
Vector3& operator- (Vector3& v, Vector3& b);
Vector4& operator- (Vector4& v, Vector4& b);
Vector2& operator* (Vector2& v, float s);
Vector3& operator* (Vector3& v, float s);
Vector4& operator* (Vector4& v, float s);
Vector2& operator- (Vector2& v);
Vector3& operator- (Vector3& v);
Vector4& operator- (Vector4& v);
Vector2& operator/ (Vector2& v, float s);
Vector3& operator/ (Vector3& v, float s);
Vector4& operator/ (Vector4& v, float s);

//Vector Methods
void VectorDemo();

Vector2 VectorAdd(Vector2 a, Vector2 b);
Vector3 VectorAdd(Vector3 a, Vector3 b);
Vector4 VectorAdd(Vector4 a, Vector4 b);

float VectorDot(Vector2 a, Vector2 b);
float VectorDot(Vector3 a, Vector3 b);
float VectorDot(Vector4 a, Vector4 b);
float VectorLength(Vector2 a);
float VectorLength(Vector3 a);
float VectorLength(Vector4 a);
Vector3 VectorCross(Vector3 a, Vector3 b);
Vector2 VectorNormalize(Vector2 a);
Vector3 VectorNormalize(Vector3 a);
Vector4 VectorNormalize(Vector4 a);
Vector2 VectorProject(Vector2 a, Vector2 b);
//Vector3 VectorProject(Vector3 a);


