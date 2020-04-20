#include <GL/glut.h>
#include <stdio.h>

float p;			// 焦准距(y^2=2px)

void display_callback(void);
void keyboard_callback(unsigned char k, int x, int y);

void Parabola(float p, int maxX, int maxY);

int main(int argc, char ** argv)
{
    int ret;
	printf("抛物线焦准距：\n");
	ret = scanf("%f", &p);
	if (ret != 1 || p <= 0)
	{
		printf("无效的参数！\n");
		return -1;
	}

	glutInit(&argc, argv);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Parabola(y*y=2px)");

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
	Parabola(p, 50, 50);
}
void keyboard_callback(unsigned char k, int x, int y)
{
	if (k == 27)
		exit(0);
}
void SetPixel(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glVertex2i(x, -y);
	glEnd();
	glFlush();
}
// 抛物线：y^2 = 2px
void Parabola(float p, int maxX, int maxY)
{
	int x, y;
	float delta, save;
	x = 0;
	y = 0;
	save = 2 * p;
	// 沿Y方向步进，绘制第一段
	delta = 1 - p;
PARABOLA_RGN_1:
	SetPixel(x, y);
	if (delta > 0)
	{
		delta -= save;
		x++;
	}
	delta += (y << 1) + 3;
	y++;
	if (x <= maxX && y < p)
		goto PARABOLA_RGN_1;
	// 沿X方向步进，绘制第二段
	x = (int)(p / 2.0f + 0.5f);
	delta = -p;
PARABOLA_RGN_2:
	SetPixel(x, y);
	if (delta <= 0)
	{
		delta += (y << 1) + 2;
		y++;
	}
	delta -= save;
	x++;
	if (x <= maxX && y <= maxY)
		goto PARABOLA_RGN_2;
}