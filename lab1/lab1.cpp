#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>
using namespace std;

GLuint window;
GLuint width = 300, height = 300;

int counterv = 0, counterf = 0, counter = 0, counter1 = 0, counter2 = 0, counter3 = 0;
int j = 0, k = 0, l = 0;
float numX[10000] = { 0.0 };
float numY[10000] = { 0.0 };
float numZ[10000] = { 0.0 };
float numX1[10000] = { 0.0 };
float numY1[10000] = { 0.0 };
float numZ1[10000] = { 0.0 };
int numfX[10000] = { 0 };
int numfY[10000] = { 0 };
int numfZ[10000] = { 0 };
float midX, midY, midZ, M;
string X1[10000];
string Y1[10000];
string Z1[10000];
int numberP = 0, numberS = 0;
float* splineX;
float* splineY;
float* splineZ;
float* tanX;
float* tanY;
float* tanZ;
float* allTanX;
float* allTanY;
float* allTanZ;
float eX = 0.0, eY = 0.0, eZ = 0.0;
float wX = 0.0, wY = 0.0, wZ = 0.0;
float sX = 1.0, sY = 0.0, sZ = 0.0;

void myDisplay();
void myReshape(int width, int height);
void myIdle();

int main(int argc, char** argv)
{
	fstream newfile2;
	newfile2.open("test.txt", ios::in); //open a file to perform read operation using file object
	if (newfile2.is_open()) {   //checking whether the file is open
		string tp;
		while (getline(newfile2, tp)) { //read data from file object and put it into string.
			for (int i = 0; i < tp.length(); i++) {
				if (tp[i] == ' ') {
					counter3 += 1;
					continue;
				}
				if (counter3 == 0) {
					X1[l].push_back(tp[i]);
				}
				if (counter3 == 1) {
					Y1[l].push_back(tp[i]);
				}
				if (counter3 == 2) {
					Z1[l].push_back(tp[i]);
				}
			}
			counter2 += 1;
			counter3 = 0;
			l += 1;
		}
		newfile2.close(); //close the file object.
	}
	for (int i = 0; i < counter2; i++) {
		stringstream num(X1[i]);
		num >> numX1[i];
		stringstream num1(Y1[i]);
		num1 >> numY1[i];
		stringstream num2(Z1[i]);
		num2 >> numZ1[i];
	}
	numberP = counter2;
	numberS = numberP - 3;
	fstream newfile;
	newfile.open("kocka.obj", ios::in); //open a file to perform read operation using file object
	if (newfile.is_open()) {   //checking whether the file is open
		string tp;
		while (getline(newfile, tp)) { //read data from file object and put it into string.
			if (tp[0] == 'v') {
				counterv += 1;
			}
			if (tp[0] == 'f') {
				counterf += 1;
			}
		}
		newfile.close(); //close the file object.
	}
	string* X{ new string[counterv]{} };
	string* Y{ new string[counterv]{} };
	string* Z{ new string[counterv]{} };
	string* fX{ new string[counterf]{} };
	string* fY{ new string[counterf]{} };
	string* fZ{ new string[counterf]{} };
	float xminmax[2], yminmax[2], zminmax[2];
	xminmax[0] = yminmax[0] = zminmax[0] = 5000;
	xminmax[1] = yminmax[1] = zminmax[1] = 0;
	fstream newfile1;
	newfile1.open("kocka.obj", ios::in); //open a file to perform read operation using file object
	if (newfile1.is_open()) {   //checking whether the file is open
		string tp;
		while (getline(newfile1, tp)) { //read data from file object and put it into string.
			if (tp[0] == 'v') {
				for (int i = 1; i < tp.length(); i++) {
					if (tp[i] == ' ') {
						counter += 1;
						continue;
					}
					if (counter == 1) {
						X[j].push_back(tp[i]);
					}
					if (counter == 2) {
						Y[j].push_back(tp[i]);
					}
					if (counter == 3) {
						Z[j].push_back(tp[i]);
					}
				}
				counter = 0;
				j += 1;
			}
			if (tp[0] == 'f') {
				for (int i = 1; i < tp.length(); i++) {
					if (tp[i] == ' ') {
						counter1 += 1;
						continue;
					}
					if (counter1 == 1) {
						fX[k].push_back(tp[i]);
					}
					if (counter1 == 2) {
						fY[k].push_back(tp[i]);
					}
					if (counter1 == 3) {
						fZ[k].push_back(tp[i]);
					}
				}
				counter1 = 0;
				k += 1;
			}
		}
		newfile.close(); //close the file object.
	}
	for (int i = 0; i < counterv; i++) {
		stringstream num(X[i]);
		num >> numX[i];
		stringstream num1(Y[i]);
		num1 >> numY[i];
		stringstream num2(Z[i]);
		num2 >> numZ[i];
	}
	for (int i = 0; i < counterf; i++) {
		stringstream num3(fX[i]);
		num3 >> numfX[i];
		stringstream num4(fY[i]);
		num4 >> numfY[i];
		stringstream num5(fZ[i]);
		num5 >> numfZ[i];
	}
	for (int i = 0; i < counterv; i++) {
		if (numX[i] < xminmax[0]) {
			xminmax[0] = numX[i];
		}
		if (numX[i] > xminmax[1]) {
			xminmax[1] = numX[i];
		}
		if (numY[i] < yminmax[0]) {
			yminmax[0] = numY[i];
		}
		if (numY[i] > yminmax[1]) {
			yminmax[1] = numY[i];
		}
		if (numZ[i] < zminmax[0]) {
			zminmax[0] = numZ[i];
		}
		if (numZ[i] > zminmax[1]) {
			zminmax[1] = numZ[i];
		}
	}
	midX = (xminmax[0] + xminmax[1]) / 2;
	midY = (yminmax[0] + yminmax[1]) / 2;
	midZ = (zminmax[0] + zminmax[1]) / 2;
	splineX = (float*)malloc(100 * numberS * sizeof(float));
	splineY = (float*)malloc(100 * numberS * sizeof(float));
	splineZ = (float*)malloc(100 * numberS * sizeof(float));
	tanX = (float*)malloc(numberS * 8 * sizeof(float));
	tanY = (float*)malloc(numberS * 8 * sizeof(float));
	tanZ = (float*)malloc(numberS * 8 * sizeof(float));
	allTanX = (float*)malloc(numberS * 200 * sizeof(float));
	allTanY = (float*)malloc(numberS * 200 * sizeof(float));
	allTanZ = (float*)malloc(numberS * 200 * sizeof(float));
	int numTan = 0, numTanAll = 0;
	for (int i = 0; i < numberS; i++) {
		float v0x = numX1[i];
		float v0y = numY1[i];
		float v0z = numZ1[i];
		float v1x = numX1[i + 1];
		float v1y = numY1[i + 1];
		float v1z = numZ1[i + 1];
		float v2x = numX1[i + 2];
		float v2y = numY1[i + 2];
		float v2z = numZ1[i + 2];
		float v3x = numX1[i + 3];
		float v3y = numY1[i + 3];
		float v3z = numZ1[i + 3];
	}
	for (int i = 0; i < numberS; i++) {
		float v0x = numX1[i];
		float v0y = numY1[i];
		float v0z = numZ1[i];
		float v1x = numX1[i + 1];
		float v1y = numY1[i + 1];
		float v1z = numZ1[i + 1];
		float v2x = numX1[i + 2];
		float v2y = numY1[i + 2];
		float v2z = numZ1[i + 2];
		float v3x = numX1[i + 3];
		float v3y = numY1[i + 3];
		float v3z = numZ1[i + 3];
		for (int t = 0; t < 100; t++) {
			double j = t / 100.0;
			float f1 = (-pow(j, 3.0) + 3 * pow(j, 2.0) - 3 * j + 1) / 6.0;
			float f2 = (3 * pow(j, 3.0) - 6 * pow(j, 2.0) + 4) / 6.0;
			float f3 = (-3 * pow(j, 3.0) + 3 * pow(j, 2.0) + 3 * j + 1) / 6.0;
			float f4 = pow(j, 3.0) / 6.0;
			splineX[100 * i + t] = f1 * v0x + f2 * v1x + f3 * v2x + f4 * v3x;
			splineY[100 * i + t] = f1 * v0y + f2 * v1y + f3 * v2y + f4 * v3y;
			splineZ[100 * i + t] = f1 * v0z + f2 * v1z + f3 * v2z + f4 * v3z;
			if (t % 25 == 0) {
				float t1 = 0.5 * (-pow(j, 2.0) + 2 * j - 1);
				float t2 = 0.5 * (3 * pow(j, 2.0) - 4 * j);
				float t3 = 0.5 * (-3 * pow(j, 2.0) + 2 * j + 1);
				float t4 = 0.5 * (pow(j, 2.0));
				tanX[numTan] = f1 * v0x + f2 * v1x + f3 * v2x + f4 * v3x;
				tanY[numTan] = f1 * v0y + f2 * v1y + f3 * v2y + f4 * v3y;
				tanZ[numTan] = f1 * v0z + f2 * v1z + f3 * v2z + f4 * v3z;
				numTan++;
				float vx = t1 * v0x + t2 * v1x + t3 * v2x + t4 * v3x;
				float vy = t1 * v0y + t2 * v1y + t3 * v2y + t4 * v3y;
				float vz = t1 * v0z + t2 * v1z + t3 * v2z + t4 * v3z;
				tanX[numTan] = tanX[numTan - 1] + vx / 3;
				tanY[numTan] = tanY[numTan - 1] + vy / 3;
				tanZ[numTan] = tanZ[numTan - 1] + vz / 3;
				numTan++;
			}
			float t1 = 0.5 * (-pow(j, 2.0) + 2 * j - 1);
			float t2 = 0.5 * (3 * pow(j, 2.0) - 4 * j);
			float t3 = 0.5 * (-3 * pow(j, 2.0) + 2 * j + 1);
			float t4 = 0.5 * (pow(j, 2.0));
			allTanX[numTanAll] = f1 * v0x + f2 * v1x + f3 * v2x + f4 * v3x;
			allTanY[numTanAll] = f1 * v0y + f2 * v1y + f3 * v2y + f4 * v3y;
			allTanZ[numTanAll] = f1 * v0z + f2 * v1z + f3 * v2z + f4 * v3z;
			numTanAll++;
			float vx = t1 * v0x + t2 * v1x + t3 * v2x + t4 * v3x;
			float vy = t1 * v0y + t2 * v1y + t3 * v2y + t4 * v3y;
			float vz = t1 * v0z + t2 * v1z + t3 * v2z + t4 * v3z;
			allTanX[numTanAll] = allTanX[numTanAll - 1] + vx / 3;
			allTanY[numTanAll] = allTanY[numTanAll - 1] + vy / 3;
			allTanZ[numTanAll] = allTanZ[numTanAll - 1] + vz / 3;
			numTanAll++;
		}
	}
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(500, 500);
	glutInit(&argc, argv);
	window = glutCreateWindow("Glut OpenGL Linija");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);
	glutMainLoop();
	return 0;
}

