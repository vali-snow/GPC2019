#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include <GL/freeglut.h>

// dimensiunea ferestrei in pixeli
#define dim 300
// numarul maxim de iteratii pentru testarea apartenentei la mult.Julia-Fatou
#define NRITER_JF 5000
// modulul maxim pentru testarea apartenentei la mult.Julia-Fatou
#define MODMAX_JF 10000000
// ratii ptr. CJuliaFatou
#define RX_JF 0.01
#define RY_JF 0.01

unsigned char prevKey;
int nivel = 0;

#pragma region JuliaF atou
class CComplex {
public:
	CComplex() : re(0.0), im(0.0) {}
	CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
	CComplex(const CComplex &c) : re(c.re), im(c.im) {}
	~CComplex() {}

	CComplex &operator=(const CComplex &c)
	{
		re = c.re;
		im = c.im;
		return *this;
	}

	double getRe() { return re; }
	void setRe(double re1) { re = re1; }

	double getIm() { return im; }
	void setIm(double im1) { im = im1; }

	double getModul() { return sqrt(re * re + im * im); }

	int operator==(CComplex &c1)
	{
		return ((re == c1.re) && (im == c1.im));
	}

	CComplex pow2()
	{
		CComplex rez;
		rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
		rez.im = 2.0 * re * im;
		return rez;
	}

	friend CComplex operator+(const CComplex &c1, const CComplex &c2);
	friend CComplex operator*(CComplex &c1, CComplex &c2);

	void print(FILE *f)
	{
		fprintf(f, "%.20f%+.20f i", re, im);
	}

private:
	double re, im;
};

CComplex operator+(const CComplex &c1, const CComplex &c2)
{
	CComplex rez(c1.re + c2.re, c1.im + c2.im);
	return rez;
}

CComplex operator*(CComplex &c1, CComplex &c2)
{
	CComplex rez(c1.re * c2.re - c1.im * c2.im,
		c1.re * c2.im + c1.im * c2.re);
	return rez;
}

class CJuliaFatou {
public:
	CJuliaFatou(bool Manderbolt = false, bool Reversed = false)
	{
		//     m.c se initializeaza implicit cu 0+0i
		m.manderbolt = Manderbolt;
		m.reversed = Reversed;
		m.nriter = NRITER_JF;
		m.modmax = MODMAX_JF;
	}

	CJuliaFatou(CComplex &c)
	{
		m.c = c;
		m.manderbolt = false;
		m.reversed = false;
		m.nriter = NRITER_JF;
		m.modmax = MODMAX_JF;
	}

	~CJuliaFatou() {}

	void setmodmax(double v) { assert(v <= MODMAX_JF); m.modmax = v; }
	double getmodmax() { return m.modmax; }

	void setnriter(int v) { assert(v <= NRITER_JF); m.nriter = v; }
	int getnriter() { return m.nriter; }

	//   testeaza daca x apartine multimii Julia-Fatou Jc
	//   returneaza 0 daca apartine, -1 daca converge finit, +1 daca converge infinit
	int isIn(CComplex &x)
	{
		int rez = 0;
		int i;
		//     tablou in care vor fi memorate valorile procesului iterativ z_n+1 = z_n * z_n + c
		CComplex z0, z1;

		z0 = x;
		for (i = 1; i < m.nriter; i++)
		{
			if (m.manderbolt)
			{
				z1 = z0 * z0 + x;
			}
			else
			{
				z1 = z0 * z0 + m.c;
				if (z1 == z0)
				{
					// x nu apartine m.J-F deoarece procesul iterativ converge finit
					rez = -1;
					break;
				}
			}
			if (z1.getModul() > m.modmax)
			{
				// x nu apartine m.J-F deoarece procesul iterativ converge la infinit
				rez = 1;
				break;
			}
			z0 = z1;
		}

		if (m.manderbolt && m.reversed)
		{
			if (i < 5)
			{
				glColor3f(1.0, 0.1, 0.1);
			}
			else if (i < 10)
			{
				glColor3f(0.1, 1.0, 0.1);
			}
			else if (i < 15)
			{
				glColor3f(0.1, 0.1, 1.0);
			}
			else
			{
				glColor3f(0.1, 0.1, 0.1);
			}
		}
		return rez;
	}

