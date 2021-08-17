#include "VectorMath.h"
using namespace std;

void VectorDemo() {
	//Demo the whole program
	cout << "Vector Demo" << endl;

	//addition and subtraction
	Vector2 a = { 2.5, 8.3 };
	Vector2 b = { -7.9, 3.1 };
	cout << "\n1. Vector Addition\nVector Addition and Subtraction work as you'd expect, in a componentwise fashion." << endl;
	cout << "so a + b = a.xyz + b.xyz - and Subtraction is just the negation of addition." << endl;
	cout << "\tVector a = " << a << endl;
	cout << "\tVector b = " << b << endl;
	cout << "\ta + b = " << a + b << endl;
	cout << "\ta - b = " << a - b << endl;

	//Vector length 
	Vector3 m = { 10.4, -22.7, 16.9 };
	cout << "\n2. Vector Length\nModulus (or vector length) is denoted by ||v|| where v is the vector." << endl;
	cout << "It applies Pythagoras' theorem to work out the Magnitude of the vector." << endl;
	cout << "\tVector m = " << m << endl;
	cout << "\t||m|| = " << VectorLength(m) << endl;

	//Vector Dot
	Vector3 x = { -4.5, -7.1, 0 };
	Vector3 y = { 10.4, -22.7, 16.9 };
	cout << "\n3. Vector Dot Product\nVector algebra can use the dot product to calculate a scalar value based on the vector." << endl;
	cout << "The dot product of two vectors is the cosine of the angle between both vectors, scaled by their magnitudes." << endl;
	cout << "\tVector x = " << x << endl;
	cout << "\tVector y = " << y << endl;
	cout << "\t x . y = " << VectorDot(x, y) << endl;

	//Vector Cross
	Vector3 u = { 12.3, -1.1, 5.2 };
	Vector3 v = { 3.1, 8.2, -2.2 };
	cout << "\n3. Vector Cross Product\nThe Cross Product is a 3D vector only form of multiplication, which returns a new vector." << endl;
	cout << "This new vector, w, where u x v = w, is orthoganal (perpendicular) to both u and v. " << endl;
	cout << "\tVector u = " << u << endl;
	cout << "\tVector v = " << v << endl;
	cout << "\t u x v = " << VectorCross(u, v) << endl;

	//Vector Normalization
	Vector4 n = { 4.4, 6.2, 1.7, 3.5 };
	cout << "\n4. Vector Normalization\nNormalizing a vector scales its components down by its magnitude, mapping them to a range of 0 to 1." << endl;
	cout << "\tVector n = " << n << endl;
	cout << "\tNormalize(n) = " << VectorNormalize(n) << endl;

	//Vector Projection
	Vector2 p = { 12.3, 15.6 };
	Vector2 g = { 4.3, 8.1 };
	cout << "\n5. Vector Projection\nProjection calculates how much of a vector acts in the direction of another. Think of it like shining a light onto the vector." << endl;
	cout << "Proj v (n) = ((v . n) / ||n||^2)n " << endl;
	cout << "Vector p = " << p << endl;
	cout << "Vector g = " << g << endl;
	cout << "Proj p (g) = " << VectorProject(p, g) << endl;

	cout << "\n\n";
}


