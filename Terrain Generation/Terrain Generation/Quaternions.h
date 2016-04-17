#pragma once
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>

#define PI 3.14159265

// Quaternion class.
class Quaternion
{
public:
	Quaternion() { }
	Quaternion(float wVal, float xVal, float yVal, float zVal)
	{
		w = wVal; x = xVal; y = yVal; z = zVal;
	}
	float getW() { return w; }
	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }

private:
	float w, x, y, z;
};

static Quaternion identityQuaternion(1.0, 0.0, 0.0, 0.0), q; // Global identity quaternion.

															 // Euler angles class.
class EulerAngles
{
public:
	EulerAngles() { }
	EulerAngles(float alphaVal, float betaVal, float gammaVal)
	{
		alpha = alphaVal; beta = betaVal; gamma = gammaVal;
	}
	float getAlpha() { return alpha; }
	float getBeta() { return beta; }
	float getGamma() { return gamma; }

private:
	float alpha, beta, gamma;
};

static EulerAngles e; // Global Euler angle value.

					  // Rotation matrix class.
class RotationMatrix
{
public:
	RotationMatrix() { }
	RotationMatrix(float matrixDataVal[16])
	{
		for (int i = 0; i < 16; i++) matrixData[i] = matrixDataVal[i];
	}
	float getMatrixData(int i) { return matrixData[i]; }

private:
	float matrixData[16];
};

// Routine to multiply two quaternions.
Quaternion multiplyQuaternions(Quaternion q1, Quaternion q2)
{
	float w1, x1, y1, z1, w2, x2, y2, z2, w3, x3, y3, z3;

	w1 = q1.getW(); x1 = q1.getX(); y1 = q1.getY(); z1 = q1.getZ();
	w2 = q2.getW(); x2 = q2.getX(); y2 = q2.getY(); z2 = q2.getZ();

	w3 = w1*w2 - x1*x2 - y1*y2 - z1*z2;
	x3 = w1*x2 + x1*w2 + y1*z2 - z1*y2;
	y3 = w1*y2 + y1*w2 + z1*x2 - x1*z2;
	z3 = w1*z2 + z1*w2 + x1*y2 - y1*x2;

	return *new Quaternion(w3, x3, y3, z3);
}

// Routine to convert the Euler angle specifying a rotation to a quaternion. 
Quaternion eulerAnglesToQuaternion(EulerAngles e)
{
	float alpha, beta, gamma;
	Quaternion *q1, *q2, *q3;

	alpha = e.getAlpha(); beta = e.getBeta(); gamma = e.getGamma();

	q1 = new Quaternion(cos((PI / 180.0) * (alpha / 2.0)), sin((PI / 180.0) * (alpha / 2.0)), 0.0, 0.0);
	q2 = new Quaternion(cos((PI / 180.0) * (beta / 2.0)), 0.0, sin((PI / 180.0) * (beta / 2.0)), 0.0);
	q3 = new Quaternion(cos((PI / 180.0) * (gamma / 2.0)), 0.0, 0.0, sin((PI / 180.0) * (gamma / 2.0)));

	return multiplyQuaternions(*q1, multiplyQuaternions(*q2, *q3));
}

// Routine to convert a quaternion specifying a rotation to a 4x4 rotation matrix in column-major order.
RotationMatrix quaternionToRotationMatrix(Quaternion q)
{
	float w, x, y, z;
	float m[16];

	w = q.getW(); x = q.getX(); y = q.getY(); z = q.getZ();

	m[0] = w*w + x*x - y*y - z*z;
	m[1] = 2.0*x*y + 2.0*w*z;
	m[2] = 2.0*x*z - 2.0*y*w;
	m[3] = 0.0;
	m[4] = 2.0*x*y - 2.0*w*z;
	m[5] = w*w - x*x + y*y - z*z;
	m[6] = 2.0*y*z + 2.0*w*x;
	m[7] = 0.0;
	m[8] = 2.0*x*z + 2.0*w*y;
	m[9] = 2.0*y*z - 2.0*w*x;
	m[10] = w*w - x*x - y*y + z*z;
	m[11] = 0.0;
	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;

	return *new RotationMatrix(m);
}

// Read global Euler angle values to global EulerAngles object e.
void readEulerAngles(EulerAngles *e)
{
	*e = *new EulerAngles(Xangle, Yangle, Zangle);
}

// Write RotationMatrix object r values to global matrixData.
void writeMatrixData(RotationMatrix r)
{
	for (int i = 0; i < 16; i++) matrixData[i] = r.getMatrixData(i);
}

// Spherical linear interpolation between unit quaternions q1 and q2 with interpolation parameter t.
Quaternion slerp(Quaternion q1, Quaternion q2, float t)
{
	float w1, x1, y1, z1, w2, x2, y2, z2, w3, x3, y3, z3;
	Quaternion q2New;
	float theta, mult1, mult2;

	w1 = q1.getW(); x1 = q1.getX(); y1 = q1.getY(); z1 = q1.getZ();
	w2 = q2.getW(); x2 = q2.getX(); y2 = q2.getY(); z2 = q2.getZ();

	// Reverse the sign of q2 if q1.q2 < 0.
	if (w1*w2 + x1*x2 + y1*y2 + z1*z2 < 0)
	{
		w2 = -w2; x2 = -x2; y2 = -y2; z2 = -z2;
	}

	theta = acos(w1*w2 + x1*x2 + y1*y2 + z1*z2);

	if (theta > 0.000001)
	{
		mult1 = sin((1 - t)*theta) / sin(theta);
		mult2 = sin(t*theta) / sin(theta);
	}

	// To avoid division by 0 and by very small numbers the approximation of sin(angle)
	// by angle is used when theta is small (0.000001 is chosen arbitrarily).
	else
	{
		mult1 = 1 - t;
		mult2 = t;
	}

	w3 = mult1*w1 + mult2*w2;
	x3 = mult1*x1 + mult2*x2;
	y3 = mult1*y1 + mult2*y2;
	z3 = mult1*z1 + mult2*z2;

	return *new Quaternion(w3, x3, y3, z3);
}