	//   afisarea multimii J-F care intersecteaza multimea argument
	void display(double xmin, double ymin, double xmax, double ymax)
	{
		glPushMatrix();
		glLoadIdentity();

		glTranslated((xmin + xmax) * 1.0 / (xmin - xmax), (ymin + ymax)  * 1.0 / (ymin - ymax), 0);
		glScaled(1.0 / (xmax - xmin), 1.0 / (ymax - ymin), 1);
		//     afisarea propriu-zisa
		glBegin(GL_POINTS);
		for (double x = xmin; x <= xmax; x += RX_JF)
			for (double y = ymin; y <= ymax; y += RY_JF)
			{
				CComplex z(x, y);
				int r = isIn(z);
				//z.print(stdout);
				if (m.manderbolt && m.reversed)
				{
					if (r != 0)
					{
						glVertex3d(x, y, 0);
					}
				}
				else
				{
					if (r == 0)
					{
						//fprintf(stdout, "   \n");
						glVertex3d(x, y, 0);
					}
					else if (r == -1)
					{
						//fprintf(stdout, "   converge finit\n");
					}
					else if (r == 1)
					{
						//fprintf(stdout, "   converge infinit\n");
					}
				}
			}
		fprintf(stdout, "STOP\n");
		glEnd();

		glPopMatrix();
	}

private:
	struct SDate {
		CComplex c;
		// nr. de iteratii
		int nriter;
		// modulul maxim
		double modmax;
		// manderbolt flag
		bool manderbolt;
		// reversed flag
		bool reversed;
	} m;
};
#pragma endregion Julia Fatou

#pragma region Arbori Si Curbe
class C2coord
{
public:
	C2coord()
	{
		m.x = m.y = 0;
	}

	C2coord(double x, double y)
	{
		m.x = x;
		m.y = y;
	}

	C2coord(const C2coord &p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
	}

