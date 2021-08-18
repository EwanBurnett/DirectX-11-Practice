#include "XMVectorMath.h"

using namespace DirectX;
using namespace std;

// Overload the "<<" operators so that we can use cout to
// output XMVECTOR objects.
ostream& operator<<(ostream& os, FXMVECTOR v)
{
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);
	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}

void XMVectorMathDemo()
{
	//XMVector Demo
	cout << "XNA Vector Demo" << endl;

	cout << "XNAMath vectors support all standard vector arithmetic functions (e.g. dot / cross product, projection, Normalization, etc.,)" << endl;
	cout << "While using XNAMath, there are a number of key conventions that must be followed." << endl;
	cout << "Specifically, when it comes to Vectors:" << endl;
	cout << "\t1. Use the type XMVECTOR for storing local or global variables" << endl;
	cout << "\t2. Use the types XMFLOAT2, XMFLOAT3 and XMFLOAT4 for storing class member variables" << endl;
	cout << "\t3. Use Loading functions to convert from an XMFLOAT type to XMVECTOR before performing any calculations" << endl;
	cout << "\t4. Do Calculations with instances of XMVECTOR" << endl;
	cout << "\t5. Use Storage functions to convert back from XMVECTOR to an XMFLOAT type" << endl;

	XMFLOAT3 a = { 15, 6.3, -22 };
	XMVECTOR x1 = XMVectorSet(4, 6, 2, 1);
	XMVECTOR x2 = XMVectorSet(-8.7, 9.6, 2.23, 0.5);

	LoadAndStore(a);
	FunctionDemo(x1, x2);
}

void LoadAndStore(XMFLOAT3 v)
{
	cout << "\nLoading and Storage Demo" << endl;
	XMVECTOR cache = XMVectorZero();
	cache = XMLoadFloat3(&v);
	cout << "\tLoading" << cache << "into XMVECTOR type object." << endl;
	cout << "\t//Do Maths with the XMVECTOR" << endl;
	cache = XMVector3Normalize(cache);
	cache = XMVectorScale(cache, 3.5);
	cout << "\tCalculated Vector = " << cache << endl;
	cout << "\tStoring result back into XMFLOAT3 object." << endl;
	XMStoreFloat3(&v, cache);
	cout << "\tXMFloat3 object = (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
}

void FunctionDemo(DirectX::FXMVECTOR x, DirectX::FXMVECTOR y)
{
	cout << "\nFunction Demo" << endl;
	cout << "Specialized Functions" << endl;

	XMVECTOR t = XMVectorZero();
	cout << "\tXMVectorZero = " << t << endl;
	t = XMVectorSplatOne();
	cout << "\tXMVectorSplatOne = " << t << endl;
	t = XMVectorSet(1.5, 2.5, 2.2, -3.5);
	cout << "\tXMVectorSet(x, y, z, w) = " << t << endl;
	t = XMVectorSplatZ(t);
	cout << "\tXMVectorSplatZ(v) = " << t << endl;
	t = XMVectorReplicate(6.2845);
	cout << "\tXMVectorReplicate(n) = " << t << endl;
}
