// Template1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "include\GL\GL.H"
#include "include\GL\GLU.H"
#include "include\GL\glut.h"
#include <corecrt_math_defines.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <sstream>

const int   SCREEN_WIDTH = 900;
const int   SCREEN_HEIGHT = 900;
const float T_MAX = 8 * M_PI;
const float U_MAX = 2 * M_PI;
const float INITIAL_CAMERA_DISTANCE = 70;

void display();
void drawBox(GLuint tex, double height, double width, double length, double initialPointX, double initialPointY, double initialPointZ);
void drawCylinder(GLuint tex, double radius, double height, double initialPointX, double initialPointY, double initialPointZ);
void drawSphere(GLuint tex, double radius, double initialPointX, double initialPointY, double initialPointZ);
double drawSpring(GLuint tex);

void reshape(int,int);
void timer(int);
void initGL();
void drawAxes();
int  initGLUT(int argc, char **argv);
GLuint LoadTexture(const char * filename);
void keyboard(unsigned char c, int x, int y);

GLuint textureWood;
GLuint textureLine;
GLuint textureSteel;
GLuint textureBrick;
int screenWidth;
int screenHeight;
float cameraDistance;
GLfloat yCubeRotate = 0.0;
GLfloat xCubeRotate = 0.0;
GLfloat zCubeRotate = 0.0;
GLfloat initialRotation = 15.0;
GLfloat zoom;
GLfloat cameraX = 0.0;
GLfloat cameraY = 0.0;
bool axisON = false;

float k = 20.0;
float position = 10.0;
float gravity = 9.81;
float mass = 5;
float velocity = 0;
float furthestPosition = position;
float timeStep = 0.1;


int main(int argc, char** argv) { //<- for normal API
	
	initGLUT(argc, argv);

	glutCreateWindow("SPRING 3D");//create widnow, hello title bar
	textureWood = LoadTexture("WoodTest.bmp");
	textureLine = LoadTexture("LineTest.bmp");
	textureSteel = LoadTexture("SteelTest.bmp");
	textureBrick = LoadTexture("BrickTest.bmp");

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	initGL();
	glutMainLoop();//enter main loop and process events
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
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
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
	case 'g':
		if (axisON) {
			axisON = false;
		}
		else {
			axisON = true;
		}
		break;
	default:
		;
	}
}

void drawBox(GLuint tex, double height, double width, double length, double initialPointX, double initialPointY, double initialPointZ) {//tekstura,wyskosc, szerokosc, dlugosc, 1 wspó³rzêdna
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex);
	glTranslatef(initialPointX, initialPointY, initialPointZ);

	//Front face
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(length, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(length, height, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, height, 0);
	glEnd();
	glPopMatrix();
	//Back face
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -width);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(length, 0, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(length, height, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, height, -width);
	glEnd();
	glPopMatrix();
	//Left face
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, height, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, height, 0);
	glEnd();
	glPopMatrix();
	//Right face(left face translated)
	glPushMatrix();
	glTranslatef(length, 0, 0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0, 0, -width);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0, height, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, height, 0);
	glEnd();
	glPopMatrix();
	//Down face
	glPushMatrix();
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(length, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(length, 0, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -width);
	glEnd();
	glPopMatrix();
	//Top face(bot face translated)
	glPushMatrix();
	glTranslatef(0, height, 0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(length, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(length, 0, -width);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -width);
	glEnd();
	glPopMatrix();

	glPopMatrix();
}

void drawCylinder(GLuint tex, double radius, double height, double initialPointX, double initialPointY, double initialPointZ) {
	glPushMatrix();
	glTranslatef(initialPointX, initialPointY, initialPointZ);
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, tex);
	GLUquadric *quadric;
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);   
	gluQuadricNormals(quadric, GLU_SMOOTH);
	//glTranslatef(initialPointX, initialPointY, initialPointZ);
	
	gluCylinder(quadric, radius, radius, height, 120, 1);
	glPopMatrix();
}

