#include <GL/glut.h>

void display_callback();
void keyboard_callback(unsigned char k, int x, int y);

void BresenhamLine(int x0, int y0, int x1, int y1);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Bresenham Line");
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

	glutDisplayFunc(display_callback);
	glutKeyboardFunc(keyboard_callback);
	glutMainLoop();
	return 0;
}

void keyboard_callback(unsigned char k, int x, int y)
{
	if (k == 27)
		exit(0);
}

void display_callback()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(4.0f);
	BresenhamLine(0, 0, 10, 4);
}

void BresenhamLine(int x0, int y0, int x1, int y1)
{
	int x, y, e, dx, dy;
	dx = x1 - x0;
	dy = y1 - y0;
	x = x0;
	y = y0;
	e = -dx;
	glBegin(GL_POINTS);
	for (int i = 0; i <= dx; i++)
	{
		glVertex2i(x, y);
		x++;
		e += 2*dy;
		if (e >= 0)
		{
			y++;
			e -= 2*dx;
		}
	}
	glEnd();
	glFlush();
}
