/* Perlin Noise header class
*/

class perlinNoise{

public:
	
	// We create the permutation matrix used in the implementation of the Perlin Noise
	// Hash lookup table as defined by Ken Perlin.  This is a randomly arranged array of all numbers from 0-255 inclusive.
	static int permutation[256];
	static int p[512];
	int repeat;

	// Contructor
	perlinNoise(int repeat);
	// Destructor
	~perlinNoise();

	// Function calculate the perlin Noise value of specific point
	double perlin(double x, double z);
	// Fade function to generate a smooth transition between gradients
	double fade(double t);
	// Incremental function
	int inc(int num);
	// Gradient function to calculate the dot product of the coordinaties associated
	// with the point
	double grad(int hash, double x, double z);
	// Function to calculate the linear interpolation
	double lerp(double a, double b, double x);
	// Octave function to make the perlin Noise generation more realistic
	double octavePerlin(double x, double z, int octaves, double persistence);
};