void drawSphere(GLuint tex, double radius, double initialPointX, double initialPointY, double initialPointZ) {
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, tex);
	GLUquadric *quadric;
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);      // Create Texture Coords  
	gluQuadricNormals(quadric, GLU_SMOOTH);   // Create Smooth Normals  
	glTranslatef(initialPointX, initialPointY, initialPointZ);
	gluSphere(quadric, radius, 100, 20);

	glPopMatrix();
}

double getXPosition(double t, double u) {
	return cos(t) * (3 + cos(u));
}

double getYPosition(double t, double u) {
	return sin(t) * (3 + cos(u));
}

double getZPosition(double t, double u) {
	float scale = t / T_MAX;
	return 0.6 * t + sin(u) + furthestPosition * scale;
}

double drawSpring(GLuint tex) {
	glPushMatrix();
	glTranslatef(-3.0, 25.0, 0.0);
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, tex);
	double Zcoordinate;

	float step = T_MAX / 300;
	for (double t = 0; t <= T_MAX; t += step) {
		for (double u = 0; u <= U_MAX; u += step) {
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(getXPosition(t, u), getYPosition(t, u), getZPosition(t, u));
			glTexCoord2f(1.0f, 0.0f); glVertex3f(getXPosition(t + step, u), getYPosition(t + step, u), getZPosition(t + step, u));
			glTexCoord2f(1.0f, 1.0f); glVertex3f(getXPosition(t + step, u + step), getYPosition(t + step, u + step), getZPosition(t + step, u + step));
			glTexCoord2f(0.0f, 1.0f); glVertex3f(getXPosition(t, u + step), getYPosition(t, u + step), getZPosition(t, u + step));
			glEnd();
			Zcoordinate = getZPosition(t, u + step);
		}
	}
	glRotatef(-90, 1, 0, 0);
	glPopMatrix();
	return Zcoordinate;
}

void getFurthestPosition(){
	float force = mass * gravity -k * position;
	float acceleration = force/mass;
	velocity = velocity + acceleration * timeStep;
	position = position + velocity * timeStep;

	furthestPosition = position;
}

void display() {
	getFurthestPosition();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -zoom);//+ + -
	glTranslatef(cameraX, cameraY, 0.0); // move camera right and left // f + r
	glRotatef(yCubeRotate, 0.0, 1.0, 0.0);//a + d
	glRotatef(xCubeRotate, 1.0, 0.0, 0.0);// w + s
	glRotatef(zCubeRotate, 0.0, 0.0, 1.0);// q + e
	//initial rotate to see better
	glRotatef(initialRotation, 1.0, 0.0, 0.0);
	glRotatef(-initialRotation, 0.0, 1.0, 0.0);
	//this elements dont move
	drawBox(textureBrick, 1.0, 20.0, 20.0, -10.0, 30.0, 10.0);	//height Y, width Z, length X
	drawCylinder(textureSteel, 1.0, 5.0, 0.0, 30.0, 0.0);//top
	drawSphere(textureSteel, 1.0, 0.0, 25.0, 0.0);//small top
	
	//this elements move
	double ZDOWN = drawSpring(textureLine);
	double smallBallY = 25.0 - ZDOWN;
	drawSphere(textureSteel, 1, 0, smallBallY, 0);//small bottom
	drawCylinder(textureSteel, 1.0, 3.0, 0.0, smallBallY, 0.0);//bottom
	drawSphere(textureSteel, 3.5, 0, smallBallY -6, 0);//big bottom

	if(axisON)drawAxes();
	glutSwapBuffers();
	glFlush();
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

void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}

float axisLength = 100;

void drawAxes()
{
	glPushMatrix();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(-axisLength, 0.0f, 0.0f);
	glVertex3f(axisLength, 0.0f, 0.0f);
	glEnd();
	glFlush();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, -axisLength, 0.0f);
	glVertex3f(0.0, axisLength, 0.0f);
	glEnd();
	glFlush();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0f, -axisLength);
	glVertex3f(0.0, 0.0f, axisLength);
	glEnd();
	glFlush();

	glPopMatrix();
}

GLuint LoadTexture(const char * filename)
{
	GLuint texture;
	int width, height;
	unsigned char * data;

	FILE * file;
	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	width = 676;
	height = 612;
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