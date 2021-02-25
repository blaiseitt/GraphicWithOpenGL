// Template1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "include\GL\GL.H"
#include "include\GL\GLU.H"
#include "include\GL\glut.h"
#include <string.h>

float layer1 = 360.0;
float layer2 = 360.0;
float layer3 = 360.0;

float trianglesAngle = 360.0;

float radius = 0.0;

float l1Dir = 0.6;
float l2Dir = 0.6;
float l3Dir = 0.6;

const float DISTANCE_TO_CENTER_X = 640;
const float DISTANCE_TO_CENTER_Y = 540;

struct Color 
{
	float R, G, B;
};

struct Point
{
	float x, y;
};

struct Coordinates 
{
	Point pA, pB, pC;
};

Coordinates flipTriangle(Coordinates c, char direction);
Coordinates nextLayer(Coordinates c);
Coordinates findNewTriangleCoordinates(Coordinates c, int quadrant);

void drawTriangle(Coordinates c, Color color, int layer) {
	
	glPushMatrix();
	Point centerOfMass;
	centerOfMass.x = (c.pA.x + c.pB.x + c.pC.x)/3;
	centerOfMass.y= (c.pA.y + c.pB.y + c.pC.y)/3;
	
	glTranslated(DISTANCE_TO_CENTER_X, DISTANCE_TO_CENTER_Y, 0.0f);
	//Obracanie wokó³ œrodka
	if (layer == 1) glRotatef(layer1, 0.0, 0.0, 1.0);
	else if (layer == 2) glRotatef(layer2, 0.0, 0.0, 1.0);
	else glRotatef(layer3, 0.0, 0.0, 1.0);
	//glRotatef(layer1, 0.0, 0.0, 1.0);
	glTranslated(-DISTANCE_TO_CENTER_X, -DISTANCE_TO_CENTER_Y, 0.0f);
	
	//przesuwanie trójk¹tów - od i do œrodka uk³adu
	//o tyle przesuwamy punkt - kierunek jeszcze nieznany
	Point tranXY;		//to tak naparwdê nie jest punkt tylko informacja o ile w kierunku X i Y nale¿y przesun¹æ punkt
	Point finalTran;
	//x
	if (centerOfMass.x < DISTANCE_TO_CENTER_X) {
		tranXY.x = (1 + radius)*(DISTANCE_TO_CENTER_X - centerOfMass.x) - (DISTANCE_TO_CENTER_X - centerOfMass.x);
		finalTran.x = -tranXY.x;
	}
	else if (centerOfMass.x > DISTANCE_TO_CENTER_X) {
		tranXY.x = (1 + radius)*(centerOfMass.x - DISTANCE_TO_CENTER_X) - (centerOfMass.x - DISTANCE_TO_CENTER_X);
		finalTran.x = tranXY.x;
	}
	else if (centerOfMass.x == DISTANCE_TO_CENTER_X) {
		tranXY.x = DISTANCE_TO_CENTER_X;
		finalTran.x = DISTANCE_TO_CENTER_X;
	}
	//y
	if (centerOfMass.y < DISTANCE_TO_CENTER_Y) {
		tranXY.y = (1 + radius)*(DISTANCE_TO_CENTER_Y - centerOfMass.y) - (DISTANCE_TO_CENTER_Y - centerOfMass.y);
		finalTran.y = -tranXY.y;
	}
	else if (centerOfMass.y > DISTANCE_TO_CENTER_Y) {
		tranXY.y = (1 + radius)*(centerOfMass.y - DISTANCE_TO_CENTER_Y) - (centerOfMass.y - DISTANCE_TO_CENTER_Y);
		finalTran.y = tranXY.y;
	}
	else if (centerOfMass.y == DISTANCE_TO_CENTER_Y) {
		tranXY.y = DISTANCE_TO_CENTER_Y;
		finalTran.y = 540;
	}
	glTranslated(finalTran.x, finalTran.y, 0.0f);
	
	//obracanie wokó³ œrodka masy trójk¹tów
	glTranslated(centerOfMass.x, centerOfMass.y, 0.0f);
	glRotatef(trianglesAngle, 0.0, 0.0, 1.0);
	glTranslated(-centerOfMass.x, -centerOfMass.y, 0.0f);
	
	glBegin(GL_TRIANGLES);// OpenGL’a state

	glColor4f(color.R, color.G, color.B, 1.0f);
	glVertex2f(c.pA.x, c.pA.y);
	glVertex2f(c.pB.x, c.pB.y);
	glVertex2f(c.pC.x, c.pC.y);
	glEnd();

	glPopMatrix();
}

char* directonMessage(float dir);

