#include "pch.h"
#include <iostream>
#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)
#define PI 3.1415926
#define N 36				//세타값
#define M 18				//파이값

//카메라 변수
float  LookAtx = 6.0;
float  LookAty = 0.0;
float  LookAtz = 5.0;
float bmx = 0.0;
float bmy = 0.0;
float bmz = 1.0;
float grav = 0.2;

float ballx = 0.0;
float bally = 0.0;
float ballz = 0.0;

int mus = 0;
int mfs = 0;
int mbs = 0;
int mls = 0;
int mrs = 0;



//경도         위도
float theta = 0, phi = 0;
float delta_theta = 0, delta_phi = 0;		//한번에 이동할 거리
float start_theta = 0, start_phi = 0;

float Radius = 1.0;
float i = 0;
float ver[N][M + 1][3];
float ver1[N][M + 1][3];

//태양광 평행하게 나감 원포인트는 평향하지 않음
float light_position[] = { 100.0, 100.0, 100.0, 0.0 };		//빛 위치		4번쨰 값(0or1)에 따라 (0)태양광(디렉셔널) 또는 (1)원포인트
float light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };		//주변광		1,2,3 칸 RGB값 4번쨰 값은 알파값
float light_diffuse[] = { 0.3, 0.6, 0.5, 1.0 };		//난반사			
float light_specular[] = { 0.5, 0.5, 0.5, 1.0 };		//정반사		

//객체 색 
float mat_ambdif[] = { 0.5, 0.5, 0.5, 1.0 };
float mat_spc[] = { 0.5, 0.5, 0.5, 1.0 };
float mat_shn[] = { 50.0 };

float fl_ambdif[] = { 0.0, 0.0, 0.0, 0.0 };
float fl_spc[] = { 0.4, 0.2, 0.2, 1.0 };
float fl_shn[] = { 10.0 };

float q_ambdif[] = { 0.7, 0.2, 0.1, 0.0 };
float q_spc[] = { 0.7, 0.2, 0.2, 1.0 };
float q_shn[] = { 50.0 };


void CirclePoint(void) {
	start_theta = 0.0;
	delta_theta = 2.0*PI / N;			//한번씩 움직일 경도를 구한것 PI(180도) 2배해서 360에 N(36) 을 나눠서 10도씩

	start_phi = -1.0*PI / 2.0;			//파이에 -1을 곱해준뒤 반으로 줄여 -90도 시작
	delta_phi = 1.0*PI / M;			//위쪽 90도까지 10도씩(180도 이동 필요) 올림 

	for (int j = 0; j <= M; j++) {
		for (int i = 0; i < N; i++) {
			theta = start_theta + i * delta_theta;
			phi = start_phi + j * delta_phi;

			ver[i][j][0] = 0.0 + Radius * cos(phi) * cos(theta);			//수학은 모르겟다
			ver[i][j][1] = 0.0 + Radius * cos(phi) * sin(theta);			//어쩄든 x,y,z점 찍음
			ver[i][j][2] = 0.0 + Radius * sin(phi);
		}
	}
}

void init(void)
{
	glClearColor(0.1, 0.1, 0.1, 0.0); // Set display-window color
	glColor3f(0.0, 0.0, 0.0);

	CirclePoint();

	glShadeModel(GL_SMOOTH);  //GL_FLAT
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//LIGHT0번 실행(0~8번까지 있음)

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//광원 이름 , 위치 ,		위치값

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	//광원 이름 , 주변광 ,	  값

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//광원 이름 , 디퓨즈 반사 ,		반사 값

	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//광원 이름 , 정반사  ,		반사 값
	glEnable(GL_DEPTH_TEST);//은면제거
}

void axis() {						//X,Y,Z축 그리는 함수
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); // x축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1000.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0); // y축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1000.0, 0.0);

	glColor3f(0.0, 0.0, 1.0); // z축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1000.0);
	glEnd();

}
//-----------------객체-----------------------
void DrawFloor() {
	glColor4fv(q_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, q_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fl_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, fl_shn);

	glBegin(GL_POLYGON);
	glVertex3f(-100.0, -100.0, -1.0);
	glVertex3f(100.0, -100.0, -1.0);
	glVertex3f(100.0, 100.0, -1.0);
	glVertex3f(-100.0, 100.0, -1.0);
	glEnd();
}

