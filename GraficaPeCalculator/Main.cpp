#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <gl/freeglut.h>
#include <iostream>
#include <fstream>

using namespace std;

#define LUNGIME 100.0
#define LATIME 20.0
#define PI 3.141592654

struct punct
{
	double x;
	double y;
}P1, P2, P3, P4, P5, P6;

struct domino
{
	punct cord;
	double ung = 0.0;
	bool angr = 0;
};

//dreapta daca e 1, stanga daca e 0
bool dir = 1;

GLuint unit;
int k;
domino lista_drept[256];
double i = 0.0;
double j = 0.0;
double alpha = 1.0, rung = 40;
double err = 0.00001;
double deltatime;
double oldtime = 0.0;

int nrdominos = 4;

double arie(punct A, punct B, punct C)
{
	return abs((A.x * B.y + B.x * C.y + C.x * A.y - C.x * B.y - A.x * C.y - B.x * A.y) / 2.0);
}

bool suprapunere(punct A, punct B, punct C, punct D, punct M)
{
	if (abs(arie(A, B, M) + arie(B, C, M) + arie(C, D, M) + arie(D, A, M) - LUNGIME * LATIME < err))
		return 1;
	return 0;
}

static void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	//piesa unitate
	unit = glGenLists(1);
	glNewList(unit, GL_COMPILE);
	glBegin(GL_QUADS);
	glVertex2f(0.0, 1.0);
	glVertex2f(1.0, 1.0);
	glVertex2f(1.0, 0.0);
	glVertex2f(0.0, 0.0);
	glEnd();
	glEndList();
}

bool ok = 1;

void update(void)
{
	double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	deltatime = t - oldtime;
	oldtime = t;
	if (dir)
	{
		for (k = 0; k < nrdominos; k++)
		{
			if (lista_drept[k].angr && lista_drept[k].ung < 90.0)
			{
				lista_drept[k].ung += rung * deltatime;
			}

			if (k != nrdominos - 1)
			{
				P1.x = lista_drept[k].cord.x - cos((lista_drept[k].ung * PI) / 180) * LATIME;
				P1.y = lista_drept[k].cord.y + sin((lista_drept[k].ung * PI) / 180) * LATIME;

				P2.x = lista_drept[k].cord.x - cos((lista_drept[k].ung * PI) / 180) * LATIME + cos(((90.0 - lista_drept[k].ung) * PI) / 180) * LUNGIME;
				P2.y = lista_drept[k].cord.y + sin((lista_drept[k].ung * PI) / 180) * LATIME + sin(((90.0 - lista_drept[k].ung) * PI) / 180) * LUNGIME;

				P3.x = lista_drept[k].cord.x + cos(((90.0 - lista_drept[k].ung) * PI) / 180) * LUNGIME;
				P3.y = lista_drept[k].cord.y + sin(((90.0 - lista_drept[k].ung) * PI) / 180) * LUNGIME;

				P4.x = lista_drept[k + 1].cord.x - cos((lista_drept[k + 1].ung * PI) / 180) * LATIME;
				P4.y = lista_drept[k + 1].cord.y + sin((lista_drept[k + 1].ung * PI) / 180) * LATIME;

				P5.x = lista_drept[k + 1].cord.x - cos((lista_drept[k + 1].ung * PI) / 180) * LATIME + cos(((90.0 - lista_drept[k + 1].ung) * PI) / 180) * LUNGIME;
				P5.y = lista_drept[k + 1].cord.y + sin((lista_drept[k + 1].ung * PI) / 180) * LATIME + sin(((90.0 - lista_drept[k + 1].ung) * PI) / 180) * LUNGIME;

				P6.x = lista_drept[k + 1].cord.x + cos(((90.0 - lista_drept[k + 1].ung) * PI) / 180) * LUNGIME;
				P6.y = lista_drept[k + 1].cord.y + sin(((90.0 - lista_drept[k + 1].ung) * PI) / 180) * LUNGIME;

				if (suprapunere(P1, P2, P3, lista_drept[k].cord, P4) || suprapunere(P4, P5, P6, lista_drept[k + 1].cord, P3))
				{
					lista_drept[k].ung -= rung * deltatime;
					lista_drept[k + 1].angr = 1;
				}
			}
		}
	}
	else
	{
		for (k = nrdominos - 1; k >= 0; k--)
		{
			if (lista_drept[k].angr && lista_drept[k].ung < 90.0)
			{
				lista_drept[k].ung += rung * deltatime;
			}

			if (k != 0)
			{
				P1.x = lista_drept[k].cord.x - cos(((90.0 - lista_drept[k].ung) * PI) / 180) * LUNGIME;
				P1.y = lista_drept[k].cord.y + sin(((90.0 - lista_drept[k].ung) * PI) / 180) * LUNGIME;

				P2.x = lista_drept[k].cord.x - cos(((90.0 - lista_drept[k].ung) * PI) / 180) * LUNGIME + cos((lista_drept[k].ung * PI) / 180) * LATIME;
				P2.y = lista_drept[k].cord.y + sin(((90.0 - lista_drept[k].ung) * PI) / 180) * LUNGIME + sin((lista_drept[k].ung * PI) / 180) * LATIME;

				P3.x = lista_drept[k].cord.x + cos((lista_drept[k].ung * PI) / 180) * LATIME;
				P3.y = lista_drept[k].cord.y + sin((lista_drept[k].ung * PI) / 180) * LATIME;

				P4.x = lista_drept[k - 1].cord.x - cos(((90.0 - lista_drept[k - 1].ung) * PI) / 180) * LUNGIME;
				P4.y = lista_drept[k - 1].cord.y + sin(((90.0 - lista_drept[k - 1].ung) * PI) / 180) * LUNGIME;

				P5.x = lista_drept[k - 1].cord.x - cos(((90.0 - lista_drept[k - 1].ung) * PI) / 180) * LUNGIME + cos((lista_drept[k - 1].ung * PI) / 180) * LATIME;
				P5.y = lista_drept[k - 1].cord.y + sin(((90.0 - lista_drept[k - 1].ung) * PI) / 180) * LUNGIME + sin((lista_drept[k - 1].ung * PI) / 180) * LATIME;

				P6.x = lista_drept[k - 1].cord.x + cos((lista_drept[k - 1].ung * PI) / 180) * LATIME;
				P6.y = lista_drept[k - 1].cord.y + sin((lista_drept[k - 1].ung * PI) / 180) * LATIME;

				if (suprapunere(P1, P2, P3, lista_drept[k].cord, P6) || suprapunere(P4, P5, P6, lista_drept[k - 1].cord, P1))
				{
					lista_drept[k].ung -= rung * deltatime;
					lista_drept[k - 1].angr = 1;
				}
			}
		}
	}
	glutPostRedisplay();
}

