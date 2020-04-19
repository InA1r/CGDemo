#include <GL/glut.h>
#include <stdio.h>

int R;

void display_callback(void);
void keyboard_callback(unsigned char k, int x, int y);

void DrawCircle(int xc, int yc, int r);

int main(int argc, char ** argv)
{
    printf("Radius(1~50):");
    scanf("%d", &R);
	if (R < 1 || R > 50)
		return 0;

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
	glPointSize(3.5f);
	DrawCircle(0, 0, R);
}
void keyboard_callback(unsigned char k, int x, int y)
{
	if (k == 27)
		exit(0);
}
void DrawCircle(int r)
{
	float x, x1, y, y1, eps;
	x = (float)r;
	y = 0.0f;
	eps = 1.0f / (float)r;
LOOP:
	SetPixel((int)x, (int)y);
	x1 = x;
	y1 = y;
	x = x1 - y1 * eps;
	y = y1 + x * eps;
	if (abs(y) > eps || abs(r - x) > eps)
		goto LOOP;
}