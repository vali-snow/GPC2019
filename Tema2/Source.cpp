#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include <float.h>

#include <GL/freeglut.h>

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
	GLfloat xmax, ymax, xmin, ymin;
	GLfloat a = 1, b = 2;
	GLfloat pi = 4 * (GLfloat)atan(1.0);
	GLfloat ratio = (GLfloat)0.05;
	GLfloat t;

	// calculul valorilor maxime/minime ptr. x si y
	// aceste valori vor fi folosite ulterior la scalare
	xmax = a - b - 1;
	xmin = a + b + 1;
	ymax = ymin = 0;
	for (t = -pi / 2 + ratio; t < pi / 2; t += ratio) {
		GLfloat x1, y1, x2, y2;
		x1 = a + b * (GLfloat)cos(t);
		xmax = (xmax < x1) ? x1 : xmax;
		xmin = (xmin > x1) ? x1 : xmin;

		x2 = a - b * (GLfloat)cos(t);
		xmax = (xmax < x2) ? x2 : xmax;
		xmin = (xmin > x2) ? x2 : xmin;

		y1 = a * (GLfloat)tan(t) + b * (GLfloat)sin(t);
		ymax = (ymax < y1) ? y1 : ymax;
		ymin = (ymin > y1) ? y1 : ymin;

		y2 = a * (GLfloat)tan(t) - b * (GLfloat)sin(t);
		ymax = (ymax < y2) ? y2 : ymax;
		ymin = (ymin > y2) ? y2 : ymin;
	}

	xmax = ((GLfloat)fabs(xmax) > (GLfloat)fabs(xmin)) ? (GLfloat)fabs(xmax) : (GLfloat)fabs(xmin);
	ymax = ((GLfloat)fabs(ymax) > (GLfloat)fabs(ymin)) ? (GLfloat)fabs(ymax) : (GLfloat)fabs(ymin);

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f((GLfloat)1, (GLfloat)0.1, (GLfloat)0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratio; t < pi / 2; t += ratio) {
		GLfloat x1, y1, x2, y2;
		x1 = (a + b * (GLfloat)cos(t)) / xmax;
		x2 = (a - b * (GLfloat)cos(t)) / xmax;
		y1 = (a * (GLfloat)tan(t) + b * (GLfloat)sin(t)) / ymax;
		y2 = (a * (GLfloat)tan(t) - b * (GLfloat)sin(t)) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratio; t < pi / 2; t += ratio) {
		GLfloat x1, y1, x2, y2;
		x1 = (a + b * (GLfloat)cos(t)) / xmax;
		x2 = (a - b * (GLfloat)cos(t)) / xmax;
		y1 = (a * (GLfloat)tan(t) + b * (GLfloat)sin(t)) / ymax;
		y2 = (a * (GLfloat)tan(t) - b * (GLfloat)sin(t)) / ymax;

		glVertex2f(x2, y2);
	}
	glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
	GLfloat pi = 4 * (GLfloat)atan(1.0);
	GLfloat xmax = 8 * pi;
	GLfloat ymax = (GLfloat)exp(1.1);
	GLfloat ratio = (GLfloat)0.05;

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f((GLfloat)1, (GLfloat)0.1, (GLfloat)0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (GLfloat x = 0; x < xmax; x += ratio) {
		GLfloat x1, y1;
		x1 = x / xmax;
		y1 = ((GLfloat)fabs(sin(x)) * (GLfloat)exp(-sin(x))) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();
}

void Display3() {
	GLfloat pi = 4 * (GLfloat)atan(1.0);
	GLfloat ratio = 0.01f;
	GLfloat xmax = 100, ymax = 1;

	xmax = xmax * 1.05f;
	ymax = ymax * 1.05f;

	glColor3f(1, 0.1f, 0.1f); // rosu
	glBegin(GL_LINE_STRIP);
	for (GLfloat xi = 0; xi <= 100; xi += ratio) {
		GLfloat x, y;
		if (xi == 0) {
			x = xi / xmax;
			y = 1 / ymax;
		}
		else {
			x = xi / xmax;
			y = (GLfloat)fabs(xi - (round(xi))) / xi / ymax;
		}
		glVertex2f(x, y);
	}
	glEnd();
}

// Melcul Lui Pascal
void Display4() {
	GLfloat pi = 4 * (GLfloat)atan(1.0);
	GLfloat ratio = 0.01f;
	GLfloat a = 0.3f, b = 0.2f;
	GLfloat xmax = FLT_MIN, ymax = FLT_MIN, xmin = FLT_MAX, ymin = FLT_MAX;

	for (GLfloat t = -pi; t < pi; t += ratio) {
		GLfloat e = 2 * (a * (GLfloat)cos(t) + b);
		GLfloat xt = e * (GLfloat)cos(t);
		if (xmax < xt) { xmax = xt; }
		if (xmin > xt) { xmin = xt; }

		GLfloat yt = e * (GLfloat)sin(t);
		if (ymax < yt) { ymax = yt; }
		if (ymin > yt) { ymin = yt; }
	}

	xmax = (GLfloat)fabs((fabs(xmax) > fabs(xmin)) ? xmax : xmin) * 1.05f;
	ymax = (GLfloat)fabs((fabs(ymax) > fabs(ymin)) ? ymax : ymin) * 1.05f;

	glColor3f(1, 0.1f, 0.1f); // rosu
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = -pi; t < pi; t += ratio) {
		GLfloat e = 2 * (a * (GLfloat)cos(t) + b);
		GLfloat x = e * (GLfloat)cos(t) / xmax;
		GLfloat y = e * (GLfloat)sin(t) / ymax;
		glVertex2d(x, y);
	}
	glEnd();
}

