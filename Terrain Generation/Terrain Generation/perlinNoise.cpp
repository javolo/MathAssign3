/* Perlin Noise implementation class
*
* In this class we are going to follow the steps that Ken Perlin created
* and an improve version was released in 2002
*
*
* Source Code adaptation from the explanation in (original Source Code in C#):
* http://flafla2.github.io/2014/08/09/perlinnoise.html
*/


#include "perlinNoise.h"

using namespace std;

// Declaration of the permutation matrix
// Hash lookup table as defined by Ken Perlin.  This is a randomly arranged array of all numbers 
// from 0-255 inclusive.
int perlinNoise::permutation[256] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};
 
int perlinNoise::p[512];
// Declaration of the double permutation array to avoid overflow
//perlinNoise::perlinNoise() {
//
//	// We init the double permutation array repeating the single permutation array
//	for (int x = 0; x<512; x++) {
//		p[x] = permutation[x % 256];
//	}
//}

// Constructor
perlinNoise::perlinNoise(int repeatParam = -1){ 
	repeat = repeatParam;

	// We init the double permutation array repeating the single permutation array
	for (int x = 0; x<512; x++) {
		p[x] = permutation[x % 256];
	}
}

// Implementation of the perlin noise
double perlinNoise::perlin(double x, double z) {

	// If we have any repeat on, change the coordinates to their "local" repetitions
	if (repeat > 0) {									
		x = (int) x%repeat;
		z = (int) z%repeat;
	}

	// Calculate the "unit cube" that the point asked will be located in
	// The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
	// plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.
	int xi = (int)x & 255;								
	int zi = (int)z & 255;								
	
	double xf = x - (int)x;								
	double zf = z - (int)z;

	// We also fade the location to smooth the result.
	double u = fade(xf);
	double w = fade(zf);

	int aa, ab, ba, bb;
	aa = p[p[xi] + zi];
	ab = p[p[xi] + inc(zi)];
	ba = p[p[inc(xi)] + zi];
	bb = p[p[inc(xi)] + inc(zi)];

	double x1, x2;
	// The gradient function calculates the dot product between a pseudorandom
	// gradient vector and the vector from the input coordinate to the 4
	// surrounding points in its unit cube.
	x1 = lerp(grad(aa, xf, zf), grad(ba, xf - 1, zf),u);	
	// This is all then lerped together as a sort of weighted average based on the faded (u,w)
	// values we made earlier.
	x2 = lerp(grad(ab, xf, zf-1), grad(bb, xf - 1, zf-1), u);
	
	return lerp(x1, x2, w);
}

// Implementation of the increment function
int perlinNoise::inc(int number) {

	number++;
	if (repeat > 0) number %= repeat;

	return number;
}

// Gradient function to calculate the dot product of a randomly selected gradient
// vector and the 8 location vectors
// With this variation of the original implementation the speed increases in the calculation
// Source: http://riven8192.blogspot.com/2010/08/calculate-perlinnoise-twice-as-fast.html
// Ken Perlin Implementation
// float u = (h < 8) ? x : y;
// float v = (h < 4) ? y : ((h == 12 || h == 14) ? x : z);
// return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
double perlinNoise::grad(int hash, double x, double z) {

	switch (hash & 0x7)
	{
	case 0x0: return  0 + z;
	case 0x1: return  x + z;
	case 0x2: return  x + 0;
	case 0x3: return  x - z;
	case 0x4: return  0 - z;
	case 0x5: return -x - z;
	case 0x6: return -x + 0;
	case 0x7: return -x + z;
	default: return 0; // never happens
	}
}

// Implementation of the fade function to smooth the gradient result of the gradient
// Fade function as defined by Ken Perlin.This eases coordinate values
// so that they will "ease" towards integral values.  This ends up smoothing
// the final output.
double perlinNoise::fade(double t) {

	// 6t^5 - 15t^4 + 10t^3
	return t * t * t * (t * (t * 6 - 15) + 10);
}

// Linear interpolation function
double perlinNoise::lerp(double a, double b, double x) {

	return a + x * (b - a);
}

// Octave function
double perlinNoise::octavePerlin(double x, double z, int octaves, double persistence) {

	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	// Used for normalizing result to 0.0 - 1.0
	double maxValue = 1;  
	for (int i = 0; i<octaves; i++) {
		total += perlin(x * frequency, z * frequency) * amplitude;

		//maxValue += amplitude;

		amplitude *= persistence;
		frequency *= 2;
	}

	return total / maxValue;
}

// Destructor
perlinNoise::~perlinNoise(){
}




