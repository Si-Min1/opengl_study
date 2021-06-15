#include "pch.h"
#include <windows.h>
#include <iostream>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)=
#include <math.h>

#define PI 3.1415926
#define N 36				//��Ÿ��
#define M 18				//���̰�
float  LookAtx = 6.0;
float  LookAty = 2.0;
float  LookAtz = 5.0;
float ballx = 0.0;
float bally = 0.0;
float ballz = 0.0;
float bmx = 0.0;
float bmy = 0.0;
float bmz = 1.0;
float grav = 0.2;

int mus = 0;
int mfs = 0;
int mbs = 0;
int mls = 0;
int mrs = 0;

static GLfloat mat_ambdif[] = { 0.1, 0.4, 0.2, 1.0 };
static GLfloat mat_spc[] = { 0.5, 0.5, 0.5, 1.0 };
static GLfloat mat_shn[] = { 9.0 };

//�浵         ����
float theta = 0, phi = 0;
float delta_theta = 0, delta_phi = 0;		//�ѹ��� �̵��� �Ÿ�
float start_theta = 0, start_phi = 0;

float Radius = 1.0;
float i = 0;
float ver[N][M + 1][3];
float ver1[N][M + 1][3];

//�¾籤 �����ϰ� ���� ������Ʈ�� �������� ����
float light_position[] = { 40.0, 40.0, 40.0, 0.0 };		//�� ��ġ		4���� ��(0or1)�� ���� (0)�¾籤(�𷺼ų�) �Ǵ� (1)������Ʈ
float light_ambient[] = { 0.1, 0.4, 0.2, 1.0 };		//�ֺ���		1,2,3 ĭ RGB�� 4���� ���� ���İ�
float light_diffuse[] = { 0.3, 0.6, 0.5, 1.0 };		//���ݻ�			
float light_specular[] = { 0.8, 0.8, 0.8, 1.0 };		//���ݻ�		

GLfloat ambient[] = { 0.5, 1.0, 1.0, 0.0 };

void CirclePoint(void) {
	start_theta = 0.0;
	delta_theta = 2.0*PI / N;			//�ѹ��� ������ �浵�� ���Ѱ� PI(180��) 2���ؼ� 360�� N(36) �� ������ 10����

	start_phi = -1.0*PI / 2.0;			//���̿� -1�� �����ص� ������ �ٿ� -90�� ����
	delta_phi = 1.0*PI / M;			//���� 90������ 10����(180�� �̵� �ʿ�) �ø� 

	for (int j = 0; j <= M; j++) {
		for (int i = 0; i < N; i++) {
			theta = start_theta + i * delta_theta;
			phi = start_phi + j * delta_phi;

			ver[i][j][0] = 0.0 + Radius * cos(phi) * cos(theta);			//������ �𸣰ٴ�
			ver[i][j][1] = 0.0 + Radius * cos(phi) * sin(theta);			//��� x,y,z�� ����
			ver[i][j][2] = 0.0 + Radius * sin(phi);
		}
	}
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // Set display-window color
	glColor3f(0.0, 0.0, 0.0);

	CirclePoint();

	glShadeModel(GL_SMOOTH);  //GL_FLAT
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); //
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//LIGHT0�� ����(0~8������ ����)

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//���� �̸� , ��ġ ,		��ġ��

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//���� �̸� , �ֺ��� ,	  ��

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//���� �̸� , ��ǻ�� �ݻ� ,		�ݻ� ��

	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//���� �̸� , ���ݻ�  ,		�ݻ� ��
	glEnable(GL_DEPTH_TEST);//��������
}

void axis() {						//X,Y,Z�� �׸��� �Լ�
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); // x�� 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1000.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0); // y�� 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1000.0, 0.0);

	glColor3f(0.0, 0.0, 1.0); // z�� 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1000.0);
	glEnd();

}

