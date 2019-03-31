#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include <Gl\freeglut.h>

unsigned char prevKey;

using namespace std;

class GrilaCarteziana {
public:
	//GrilaCarteziana() : re(0.0), im(0.0) {}
	GrilaCarteziana(int nrlinii, int nrcoloane) {
		
		drawGrid(nrlinii, nrcoloane);


	}
	void drawGrid(int nrlinii, int nrcoloane)
	{
		ratie_x = (2.0 - 2 * padding) / (float)nrcoloane;
		ratie_y = (2.0 - 2 * padding) / (float)nrlinii;
		this->nrlinii = nrlinii;
		this->nrcoloane = nrcoloane;
		point_radius = (ratie_x < ratie_y) ? ratie_x / 3 : ratie_y / 3;
		coord_min = -1 + padding;
		coord_max = 1 - padding;
		glColor3d(0.0, 0.0, 0.0);

		glBegin(GL_LINES);
		for (double i = coord_min; i <= coord_max; i += ratie_x) {
			glVertex2d(coord_min, i);
			glVertex2d(coord_max, i);
		}
		for (double j = coord_min; j <= coord_max; j += ratie_x) {
			glVertex2d(j, coord_min);
			glVertex2d(j, coord_max);
		}
		glVertex2d(coord_min, coord_max);
		glVertex2d(coord_max, coord_max);
		glVertex2d(coord_max, coord_max);
		glVertex2d(coord_max, coord_min);
		glEnd();
	}
	void setOrigin(int x_origin, int y_origin) {
		this->x_origin = x_origin;
		this->y_origin = y_origin;
	}

	~GrilaCarteziana() {}
private: void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle

							 //GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * 3.14;
	glColor3f(0.3, 0.3, 0.3); // rosu
	glBegin(GL_POLYGON);
	glVertex2f(x, y); // center of circle
	for (i = triangleAmount; i >= 0; i--) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
}
public: void writePixel(int linie, int coloana) {
	if ((linie<0 || linie>nrlinii) || (coloana<0 || coloana>nrcoloane))
		return;
	double pixel_x, pixel_y;
	double coord_origin_x = getPixelPosition_x(this->x_origin),
		coord_origin_y = getPixelPosition_x(this->y_origin);

	pixel_x = coord_origin_x + ratie_x * coloana;   //aflam x-ul punctului de intersectie 
	pixel_y = coord_origin_y + ratie_y * linie;     //aflam y-ul punctului de intersectie


	drawFilledCircle(pixel_x, pixel_y, point_radius);

}
public: double getPixelPosition_x(int number) {
	return coord_min + number * ratie_x;
}
public: double getPixelPosition_y(int number) {
	return coord_min + number * ratie_y;
}
public: void drawRedLine(int x_start, int y_start, int x_end, int y_end) {

	double coord_origin_x = getPixelPosition_x(this->x_origin),
		coord_origin_y = getPixelPosition_x(this->y_origin);

	double x1 = coord_origin_x + ratie_x * x_start;
	double y1 = coord_origin_y + ratie_y * y_start;
	double x2 = coord_origin_x + ratie_x * x_end;
	double y2 = coord_origin_y + ratie_y * y_end;

	glLineWidth(3);
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
	glLineWidth(2);

}

public: void AfiseazaSegmentDreapta3(int x0, int y0, int xn, int yn, bool grosime)
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


private:
	int nrlinii, nrcoloane;
	int x_origin = 0, y_origin = 0;
	double padding = 0.1, coord_min, coord_max, ratie_x, ratie_y, point_radius;
};


void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(2);

	glPolygonMode(GL_FRONT, GL_LINE);
}


void Display1() {
	GrilaCarteziana grila = GrilaCarteziana(15, 15);

	grila.drawRedLine(0, 15, 15, 10);
	grila.AfiseazaSegmentDreapta3(0, 15, 15, 10, true);

	grila.drawRedLine(0, 0, 15, 7);
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
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
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


