#include <GL/glut.h>
#include <stdio.h>

int xCenter, yCenter, Rx, Ry;

void display_callback(void);
void keyboard_callback(unsigned char k, int x, int y);

void draw(int xCenter, int yCenter, int Rx, int Ry);

int main(int argc, char ** argv)
{
    int ret;
    printf("INPUT <xCenter> <yCenter> <Rx> <Ry>:\n");
    ret = scanf("%d%d%d%d", &xCenter, &yCenter, &Rx, &Ry);
    if(ret != 4 || Rx <= 0 || Ry <= 0)
    {
        printf("INVALID PARAMETERS!\n");
        return -1;
    }
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Circle Demo (mid point)");

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0f, 50.0f, -50.0f, 50.0f, -1.0f, 1.0f);

	glutDisplayFunc(display_callback);
	glutKeyboardFunc(keyboard_callback);
	glutMainLoop();

	return 0;
}
void display_callback() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2i(-50, 0);
	glVertex2i(50, 0);
	glVertex2i(0, 50);
	glVertex2i(0, -50);
	glEnd();
	glFlush();

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(3.0f);
	draw(xCenter, yCenter, Rx, Ry);
}
void keyboard_callback(unsigned char k, int x, int y)
{
	if (k == 27)
		exit(0);
}
void SetPixel(int xCenter, int yCenter, int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(xCenter + x, yCenter + y);
	glVertex2i(xCenter + x, yCenter - y);
	glVertex2i(xCenter - x, yCenter + y);
	glVertex2i(xCenter - x, yCenter - y);
	glEnd();
	glFlush();
}
void draw(int xCenter, int yCenter, int Rx, int Ry)
{
    int p, x, y, px, py, Rx2, Ry2, twoRx2, twoRy2;
	Rx2 = Rx * Rx;
	Ry2 = Ry * Ry;
	twoRx2 = Rx2 << 1;
	twoRy2 = Ry2 << 1;
	p = (int)(Ry2 - (Rx2 * Ry) + 0.25 * Rx2 + 0.5);
	x = 0;
	y = Ry;
	px = 0;
	py = twoRx2 * y;
	// 区域一
	SetPixel(xCenter, yCenter, x, y);
	while (px < py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
		{
			p += Ry2 + px;
		}
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		SetPixel(xCenter, yCenter, x, y);

	}
	// 区域二
	p = (int)(Ry2 * (x + 0.5)*(x + 0.5) + Rx2 * (y - 1)*(y - 1) - Rx2 * Ry2 + 0.5);
	while (y > 0)
	{
		y--;
		py -= twoRx2;
		if (p > 0)
		{
			p += Rx2 - py;
		}
		else
		{
			x++;
			px += twoRy2;
			p += Rx2 - py + px;
		}
		SetPixel(xCenter, yCenter, x, y);
	}
}