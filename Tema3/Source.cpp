#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <Gl\freeglut.h>

using namespace std;
double dim_x, dim_y;
unsigned char prevKey;

class GrilaCarteziana {
private:
	int nrlinii, nrcoloane, x_origin = 0, y_origin = 0;
	double ratie_x, ratie_y, point_radius;
	double min_x, min_y, max_x, max_y;

	void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
		int triangleAmount = 20;
		GLfloat twicePi = 2.0f * 3.14;
		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_POLYGON);
		glVertex2f(x, y);
		for (int i = triangleAmount; i >= 0; i--) {
			float radiusx = radius * cos(i *  twicePi / triangleAmount);
			float radiusy = radius * sin(i * twicePi / triangleAmount);
			if (dim_x > dim_y) {
				radiusx *= dim_y / dim_x;
			}
			else {
				radiusy *= dim_x / dim_y;
			}
			float vx = x + radiusx;
			float vy = y + radiusy;
			glVertex2f(vx, vy);
		}

		glEnd();
	}

public:
	GrilaCarteziana() {}

	GrilaCarteziana(int nlinii, int ncoloane) {
		nrlinii = nlinii;
		nrcoloane = ncoloane;
		drawGrid(nrlinii, nrcoloane);
	}

	~GrilaCarteziana() {}

	void drawGrid(int nrlinii, int nrcoloane)
	{
		double padding = 0.1;
		min_x = -1 + padding;
		max_x = 1 - padding;
		min_y = -1 + padding;
		max_y = 1 - padding;
		ratie_x = (2.0 - 2 * padding) / (float)nrcoloane;
		ratie_y = (2.0 - 2 * padding) / (float)nrlinii;

		if (dim_x > dim_y)
		{
			ratie_x *= dim_y / dim_x;
			min_x *= dim_y / dim_x;
			max_x *= dim_y / dim_x;
		}
		else
		{
			ratie_y *= dim_x / dim_y;
			min_y *= dim_x / dim_y;
			max_y *= dim_x / dim_y;
		}

		point_radius = (ratie_x < ratie_y) ? ratie_y / 3 : ratie_x / 3;

		glColor3d(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		for (double i = min_y; i <= max_y + ratie_y * 0.1; i += ratie_y) {
			glVertex2d(min_x, i);
			glVertex2d(max_x, i);
		}
		for (double j = min_x; j <= max_x + ratie_x * 0.1; j += ratie_x) {
			glVertex2d(j, min_y);
			glVertex2d(j, max_y);
		}
		glEnd();
	}

	double getPixelPosition_x(int number) {
		return min_x + number * ratie_x;
	}

	double getPixelPosition_y(int number) {
		return min_y + number * ratie_y;
	}

	void writePixel(int linie, int coloana) {
		if ((linie<0 || linie>nrlinii) || (coloana<0 || coloana>nrcoloane)) {
			return;
		}
		double pixel_x = getPixelPosition_x(coloana);
		double pixel_y = getPixelPosition_y(linie);
		drawFilledCircle(pixel_x, pixel_y, point_radius);
	}

	void drawRedLine(int x_start, int y_start, int x_end, int y_end) {
		double x1 = getPixelPosition_x(x_start);
		double y1 = getPixelPosition_y(y_start);
		double x2 = getPixelPosition_x(x_end);
		double y2 = getPixelPosition_y(y_end);

		glLineWidth(3);
		glColor3f(1, 0.1, 0.1);
		glBegin(GL_LINES);
		glVertex2d(x1, y1);
		glVertex2d(x2, y2);
		glEnd();
		glLineWidth(2);
	}

	void AfiseazaSegmentDreapta3(int x0, int y0, int xn, int yn, bool grosime)
	{
		if (y0 < yn)
		{
			int dx = (xn - x0),
				dy = (yn - y0);
			int d = 2 * dy - dx, //variabila de decizie
				dE = 2 * dy,
				dNE = 2 * (dy - dx); //valoare decizie pixel NE
			int x = x0, y = y0;
			writePixel(y, x);
			if (grosime)
				writePixel(y, x + 1);
			while (x < xn)
			{
				if (d <= 0) //M deasupra dreptei
				{
					d += dE;
					x++;
				}
				else  //M sub dreapta
				{
					d += dNE;
					x++;
					y++;
				}
				writePixel(y, x);
				if (grosime)
				{
					writePixel(y, x + 1);
					writePixel(y, x - 1);
				}

			}
		}
		else
		{
			int dx = (xn - x0),
				dy = -1 * (yn - y0);
			int d = 2 * dy - dx,
				dE = 2 * dy,
				dNE = 2 * (dy - dx);
			int x = x0, y = y0;
			writePixel(y, x);
			if (grosime)
				writePixel(y - 1, x);
			while (x < xn)
			{
				if (d < 0)
				{
					d += dE;
					x++;
				}
				else
				{
					d += dNE;
					x++;
					y--;
				}
				writePixel(y, x);
				if (grosime)
				{
					writePixel(y + 1, x);
					writePixel(y - 1, x);
				}
			}
		}
	}
};

void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glLineWidth(2);
	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display1() {
	GrilaCarteziana grila = GrilaCarteziana(15, 15);

	grila.drawRedLine(0, 15, 15, 10);
	grila.drawRedLine(0, 0, 15, 7);

	grila.AfiseazaSegmentDreapta3(0, 15, 15, 10, true);
	grila.AfiseazaSegmentDreapta3(0, 0, 15, 7, false);
}

void Display(void) {
	switch (prevKey) {
	case '1':
		glClear(GL_COLOR_BUFFER_BIT);
		Display1();
		break;
	default:
		break;
	}
	glFlush();
}

void Reshape(int w, int h) {
	dim_x = double(w);
	dim_y = double(h);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) {
		exit(0);
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	int dim = 500;
	glutInit(&argc, argv);
	glutInitWindowSize(dim, dim);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow(argv[0]);
	Init();
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyboardFunc);
	glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
}