	C2coord &operator=(C2coord &p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	int operator==(C2coord &p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

protected:
	struct SDate
	{
		double x, y;
	} m;
};

class CPunct : public C2coord
{
public:
	CPunct() : C2coord(0.0, 0.0)
	{}

	CPunct(double x, double y) : C2coord(x, y)
	{}

	CPunct &operator=(const CPunct &p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	void getxy(double &x, double &y)
	{
		x = m.x;
		y = m.y;
	}

	int operator==(CPunct &p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

	void marcheaza()
	{
		glBegin(GL_POINTS);
		glVertex2d(m.x, m.y);
		glEnd();
	}

	void print(FILE *fis)
	{
		fprintf(fis, "(%+f,%+f)", m.x, m.y);
	}

};

class CVector : public C2coord
{
public:
	CVector() : C2coord(0.0, 0.0)
	{
		normalizare();
	}

	CVector(double x, double y) : C2coord(x, y)
	{
		normalizare();
	}

	CVector &operator=(CVector &p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	int operator==(CVector &p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

	CPunct getDest(CPunct &orig, double lungime)
	{
		double x, y;
		orig.getxy(x, y);
		CPunct p(x + m.x * lungime, y + m.y * lungime);
		return p;
	}

	void rotatie(double grade)
	{
		double x = m.x;
		double y = m.y;
		double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
		m.x = x * cos(t) - y * sin(t);
		m.y = x * sin(t) + y * cos(t);
		normalizare();
	}

	void deseneaza(CPunct p, double lungime)
	{
		double x, y;
		p.getxy(x, y);
		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x + m.x * lungime, y + m.y * lungime);
		glEnd();
	}

	void print(FILE *fis)
	{
		fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
	}

private:
	void normalizare()
	{
		double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
		if (d != 0.0)
		{
			C2coord::m.x = C2coord::m.x * 1.0 / d;
			C2coord::m.y = C2coord::m.y * 1.0 / d;
		}
	}
};

class Patratele
{
public:
	void patratele(double lungime, int nivel, CPunct &p, CVector &v)
	{
		if (nivel != 0)
		{
			CPunct p1;
			//stanga sus
			moveDirectie(90, lungime * 2 / 9, 90, lungime * 2 / 9, 180, p, v);
			drawPatrat(lungime / 9, p, v);
			p1 = p;
			patratele(lungime / 3, nivel - 1, p1, v);
			//mijloc sus
			moveDirectie(0, lungime / 3, 0, p, v);
			drawPatrat(lungime / 9, p, v);
			p1 = p;
			patratele(lungime / 3, nivel - 1, p1, v);
			//dreapta sus
			moveDirectie(0, lungime / 3, 0, p, v);
			drawPatrat(lungime / 9, p, v);
			p1 = p;
			patratele(lungime / 3, nivel - 1, p1, v);
			//dreapta mijloc
			moveDirectie(-90, lungime / 3, 90, p, v);
			drawPatrat(lungime / 9, p, v);
			p1 = p;
			patratele(lungime / 3, nivel - 1, p1, v);
			//dreapta jos
			moveDirectie(-90, lungime / 3, 90, p, v);
			drawPatrat(lungime / 9, p, v);
			p1 = p;
			patratele(lungime / 3, nivel - 1, p1, v);
			//mijloc jos
			moveDirectie(180, lungime / 3, 180, p, v);
			drawPatrat(lungime / 9, p, v);
			p1 = p;
			patratele(lungime / 3, nivel - 1, p1, v);
			//stanga jos
			moveDirectie(180, lungime / 3, 180, p, v);
			drawPatrat(lungime / 9, p, v);
			p1 = p;
			patratele(lungime / 3, nivel - 1, p1, v);
			//stanga mijloc
			moveDirectie(90, lungime / 3, -90, p, v);
			drawPatrat(lungime / 9, p, v);
			p1 = p;
			patratele(lungime / 3, nivel - 1, p1, v);
		}

	}

	void moveDirectie(int u1, double l1, int u2, CPunct &p, CVector &v) {
		v.rotatie(u1);
		p = v.getDest(p, l1);
		v.rotatie(u2);
	}

	void moveDirectie(int u1, double l1, int u2, double l2, int u3, CPunct &p, CVector &v) {
		v.rotatie(u1);
		p = v.getDest(p, l1);
		v.rotatie(u2);
		p = v.getDest(p, l2);
		v.rotatie(u3);
	}

	void drawPatrat(double lungime, CPunct &p, CVector &v) {
		v.deseneaza(p, lungime);
		p = v.getDest(p, lungime);
		v.rotatie(-90);
		v.deseneaza(p, lungime);
		p = v.getDest(p, lungime);
		v.rotatie(-90);
		v.deseneaza(p, lungime);
		p = v.getDest(p, lungime);
		v.rotatie(-90);
		v.deseneaza(p, lungime);
		p = v.getDest(p, lungime);
		v.rotatie(-90);
	}

	void afisare(double lungime, int nivel)
	{
		CVector v(1.0, 0.0);
		CPunct p(-0.75, 0.75);
		//cadru
		drawPatrat(lungime, p, v);
		moveDirectie(0, lungime / 3, -90, lungime / 3, 90, p, v);
		drawPatrat(lungime / 3, p, v);
		//interior
		patratele(lungime, nivel, p, v);
	}
};

class CArbore42 {
public:
	void arbore42(double lungime,
		int nivel,
		double factordiviziune,
		CPunct p,
		CVector v)
	{
		assert(factordiviziune != 0);
		CPunct p1, p2;
		if (nivel == 0)
		{
		}
		else
		{
			v.rotatie(-45);
			v.deseneaza(p, lungime);
			p1 = v.getDest(p, lungime);
			arbore42(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

			v.rotatie(90);
			v.deseneaza(p, lungime);
			p1 = v.getDest(p, lungime);
			p2 = p1;

			v.rotatie(+15);
			v.deseneaza(p1, lungime);
			p1 = v.getDest(p1, lungime);
			arbore42(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

			p1 = p2;
			v.rotatie(-60);
			v.deseneaza(p1, lungime);
			p1 = v.getDest(p1, lungime);
			p2 = p1;

			v.rotatie(+30);
			v.deseneaza(p1, lungime / 2);
			p1 = v.getDest(p1, lungime / 2);
			arbore42(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

			p1 = p2;
			v.rotatie(-120);
			v.deseneaza(p1, lungime / 2);
			p1 = v.getDest(p1, lungime / 2);
			arbore42(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
		}
	}

	void afisare(double lungime, int nivel)
	{
		CVector v(0.0, -1.0);
		CPunct p(0.0, 1.0);

		v.deseneaza(p, 0.25);
		p = v.getDest(p, 0.25);
		arbore42(lungime, nivel, 0.4, p, v);
	}
};

class Triunghiuri
{
public:
	void triunghiuri(double lungime, int nivel, CPunct &p, CVector &v, int d)
	{
		if (nivel % 2 == 0) {
			draw(lungime, nivel, p, v, +60);
		}
		else {
			v.rotatie(60);
			draw(lungime, nivel, p, v, -60);
		}
	}

	void draw(double lungime, int nivel, CPunct &p, CVector &v, int unghi) {
		if (nivel == 0) {
			v.deseneaza(p, lungime);
			p = v.getDest(p, lungime);
		}
		else {
			draw(lungime / 2, nivel - 1, p, v, -unghi);
			v.rotatie(unghi);
			draw(lungime / 2, nivel - 1, p, v, unghi);
			v.rotatie(unghi);
			draw(lungime / 2, nivel - 1, p, v, -unghi);
		}
	}

	void afisare(double lungime, int nivel)
	{
		CVector v(0.0, 1.0);
		CPunct p(0.0, -0.5);

		triunghiuri(lungime, nivel, p, v, 1);
	}
};
#pragma endregion Arbori Si Curbe

//Multimea Mandelbrot
void Display1() {
	CJuliaFatou cjf(true, false);

	glColor3f(1.0, 0.1, 0.1);
	cjf.setnriter(30);
	cjf.display(-2, -2, 2, 2);
}

//Anti-Multimea Mandelbrot colorata
void Display2() {
	CJuliaFatou cjf(true, true);

	cjf.setnriter(30);
	cjf.display(-2, -2, 2, 2);
}

//Banc cu patratele
void Display3() {
	Patratele patra;
	patra.afisare(1.5, nivel);

	char c[3];
	sprintf(c, "%2d", nivel);
	glRasterPos2d(-0.98, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

	glRasterPos2d(-1.0, -0.9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');

	nivel++;
}

// 4.2 arbore bizar
void Display4() {
	CArbore42 ca42;

	char c[3];
	sprintf(c, "%2d", nivel);
	glRasterPos2d(-0.90, -0.90);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

	glRasterPos2d(-0.90, 0.90);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '4');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '2');

	glPushMatrix();
	glLoadIdentity();
	glScaled(0.4, 0.4, 1);
	glTranslated(-0.6, 0.77, 0.0);
	ca42.afisare(1, nivel);
	glPopMatrix();

	nivel++;
}

//Triunghi din linie
void Display5() {
	Triunghiuri tri;
	tri.afisare(1.0, nivel);

	char c[3];
	sprintf(c, "%2d", nivel);
	glRasterPos2d(-0.98, -0.98);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

	glRasterPos2d(-1.0, -0.9);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'g');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');

	nivel++;
}

void Init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glLineWidth(1);
	glPointSize(3);
	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	switch (prevKey) {
	case '0':
		glClear(GL_COLOR_BUFFER_BIT);
		nivel = 0;
		fprintf(stderr, "nivel = %d\n", nivel);
		break;
	case '1':
		glClear(GL_COLOR_BUFFER_BIT);
		Display1();
		break;
	case '2':
		glClear(GL_COLOR_BUFFER_BIT);
		Display2();
		break;
	case '3':
		glClear(GL_COLOR_BUFFER_BIT);
		Display3();
		break;
	case '4':
		glClear(GL_COLOR_BUFFER_BIT);
		Display4();
		break;
	case '5':
		glClear(GL_COLOR_BUFFER_BIT);
		Display5();
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