void Display5() {
	//too hard
}

//cicloida
void Display6() {
	GLfloat pi = 4 * (GLfloat)atan(1.0);
	GLfloat ratio = 0.01f;
	GLfloat a = 0.1f, b = 0.2f;
	GLfloat xmax = FLT_MIN, ymax = FLT_MIN, xmin = FLT_MAX, ymin = FLT_MAX;

	for (GLfloat t = -13; t <= 13; t += ratio) {
		GLfloat xt = -a * t - b * (GLfloat)sin(t);
		if (xmax < xt) { xmax = xt; }
		if (xmin > xt) { xmin = xt; }

		GLfloat yt = -a - b * (GLfloat)cos(t);
		if (ymax < yt) { ymax = yt; }
		if (ymin > yt) { ymin = yt; }
	}

	xmax = (GLfloat)fabs((fabs(xmax) > fabs(xmin)) ? xmax : xmin) * 1.05f;
	ymax = -(GLfloat)fabs((fabs(ymax) > fabs(ymin)) ? ymax : ymin) * 1.75f; // y inversed

	glColor3f(1, 0.1f, 0.1f); // rosu
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = -13; t <= 13; t += ratio) {
		GLfloat x = (-a * t - b * (GLfloat)sin(t)) / xmax;
		GLfloat y = (-a - b * (GLfloat)cos(t)) / ymax;
		glVertex2d(x, y);
	}
	glEnd();
}

//epicicloida
void Display7() {
	GLfloat pi = 4 * (GLfloat)atan(1.0);
	GLfloat R = 0.1f, r = 0.3f;
	GLfloat ratio = 0.05f;
	GLfloat xmax = FLT_MIN, ymax = FLT_MIN, xmin = FLT_MAX, ymin = FLT_MAX;

	for (GLfloat t = 0; t < 2 * pi; t += ratio) {
		GLfloat xt = (R + r) * (GLfloat)cos((r / R) * t) - r * (GLfloat)cos(t + (r / R)*t);
		if (xmax < xt) { xmax = xt; }
		if (xmin > xt) { xmin = xt; }

		GLfloat yt = (R + r) * (GLfloat)sin((r / R) * t) - r * (GLfloat)sin(t + (r / R)*t);
		if (ymax < yt) { ymax = yt; }
		if (ymin > yt) { ymin = yt; }
	}

	xmax = (GLfloat)fabs((fabs(xmax) > fabs(xmin)) ? xmax : xmin) * 1.1f;
	ymax = (GLfloat)fabs((fabs(ymax) > fabs(ymin)) ? ymax : ymin) * 1.1f;

	glColor3f(1, 0.1f, 0.1f); // rosu
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = 0; t < 2 * pi; t += ratio) {
		GLfloat x = ((R + r) * (GLfloat)cos((r / R) * t) - r * (GLfloat)cos(t + (r / R)*t)) / xmax;
		GLfloat y = ((R + r) * (GLfloat)sin((r / R) * t) - r * (GLfloat)sin(t + (r / R)*t)) / ymax;
		glVertex2f(x, y);
	}
	glEnd();
}

