/* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above notice and this permission notice shall be included in all copies
* or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
/* File for "Terrain" lesson of the OpenGL tutorial on
* www.videotutorialsrock.com
*/
// Test changes change the version right_


#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include "../../Libraries/include/GL/glut.h"
#include "../../Libraries/include/GL/freeglut.h"
#endif

#include "perlinNoise.h"
#include "imageloader.h"
#include "terrain.h"
#include "diamondSquare.h"
#include "vec3f.h"


using namespace std;

// Declaration of variables when we import a matrix manually input to generate a heightmap
int M[9][9] = {
	{ 1,1, 1, 1, 1,1,1,1,1},
	{ 1,1, 1, 1, 1,1,1,1,1 },
	{ 1,1, 3, 2, 3,3,4,2,1},
	{ 1,1, 2, 1, 2,2,2,2,1 },
	{ 1,1, 2, 1, 2,2,2,2,1 },
	{ 1,1, 2, 1, 2,2,2,2,1 },
	{ 1,1, 2, 2, 3,3,3,3,2 },
	{ 1,1, 2, 2, 3,4,4,3,3 },
	{ 1,1, 2, 1, 1,1,1,1,1 } };

// We declare the normal vector that we are going to use for calculations
Vec3f** normalsVector;
int imageWidth;
int imageHeight;
float _angle = 60.0f;

// Function to init the normal Vector double array
void initNormalVector(int width, int height) {

	normalsVector = new Vec3f*[height];
	for (int i = 0; i < height; i++) {
		normalsVector[i] = new Vec3f[width];
	}
}

// Function to generate the heightmap from the perlin noise module 
terrain* generateNoiseTerrain() {

	perlinNoise generator = perlinNoise::perlinNoise(-1);

	terrain* t = new terrain(60, 60);

	for (double y = 0; y < 60; y+=0.9) {
		for (double x = 0; x < 60; x+=0.9) {
			float h = generator.octavePerlin(x, y, 4, 1.5);
			t->setHeight(x, y, h);
		}
	}
	t->computeNormals();
	return t;
}

// Method called in the draw scene to generate the terrain with the Diamond Square
// Algorithm
terrain* generateDiamondSquareTerrain() {

	// The terrain has to be power of two so let´s set up the step
	int step = 5;
	int width = pow(2, step);
	int length = pow(2, step);

	// Sample size and featured size variables
	int samplesize = 4;
	double scale = 0.5;

	// Generate the terrain with the heigth set to 0
	terrain* t = new terrain(width, length, samplesize);


	// We create a diamondSquare object
	diamondSquare* ds = new diamondSquare(width, length);
	// We passed the double array init
	ds->hs = t->hs;
	// We call the method to calculate the matrix of height with the diamond square method
	while (samplesize > 1){
		ds->diamondSquareGen(samplesize, scale);

		samplesize /= 2;
		scale /= 2.0;
	}
	// Get back the matrix of values
	t->hs = ds->hs;
	
	// Compute the normals of the matrix generated
	t->computeNormals();
	return t;
}

//Loads a terrain from a heightmap.  The heights of the terrain range from
//-height / 2 to height / 2.
terrain* loadTerrain(const char* filename, float height) {
	Image* image = loadBMP(filename);
	terrain* t = new terrain(image->width, image->height);
	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {
			unsigned char color =
				(unsigned char)image->pixels[3 * (y * image->width + x)];
			float h = height * ((color / 255.0f) - 0.5f);
			t->setHeight(x, y, h);
		}
	}

	delete image;
	t->computeNormals();
	return t;
}

terrain* _terrain;

void cleanup() {
	delete _terrain;
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		cleanup();
		exit(0);
	}
}

template <class T> const T& maxVal(const T& a, const T& b) {
	return (a<b) ? b : a;     // or: return comp(a,b)?b:a; for version (2)
}


void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

// Render function with Triangle Strips
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.0f, 2.0f, -20.0f);
	//glRotatef(45.0f, 100.0f, -40.0f, 0.0f);
	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);

	

	GLfloat ambientColor[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	GLfloat lightColor0[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat lightPos0[] = { -0.5f, 0.8f, 0.1f, 0.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	float scale = 10.0f / maxVal(_terrain->width() - 1, _terrain->length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float)(_terrain->width() - 1) / 2,
		0.0f,
		-(float)(_terrain->length() - 1) / 2);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int MAP_SIZE = 8;
	//glPushMatrix();
	glColor3f(0.3f, 0.9f, 0.0f);
	for (int z = 0; z < _terrain->length()-1; z++) {
	
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < _terrain->width()-1; x++) {

			// draw vertex 0 (A)
			glVertex3f(x, _terrain->getHeight(x, z), z);
			// draw vertex 3 (D)
			glVertex3f(x, _terrain->getHeight(x, z + 1), z + 1);
			// Set the normal second triangle
			Vec3f normal = _terrain->getNormal(x + 1, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			// draw vertex 1 (B) 
			glVertex3f(x + 1, _terrain->getHeight(x + 1, z), z);

			// Set the normal of the first triangle
			normal = _terrain->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			// draw vertex 2 (C)
			glVertex3f(x + 1, _terrain->getHeight(x + 1, z + 1), z + 1);
			
			
		}
		glEnd();
	}
	glutSwapBuffers();
}



void update(int value) {
	_angle += 1.0f;
	if (_angle > 360) {
		_angle -= 360;
	}

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1500, 800);

	glutCreateWindow("");
	initRendering();

	// We calculate the normals here
	/*imageHeight = 8;
	imageWidth = 7;
	initNormalVector(imageWidth, imageHeight);
	calculateNormalsMatrix();*/

	// Loading a terrain with a bitmap
	//_terrain = loadTerrain("firstheightmap.bmp", 20);

	// Generate a terrain with Perlin Noise Function
	//_terrain = generateNoiseTerrain();

	// Generate a terrain with Diamond Square Algorithm
	_terrain = generateDiamondSquareTerrain();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);

	glutMainLoop();
	return 0;
}