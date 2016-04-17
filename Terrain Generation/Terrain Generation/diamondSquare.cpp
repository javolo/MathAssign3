#include "diamondSquare.h"
#include <cstdlib>

using namespace std;

diamondSquare::diamondSquare(int width, int length)
{

	w = width;
	l = length;

	hs = new float*[length];
	for (int i = 0; i < length; i++) {
		hs[i] = new float[width];
	}
}


diamondSquare::~diamondSquare()
{
}

// Method to calculate the random number between -1 and 1
float diamondSquare::randomNumber() {

	int random = rand() % 10;

	return random;
}

// Method to return a specific value in the array of heights
double diamondSquare::sample(int x, int y)
{
	return hs[x & (w-1)][y & (l - 1)];
}


// Method to set a specific height in the array
void diamondSquare::setSample(int x, int y, double value)
{
	hs[x][y] = value;
}

// Method that calculates the height in the each point of the square
void diamondSquare::sampleSquare(int x, int y, int size, double value)
{
	int hs = size / 2;

	// a     b 
	//
	//    x
	//
	// c     d

	double a = sample(x - hs, y - hs);
	double b = sample(x + hs, y - hs);
	double c = sample(x - hs, y + hs);
	double d = sample(x + hs, y + hs);

	setSample(x, y, ((a + b + c + d) / 4.0) + value);

}

// Method that calculates the height in the points of the diamond
void diamondSquare::sampleDiamond(int x, int y, int size, double value)
{
	int hs = size / 2;

	//   c
	//
	//a  x  b
	//
	//   d

	double a = sample(x - hs, y);
	double b = sample(x + hs, y);
	double c = sample(x, y - hs);
	double d = sample(x, y + hs);

	setSample(x, y, ((a + b + c + d) / 4.0) + value);
}

// Method that iterate through the squares and diamonds to generate the heights
void diamondSquare::diamondSquareGen(int stepsize, double scale)
{

	int halfstep = stepsize / 2;

	for (int y = halfstep; y < l + halfstep; y += stepsize)
	{
		for (int x = halfstep; x < w + halfstep; x += stepsize)
		{
			sampleSquare(x, y, stepsize, randomNumber() * scale);
		}
	}

	for (int y = 0; y < l; y += stepsize)
	{
		for (int x = 0; x < w; x += stepsize)
		{
			sampleDiamond(x + halfstep, y, stepsize, randomNumber() * scale);
			sampleDiamond(x, y + halfstep, stepsize, randomNumber() * scale);
		}
	}

}