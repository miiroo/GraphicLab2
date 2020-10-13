#include<math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iterator> 
#include <vector>

#if defined(linux) || defined(_WIN32)
//#include"C:\Program Files (x86)\Microsoft Visual Studio 11.0\GL\glut.h"/*ÄëÿLinux è Windows*/
#include"GL\glut.h"
#else
#include<GLUT/GLUT.h>/*Äëÿ Mac OS*/
#endif

using namespace std;

struct Point {
	float x;
	float y;
	float z;
};

const int MINX = 0, MINY = 0, MAXX = 15, MAXY = 10;
vector <Point> points;
list <int> code;
Point currPoint;
int task = 1;


void reshape(int w, int h);
void display();
void readFromFile(string path);
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
	case GLUT_KEY_F1:
		task = 1;
		break;
	case GLUT_KEY_F2:
		task = 2;
		break;
	case GLUT_KEY_F3:
		task = 3;
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
				f >> p.x >> p.y >> p.z;
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
				//cout << m << endl;
			}
		}
		else {
			int shiftForDots = points.size();
			int pointNumber;
			Point p;
			f >> pointNumber;
			for (int i = 0; i < pointNumber; i++)
			{
				f >> p.x >> p.y >> p.z;
				points.push_back(p);
				//		cout << p.x << " " << p.y << endl;
			}
			cout << endl;
			int movesNumber, m;
			f >> movesNumber;
			for (int i = 0; i < movesNumber; i++)
			{
				f >> m;
				m < 0 ? m -= shiftForDots : m += shiftForDots;
				code.push_back(m);
			//	cout << m << endl;
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
//A 14 S 12
void drawTo(int dto, float shiftX, float shiftY) {
		Point p = points[dto - 1];
		if (dto > 14 && dto != 20 && dto !=26) {
			float arr[3][3];
			arr[0][0] = currPoint.x;
			arr[0][1] = currPoint.y;
			arr[0][2] = currPoint.z;
			arr[2][0] = p.x;
			arr[2][1] = p.y;
			arr[2][2] = p.z;
			
			currPoint.x == p.x ? arr[1][0] = p.x - 0.3 : arr[1][0] = ((currPoint.x + p.x) / 2) - 0.2;

			if (currPoint.y == p.y && p.y > 4) arr[1][1] = p.y + 0.8;
			else {
				if (currPoint.y == p.y && p.y <= 4)  arr[1][1] = p.y - 0.8;
				else 
					arr[1][1] = (currPoint.y + p.y) / 2;
			}

			arr[1][2] = 0;
			
			GLfloat node[8] = { 0, 0, 0, 1, 1, 1};
			GLUnurbs* nurbs = gluNewNurbsRenderer();
			gluBeginCurve(nurbs);
			gluNurbsCurve(nurbs, 6, node, 3, &arr[0][0], 3, GL_MAP1_VERTEX_3);
			gluEndCurve(nurbs);
		}
		else {
			Point p = points[dto - 1];
			glColor3d(1, 1, 1);
			glBegin(GL_LINES);
			glVertex2i(currPoint.x, currPoint.y);
			glVertex2i(p.x + shiftX, p.y + shiftY);
			glEnd();
		}
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
}

GLfloat massiv[4][3] = { { 7, 6, 0 },{ 9, 7, 0 },{ 9, 5, 0 },{ 7, 3, 0 } };
GLfloat massiv2[4][3] = { { 7, 6, 0 },{ 5, 7, 0 },{ 5, 5, 0 },{ 7, 3, 0 } };

void drawBez() {
	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &massiv[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 15; i++)
		glEvalCoord1f((GLfloat)i / 15);
	glEnd();

	glMap1f(GL_MAP1_VERTEX_3, 0, 1, 3, 4, &massiv2[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 15; i++)
		glEvalCoord1f((GLfloat)i / 15);
	glEnd();

}

void drawSplain() {
	GLfloat node[8] = { 0, 0, 0, 0, 1, 1, 1, 1 };
	GLUnurbs* nurbs = gluNewNurbsRenderer();
	gluBeginCurve(nurbs);
	gluNurbsCurve(nurbs, 8, node, 3, &massiv[0][0], 4, GL_MAP1_VERTEX_3);
	gluEndCurve(nurbs);

	gluBeginCurve(nurbs);
	gluNurbsCurve(nurbs, 8, node, 3, &massiv2[0][0], 4, GL_MAP1_VERTEX_3);
	gluEndCurve(nurbs);
}

void display() {

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	switch (task) {
		/*Bezje*/
	case 1:
		drawBez();
		break;

		/*B-splain*/
	case 2:
		drawSplain();
		break;

	case 3:
		drawOrMove(0, 0);
		
	}
	glutSwapBuffers();
}