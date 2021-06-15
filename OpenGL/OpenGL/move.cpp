// pch.cpp: �̸� �����ϵ� ����� �ش��ϴ� �ҽ� �����Դϴ�. �����Ϸ��� �������� �ʿ��մϴ�.

#include "pch.h"

// �Ϲ������� �� ������ ���������� �̸� �����ϵ� ����� ����ϴ� ��� �����մϴ�.
#include "pch.h"
#include <iostream>
#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)

#define Width 400
#define Height 400

#define PI	3.1415926


int		Ngon = 5;
int		point_num = 100;
int		point_array[100][2];
float	radius = 20.0;

float	left = -200;
float	bottom = -200;

int		draw_type = GL_POLYGON;

void MyReshape(int w, int h) {
	glViewport(0, 0, w / 2, h / 2);//�����ִ� �κ��� ũ��� ��ġ(0,0) ����(w,h)����

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left,  Width, bottom, Height); // ī�޶������ �����
}


void	Draw_Ngon(float cx, float cy) {
	float	delta = 12.0 * PI / Ngon;

	glPointSize(3.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(draw_type);
	for (int i = 0; i < Ngon; i++) {
		glVertex2f(cx + radius * cos(delta*i), cy + radius * sin(delta*i));
	}
	glEnd();
}

void RenderScene(void) {
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, left + Width, bottom, bottom + Height);

	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2i(-100, -100);
	glVertex2i(100, -100);
	glVertex2f(100, 100);
	glVertex2f(-100, 100);
	glEnd();

	
	glColor3f(0.0, 1.0, 1.0);
	glRectf(10.0, 15.0,15.0,10.0 );

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(0, 0);
	glVertex2i(200, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 200.0);
	glEnd();


	for (int i = 0; i < point_num; i++) {
		Draw_Ngon(point_array[i][0], point_array[i][1]);
	}

	glFlush();
}

void MyMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {					// ȭ�鿡 ������ ���� 1���ø� 
		point_array[point_num][0] = left + x;
		point_array[point_num][1] = bottom + (Height - y);
		point_num++;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {			//ȭ���� ������ �ʱ�ȭ ������ ���ƴ� ���� �ȱ׷��� �׷���
		point_num = 0;
	}
	glutPostRedisplay();
}

void MyKey(unsigned char key, int x, int y) {

	switch (key) {
	case	'a':	radius++;		break;
	case	's':	radius--;		break;
	case	'n':	Ngon++;			break;
	case	'm':	if (Ngon > 3) Ngon--; 	break;
	default: break;
	}

	glutPostRedisplay();
}

void MySpecial(int key, int x, int y) {
	switch (key) {
	case	GLUT_KEY_LEFT:	left--;		break;
	case	GLUT_KEY_RIGHT:	left++;		break;
	case	GLUT_KEY_UP:	bottom++;	break;
	case	GLUT_KEY_DOWN:	bottom--;	break;

	case GLUT_KEY_F1:	draw_type = GL_POINTS;		break;
	case GLUT_KEY_F2:	draw_type = GL_LINES;		break;
	case GLUT_KEY_F3:	draw_type = GL_TRIANGLES;	break;
	case GLUT_KEY_F4:	draw_type = GL_QUADS;		break;
	case GLUT_KEY_F5:	draw_type = GL_LINE_STRIP;	break;
	case GLUT_KEY_F6:	draw_type = GL_LINE_LOOP;	break;
	case GLUT_KEY_F7:	draw_type = GL_POLYGON;		break;
	default: break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Drawing Ngons on Window");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(MyReshape);
	glutMouseFunc(MyMouse);
	glutSpecialFunc(MySpecial);
	glutKeyboardFunc(MyKey);
	glutMainLoop();
}