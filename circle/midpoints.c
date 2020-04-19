#include <GL/glut.h>
#include <stdio.h>

int R;

void display_callback(void);
void keyboard_callback(unsigned char k, int x, int y);

void DrawCircle(int r);

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
	DrawCircle(R);
}
void keyboard_callback(unsigned char k, int x, int y)
{
	if (k == 27)
		exit(0);
}
void DrawH(int y, int left, int right)
{
	glBegin(GL_POINTS);
	while (left <= right)
	{
		glVertex2i(left, y);
		glVertex2i(-left, y);
		glVertex2i(left, -y);
		glVertex2i(-left, -y);
		glVertex2i(y, left);
		glVertex2i(-y, left);
		glVertex2i(y, -left);
		glVertex2i(-y, -left);
		left++;
	}
	glEnd();
	glFlush();
}
void DrawCircle(int R)
{
	int x, x0, y, D, E;
	x = 0;
	x0 = 0;
	y = R;
	D = 1 - R;
	E = 3;
REPEAT:
	while (D < 0)
	{
		D = D + E;
		E = E + 2;
		x++;
	}
	DrawH(y, x0, x);
	E = E + 2;
	D = D + E - (y << 1);
	x++;
	y--;
	x0 = x;
	if (x <= y)
		goto REPEAT;
}