//Overload the << operators to be able to output 2, 3 and 4 dimentional Vector objects
std::ostream& operator<< (std::ostream& os, Vector2 v) {
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

std::ostream& operator<< (std::ostream& os, Vector3 v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

std::ostream& operator<< (std::ostream& os, Vector4 v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return os;
}


Vector2& operator+ (Vector2& v, Vector2& b) {
	v.x += b.x;
	v.y += b.y;
	return v;
}

Vector3& operator+ (Vector3& v, Vector3& b) {
	v.x += b.x;
	v.y += b.y;
	v.z += b.z;
	return v;
}

Vector4& operator+ (Vector4& v, Vector4& b) {
	v.x += b.x;
	v.y += b.y;
	v.z += b.z;
	v.w += b.w;
	return v;
}

Vector2& operator- (Vector2& v, Vector2& b) {
	v.x -= b.x;
	v.y -= b.y;
	return v;
}

Vector3& operator- (Vector3& v, Vector3& b) {
	v.x -= b.x;
	v.y -= b.y;
	v.z -= b.z;
	return v;
}

Vector4& operator- (Vector4& v, Vector4& b) {
	v.x -= b.x;
	v.y -= b.y;
	v.z -= b.z;
	v.w -= b.w;
	return v;
}

Vector2& operator* (Vector2& v, float s) {
	v.x *= s;
	v.y *= s;
	return v;
}
Vector3& operator* (Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}
Vector4& operator* (Vector4& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	v.w *= s;
	return v;
}


Vector2& operator- (Vector2& v) {
	v.x *= -1;
	v.y *= -1;
	return v;
}

Vector3& operator- (Vector3& v) {
	v.x *= -1;
	v.y *= -1;
	v.z *= -1;

	return v;
}

Vector4& operator- (Vector4& v) {
	v.x *= -1;
	v.y *= -1;
	v.z *= -1;
	v.w *= -1;
	return v;
}

Vector2& operator/ (Vector2& v, float s) {
	v.x /= s;
	v.y /= s;
	return v;
}

Vector3& operator/ (Vector3& v, float s) {
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return v;
}

Vector4& operator/ (Vector4& v, float s) {
	v.x /= s;
	v.y /= s;
	v.z /= s;
	v.w /= s;
	return v;
}

Vector2 VectorAdd(Vector2 a, Vector2 b) {
	Vector2 out;

	out.x = a.x + b.x;
	out.y = a.y + b.y;

	return out;
}

Vector3 VectorAdd(Vector3 a, Vector3 b) {
	Vector3 out;
	out.x = a.x + b.x;
	out.y = a.y + b.y;
	out.z = a.z + b.z;

	return out;
}

Vector4 VectorAdd(Vector4 a, Vector4 b) {
	Vector4 out;
	out.x = a.x + b.x;
	out.y = a.y + b.y;
	out.z = a.z + b.z;
	out.w = a.w + b.w;

	return out;
}

float VectorDot(Vector2 a, Vector2 b) {
	float out = (a.x * b.x) + (a.y * b.y);
	return out;
}

float VectorDot(Vector3 a, Vector3 b) {
	float out = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return out;
}

float VectorDot(Vector4 a, Vector4 b) {
	float out = (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
	return out;
}

float VectorLength(Vector2 a) {

	return sqrt((a.x * a.x) + (a.y * a.y));
}

float VectorLength(Vector3 a) {

	return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

float VectorLength(Vector4 a) {

	return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z) + (a.w * a.w));
}

Vector3 VectorCross(Vector3 a, Vector3 b) {
	Vector3 out;

	out.x = (a.y * b.z) - (a.z * b.y);
	out.y = (a.z * b.x) - (a.x * b.z);
	out.z = (a.x * b.y) - (a.y * b.x);

	return out;
}

Vector2 VectorNormalize(Vector2 a)
{
	float mod = VectorLength(a);
	return a / mod;
}

Vector3 VectorNormalize(Vector3 a)
{
	float mod = VectorLength(a);
	return a / mod;
}

Vector4 VectorNormalize(Vector4 a)
{
	float mod = VectorLength(a);
	return a / mod;
}

Vector2 VectorProject(Vector2 a, Vector2 b)
{
	Vector2 unit = VectorNormalize(b);
	float scale = VectorDot(a, unit);
	Vector2 out = unit * scale;
	return out;
}

//Vector3 VectorProject(Vector3 a, Vector3 b)
//{
//	float cacheA[3] = { 0, 0, 0 };
//
//	for (int i = 0; i < 2; i++) {
//		cache[i] = 
//	}
//
//	Vector3 unit = VectorNormalize(b);
//	float scale = VectorDot(a, unit);
//	return unit * scale;
//}





