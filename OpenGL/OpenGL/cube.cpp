#include "pch.h"
#include <iostream>
#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)

#define	PI	3.1415926

float	theta = 0.5;
float	phi = 0.0;
float	radius = 2.0;
float	time_on = 0.0;
int		viewing_type = 0;
int		camera_move = 0;
int		departure = 0;
struct {
	float	x;
	float	y;
	float	z;
} start_camera, now_camera;



GLfloat		vertices[][3] = {
	{ -1.0, -1.0,  1.0 },	// 0 
	{ -1.0,  1.0,  1.0 },	// 1
	{ 1.0,  1.0,  1.0 },	// 2
	{ 1.0, -1.0,  1.0 },	// 3
	{ -1.0, -1.0, -1.0 },	// 4
	{ -1.0,  1.0, -1.0 },	// 5
	{ 1.0,  1.0, -1.0 },	// 6
	{ 1.0, -1.0, -1.0 }		// 7 
};


GLfloat		colors[][3] = {
	{ 1.0, 0.0, 0.0 },	// red
	{ 1.0, 1.0, 1.0 },	// white
	{ 1.0, 1.0, 0.0 },	// yellow
	{ 0.0, 1.0, 0.0 },	// green
	{ 0.0, 0.0, 1.0 },	// blue
	{ 1.0, 0.0, 1.0 }   // magenta
};



void polygon(int a, int b, int c, int d) {
	glColor3fv(colors[a]);
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[a]);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glEnd();
}

void cube(void) {
	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(3, 0, 4, 7);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(1, 2, 6, 5);
	polygon(5, 4, 0, 1);
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	radius = 5.0;
	theta = 0.0;
	phi = 0.0;
	now_camera.x = radius * cos(theta)*cos(phi);
	now_camera.y = radius * sin(theta)*cos(phi);
	now_camera.z = radius * sin(phi);
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 20.0);
}

void draw_axis(void)
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); // x√‡ 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0); // y√‡ 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);

	glColor3f(0.0, 0.0, 1.0); // z√‡ 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}


void display(void)
{
	float	x, y, z;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (viewing_type == 0) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0, 1.0, 1.0, 20.0);
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-4.0, 4.0, -4.0, 4.0, 1.0, 15.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (departure == 1)
		time_on += 0.001;

	if (camera_move == 1) {
		if (departure == 0) {
			start_camera.x = now_camera.x;
			start_camera.y = now_camera.y;
			start_camera.z = now_camera.z;
			departure = 1;
		}
		else if (departure == 1) {
			now_camera.x = (1.0 - time_on) * start_camera.x + time_on * (-1.0 * start_camera.x);
			now_camera.y = (1.0 - time_on) * start_camera.y + time_on * (-1.0 * start_camera.y);
			now_camera.z = (1.0 - time_on) * start_camera.z + time_on * (-1.0 * start_camera.z);
		}
	}

	gluLookAt(now_camera.x, now_camera.y, now_camera.z, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	//Camera Setting  Camera Position: (x, y, z), Gazing Point: (0, 0, 0), UpVector : z-axis  (0, 0, 1)


	draw_axis();
	cube();

	glFlush();
	glutSwapBuffers();
}

void special_key(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:		theta += 0.01;
		break;
	case GLUT_KEY_RIGHT:	theta -= 0.01;
		break;
	case GLUT_KEY_UP:		phi += 0.01;
		break;
	case GLUT_KEY_DOWN:		phi -= 0.01;
		break;
	default: break;
	}
	now_camera.x = radius * cos(theta)*cos(phi);
	now_camera.y = radius * sin(theta)*cos(phi);
	now_camera.z = radius * sin(phi);

	glutPostRedisplay();
}

void Mykey(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':	radius += 0.1;		break;
	case 's':	radius -= 0.1;		break;
	case '1':	viewing_type = 0;	break;
	case '2':	viewing_type = 1;	break;
	case '8':	departure = (departure++) % 2;	break;
	case '9':	camera_move = 0;
		departure = 0;
		time_on = 0.0;
		now_camera.x = start_camera.x;
		now_camera.y = start_camera.y;
		now_camera.z = start_camera.z;
		break;
	case '0':	camera_move = 1;	break;
	default:						break;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("cube");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special_key);
	glutKeyboardFunc(Mykey);
	glutIdleFunc(display);
	glutMainLoop();
}