void DrawShape() {
	glColor4fv(mat_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shn);

	for (int j = 0; j < M; j++) {
		for (int i = 0; i < N; i++) {
			glBegin(GL_POLYGON);
			glNormal3fv(ver[i][j]);
			glVertex3fv(ver[i][j]);
			glNormal3fv(ver[(i + 1) % N][j]);
			glVertex3fv(ver[(i + 1) % N][j]);				//%N은 i[N]에 아무값도 없음과 동시에 i[0]과 같은 자리임으로 나머지를 통해 쓴다
			glNormal3fv(ver[(i + 1) % N][j + 1]);
			glVertex3fv(ver[(i + 1) % N][j + 1]);
			glNormal3fv(ver[i][j + 1]);
			glVertex3fv(ver[i][j + 1]);
			glEnd();
		}
	}
	glEnd();
}
//------------------------------------------------
void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);	//렌즈
	glLoadIdentity();
	gluPerspective(75.0, 1.0, 1.0, 500.0);

}

void CamMove(float x, float y, float z) {
	LookAtx += x;
	LookAty += y;
	LookAtz += z;
}
//-----------이동 거리 등등----------------
void moveUp() {
	bmz += 0.016;
	if (bmz >= 3) {
		mus = 0;
	}
}
void moveFront() {
	bmx -= 0.008;
}
void moveBack() {
	bmx += 0.008;
}
void moveL() {
	bmy -= 0.008;
}
void moveR() {
	bmy += 0.008;
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
void gravity() {
	if (bmz > 0) { bmz -= 0.008; }
	else if (bmz < 0) { bmz += 0.008; }
}
//-------------------------------------------



void Camera_setting(void) {
	glMatrixMode(GL_PROJECTION);	//렌즈
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 500.0);
	//	y축 기준 60도, 비율1:1, 1부터 500거리까지 보겟다

	glMatrixMode(GL_MODELVIEW);	//카메라 위치 //필드상에 놓는 역할
	glLoadIdentity();
	gluLookAt(
		LookAtx, LookAty, LookAtz,
		ballx, bally, ballz,//	을바라본다
		0.0, 0.0, 1.0);
}					//카메라 함수

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//색갈 및 깊이 버퍼 유무 리셋
	//CamMove(ballx, bally, ballz);
	Camera_setting();
	DrawFloor();
	axis();
	//mMove(bmx, bmy, bmz);
	gravity();
	checkbutton();
	ballx = +bmx;
	bally = +bmy;
	ballz = +bmz;
	glTranslatef(ballx, bally, ballz);
	//glTranslatef(ballx + bmx, bally + bmy, ballz + bmz);
	DrawShape();

	glFlush();					//보냄
	glutSwapBuffers();
}

void UpSpecial(int key, int x, int y) {
	//키보드 릴리스 될떄 실행ㅁ
	switch (key) {
	case	GLUT_KEY_UP:
		mfs = 0;
		break;
	case	GLUT_KEY_LEFT:
		mls = 0;
		break;
	case	GLUT_KEY_RIGHT:
		mrs = 0;
		break;
	case	GLUT_KEY_DOWN:
		mbs = 0;
		break;
	case	GLUT_KEY_F1:
		if (mus == 0) {
			mus = 1;
		}
		break;
	}
}

void MySpecial(int key, int x, int y) {
	switch (key) {
	case	GLUT_KEY_UP:
		mfs = 1;
		break;
	case	GLUT_KEY_LEFT:
		mls = 1;
		break;
	case	GLUT_KEY_RIGHT:
		mrs = 1;
		break;
	case	GLUT_KEY_DOWN:
		mbs = 1;
		break;
	case	GLUT_KEY_F1:
		mus = 1;
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
	glutSpecialUpFunc(UpSpecial);
	glutMainLoop();
}
