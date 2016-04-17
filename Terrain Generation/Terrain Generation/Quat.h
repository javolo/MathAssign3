/****************************************************************************

Class Quaternion to implement all the functions related with that

-----------------------------------------------------------------------

01/02/2016	David Barbera
			Javier de la Osa

****************************************************************************/


#pragma once
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Matrices.h"
#include "Vectors.h"

#define PI 3.14159265


class Quat {

public:

	// Quaternion elements.
	float X; 
	float Y;
	float Z;
	float W; 

	/////////////////////////////// Definition of Headers ////////////////////////

	// 1. Default Constructor
	Quat() : W(1.0f), X(0.0f), Y(0.0f), Z(0.0f)  {};

	// 2. Constructor with four components
	Quat(float w, float x, float y, float z) : W(w), X(x), Y(y), Z(z)  { }

	// 2. Constructor which converts a matrix to a quaternion
	Quat(const Matrix4& mat);

	// 3. Constructor which converts euler angles (radians) to a quaternion
	Quat(float x, float y, float z);

	// 4. Constructor which converts euler angles (radians) to a quaternion
	Quat(const Vector3& vec);

	// 5. Assignment operator (quaternion = quaternion)
	inline Quat& operator=(const Quat& other);

	// 6. Add operator
	inline Quat operator+(const Quat& other) const;

	// 7. Multiplication operator
	inline Quat operator*(const Quat& other) const;

	// 8. Sets new quaternion based on euler angles (radians)
	inline Quat& set(float x, float y, float z);

	// 9. Creates a matrix from this quaternion
	inline Matrix4 getMatrix() const;

	// 10. Conver Euler Angles to Quaternions
	Quat& eulerAnglesToQuaternion(const Vector3& vec);


	/////////////////////////////// Definition of Headers ////////////////////////
};

// 2. Constructor which converts a matrix to a quaternion
inline Quat::Quat(const Matrix4& mat)
{
	(*this) = mat;
}

// 3. Constructor which converts euler angles to a quaternion from three numbers
inline Quat::Quat(float x, float y, float z)
{
	set(x, y, z);
}


// 4. Constructor which converts euler angles to a quaternion from a Vector3
inline Quat::Quat(const Vector3& vec)
{
	set(vec.x, vec.y, vec.z);
}

// 5. Assignment operator (quaternion = quaternion)
inline Quat& Quat::operator=(const Quat& other)
{
	W = other.W;
	X = other.X;
	Y = other.Y;
	Z = other.Z;
	
	return *this;
}

// 6. Add operator
inline Quat Quat::operator+(const Quat& b) const
{
	Quat tmp;

	tmp.W = W + b.W;
	tmp.X = X + b.X;
	tmp.Y = Y + b.Y;
	tmp.Z = Z + b.Z;

	return tmp;
}

// 7. Multiplication operator
inline Quat Quat::operator*(const Quat& other) const
{
	Quat tmp;

	tmp.W = (W * other.W) - (X * other.X) - (Y * other.Y) - (Z * other.Z);
	tmp.X = (X * other.W) + (W * other.X) + (Z * other.Y) - (Y * other.Z);
	tmp.Y = (Y * other.W) + (W * other.Y) + (X * other.Z) - (Z * other.X);
	tmp.Z = (Z * other.W) + (W * other.Z) + (Y * other.X) - (X * other.Y);

	return tmp;
}

// 8. Sets new quaternion based on euler angles
inline Quat& Quat::set(float x, float y, float z)
{
	return set(x, y , z);
}

// 9. Creates a matrix from this quaternion
inline Matrix4 Quat::getMatrix() const
{
	Matrix4 dest;

	dest[0] = W*W + X*X - Y*Y - Z*Z;
	dest[1] = 2.0f*X*Y + 2.0f*Z*W;
	dest[2] = 2.0f*X*Z - 2.0f*Y*W;
	dest[3] = 0.0f;

	dest[4] = 2.0f*X*Y - 2.0f*Z*W;
	dest[5] = W*W - X*X + Y*Y- Z*Z;
	dest[6] = 2.0f*Z*Y + 2.0f*X*W;
	dest[7] = 0.0f;

	dest[8] = 2.0f*X*Z + 2.0f*Y*W;
	dest[9] = 2.0f*Z*Y - 2.0f*X*W;
	dest[10] = W*W - X*X - Y*Y + Z*Z;
	dest[11] = 0.0f;

	dest[12] = 0.0f;
	dest[13] = 0.0f;
	dest[14] = 0.0f;
	dest[15] = 1.f;

	return dest;
}


// 10. Routine to convert the Euler angle specifying a rotation to a quaternion. 
Quat& Quat::eulerAnglesToQuaternion(const Vector3& vec)
{
	float alpha, beta, gamma;
	//Quat *q1, *q2, *q3;
	Quat q,q4;

	alpha = vec[0]; beta = vec[1]; gamma = vec[2];

	Quat q1(cos((PI / 180.0) * (alpha / 2.0)), sin((PI / 180.0) * (alpha / 2.0)), 0.0f, 0.0f);
	Quat q2(cos((PI / 180.0) * (beta / 2.0)), 0.0f, sin((PI / 180.0) * (beta / 2.0)), 0.0f);
	Quat q3(cos((PI / 180.0) * (gamma / 2.0)), 0.0f, 0.0f, sin((PI / 180.0) * (gamma / 2.0)));
	q4= (q3*q2);
	q= q4*q1;

	printf("j\tq1\t%f %f %f %f	0\n", q1.W, q1.X, q1.Y, q1.Z);
	printf("j\tq2\t%f %f %f %f	0\n", q2.W, q2.X, q2.Y, q2.Z);
	printf("j\tq3\t%f %f %f %f	0\n", q3.W, q3.X, q3.Y, q3.Z);
	printf("j\tq3\t%f %f %f %f	0\n", q4.W, q4.X, q4.Y, q4.Z);
	printf("j\tq3\t%f %f %f %f	0\n", q.W, q.X, q.Y, q.Z);

	X = q.X;
	Y = q.Y;
	Z = q.Z;
	W = q.W;

	return (q3*q2)*q1;
}
