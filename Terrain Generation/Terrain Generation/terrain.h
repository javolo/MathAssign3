

#include "vec3f.h"

class terrain
{

private:
	//Width of the image
	int w;
	//Length of the image
	int l;
	// Array of normal (Matrix where each position [x][z] stores a vector with the components of the normal)
	Vec3f** normals;

	bool computedNormals; //Whether normals is up-to-date



public:
	//Heights (Matrix where the value at [x][z] is the height)
	float** hs;
	// Constructor
	terrain(int width, int length);
	// Constructor for the Diamond Square method
	terrain(int width, int length, int featureSize);
	// Destructor
	~terrain();

	// Getter for the height and width of the terrain
	int width();
	int length();

	// Setter for the height and width of the terrain
	void setHeight(int x, int z, float y);
	float getHeight(int x, int z);

	// Function to compute the normal at a specific point through the four edges
	void computeNormals();
	// Function to somooth the normal at specific ratio to reduce the peaks in the mountains of the terrain
	void smoothNormals(Vec3f** normals2);
	// Get the normal of a point given its coordinates
	Vec3f getNormal(int x, int z);

};