void customDescription(float x, float y, char* msg) {
	//wyswietlenie informacji na temet kierunku obracania sie poszczegolnych pierscieni
	int len, i;
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(x, y + 20);
	len = (int)strlen(msg);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
	}
}

void briefDescription(float x, float y, float layerDirection, char layerNumber) {
	//wyswietlenie informacji na temet kierunku obracania sie poszczegolnych pierscieni
	int len, i;
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(x,y+20);
	char information[] = "Pierscien numer ";
	len = (int)strlen(information);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, information[i]);
	}
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, layerNumber);
	glRasterPos2f(x, y);
	char* string = directonMessage(layerDirection);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

//define necessary colors
struct Color Red = { 1.0f, 0.0f, 0.0f };
struct Color Yellow = { 1.0f, 1.0f, 0.0f };
struct Color Magenta = { 1.0f, 0.0f, 1.0f };
struct Color Green = { 0.0f, 1.0f, 0.0f };
struct Color Orange = { 1.0f, 0.4f, 0.0f };
struct Color Blue = { 0.0f, 0.0f, 1.0f };

//Initial coordinates

struct Coordinates coordinates21;
struct Coordinates coordinates22;
struct Coordinates coordinates23;

struct Coordinates coordinates11;
struct Coordinates coordinates12;
struct Coordinates coordinates13;

struct Coordinates coordinates41;
struct Coordinates coordinates42;
struct Coordinates coordinates43;

struct Coordinates coordinates31;
struct Coordinates coordinates32;
struct Coordinates coordinates33;

struct Coordinates coordinates221;
struct Coordinates coordinates222;

struct Coordinates coordinates121;
struct Coordinates coordinates122;

struct Coordinates coordinates421;
struct Coordinates coordinates422;

struct Coordinates coordinates321;
struct Coordinates coordinates322;

struct Coordinates coordinates231;
struct Coordinates coordinates131;
struct Coordinates coordinates431;
struct Coordinates coordinates331;

void setCoordinatesValues(float a, float b, float c);

void MyDisplay(void) {
	// The new scene
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	//Zewnêtrzna
	drawTriangle(coordinates21, Red,1);
	drawTriangle(coordinates22, Yellow,1);
	drawTriangle(coordinates23, Magenta,1);
	drawTriangle(coordinates11, Red,1);
	drawTriangle(coordinates12, Yellow,1);
	drawTriangle(coordinates13, Magenta,1);
	drawTriangle(coordinates41, Red,1);
	drawTriangle(coordinates42, Yellow,1);
	drawTriangle(coordinates43, Magenta,1);
	drawTriangle(coordinates31, Red,1);
	drawTriangle(coordinates32, Yellow,1);
	drawTriangle(coordinates33, Magenta,1);

	//œrodkowa
	drawTriangle(coordinates221, Green,2);
	drawTriangle(coordinates222, Orange,2);
	drawTriangle(coordinates121, Green,2);
	drawTriangle(coordinates122, Orange,2);
	drawTriangle(coordinates421, Green,2);
	drawTriangle(coordinates422, Orange,2);
	drawTriangle(coordinates321, Green,2);
	drawTriangle(coordinates322, Orange,2);

	//wewnêtrzna
	drawTriangle(coordinates231, Blue,3);
	drawTriangle(coordinates131, Blue,3);
	drawTriangle(coordinates431, Blue,3);
	drawTriangle(coordinates331, Blue,3);

	
	briefDescription(25.0f, 870.0f, l1Dir, '1');
	briefDescription(25.0f, 820.0f, l2Dir, '2');
	briefDescription(25.0f, 770.0f, l3Dir, '3');
	customDescription(25.0f, 900.0f, "Sterowanie: 1,2,3,4,5,6");

	glFlush();//start processing buffered OpenGL routines
}

void refreshActions(int);

void MyInit(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);//select clearing (background) color
									 /* initialize viewing values */
	glViewport(0, 0, 300, 300);//window origin and size
	glMatrixMode(GL_PROJECTION);//

	glLoadIdentity();//=1
	gluOrtho2D(0.0, 1280, 0.0, 960.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//=1
}

void keyboard(unsigned char c, int x, int y);

int main(int argc, char** argv) { //<- for normal API
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//single buffer and RGBA
	glutInitWindowSize(1200, 1000);//initial window size
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Grafika komputerowa - zadanie 1");//create widnow, hello title bar
	
	glutTimerFunc(0, refreshActions, 0);
	MyInit();
	setCoordinatesValues(340.0f, 440.0f, 440.0f);		//340.0f, 440.0f, 440.0f
	glutDisplayFunc(MyDisplay);//
	
	glutKeyboardFunc(keyboard);
	glutMainLoop();//enter main loop and process events
	return 0;
}