void deseneaza(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// desen masa
	glColor3f(0.65, 0.16, 0.16);
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glVertex2f(0.0, 200.0);
	glVertex2f(800.0, 200.0);
	glVertex2f(100.0, 0.0);
	glVertex2f(100.0, 200.0);
	glVertex2f(700.0, 0.0);
	glVertex2f(700.0, 200.0);
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//dominos
	for (k = 0; k < nrdominos; k++)
	{
		glPushMatrix();
		glTranslated(lista_drept[k].cord.x, lista_drept[k].cord.y, 0.0); // Vf, stivei: T
		if (dir) glTranslated(LATIME, 0.0, 0.0); // Vf, stivei: T
		if (dir) glRotated(lista_drept[k].ung, 0.0, 0.0, -1.0); // vf. stivei: T o R
		else glRotated(lista_drept[k].ung, 0.0, 0.0, 1.0);
		glScaled(LATIME, LUNGIME, 0.0);
		if (dir) glTranslated(-1.0, 0.0, 0.0);
		if (lista_drept[k].angr) glColor3f(1.0, 0.0, 0.0);
		else glColor3f(0.0, 1.0, 0.0);
		glCallList(unit);
		/*glVertex2f(lista_drept[k].x, lista_drept[k].y+lungime);
		glVertex2f(lista_drept[k].x+latime, lista_drept[k].y+lungime);
		glVertex2f(lista_drept[k].x+latime, lista_drept[k].y);
		glVertex2f(lista_drept[k].x, lista_drept[k].y);
		*/
		glPopMatrix();
	}

	//punctele de rotatie dreapta jos
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(5);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);

	for (k = 0; k < nrdominos; k++)
	{
		if (dir) glVertex2f(lista_drept[k].cord.x + LATIME, lista_drept[k].cord.y);
		else glVertex2f(lista_drept[k].cord.x, lista_drept[k].cord.y);
	}
	glEnd();

	glutSwapBuffers();
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-800.0, 800.0, -600.0, 600.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void miscad(void)
{
	i = i + alpha;
	if (i > 750.0)
		alpha = -1.0;
	else if (i < 0.0)
		alpha = 1.0;
	//j = j + ung;

	glutPostRedisplay();
}

void miscas(void)
{
	i = i + alpha;
	if (i < 0.0)
		alpha = 1.0;
	else if (i > 750.0)
		alpha = -1.0;
	//j = j + ung;

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			//ung = 15; alpha = -1.0; glutIdleFunc(miscas);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			//ung = 15; alpha = 1.0; glutIdleFunc(miscad);
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
		{
			//ung = 0.0;
		}
		break;
	default:
		break;
	}
}


void main(int argc, char** argv)
{

	
	lista_drept[0].cord.x = 200;
	lista_drept[0].cord.y = 200;
	lista_drept[1].cord.x = 300.3;
	lista_drept[1].cord.y = 200;
	lista_drept[2].cord.x = 400.6;
	lista_drept[2].cord.y = 200;
	lista_drept[3].cord.x = 500.9;
	lista_drept[3].cord.y = 200;
	
	/*
	lista_drept[0].cord.x = 200;
	lista_drept[0].cord.y = 200;
	lista_drept[1].cord.x = 210;
	lista_drept[1].cord.y = 200;
	lista_drept[2].cord.x = 250;
	lista_drept[2].cord.y = 200;
	lista_drept[3].cord.x = 300;
	lista_drept[3].cord.y = 200;
	*/

	lista_drept[nrdominos - 1].angr = 1;
	dir = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Domino2D");
	init();
	glutDisplayFunc(deseneaza);
	glutIdleFunc(update);
	//glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMainLoop();
}


