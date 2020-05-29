#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

// 如果你使用MSVC，那么你可以考虑屏蔽警告
// #pragma warning(disable:4244)

const int WND_WIDTH = 600;
const int WND_HEIGHT = 600;
const float ORTHO_H = 50.0f;
const float ORTHO_V = 50.0f;

static int startX, startY;
static int targetX, targetY;

void DisplayCallback();
void KeyBoardCallback(unsigned char k, int x, int y);
void DrawLine(int x0, int y0, int x1, int y1);

static inline bool InRange(float a, float left, float right)
{
	return (a >= left && a <= right);
}
int main(int argc, char** argv)
{
	std::cout << "INPUT <start X> <start Y> <target X> <target Y>:" << std::endl;
	std::cin >> startX >> startY >> targetX >> targetY;
	
	if(!InRange(startX, -ORTHO_H, ORTHO_H)
		|| !InRange(startY, -ORTHO_V, ORTHO_V)
		|| !InRange(targetX, -ORTHO_H, ORTHO_H)
		|| !InRange(targetX, -ORTHO_V, ORTHO_V)
	) {
		exit(1);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WND_WIDTH, WND_HEIGHT);
	glutCreateWindow("General Approach");

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-ORTHO_H, ORTHO_H, -ORTHO_V, ORTHO_V);

	glutDisplayFunc(DisplayCallback);
	glutKeyboardFunc(KeyBoardCallback);
	glutMainLoop();
	return 0;
}

// 按下ESC键退出
void KeyBoardCallback(unsigned char key, int x, int y)
{
	if (key == 0x1B)
		exit(0);
}

void DisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINES);
	// 坐标轴
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-ORTHO_H, 0.0f);
	glVertex2f(ORTHO_H, 0.0f);
	glVertex2f(0.0f, ORTHO_V);
	glVertex2f(0.0f, -ORTHO_V);
	// 标准线
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i(startX, startY);
	glVertex2i(targetX, targetY);
	glEnd();
	// 直线段的扫描转换
	glPointSize(3.0f);
	DrawLine(startX, startY, targetX, targetY);
	glFlush();
}

// DDA
void DrawLine(int x0, int y0, int x1, int y1)
{
	float x, y, xIn, yIn;
	x = x0;
	y = y0;
	x1 -= x0;			// δx
	y1 -= y0;			// δy
	x0 = abs(x1);		// x0保存步进次数
	y0 = abs(y1);		// y0是临时变量
	if (x0 < y0)
		x0 = y0;
	if (x0 != 0)		// max(|δx|,|δy|)=0说明起点和终点重合了
	{
		xIn = (float)x1 / (float)x0;
		yIn = (float)y1 / (float)x0;
	}
	glBegin(GL_POINTS);
	for (y0 = 0; y0 <= x0; y0++)	// y0是计数变量
	{
		glVertex2i((int)roundf(x), (int)roundf(y));
		// 如果上面max(|δx|,|δy|)=0，那么此处将使用未初始化的xIn和yIn
		// 但是由于此时循环只执行一次，所以并无影响
		x += xIn;
		y += yIn;
	}
	glEnd();
}