void keyboard(unsigned char c, int x, int y) {
	if (c == 27) {
		exit(0);
	}
	else if (c == '1')
		l1Dir = 0.6;
	else if (c =='2')
		l1Dir = -0.6;
	else if (c == '3')
		l2Dir = 0.6;
	else if (c == '4')
		l2Dir = -0.6;
	else if (c == '5')
		l3Dir = 0.6;
	else if (c == '6')
		l3Dir = -0.6;
}


bool broaden = true;

void refreshActions(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, refreshActions, 0); //miliseconds per frame, wiêc 1000/60 oznacza ¿e bêdziemy mieli 60 fps - jedna klatka co 16,666ms
	
	layer1 += l1Dir;
	layer2 += l2Dir;
	layer3 += l3Dir;
	trianglesAngle += 1.8;

	if (broaden) {
		radius += 0.0025;
		if (radius >= 0.5) {
			broaden = false;
		}
	}
	else {
		radius -= 0.0025;
		if (radius <= 0.0) {
			broaden = true;
		}
	}
	
}

char* directonMessage(float dir) {
	if (dir > 0)
		return "obraca sie w lewo.";
	else
		return "obraca sie w prawo.";
}

void setCoordinatesValues(float a, float b, float c) {
	//Druga æwiartka zewnêtrzna
	coordinates21 = { { a, a+200.0f }, { b, b+100.0f }, { c, c+200.0f } };
	coordinates22 = findNewTriangleCoordinates(coordinates21, 2);
	coordinates23 = findNewTriangleCoordinates(coordinates22, 2);
	//Pierwsza æwiartka zewnêtrzna
	coordinates11 = flipTriangle(coordinates23, 'R');
	coordinates12 = findNewTriangleCoordinates(coordinates11, 1);
	coordinates13 = findNewTriangleCoordinates(coordinates12, 1);
	//Czwarta æwiartka zewnêtrzna
	coordinates41 = flipTriangle(coordinates13, 'D');
	coordinates42 = findNewTriangleCoordinates(coordinates41, 4);
	coordinates43 = findNewTriangleCoordinates(coordinates42, 4);
	//Trzecia æwiartka zewnêtrzna
	coordinates31 = flipTriangle(coordinates43, 'L');
	coordinates32 = findNewTriangleCoordinates(coordinates31, 3);
	coordinates33 = findNewTriangleCoordinates(coordinates32, 3);
	//Druga æwiartka œrodkowa
	coordinates221 = nextLayer(coordinates21);
	coordinates222 = findNewTriangleCoordinates(coordinates221, 2);
	//Pierwsza æwiartka œrodkowa
	coordinates121 = flipTriangle(coordinates222, 'R');
	coordinates122 = findNewTriangleCoordinates(coordinates121, 1);
	//Czwarta æwiartka œrodkowa
	coordinates421 = flipTriangle(coordinates122, 'D');
	coordinates422 = findNewTriangleCoordinates(coordinates421, 4);
	//Trzecia æwiartka srodkowa
	coordinates321 = flipTriangle(coordinates422, 'L');
	coordinates322 = findNewTriangleCoordinates(coordinates321, 3);
	//Druga æwiartka wewnêtrzna
	coordinates231 = nextLayer(coordinates221);
	//Pierwsza æwiartka wewnêtrzna
	coordinates131 = flipTriangle(coordinates231, 'R');
	//Czwarta æwiartka wewnêtrzna
	coordinates431 = flipTriangle(coordinates131, 'D');
	//Trzecia æwiartka wewnêtrzna
	coordinates331 = flipTriangle(coordinates431, 'L');
}

#pragma region CoordinateMethods
Coordinates findNewTriangleCoordinates(Coordinates c, int quadrant) {

	Coordinates newCoordinates;

	switch (quadrant) {
	case 1:
		newCoordinates.pA.x = c.pA.x + 100;
		newCoordinates.pA.y = c.pA.y - 100;
		newCoordinates.pB.x = c.pB.x + 100;
		newCoordinates.pB.y = c.pB.y - 100;
		newCoordinates.pC.x = c.pC.x + 100;
		newCoordinates.pC.y = c.pC.y - 100;
		return newCoordinates;
	case 2:
		newCoordinates.pA.x = c.pA.x + 100;
		newCoordinates.pA.y = c.pA.y + 100;
		newCoordinates.pB.x = c.pB.x + 100;
		newCoordinates.pB.y = c.pB.y + 100;
		newCoordinates.pC.x = c.pC.x + 100;
		newCoordinates.pC.y = c.pC.y + 100;
		return newCoordinates;
	case 3:
		newCoordinates.pA.x = c.pA.x - 100;
		newCoordinates.pA.y = c.pA.y + 100;
		newCoordinates.pB.x = c.pB.x - 100;
		newCoordinates.pB.y = c.pB.y + 100;
		newCoordinates.pC.x = c.pC.x - 100;
		newCoordinates.pC.y = c.pC.y + 100;
		return newCoordinates;
	case 4:
		newCoordinates.pA.x = c.pA.x - 100;
		newCoordinates.pA.y = c.pA.y - 100;
		newCoordinates.pB.x = c.pB.x - 100;
		newCoordinates.pB.y = c.pB.y - 100;
		newCoordinates.pC.x = c.pC.x - 100;
		newCoordinates.pC.y = c.pC.y - 100;
		return newCoordinates;
	default:
		return c;
	}
}

