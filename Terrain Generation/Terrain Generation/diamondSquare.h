#pragma once

class diamondSquare
{

private:
	

public:
	// Definition
	//Heights (Matrix where the value at [x][z] is the height)
	float** hs;
	//Width of the image
	int w;
	//Length of the image
	int l;

	// Method to set the value in the array
	double sample(int x, int y);
	// Method to set the initial height in the array
	void setSample(int x, int y, double value);
	// Method that calculate the diamond and square step
	void diamondSquareGen(int stepsize, double scale);
	// Diamond step method
	void sampleDiamond(int x, int y, int size, double value);
	// Square step method
	void sampleSquare(int x, int y, int size, double value);
	// Method to calculate random number between -1 and 1
	float randomNumber();


	diamondSquare(int width, int length);

	~diamondSquare();
};