void DrawFloor() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(-1000.0, -1000.0, -1.0);
	glVertex3f(1000.0, -1000.0, -1.0);
	glVertex3f(1000.0, 1000.0, -1.0);
	glVertex3f(-1000.0, 1000.0, -1.0);
	glEnd();
}

void DrawShape() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambdif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_ambdif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spc);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shn);

	for (int j = 0; j < M; j++) {
		for (int i = 0; i < N; i++) {
			glBegin(GL_POLYGON);
			glNormal3fv(ver[i][j]);
			glVertex3fv(ver[i][j]);
			glNormal3fv(ver[(i + 1) % N][j]);
			glVertex3fv(ver[(i + 1) % N][j]);				//%N�� i[N]�� �ƹ����� ������ ���ÿ� i[0]�� ���� �ڸ������� �������� ���� ����
			glNormal3fv(ver[(i + 1) % N][j + 1]);
			glVertex3fv(ver[(i + 1) % N][j + 1]);
			glNormal3fv(ver[i][j + 1]);
			glVertex3fv(ver[i][j + 1]);
			glEnd();
		}
	}
	glEnd();
}

void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);	//����
	glLoadIdentity();
	gluPerspective(75.0, 1.0, 1.0, 500.0);

}

void Camera_setting(void) {
	glMatrixMode(GL_PROJECTION);	//����
	glLoadIdentity();
	gluPerspective(75.0, 1.0, 1.0, 500.0);
	//	y�� ���� 75��, ����1:1, 1���� 500�Ÿ����� ���ٴ�

	glMatrixMode(GL_MODELVIEW);	//ī�޶� ��ġ //�ʵ�� ���� ����
	glLoadIdentity();
	gluLookAt(
		LookAtx, LookAty, LookAtz,
		ballx, bally, ballz,//	���ٶ󺻴�
		0.0, 0.0, 1.0);	//	
}

void CamMove(float x, float y, float z) {
}

void moveUp() {
	bmz += 0.004;
	if (bmz >= 3) {
		mus = 0;
	}
}
void moveFront() {
	bmx -= 0.002;
	if (bmx <= -3 || mbs == 1) {
		mfs = 0;
	}
}
void moveBack() {
	bmx += 0.002;
	if (bmx >= 3 || mfs == 1) {
		mbs = 0;
	}
}
void moveL() {
	bmy -= 0.002;
	if (bmy <= -3 || mrs == 1) {
		mls = 0;
	}
}
void moveR() {
	bmy += 0.002;
	if (bmy >= 3 || mls == 1) {
		mrs = 0;
	}
}

void gravity() {
	if (bmz > 0) { bmz -= 0.002; }
	else if (bmz < 0) { bmz += 0.002; }
}
void checkbutton() {
	if (mus == 1) {
		moveUp();
	}
	if (mfs == 1) {
		moveFront();
	}
	if (mls == 1) {
		moveL();
	}
	if (mrs == 1) {
		moveR();
	}
	if (mbs == 1) {
		moveBack();
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//���� �� ���� ���� ���� ����
	Camera_setting();
	//DrawFloor();
	axis();
	//mMove(bmx, bmy, bmz);
	gravity();
	checkbutton();

	glPushMatrix();
	glTranslatef(ballx + bmx, bally + bmy, ballz + bmz);
	DrawShape();
	glPopMatrix();

	glFlush();					//����
	glutSwapBuffers();
}


void MySpecial(int key, int x, int y) {
	switch (key) {
	case	GLUT_KEY_UP:
		if (mfs == 0) {
			mfs = 1;
		}
		break;
	case	GLUT_KEY_LEFT:
		if (mls == 0) {
			mls = 1;
		}
		break;
	case	GLUT_KEY_RIGHT:
		if (mrs == 0) {
			mrs = 1;
		}
		break;
	case	GLUT_KEY_DOWN:
		if (mbs == 0) {
			mbs = 1;
		}
		break;
	case	GLUT_KEY_F1:
		if (mus == 0) {
			mus = 1;
		}

		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("cube");
	init();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(MySpecial);
	glutMainLoop();
}