Coordinates flipTriangle(Coordinates c, char direction) {

	Coordinates newCoordinates;
	switch (direction) {
	case 'L':
		if (c.pA.x == c.pB.x) {
			newCoordinates.pA = c.pA;
			newCoordinates.pB = c.pB;
			newCoordinates.pC.x = c.pC.x - 200;
			newCoordinates.pC.y = c.pC.y;
			return newCoordinates;
		}
		else if (c.pA.x == c.pC.x) {
			newCoordinates.pA = c.pA;
			newCoordinates.pC = c.pC;
			newCoordinates.pB.x = c.pB.x - 200;
			newCoordinates.pB.y = c.pB.y;
			return newCoordinates;
		}
		else {
			newCoordinates.pB = c.pB;
			newCoordinates.pC = c.pC;
			newCoordinates.pA.x = c.pA.x - 200;
			newCoordinates.pA.y = c.pA.y;
			return newCoordinates;
		}
		break;
	case 'R':
		if (c.pA.x == c.pB.x) {
			newCoordinates.pA = c.pA;
			newCoordinates.pB = c.pB;
			newCoordinates.pC.x = c.pC.x + 200;
			newCoordinates.pC.y = c.pC.y;
			return newCoordinates;
		}
		else if (c.pA.x == c.pC.x) {
			newCoordinates.pA = c.pA;
			newCoordinates.pC = c.pC;
			newCoordinates.pB.x = c.pB.x + 200;
			newCoordinates.pB.y = c.pB.y;
			return newCoordinates;
		}
		else {
			newCoordinates.pB = c.pB;
			newCoordinates.pC = c.pC;
			newCoordinates.pA.x = c.pA.x + 200;
			newCoordinates.pA.y = c.pA.y;
			return newCoordinates;
		}
	case 'D':
		if (c.pA.y == c.pB.y) {
			newCoordinates.pA = c.pA;
			newCoordinates.pB = c.pB;
			newCoordinates.pC.x = c.pC.x;
			newCoordinates.pC.y = c.pC.y - 200;
			return newCoordinates;
		}
		else if (c.pA.y == c.pC.y) {
			newCoordinates.pA = c.pA;
			newCoordinates.pC = c.pC;
			newCoordinates.pB.x = c.pB.x;
			newCoordinates.pB.y = c.pB.y - 200;
			return newCoordinates;
		}
		else {
			newCoordinates.pB = c.pB;
			newCoordinates.pC = c.pC;
			newCoordinates.pA.x = c.pA.x;
			newCoordinates.pA.y = c.pA.y - 200;
			return newCoordinates;
		}
	case 'U':
		if (c.pA.y == c.pB.y) {
			newCoordinates.pA = c.pA;
			newCoordinates.pB = c.pB;
			newCoordinates.pC.x = c.pC.x;
			newCoordinates.pC.y = c.pC.y + 200;
			return newCoordinates;
		}
		else if (c.pA.y == c.pC.y) {
			newCoordinates.pA = c.pA;
			newCoordinates.pC = c.pC;
			newCoordinates.pB.x = c.pB.x;
			newCoordinates.pB.y = c.pB.y + 200;
			return newCoordinates;
		}
		else {
			newCoordinates.pB = c.pB;
			newCoordinates.pC = c.pC;
			newCoordinates.pA.x = c.pA.x;
			newCoordinates.pA.y = c.pA.y + 200;
			return newCoordinates;
		}
		break;
	default:
		return c;

	}
}

Coordinates nextLayer(Coordinates c) {
	Coordinates newCoordinates;
	newCoordinates.pA.x = c.pA.x + 100;
	newCoordinates.pA.y = c.pA.y;
	newCoordinates.pB.x = c.pB.x + 100;
	newCoordinates.pB.y = c.pB.y;
	newCoordinates.pC.x = c.pC.x + 100;
	newCoordinates.pC.y = c.pC.y;
	return newCoordinates;
}
#pragma endregion