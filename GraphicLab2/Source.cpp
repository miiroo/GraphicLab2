#include<math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iterator> 
#include <vector>

#if defined(linux) || defined(_WIN32)
#include"GL\glut.h"/*ÄëÿLinux è Windows*/
#else
#include<GLUT/GLUT.h>/*Äëÿ Mac OS*/
#endif


using namespace std;

struct Point {
	float x;
	float y;
};

const int MINX = 0, MINY = 0, MAXX = 15, MAXY = 15;
vector <Point> points;
list <int> code;


Point currPoint;

GLdouble planeX = -1;
GLdouble planeY = 1;
GLdouble planeZ = 0;
GLdouble planeD = 0;

void readFromFile(string path);
void reshape(int w, int h);
void display();
void pressedKey(int key, int x, int y);

int main(int argc, char* argv[]) {
	readFromFile("a.txt");
	readFromFile("s.txt");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 500);
	glutCreateWindow("OpenGL lesson 2");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutSpecialFunc(pressedKey);
	glutMainLoop();
	return 0;
}
void pressedKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		if (planeY <0)
		planeD++; 
		else planeD--;
		break;
	case GLUT_KEY_DOWN:
		if (planeY <0)
		planeD--; 
		else planeD++;
		break;
	case GLUT_KEY_LEFT:
		planeX+=0.2; break;
	case GLUT_KEY_RIGHT:
		planeX-=0.2; break;
	case GLUT_KEY_F1:
		planeY *= -1;
		planeD *= -1;
		break;
	case GLUT_KEY_F2:
		planeX = -1;
		planeY = 1;
		planeD = 0;
		break;
	default: break;
	}
	display();
}


void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(MINX, MAXX, MINY, MAXY);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void readFromFile(string path)
{
	fstream f(path, ios::in);
	cout << path << endl;
	if (f.is_open()) {
		if (points.empty()) {
			int pointNumber;
			Point p;
			f >> pointNumber;
			for (int i = 0; i < pointNumber; i++)
			{
				f >> p.x >> p.y;
				points.push_back(p);
				//		cout << p.x << " " << p.y << endl;
			}
			cout << endl;
			int movesNumber, m;
			f >> movesNumber;
			for (int i = 0; i < movesNumber; i++)
			{
				f >> m;
				code.push_back(m);
						cout << m << endl;
			}
		}
		else {
			int shiftForDots = points.size();
			int pointNumber;
			Point p;
			f >> pointNumber;
			for (int i = 0; i < pointNumber; i++)
			{
				f >> p.x >> p.y;
				points.push_back(p);
				//		cout << p.x << " " << p.y << endl;
			}
			cout << endl;
			int movesNumber, m;
			f >> movesNumber;
			for (int i = 0; i < movesNumber; i++)
			{
				f >> m;
				m < 0 ? m-=shiftForDots : m += shiftForDots;
				code.push_back(m);
				cout << m << endl;
			}
		}
	//	cout << endl;
	}
	f.close();
}


void moveTo(int mto, float shiftX, float shiftY) {
	mto = abs(mto) - 1;
	currPoint.x = points[mto].x + shiftX;
	currPoint.y = points[mto].y + shiftY;

}

void drawTo(int dto, float shiftX, float shiftY) {
	dto = dto - 1;
	//  points[dto];
	Point p = points[dto];
	glColor3d(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2i(currPoint.x, currPoint.y);
	glVertex2i(p.x + shiftX, p.y + shiftY);

	glEnd();
	currPoint.x = p.x + shiftX;
	currPoint.y = p.y + shiftY;
}

void drawOrMove(float shiftX, float shiftY) {

	for (auto it = code.begin(); it != code.end(); it++) {
		if ((*it) < 0) {
			moveTo((*it), shiftX, shiftY);
		}
		else {
			drawTo((*it), shiftX, shiftY);
		}
	}

//	code.clear();
//	points.clear();
}

void display() {



	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_CLIP_PLANE0);
	//x y z d
	//Ax+By+d = 0
	//Ax+By = -d
	GLdouble arr[4] = { planeX, planeY, planeZ, planeD };
	glClipPlane(GL_CLIP_PLANE0, arr);
	glColor3d(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2i(0, -1*planeD/planeY);
	glVertex2i(MAXX,-1*(planeD+MAXX*planeX)/planeY);
	glEnd();
	cout << "X: " << planeX << " Y: " << planeY << " Z:" << planeZ << " D: " << planeD << endl;
 

	/*
	float shiftX = 5;
	float shiftY = 5;
	readFromFile("points.txt");
	drawOrMove(shiftX, shiftY);
	*/

	//readFromFile("a.txt");
	//drawOrMove(0, 0);


	//readFromFile("s.txt");
	drawOrMove(0, 0);

	glDisable(GL_CLIP_PLANE0);

	glutSwapBuffers();
}