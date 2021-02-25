// Template1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "include\GL\GL.H"
#include "include\GL\GLU.H"
#include "include\GL\glut.h"
#include <math.h>

//CONSTANTS
const int   SCREEN_WIDTH = 1600;
const int   SCREEN_HEIGHT = 900;
const float INITIAL_CAMERA_DISTANCE = 200;

const float SUN_RADIUS = 10.0;//real=109
const float EARTH_DISTANCE= 25.0+SUN_RADIUS;
const float EARTH_RADIUS = 1.0;

const float MERCURY_RADIUS = 0.3825*EARTH_RADIUS;
const float MERCURY_DISTANCE = 0.3871*EARTH_DISTANCE;

const float VENUS_RADIUS = 0.9489*EARTH_RADIUS;
const float VENUS_DISTANCE = 0.7233*EARTH_DISTANCE;

const float MARS_RADIUS = 0.5335*EARTH_RADIUS;
const float MARS_DISTANCE = 1.5237*EARTH_DISTANCE;

const float JUPITER_RADIUS = 6*EARTH_RADIUS;//real 11.21
const float JUPITER_DISTANCE = 3*EARTH_DISTANCE;//real 5.2

const float SATURN_RADIUS = 5*EARTH_RADIUS;//real 9.44
const float SATURN_DISTANCE = 5*EARTH_DISTANCE;//real 9.5

const float URANUS_RADIUS = 2*EARTH_RADIUS;//real 4.0
const float URANUS_DISTANCE = 6*EARTH_DISTANCE;//real 19.2

const float NEPTUNE_RADIUS = 1.8*EARTH_RADIUS;//real 3.88
const float NEPTUNE_DISTANCE = 7.0*EARTH_DISTANCE;//real 30.1

int  initGLUT(int argc, char **argv);
void keyboard(unsigned char c, int x, int y);
void timer(int);
void initGL();
void display();
void drawSphere(GLuint tex, double radius, double initialPointX, double initialPointY, double initialPointZ);
void drawRing(double initialPointX, double initialPointY, double initialPointZ);
void reshape(int, int);
void rotateCertainPlanet(GLuint tex);
void rotateAroundSun(GLuint tex);
GLuint LoadTexture(const char * filename);

GLuint textureSun;
GLuint textureMercury;
GLuint textureVenus;
GLuint textureEarth;
GLuint textureMars;
GLuint textureJupiter;
GLuint textureSaturn;
GLuint textureUranus;
GLuint textureNeptune;
GLuint textureMoon;

GLfloat yCubeRotate = 0.0;
GLfloat xCubeRotate = -90.0;
GLfloat zCubeRotate = 0.0;
GLfloat initialRotation = 15.0;
GLfloat cameraX = 0.0;
GLfloat cameraY = 0.0;
GLfloat zoom;
int screenWidth;
int screenHeight;

//timer variables
float sunOwnAxis,mercuryOwnAxis, venusOwnAxis, earthOwnAxis, marsOwnAxis, jupiterOwnAxis, saturnOwnAxis, uranusOwnAxis, neptuneOwnAxis= 0.0;
float sunPos, mercuryPos, venusPos, earthPos, marsPos, jupiterPos, saturnPos, uranusPos, neptunePos = 0.0;
float posSpeed = 40.0f;
float axisRotationSpeed = 40.0f;

int main(int argc, char** argv) {
	initGLUT(argc, argv);
	glutCreateWindow("SOLAR SYSTEM");
	textureSun = LoadTexture("SunTex.bmp");
	textureMercury = LoadTexture("MercuryTex.bmp");
	textureVenus = LoadTexture("VenusTex.bmp");
	textureEarth = LoadTexture("EarthTex.bmp");
	textureMars = LoadTexture("MarsTex.bmp");
	textureJupiter = LoadTexture("JupiterTex.bmp");
	textureSaturn = LoadTexture("SaturnTex.bmp");
	textureUranus = LoadTexture("UranusTex.bmp");
	textureNeptune = LoadTexture("NeptuneTex.bmp");
	textureMoon = LoadTexture("MoonTex.bmp");

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	initGL();
	glutMainLoop();
	return 0;
}

