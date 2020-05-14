#include <GL/glut.h>
#include <cctype>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>

#include "def.h"

const char PointsFile[] = "./pt.txt";
int bottomY;
int topY;
std::vector<POINT> myPT;
std::vector<EDGE> myNET;
std::vector<EDGE> myAET;

void SetPixelH(int y, GLfloat left, GLfloat right)
{
    int L, R;
    L = (int)roundf(left);  // (int)(x+0.5f)这种方法只对正数有效
    R = (int)roundf(right);
    if(L < left)  L++;
    if(R > right) R--;
    glBegin(GL_POINTS);
    while(L <= R)
        glVertex2i(L++, y);
    glEnd();
    glFlush();
}
bool CompMinX(const EDGE &a, const EDGE &b)
{
    return a.xOfMinY < b.xOfMinY;
}
int DrawPolygon()
{
    int i, j, netSize, cnt;
    float xL;
    j = 0;
    cnt = 0;
    netSize = (int)myNET.size();
    for (i = bottomY; i <= topY; i++)
    {
        while (j < netSize && i == myNET[j].yMin)
        {
            myAET.push_back(myNET[j]);
            j++;
        }
        std::sort(myAET.begin(), myAET.end(), CompMinX);
        for (auto k = myAET.begin(); k != myAET.end();)
        {
            //
            // 尚未解决的、繁琐的问题：
            // 扫描过程中扫描线与水平边重合时，不能得到正确的填充区间，从而遗漏某一段像素
            //
            if (k->yMax != i)
            {
                cnt++;
                if (cnt & 1)
                {
                    xL = k->xOfMinY;
                }
                else
                {
                    DBG_PRNT("Set pixels from (%.2f,%d) to (%.2f,%d).\n", xL, i, k->xOfMinY, i);
                    SetPixelH(i, xL, k->xOfMinY);
                }
                k->xOfMinY += k->deltaX;
                k++;
            }
            else
            {
                k = myAET.erase(k);
            }
        }
        if (cnt & 1)
        {
            printf("Odd number of intersections(cnt = %d).\n", cnt);
            return 1;
        }
    }
    return 0;
}
void display_callback()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // 黑色坐标轴
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2i(-50, 0);
    glVertex2i(50, 0);
    glVertex2i(0, 50);
    glVertex2i(0, -50);
    glEnd();
    glFlush();
    // 扫描转换
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(3.0f);
    DrawPolygon();
    // 多边形的标准边界
    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for (auto &&i : myPT)
    {
        glVertex2i(i.x, i.y);
    }
    glEnd();
    glFlush();
}
// 按下ESC键退出
void keyboard_callback(unsigned char k, int x, int y)
{
    if (k == 27)
        exit(0);
}
// 从文件中读取多边形的各个定点（只认识数字）
void BuildPT(const char *ptFile, std::vector<POINT> &apt)
{
    int x, y, ch, ret, cnt;
    float number;
    FILE *pf;
    pf = fopen(ptFile, "r");
    if (pf == NULL)
    {
        ret = 1;
        goto ERR_BUILD_PT;
    }
    x = 0;
    cnt = 0;
    rewind(pf);
LP_READ_COORD:
    ch = fgetc(pf);
    if (ch == EOF)
    {
        goto REACH_FILE_END;
    }
    if (!(ch == '+' || ch == '-'))
    {
        if (!isdigit(ch))
        {
            goto LP_READ_COORD;
        }
    }
    else
    {
        ret = fgetc(pf);
        if (ret == EOF)
        {
            goto REACH_FILE_END;
        }
        if (!isdigit(ret))
        {
            goto LP_READ_COORD;
        }
        ret = ungetc(ret, pf);
        if (ret == EOF)
        {
            ret = 2;
            goto ERR_BUILD_PT;
        }
    }
    ret = ungetc(ch, pf);
    if (ret == EOF)
    {
        ret = 2;
        goto ERR_BUILD_PT;
    }
    fscanf(pf, "%f", &number);
    cnt++;
    if (cnt & 1)
    {
        x = (int)number;
    }
    else
    {
        y = (int)number;
        DBG_PRNT("READ POINT: (%d, %d)\n", x, y);
        apt.push_back({x, y});
    }
    goto LP_READ_COORD;
REACH_FILE_END:
    if (cnt & 1)
    {
        ret = 3;
        goto ERR_BUILD_PT;
    }
    ret = fclose(pf);
    if (ret == EOF)
    {
        ret = 4;
        goto ERR_BUILD_PT;
    }
    cnt = (int)apt.size();
    if (cnt < 3)
    {
        ret = 5;
        goto ERR_BUILD_PT;
    }
    DBG_PRNT("TOTAL READ %d POINTS.\n", cnt);
    // 确保它是闭合的
    x = apt[0].x;
    y = apt[0].y;
    cnt--;
    if (apt[cnt].x != x || apt[cnt].y != y)
    {
        apt.push_back({x, y});
    }
    return;
ERR_BUILD_PT:
    switch (ret)
    {
    case 1:
        printf("Cannot open file \"%s\".\n", ptFile);
        break;
    case 2:
        printf("Function \"ungetc()\" failed.\n");
        break;
    case 3:
        printf("There is no ordinate after X=%d.\n", x);
        break;
    case 4:
        printf("Failed to close file \"%s\".\n", ptFile);
        break;
    case 5:
        printf("At least 3 points are required but you only provided %d.\n", cnt);
        break;
    default:
        break;
    }
    exit(1);
}
bool CompMinY(const EDGE &a, const EDGE &b)
{
    return a.yMin < b.yMin;
}
void BuildNET(const std::vector<POINT> &apt, std::vector<EDGE> &net)
{
    int i, j, sz;
    EDGE edge;
    sz = (int)apt.size();
    if (sz < 2)
    {
        printf("BuildNET(): At least 2 points are required.\n");
        exit(1);
    }
    for (i = 1; i < sz; i++)
    {
        j = apt[i].y - apt[i - 1].y;

        // 为了方便，我没有把水平边加入到表中，这导致了上述BUG（上面的代码本身也有问题）
        if (j == 0)
            continue;

        if (j < 0)
        {
            edge.yMin = apt[i].y;
            edge.xOfMinY = (float)apt[i].x;
            edge.yMax = apt[i - 1].y;
        }
        else
        {
            edge.yMax = apt[i].y;
            edge.yMin = apt[i - 1].y;
            edge.xOfMinY = (float)apt[i - 1].x;
        }
        edge.deltaX = (float)(apt[i].x - apt[i - 1].x);
        edge.deltaX /= j;
        net.push_back(edge);
    }
    sz = (int)net.size();
    std::sort(net.begin(), net.end(), CompMinY);
    for (i = 0; i < sz; i++)
    {
        edge = net[i];
        DBG_PRNT("BUILD EDGE:  yMin=%d  <xOfMinY=%.1f, delta=%.2f, yMax=%d>\n",
                 edge.yMin, edge.xOfMinY, edge.deltaX, edge.yMax);
    }
    DBG_PRNT("BUILD %d EDGE(S)\n", sz);
}
int main(int argc, char *argv[])
{
    int i;
    BuildPT(PointsFile, myPT);
    BuildNET(myPT, myNET);
    i = (int)myNET.size();
    if (i == 0)
    {
        // 所有边都是水平的
        printf("Invalid shape.");
        return -1;
    }
    bottomY = myNET[0].yMin;
    topY = bottomY;
    while ((--i) > 0)
    {
        if (myNET[i].yMax > topY)
        {
            topY = myNET[i].yMax;
        }
    }

    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Polygon");

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