#include "pch.h"
#include <iostream>
#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)

float	xx = 10.0;
float	yy = 50.0;
float	xcenter, ycenter;
int		rsize = 100;
float	xstep = 1.3;
float	ystep = 0.9;
float	winWidth;
float	winHeight;


void	Init(void) {
	winHeight = 500.0;
	winWidth = 500.0;
}

void	ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);

	if (w <= h) {
		winHeight = 500.0 * h / w;
		winWidth = 500.0;
	}
	else {
		winWidth = 500.0 * w / h;
		winHeight = 500.0;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, winWidth, 0, winHeight, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);

}

void RenderScene(void)
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(xx, yy);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(xx + rsize, yy);
	glColor3f(1.0, 1.0, 0.0);
	glVertex2f(xx + rsize, yy + rsize);
	glColor3f(1.0, 0.5, 0.5);
	glVertex2f(xx, yy + rsize);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(winWidth / 2.0 - rsize / 2.0, winHeight / 2.0 - rsize / 2.0);
	glVertex2f(winWidth / 2.0 + rsize / 2.0, winHeight / 2.0 - rsize / 2.0);
	glVertex2f(winWidth / 2.0 + rsize / 2.0, winHeight / 2.0 + rsize / 2.0);
	glVertex2f(winWidth / 2.0 - rsize / 2.0, winHeight / 2.0 + rsize / 2.0);
	glEnd();


	glFlush();
	glutSwapBuffers();
}

void  IdleFunction(void)
{
	if (xx > winWidth - rsize || xx < 0)
		xstep = -xstep;
	if (yy > winHeight - rsize || yy < 0)
		ystep = -ystep;
	xcenter = xx + rsize / 2.0;
	ycenter = yy + rsize / 2.0;

	if ((winWidth / 2.0 - rsize <= xcenter) && (xcenter <= winWidth / 2.0 - rsize / 2.0)
		&& (ycenter <= -1.0 * (xcenter - winWidth / 2.0) + winHeight / 2.0)
		&& (ycenter >= (xcenter - winWidth / 2.0) + winHeight / 2.0))
		xstep = -xstep;
	else if ((winWidth / 2.0 + rsize / 2.0 <= xcenter) && (xcenter <= winWidth / 2.0 + rsize)
		&& (ycenter >= -1.0 * (xcenter - winHeight / 2.0) + winHeight / 2.0)
		&& (ycenter <= (xcenter - winWidth / 2.0) + winHeight / 2.0))
		xstep = -xstep;
	if ((winHeight / 2.0 - rsize <= ycenter) && (ycenter <= winHeight - rsize / 2.0)
		&& (ycenter <= (xcenter - winWidth / 2.0) + winHeight / 2.0)
		&& (ycenter <= -1.0 * (xcenter - winWidth / 2.0) + winHeight / 2.0))
		ystep = -ystep;
	else if ((winHeight / 2.0 + rsize / 2.0 <= ycenter) && (ycenter <= winHeight / 2.0 + rsize)
		&& (ycenter >= -1.0 * (xcenter - winWidth / 2.0) + winHeight / 2.0)
		&& (ycenter >= (xcenter - winWidth / 2.0) + winHeight / 2.0))
		ystep = -ystep;

	if (xx > winWidth - rsize)
		xx = winWidth - rsize - 1;
	if (yy > winHeight - rsize)
		yy = winHeight - rsize - 1;

	xx += xstep;
	yy += ystep;
	RenderScene();

}

void SpecialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	default:				break;
	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	Init();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Primitives");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKey);
	glutIdleFunc(IdleFunction);
	glutMainLoop();
	return 0;
}