int initGLUT(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);//single buffer and RGBA
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);//initial window size
	glutInitWindowPosition(200, 50);
	zoom = INITIAL_CAMERA_DISTANCE;
	return 0;
}

void initGL() {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
}

void reshape(int w, int h) {
	screenWidth = w;
	screenHeight = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)(screenWidth) / screenHeight, 1.0, 1000.0);// FOV(k¹t), AspectRatio(2/h), NearClip(najblizej), FarClip(najdalej)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	//initial adjustments
	glTranslatef(0.0, 0.0, -zoom);//+ + -
	glTranslatef(cameraX, cameraY, 0.0); // move camera right and left // f + r
	glRotatef(yCubeRotate, 0.0, 1.0, 0.0);//a + d
	glRotatef(xCubeRotate, 1.0, 0.0, 0.0);// w + s
	glRotatef(zCubeRotate, 0.0, 0.0, 1.0);// q + e
	GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//GLfloat lightAmbient1[] = { 0.0, 0.0, 0.0, 1.0 };
	//GLfloat lightDiffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat lightSpecular1[] = { 0.0,0.0, 0.0, 1.0 };
	//
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient1);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse1);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular1);


	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	//sun
	drawSphere(textureSun, SUN_RADIUS, 0.0, 0.0, 0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	drawSphere(textureMercury, MERCURY_RADIUS, MERCURY_DISTANCE, 0.0, 0.0);
	drawSphere(textureVenus, VENUS_RADIUS, VENUS_DISTANCE, 0.0, 0.0);
	drawSphere(textureEarth, EARTH_RADIUS, EARTH_DISTANCE, 0.0, 0.0);
	drawSphere(textureMars, MARS_RADIUS, MARS_DISTANCE, 0.0, 0.0);
	drawSphere(textureJupiter, JUPITER_RADIUS, JUPITER_DISTANCE, 0.0, 0.0);
	drawSphere(textureSaturn, SATURN_RADIUS, SATURN_DISTANCE, 0.0, 0.0);
	drawSphere(textureUranus, URANUS_RADIUS, URANUS_DISTANCE, 0.0, 0.0);
	drawSphere(textureNeptune, NEPTUNE_RADIUS, NEPTUNE_DISTANCE, 0.0, 0.0);
	glutSwapBuffers();
	glFlush();
}

void rotateCertainPlanet(GLuint tex) {
	if (tex == textureSun)glRotatef(sunOwnAxis, 0.0, 0.0, 1.0);
	if (tex == textureMercury)glRotatef(mercuryOwnAxis, 0.0, 0.0, 1.0);
	if (tex == textureVenus)glRotatef(venusOwnAxis, 0.0, 0.0, 1.0);
	if (tex == textureEarth)glRotatef(earthOwnAxis, 0.0, 0.0, 1.0);
	if (tex == textureMars)glRotatef(marsOwnAxis, 0.0, 0.0, 1.0);
	if (tex == textureJupiter)glRotatef(jupiterOwnAxis, 0.0, 0.0, 1.0);
	if (tex == textureSaturn)glRotatef(saturnOwnAxis, 0.0, 0.0, 1.0);
	if (tex == textureUranus)glRotatef(uranusOwnAxis, 0.0, 0.0, 1.0);
	if (tex == textureNeptune)glRotatef(neptuneOwnAxis, 0.0, 0.0, 1.0);
}

void rotateAroundSun(GLuint tex) {
	if (tex == textureSun)glRotatef(sunPos, 0.0, 0.0, 1.0);
	if (tex == textureMercury)glRotatef(mercuryPos, 0.0, 0.0, 1.0);
	if (tex == textureVenus)glRotatef(venusPos, 0.0, 0.0, 1.0);
	if (tex == textureEarth)glRotatef(earthPos, 0.0, 0.0, 1.0);
	if (tex == textureMars)glRotatef(marsPos, 0.0, 0.0, 1.0);
	if (tex == textureJupiter)glRotatef(jupiterPos, 0.0, 0.0, 1.0);
	if (tex == textureSaturn)glRotatef(saturnPos, 0.0, 0.0, 1.0);
	if (tex == textureUranus)glRotatef(uranusPos, 0.0, 0.0, 1.0);
	if (tex == textureNeptune)glRotatef(neptunePos, 0.0, 0.0, 1.0);
}

