#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <gl/freeglut.h>
#include <iostream>
#include <fstream>

using namespace std;

#define WIDTH 1920.0
#define HEIGHT 1080.0
#define TABLE_HEIGHT 400.0
#define TABLE_GAP 200.0
#define LUNGIME 100.0
#define LATIME 20.0
#define PI 3.141592654

struct punct
{
	double x;
	double y;
	punct()
	{
		x = 0.0;
		y = 0.0;
	}
} P1, P2, P3, P4, P5, P6, P7, P8;

struct domino
{
	punct cord;
	double ung = 0.0;
	bool angr = 0;
} *lista_drept, * lista_aux;

GLuint unit;
double rung = 70;
double fall = 100;
double err = 0.00001;
double deltatime;
double oldtime = 0.0;
double message_duration = 2.0;
double message1_time = -2.1;
double message2_time = -2.1;
double message3_time = -2.1;
double message4_time = -2.1;
double message5_time = -2.1;

int nrdominos = 0;

bool darkmode = 1;
bool started = 0;
bool dir = 1; //dreapta daca e 1, stanga daca e 0
int selected = -1; //-1 cand nu e selectat, pozitia cand e selectat

void glPrintf(int x, int y, char* sir, int lungime)
{
	if (darkmode) glColor3f(1.0, 1.0, 1.0);
	else glColor3f(0.0, 0.0, 0.0);
	for (int i = 0; i < lungime; i++)
	{
		glRasterPos2i(10 + x + i * 10, y - 20);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, sir[i]);
	}
}

void glPrintfErr(int x, int y, char* sir, int lungime)
{
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < lungime; i++)
	{
		glRasterPos2i(x + i * 15, y);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, sir[i]);
	}
}

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
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0, WIDTH, 0.0, HEIGHT, -1.0, 1.0);

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