int t = 0;
void myReshape(int w, int h)
{
	width = w; height = h;               //promjena sirine i visine prozora
	glViewport(0, 0, width, height);	//  otvor u prozoru

	glMatrixMode(GL_PROJECTION);		//	matrica projekcije
	glLoadIdentity();					//	jedinicna matrica
	gluPerspective(90, 1, 1, 100);
	gluOrtho2D(0, width, 0, height);	//	okomita projekcija
	glMatrixMode(GL_MODELVIEW);			//	matrica pogleda
	glLoadIdentity();					//	jedinicna matrica
	glTranslatef(0, 0, -80);
	glRotatef(90, 0, 1, 0);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
	glPointSize(1.0);					//	postavi velicinu tocke za liniju
	glColor3f(0.0f, 0.0f, 0.0f);		//	postavi boju linije
}

void myDisplay()
{
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(-5.0, -5.0, -75.0);

	glMatrixMode(GL_PROJECTION);		//	matrica projekcije
	glLoadIdentity();					//	jedinicna matrica
	gluPerspective(90, 1, 1, 100);
	glMatrixMode(GL_MODELVIEW);			//	matrica pogleda
	glLoadIdentity();					//	jedinicna matrica
	glTranslatef(0, 0, -100);
	glRotatef(90, 0, 0, 1);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 100 * numberS; i++) {
		glVertex3f(splineX[i], splineY[i], splineZ[i]);
	}
	glEnd();
	glBegin(GL_LINES);
	for (int i = 0; i < numberS * 8; i += 2) {
		glVertex3f(tanX[i], tanY[i], tanZ[i]);
		glVertex3f(tanX[i + 1], tanY[i + 1], tanZ[i + 1]);
	}
	glEnd();
	glTranslatef(splineX[t], splineY[t], splineZ[t]);
	glScalef(3, 3, 3);
	eX = allTanX[2 * t + 1] - allTanX[2 * t];
	eY = allTanY[2 * t + 1] - allTanY[2 * t];
	eZ = allTanZ[2 * t + 1] - allTanZ[2 * t];
	wX = sY * eZ - eY * sZ;
	wY = eX * sZ - sX * eZ;
	wZ = sX * eY - sY * eX;
	double apsS = pow(pow((double)sX, 2.0) + pow((double)sY, 2.0) + pow((double)sZ, 2.0), 0.5);
	double apsE = pow(pow((double)eX, 2.0) + pow((double)eY, 2.0) + pow((double)eZ, 2.0), 0.5);
	double se = sX * eX + sY * eY + sZ * eZ;
	double angle = acos(se / (apsS * apsE));
	angle = angle / (2 * M_PI) * 360;
	glRotatef(angle, wX, wY, wZ);
	glTranslatef(-midX, -midY, -midZ);
	glBegin(GL_LINES);
	for (int i = 0; i < counterf; i++) {
		float v1x = numX[numfX[i]];
		float v1y = numY[numfX[i]];
		float v1z = numZ[numfX[i]];
		float v2x = numX[numfY[i]];
		float v2y = numY[numfY[i]];
		float v2z = numZ[numfY[i]];
		float v3x = numX[numfZ[i]];
		float v3y = numX[numfZ[i]];
		float v3z = numX[numfZ[i]];
		glVertex3f(v1x, v1y, v1z);
		glVertex3f(v2x, v2y, v2z);
		glVertex3f(v2x, v2y, v2z);
		glVertex3f(v3x, v3y, v3z);
		glVertex3f(v3x, v3y, v3z);
		glVertex3f(v1x, v1y, v1z);
	}
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(midX, midY, midZ);
	glVertex3f(midX + 2.5, midY, midZ);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(midX, midY, midZ);
	glVertex3f(midX, midY + 2.5, midZ);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(midX, midY, midZ);
	glVertex3f(midX, midY, midZ + 2.5);
	glColor3f(0.0, 0.0, 0.0);
	glEnd();
	t++;
	if (t == 100 * numberS) {
		t = 0;
	}
	glFlush();
}
int currentTime = 0; int previousTime = 0;
void myIdle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	if (timeInterval > 10) {
		myDisplay();
		previousTime = currentTime;
	}
}