//hipocicloida
void Display8() {
	GLfloat pi = 4 * (GLfloat)atan(1.0);
	GLfloat R = 0.1f, r = 0.3f;
	GLfloat ratio = 0.05f;
	GLfloat xmax = FLT_MIN, ymax = FLT_MIN, xmin = FLT_MAX, ymin = FLT_MAX;

	for (GLfloat t = 0; t <= 2 * pi; t += ratio) {
		GLfloat xt = (R - r) * (GLfloat)cos((r / R) * t) - r * (GLfloat)cos(t - (r / R)*t);
		if (xmax < xt) { xmax = xt; }
		if (xmin > xt) { xmin = xt; }

		GLfloat yt = (R - r) * (GLfloat)sin((r / R) * t) - r * (GLfloat)sin(t - (r / R)*t);
		if (ymax < yt) { ymax = yt; }
		if (ymin > yt) { ymin = yt; }
	}

	xmax = (GLfloat)fabs((fabs(xmax) > fabs(xmin)) ? xmax : xmin) * 1.75f;
	ymax = (GLfloat)fabs((fabs(ymax) > fabs(ymin)) ? ymax : ymin) * 1.75f;

	glColor3f(1, 0.1f, 0.1f); // rosu
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = 0; t <= 2 * pi; t += ratio) {
		GLfloat x = ((R - r) * (GLfloat)cos((r / R) * t) - r * (GLfloat)cos(t - (r / R)*t)) / xmax;
		GLfloat y = ((R - r) * (GLfloat)sin((r / R) * t) - r * (GLfloat)sin(t - (r / R)*t)) / ymax;
		glVertex2f(x, y);
	}
	glEnd();
}
void Display9() {
	GLfloat pi = 4 * (GLfloat)atan(1.0);
	GLfloat a = 0.04f;
	const GLfloat eConst = (GLfloat)M_E;
	GLfloat ratio = 0.001f;
	GLfloat xmax = FLT_MIN, ymax = FLT_MIN, xmin = FLT_MAX, ymin = FLT_MAX;

	for (GLfloat t = -pi / 4 + ratio; t < pi / 4; t += ratio) {
		GLfloat r1 = a * (GLfloat)sqrt(2 * cos(2 * t));
		GLfloat xt1 = r1 * (GLfloat)cos(t);
		if (xmax < xt1) { xmax = xt1; }
		if (xmin > xt1) { xmin = xt1; }
		GLfloat yt1 = r1 * (GLfloat)sin(t);
		if (ymax < yt1) { ymax = yt1; }
		if (ymin > yt1) { ymin = yt1; }

		GLfloat r2 = (-a) * (GLfloat)sqrt(2 * cos(2 * t));
		GLfloat xt2 = r2 * (GLfloat)cos(t);
		if (xmax < xt2) { xmax = xt2; }
		if (xmin > xt2) { xmin = xt2; }
		GLfloat yt2 = r2 * (GLfloat)sin(t);
		if (ymax < yt2) { ymax = yt2; }
		if (ymin > yt2) { ymin = yt2; }
	}

	xmax = (GLfloat)fabs((fabs(xmax) > fabs(xmin)) ? xmax : xmin) * 1.75f;
	ymax = (GLfloat)fabs((fabs(ymax) > fabs(ymin)) ? ymax : ymin) * 1.75f;

	glColor3f(1, 0.1f, 0.1f); // rosu
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = -pi / 4 + ratio; t < pi / 4; t += ratio) {
		GLfloat r1 = a * (GLfloat)sqrt(2 * cos(2 * t));
		GLfloat x = r1 * (GLfloat)cos(t) / xmax;
		GLfloat y = r1 * (GLfloat)sin(t) / ymax;
		glVertex2f(x, y);
	}
	for (GLfloat t = pi / 4 - ratio; t > - pi / 4; t -= ratio) {
		GLfloat r2 = (-a) * (GLfloat)sqrt(2 * cos(2 * t));
		GLfloat x = r2 * (GLfloat)cos(t) / xmax;
		GLfloat y = r2 * (GLfloat)sin(t) / ymax;
		glVertex2f(x, y);
	}
	glEnd();
}
void Display10() {
	GLfloat pi = (GLfloat)M_PI;
	GLfloat a = 0.02f;
	GLfloat e = (GLfloat)M_E;
	GLfloat ratio = 0.001f;
	GLfloat xmax = FLT_MIN, ymax = FLT_MIN, xmin = FLT_MAX, ymin = FLT_MAX;

	for (GLfloat t = 0; t < pi; t += ratio) {
		GLfloat r = a * (GLfloat)pow(e, 1+t);
		GLfloat xt = r * (GLfloat)cos(t);
		if (xmax < xt) { xmax = xt; }
		if (xmin > xt) { xmin = xt; }
		GLfloat yt = r * (GLfloat)sin(t);
		if (ymax < yt) { ymax = yt; }
		if (ymin > yt) { ymin = yt; }
	}

	xmax = (GLfloat)fabs((fabs(xmax) > fabs(xmin)) ? xmax : xmin) * 1.05f;
	ymax = (GLfloat)fabs((fabs(ymax) > fabs(ymin)) ? ymax : ymin) * 1.75f;

	glColor3f(1, 0.1f, 0.1f); // rosu
	glBegin(GL_LINE_STRIP);
	for (GLfloat t = 0; t < pi; t += ratio) {
		GLfloat r = a * (GLfloat)pow(e, 1 + t);
		GLfloat x = r * (GLfloat)cos(t) / xmax;
		GLfloat y = r * (GLfloat)sin(t) / ymax;
		glVertex2f(x, y);
	}
	glEnd();
}

void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	//   glPointSize(4);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		Display1();
		break;
	case '2':
		Display2();
		break;
	case '3':
		Display3();
		break;
	case '4':
		Display4();
		break;
	case '5':
		Display5();
		break;
	case '6':
		Display6();
		break;
	case '7':
		Display7();
		break;
	case '8':
		Display8();
		break;
	case '9':
		Display9();
		break;
	case '0':
		Display10();
		break;
	default:
		break;
	}

	glFlush();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}