void update(void)
{

	double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	deltatime = t - oldtime;
	oldtime = t;

	for (int i = 0; i < nrdominos; i++)
	{
		if (lista_drept[i].cord.y > TABLE_HEIGHT)
		{
			lista_drept[i].cord.y -= fall * deltatime;
		}
		if (lista_drept[i].cord.y < TABLE_HEIGHT) lista_drept[i].cord.y = TABLE_HEIGHT;
	}

	if (dir)
	{
		for (int i = 0; i < nrdominos; i++)
		{
			if (lista_drept[i].angr && lista_drept[i].ung < 90.0)
			{
				lista_drept[i].ung += rung * deltatime;
			}
			if (lista_drept[i].ung > 90.0) lista_drept[i].ung = 90.0;

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

	if (darkmode) glClearColor(0.0, 0.0, 0.0, 0.0);
	else glClearColor(1.0, 0.85, 0.73, 0.0);

	//legenda
	char sp[250];
	sprintf_s(sp, "%s", "LEGENDA:");
	glPrintf(0, HEIGHT, sp, strlen(sp));
	sprintf_s(sp, "%s", "F - Fisier Double Domino");
	glPrintf(0, HEIGHT - 20, sp, strlen(sp));
	sprintf_s(sp, "%s", "R - Reset");
	glPrintf(0, HEIGHT - 40, sp, strlen(sp));
	sprintf_s(sp, "%s", "Click Stanga - Adaugare domino");
	glPrintf(0, HEIGHT - 60, sp, strlen(sp));
	sprintf_s(sp, "%s", "Click Dreapta - Selectare domino start");
	glPrintf(0, HEIGHT - 80, sp, strlen(sp));
	sprintf_s(sp, "%s", "< - Rotirea dominoului selectat spre stanga");
	glPrintf(0, HEIGHT - 100, sp, strlen(sp));
	sprintf_s(sp, "%s", "> - Rotirea dominoului selectat spre dreapta");
	glPrintf(0, HEIGHT - 120, sp, strlen(sp));
	sprintf_s(sp, "%s", "M - Schimba Mode: Darkmode / Lightmode");
	glPrintf(0, HEIGHT - 140, sp, strlen(sp));

	//mesaje de eroare
	if (oldtime - message1_time < message_duration)
	{
		sprintf_s(sp, "%s", "Este nevoie de Reset (R) pentru a putea continua!");
		glPrintfErr(WIDTH - 15 * strlen(sp) - 35, HEIGHT - 50, sp, strlen(sp));
	}
	if (oldtime - message2_time < message_duration)
	{
		sprintf_s(sp, "%s", "Pozitie domino invalida!");
		glPrintfErr(WIDTH - 15 * strlen(sp) - 35, HEIGHT - 100, sp, strlen(sp));
	}
	if (oldtime - message3_time < message_duration)
	{
		sprintf_s(sp, "%s", "Nu exista niciun domino in acest loc!");
		glPrintfErr(WIDTH - 15 * strlen(sp) - 35, HEIGHT - 150, sp, strlen(sp));
	}
	if (oldtime - message4_time < message_duration)
	{
		sprintf_s(sp, "%s", "Trebuie selectat un domino inainte de alegerea directiei de rotatie!");
		glPrintfErr(WIDTH - 15 * strlen(sp) - 35, HEIGHT - 200, sp, strlen(sp));
	}
	if (oldtime - message5_time < message_duration)
	{
		sprintf_s(sp, "%s", "Asteptati sa fie toate dominourile pe masa pentru a incepe rotatia!");
		glPrintfErr(WIDTH - 15 * strlen(sp) - 35, HEIGHT - 250, sp, strlen(sp));
	}

	// desen masa
	glColor3f(0.65, 0.16, 0.16);
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glVertex2f(0.0, TABLE_HEIGHT);
	glVertex2f(WIDTH, TABLE_HEIGHT);
	glVertex2f(TABLE_GAP, 0.0);
	glVertex2f(TABLE_GAP, TABLE_HEIGHT);
	glVertex2f(WIDTH - TABLE_GAP, 0.0);
	glVertex2f(WIDTH - TABLE_GAP, TABLE_HEIGHT);
	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//dominos
	for (int i = 0; i < nrdominos; i++)
	{
		glPushMatrix();
		glTranslated(lista_drept[i].cord.x, lista_drept[i].cord.y, 0.0);
		if (dir) glTranslated(LATIME, 0.0, 0.0);
		if (dir) glRotated(lista_drept[i].ung, 0.0, 0.0, -1.0);
		else glRotated(lista_drept[i].ung, 0.0, 0.0, 1.0);
		glScaled(LATIME, LUNGIME, 0.0);
		if (dir) glTranslated(-1.0, 0.0, 0.0);
		if (lista_drept[i].angr) glColor3f(1.0, 0.0, 0.0);
		else if (i == selected) glColor3f(1.0, 0.5, 0.0);
		else glColor3f(0.0, 1.0, 0.0);
		glCallList(unit);
		glPopMatrix();
	}

	//punctele de rotatie
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

void mouse(int button, int state, int x, int y)
{
	double X = x * WIDTH / glutGet(GLUT_WINDOW_WIDTH);
	double Y = HEIGHT - y * HEIGHT / glutGet(GLUT_WINDOW_HEIGHT); // pt ca avem originea in stanga jos
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			if (started)
			{
				message1_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
				break;
			}
			bool ok = 1;
			if (Y < TABLE_HEIGHT)
			{
				message2_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
				break;
			}
			for (int i = 0; i < nrdominos; i++)
			{
				if (abs(lista_drept[i].cord.x - X) < 2 * LATIME) ok = 0;
			}
			if (!ok)
			{
				message2_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
				break;
			}
			nrdominos++;
			lista_aux = new domino[nrdominos];
			for (int i = 0; i < nrdominos - 1; i++)
			{
				if (lista_drept[i].cord.x < X) lista_aux[i] = lista_drept[i];
				if (ok && lista_drept[i].cord.x > X)
				{
					if (i <= selected) selected++;
					lista_aux[i].cord.x = X;
					lista_aux[i].cord.y = Y; // dominouri in aer
					ok = 0;
				}
				if (!ok && lista_drept[i].cord.x > X) lista_aux[i + 1] = lista_drept[i];
			}
			if (ok)
			{
				lista_aux[nrdominos - 1].cord.x = X;
				lista_aux[nrdominos - 1].cord.y = Y;
			}
			delete[] lista_drept;
			lista_drept = new domino[nrdominos];
			for (int i = 0; i < nrdominos; i++)
			{
				lista_drept[i] = lista_aux[i];
			}
			delete[] lista_aux;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			if (started)
			{
				message1_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
				break;
			}
			bool ok = 0;
			for (int i = 0; i < nrdominos; i++)
			{
				P1.x = lista_drept[i].cord.x;
				P1.y = lista_drept[i].cord.y + LUNGIME;

				P2.x = lista_drept[i].cord.x + LATIME;
				P2.y = lista_drept[i].cord.y + LUNGIME;

				P3.x = lista_drept[i].cord.x + LATIME;
				P3.y = lista_drept[i].cord.y;

				P4.x = X;
				P4.y = Y;

				if (suprapunere(lista_drept[i].cord, P1, P2, P3, P4))
				{
					selected = i;
					ok = 1;
				}
			}
			if (!ok)
			{
				message3_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
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
	case 'R':
		started = 0;
		selected = -1;
		nrdominos = 0;
		delete[] lista_drept;
		lista_drept = new domino[nrdominos];
		break;
	case 'f':
	case 'F':
	{
		ifstream F("double_domino.txt");
		F >> nrdominos;
		started = 0;
		selected = -1;
		delete[] lista_drept;
		lista_drept = new domino[nrdominos];
		for (int i = 0; i < nrdominos; i++)
		{
			F >> lista_drept[i].cord.x;
			lista_drept[i].cord.y = TABLE_HEIGHT;
		}
		F.close();
		break;
	}
	case 'm':
	case 'M':
		darkmode = !darkmode;
		break;
	default:
		break;
	}
}
void special(int key, int x, int y)
{
	bool grounded = 1;
	for (int i = 0; i < nrdominos; i++)
	{
		if (lista_drept[i].cord.y != TABLE_HEIGHT)
		{
			grounded = 0;
			break;
		}
	}
	switch (key) {
	case GLUT_KEY_LEFT:
		if (started)
		{
			message1_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			break;
		}
		if (selected == -1)
		{
			message4_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			break;
		}
		if (!grounded)
		{
			message5_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			break;
		}
		dir = 0;
		lista_drept[selected].angr = 1;
		started = 1;
		break;
	case GLUT_KEY_RIGHT:
		if (started)
		{
			message1_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			break;
		}
		if (selected == -1)
		{
			message4_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			break;
		}
		if (!grounded)
		{
			message5_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			break;
		}
		dir = 1;
		lista_drept[selected].angr = 1;
		started = 1;
		break;
	default:
		break;
	}
}


void main(int argc, char** argv)
{
	lista_drept = new domino[nrdominos];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Domino2D");
	init();
	glutDisplayFunc(deseneaza);
	glutIdleFunc(update);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();

	delete[] lista_drept;
}


