#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <gl/freeglut.h>
#include <iostream>
#include <fstream>

using namespace std;

#define LUNGIME 100.0
#define LATIME 20.0
#define TABLE_HEIGHT 200.0
#define PI 3.141592654

struct punct
{
	double x;
	double y;
} P1, P2, P3, P4, P5, P6, P7, P8;

struct domino
{
	punct cord;
	double ung = 0.0;
	bool angr = 0;
};

bool dir = 1; //dreapta daca e 1, stanga daca e 0
int selected = -1; //-1 cand nu e selectat, pozitia cand e selectat

GLuint unit;
domino* lista_drept;
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

int comp(const void* a, const void* b)
{
	domino* A = (domino*)a;
	domino* B = (domino*)b;
	return A->cord.x - B->cord.x;
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
		for (int i = 0; i < nrdominos; i++)
		{
			if (lista_drept[i].angr && lista_drept[i].ung < 90.0)
			{
				lista_drept[i].ung += rung * deltatime;
			}

			if (i != nrdominos - 1)
			{
				P1.x = lista_drept[i].cord.x + LATIME - cos((lista_drept[i].ung * PI) / 180) * LATIME;
				P1.y = lista_drept[i].cord.y + sin((lista_drept[i].ung * PI) / 180) * LATIME;

				P2.x = lista_drept[i].cord.x + LATIME - cos((lista_drept[i].ung * PI) / 180) * LATIME + cos(((90.0 - lista_drept[i].ung) * PI) / 180) * LUNGIME;
				P2.y = lista_drept[i].cord.y + sin((lista_drept[i].ung * PI) / 180) * LATIME + sin(((90.0 - lista_drept[i].ung) * PI) / 180) * LUNGIME;

				P3.x = lista_drept[i].cord.x + LATIME + cos(((90.0 - lista_drept[i].ung) * PI) / 180) * LUNGIME;
				P3.y = lista_drept[i].cord.y + sin(((90.0 - lista_drept[i].ung) * PI) / 180) * LUNGIME;

				P4.x = lista_drept[i].cord.x + LATIME;
				P4.y = lista_drept[i].cord.y;

				P5.x = lista_drept[i + 1].cord.x + LATIME - cos((lista_drept[i + 1].ung * PI) / 180) * LATIME;
				P5.y = lista_drept[i + 1].cord.y + sin((lista_drept[i + 1].ung * PI) / 180) * LATIME;

				P6.x = lista_drept[i + 1].cord.x + LATIME - cos((lista_drept[i + 1].ung * PI) / 180) * LATIME + cos(((90.0 - lista_drept[i + 1].ung) * PI) / 180) * LUNGIME;
				P6.y = lista_drept[i + 1].cord.y + sin((lista_drept[i + 1].ung * PI) / 180) * LATIME + sin(((90.0 - lista_drept[i + 1].ung) * PI) / 180) * LUNGIME;

				P7.x = lista_drept[i + 1].cord.x + LATIME + cos(((90.0 - lista_drept[i + 1].ung) * PI) / 180) * LUNGIME;
				P7.y = lista_drept[i + 1].cord.y + sin(((90.0 - lista_drept[i + 1].ung) * PI) / 180) * LUNGIME;

				P8.x = lista_drept[i + 1].cord.x + LATIME;
				P8.y = lista_drept[i + 1].cord.y;

				if (suprapunere(P1, P2, P3, P4, P5) || suprapunere(P5, P6, P7, P8, P3))
				{
					lista_drept[i].ung -= rung * deltatime;
					lista_drept[i + 1].angr = 1;
				}
			}
		}
	}
	else
	{
		for (int i = nrdominos - 1; i >= 0; i--)
		{
			if (lista_drept[i].angr && lista_drept[i].ung < 90.0)
			{
				lista_drept[i].ung += rung * deltatime;
			}

			if (i != 0)
			{
				P1.x = lista_drept[i].cord.x - cos(((90.0 - lista_drept[i].ung) * PI) / 180) * LUNGIME;
				P1.y = lista_drept[i].cord.y + sin(((90.0 - lista_drept[i].ung) * PI) / 180) * LUNGIME;

				P2.x = lista_drept[i].cord.x - cos(((90.0 - lista_drept[i].ung) * PI) / 180) * LUNGIME + cos((lista_drept[i].ung * PI) / 180) * LATIME;
				P2.y = lista_drept[i].cord.y + sin(((90.0 - lista_drept[i].ung) * PI) / 180) * LUNGIME + sin((lista_drept[i].ung * PI) / 180) * LATIME;

				P3.x = lista_drept[i].cord.x + cos((lista_drept[i].ung * PI) / 180) * LATIME;
				P3.y = lista_drept[i].cord.y + sin((lista_drept[i].ung * PI) / 180) * LATIME;

				P4.x = lista_drept[i - 1].cord.x - cos(((90.0 - lista_drept[i - 1].ung) * PI) / 180) * LUNGIME;
				P4.y = lista_drept[i - 1].cord.y + sin(((90.0 - lista_drept[i - 1].ung) * PI) / 180) * LUNGIME;

				P5.x = lista_drept[i - 1].cord.x - cos(((90.0 - lista_drept[i - 1].ung) * PI) / 180) * LUNGIME + cos((lista_drept[i - 1].ung * PI) / 180) * LATIME;
				P5.y = lista_drept[i - 1].cord.y + sin(((90.0 - lista_drept[i - 1].ung) * PI) / 180) * LUNGIME + sin((lista_drept[i - 1].ung * PI) / 180) * LATIME;

				P6.x = lista_drept[i - 1].cord.x + cos((lista_drept[i - 1].ung * PI) / 180) * LATIME;
				P6.y = lista_drept[i - 1].cord.y + sin((lista_drept[i - 1].ung * PI) / 180) * LATIME;

				if (suprapunere(P1, P2, P3, lista_drept[i].cord, P6) || suprapunere(P4, P5, P6, lista_drept[i - 1].cord, P1))
				{
					lista_drept[i].ung -= rung * deltatime;
					lista_drept[i - 1].angr = 1;
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
	glVertex2f(0.0, TABLE_HEIGHT);
	glVertex2f(800.0, TABLE_HEIGHT);
	glVertex2f(100.0, 0.0);
	glVertex2f(100.0, TABLE_HEIGHT);
	glVertex2f(700.0, 0.0);
	glVertex2f(700.0, TABLE_HEIGHT);
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//dominos
	for (int i = 0; i < nrdominos; i++)
	{
		glPushMatrix();
		glTranslated(lista_drept[i].cord.x, lista_drept[i].cord.y, 0.0); // Vf, stivei: T
		if (dir) glTranslated(LATIME, 0.0, 0.0); // Vf, stivei: T
		if (dir) glRotated(lista_drept[i].ung, 0.0, 0.0, -1.0); // vf. stivei: T o R
		else glRotated(lista_drept[i].ung, 0.0, 0.0, 1.0);
		glScaled(LATIME, LUNGIME, 0.0);
		if (dir) glTranslated(-1.0, 0.0, 0.0);
		if (lista_drept[i].angr) glColor3f(1.0, 0.0, 0.0);
		else if (i == selected) glColor3f(1.0, 0.5, 0.0);
		else glColor3f(0.0, 1.0, 0.0);
		glCallList(unit);
		/*glVertex2f(lista_drept[i].x, lista_drept[i].y+lungime);
		glVertex2f(lista_drept[i].x+latime, lista_drept[i].y+lungime);
		glVertex2f(lista_drept[i].x+latime, lista_drept[i].y);
		glVertex2f(lista_drept[i].x, lista_drept[i].y);
		*/
		glPopMatrix();
	}

	//punctele de rotatie dreapta jos
	glColor3f(0.0, 0.0, 1.0);
	glPointSize(5);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);

	for (int i = 0; i < nrdominos; i++)
	{
		if (lista_drept[i].angr)
		{
			if (dir) glVertex2f(lista_drept[i].cord.x + LATIME, lista_drept[i].cord.y);
			else glVertex2f(lista_drept[i].cord.x, lista_drept[i].cord.y);
		}
	}
	glEnd();

	glutSwapBuffers();
}

/*
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
*/
void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			nrdominos++;
			realloc(lista_drept, nrdominos);
			lista_drept[nrdominos - 1].cord.x = x;
			lista_drept[nrdominos - 1].cord.y = TABLE_HEIGHT;
			qsort(lista_drept, nrdominos, sizeof(domino), comp);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			for (int i = 0; i < nrdominos; i++)
			{
				P1.x = lista_drept[i].cord.x;
				P1.y = lista_drept[i].cord.y + LUNGIME;

				P2.x = lista_drept[i].cord.x + LATIME;
				P2.y = lista_drept[i].cord.y + LUNGIME;

				P3.x = lista_drept[i].cord.x + LATIME;
				P3.y = lista_drept[i].cord.y;

				P4.x = x;
				P4.y = 600 - y;

				if (suprapunere(lista_drept[i].cord, P1, P2, P3, P4))
				{
					selected = i;
					cout << i << endl;
				}
			}
		}
		break;
	default:
		break;
	}
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
		nrdominos = 0;
		delete[] lista_drept;
		lista_drept = new domino[nrdominos];
		break;
	default:
		break;
	}
}
void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		dir = 0;
		lista_drept[selected].angr = 1;
		break;
	case GLUT_KEY_RIGHT:
		dir = 1;
		lista_drept[selected].angr = 1;
		break;
	default:
		break;
	}
}


void main(int argc, char** argv)
{
	lista_drept = new domino[nrdominos];
	
	
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

	lista_drept[nrdominos - 1].angr = 1;
	dir = 0;
	*/
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
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();
}