void drawSphere(GLuint tex, double radius, double initialPointX, double initialPointY, double initialPointZ) {
	glPushMatrix();

	rotateAroundSun(tex);
	
	if (tex == textureSaturn) {
		drawRing(initialPointX, initialPointY, initialPointZ);
	}

	glTranslatef(initialPointX, initialPointY, initialPointZ);
	rotateCertainPlanet(tex);
	glTranslatef(-initialPointX, -initialPointY, -initialPointZ);

	glBindTexture(GL_TEXTURE_2D, tex);
	GLUquadric *quadric;
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);      // Create Texture Coords  
	gluQuadricNormals(quadric, GLU_SMOOTH);   // Create Smooth Normals  
	glTranslatef(initialPointX, initialPointY, initialPointZ);
	gluSphere(quadric, radius, 100, 20);

	glPopMatrix();
}

void drawRing(double initialPointX, double initialPointY, double initialPointZ) {
	glPushMatrix();
	//ring
	glBindTexture(GL_TEXTURE_2D, 0);
	glTranslatef(initialPointX, initialPointY, initialPointZ);
	glRotatef(-10.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.2, 6.0, 100.0, 100.0);
	glutWireTorus(0.5, 6.0, 30.0, 30.0);
	glPopMatrix();
}

void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);

	//rotating around its own axis
	sunOwnAxis += 1.0 / 25.375;	//25d 9h
	mercuryOwnAxis += 1.0 / 58.625;	//58d 15h
	venusOwnAxis += 1.0 / 243.0;	//243d
	earthOwnAxis += 1.0 / 1.0;		//23h 56min
	marsOwnAxis += 1.0 / 1.025;		//24h37min
	jupiterOwnAxis += 1.0 / 0.413;	//9h55min
	saturnOwnAxis += 1.0 / 0.4438;	//10h39min
	uranusOwnAxis += 1.0 / 0.718;	//17h 14min
	neptuneOwnAxis += 1.0 / 0.67;	//16h6min

	//rotating around sun
	mercuryPos += 1.0 / 88.0*posSpeed;
	venusPos += 1.0 / 224.7*posSpeed;
	earthPos += 1.0 / 365.25*posSpeed;
	marsPos += 1.0 / 686.96*posSpeed;
	jupiterPos += 1.0 / 4333.29*posSpeed;
	saturnPos += 1.0 / 10756.2*posSpeed;
	uranusPos += 1.0 / 30707.49*posSpeed;
	neptunePos += 1.0 / 60223.35*posSpeed;
}

void keyboard(unsigned char c, int x, int y) {

	switch (c) {
	case 'a':
		yCubeRotate += 1;
		break;
	case 'd':
		yCubeRotate -= 1;
		break;
	case 'w':
		xCubeRotate += 1;
		break;
	case 's':
		xCubeRotate -= 1;
		break;
	case 'q':
		zCubeRotate += 1;
		break;
	case 'e':
		zCubeRotate -= 1;
		break;
	case '+':
		zoom -= 0.25;;
		break;
	case '-':
		zoom += 0.25;
		break;
	case 'z':
		cameraX += 0.25;
		break;
	case 'c':
		cameraX -= 0.25;
	case 'r':
		cameraY += 0.25;
		break;
	case 'f':
		cameraY -= 0.25;
		break;
	default:
		;
	}
}
GLuint LoadTexture(const char * filename)
{
	GLuint texture;
	int width, height;
	unsigned char * data;

	FILE * file;
	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	width = 2048;
	height = 1024;
	data = (unsigned char *)malloc(width * height * 3);
	//int size = fseek(file,);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}