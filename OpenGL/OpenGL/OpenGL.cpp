#include "pch.h"
#include <windows.h>
#include <gl/glut.h>
#include <iostream>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926	
#define M 36
#define N 18

//카메라 변수
float  LookAtx = 6.0;
float  LookAty = 0.0;
float  LookAtz = 5.0;
float grav = 0.2;

//공 이동 변수
float bmx = 0.0;
float bmy = 0.0;
float bmz = 3.0;

//공 시작 위치
float ballx = 0.0;
float bally = 0.0;
float ballz = 0.0;

float AngleX;
float AngleY;

int mus = 0;
int mfs = 0;
int mbs = 0;
int mls = 0;
int mrs = 0;
int jc = 0;
float cz = 0;
int cc = 0;

int ch = 0;
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
float light_ambient[] = { 1.0, 1.0, 3.0, 1.0 };		//주변광		1,2,3 칸 RGB값 4번쨰 값은 알파값
float light_diffuse[] = { 0.3, 0.6, 0.5, 1.0 };			//난반사			
float light_specular[] = { 0.5, 0.5, 0.5, 1.0 };		//정반사		

//객체 색 
float mat_ambdif[] = { 0.5, 0.5, 0.5, 1.0 };
float mat_spc[] = { 0.5, 0.5, 0.5, 1.0 };
float mat_shn[] = { 50.0 };

float fl_ambdif[] = { 0.0, 0.5, 0.0, 0.0 };
float fl_spc[] = { 0.4, 0.2, 0.2, 1.0 };
float fl_shn[] = { 10.0 };

float q_ambdif[] = { 0.9, 0.2, 0.1, 0.0 };
float q_spc[] = { 0.7, 0.2, 0.2, 1.0 };
float q_shn[] = { 50.0 };

float w_ambdif[] = { 1, 1, 1, 1 };
float w_spc[] = { 0.7, 0.2, 0.2, 1.0 };
float w_shn[] = { 50.0 };

float b_ambdif[] = { 0.0, 0.0, 0.5, 1 };
float b_spc[] = { 0.7, 0.2, 0.2, 1.0 };
float b_shn[] = { 50.0 };

float c_ambdif[] = { 0.1, 0.3, 0.4, 0 };
float c_spc[] = { 0.7, 0.2, 0.2, 1.0 };
float c_shn[] = { 50.0 };


struct Point {
	float x;
	float y;
	float z;
};

GLfloat Delta = 0.0;
GLfloat vel = 0.01;
GLfloat Delta1 = 0.0;
GLfloat vel1 = 0.005;

GLfloat Delta2 = 0.0;
GLfloat vel2 = 0.005;
GLfloat Delta3 = 0.0;
GLfloat vel3 = 0.005;
GLfloat Delta4 = 0.0;
GLfloat vel4 = 0.005;
GLfloat Delta5 = 0.0;
GLfloat vel5 = 0.01;
GLfloat Delta7 = 0.0;
GLfloat vel7 = 0.005;
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

struct Runcube {
	Point min;
	Point max;
	Point center;
	float	x;
	float	y;
	float   z;

};
struct Runcubee {
	Point min;
	Point max;
	Point center;
	float	x;
	float	y;
	float   z;

};
struct Undergr {
	Point min;
	Point max;
	float	x;
	float	y;
	float   z;

};
struct Undergrr {
	Point min;
	Point max;
	float	x;
	float	y;
	float   z;

};
struct Undergrrr {
	Point min;
	Point max;
	float	x;
	float	y;
	float   z;

};
struct Undergrrrr {
	Point min;
	Point max;
	float	x;
	float	y;
	float   z;

};
struct Ground {
	Point min;
	Point max;
	float	x;
	float	y;
	float   z;

};
struct Runcu {
	Point min;
	Point max;
	Point center;
	float	x;
	float	y;
	float   z;

};
Runcube *runcube;
Undergr *undergr;
Ground *ground;
Undergrr *undergrr;
Runcubee *runcubee;
Undergrrr *undergrrr;
Undergrrrr *undergrrrr;
Runcu *runcu;
void init(void) {
	glClearColor(0.7, 0.7, 0.7, 0.0); // Set display-window color
	glColor3f(0.0, 0.0, 0.0);

	CirclePoint();

	glShadeModel(GL_SMOOTH);  //GL_FLAT
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);//은면제거

	runcube = new Runcube[10];
	undergr = new Undergr[10];
	ground = new Ground[10];
	undergrr = new Undergrr[10];
	runcubee = new Runcubee[10];
	undergrrr = new Undergrrr[10];
	undergrrrr = new Undergrrrr[10];
	runcu = new Runcu[10];
}


void DrawShape() {
	glColor4fv(w_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, w_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, w_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, w_shn);

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
void  ground1(int index, int yy) { //땅생성코드 인덱스값에 따라 땅의 위치가 변함
	glColor4fv(fl_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, fl_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, fl_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, fl_shn);
	ground[index].max.x = 3 - (index * 8);
	ground[index].min.x = -3 - (index * 8);
	ground[index].max.y = 3 + yy;
	ground[index].min.y = -3 + yy;
	ground[index].max.z = 0.5 + (index * 3);
	ground[index].min.z = -0.5 + (index * 3);

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(ground[index].min.x, ground[index].max.y, ground[index].min.z);
	glVertex3f(ground[index].max.x, ground[index].max.y, ground[index].min.z);
	glVertex3f(ground[index].max.x, ground[index].min.y, ground[index].min.z);
	glVertex3f(ground[index].min.x, ground[index].min.y, ground[index].min.z);

	//옆x
	glVertex3f(ground[index].min.x, ground[index].min.y, ground[index].max.z);
	glVertex3f(ground[index].max.x, ground[index].min.y, ground[index].max.z);
	glVertex3f(ground[index].min.x, ground[index].min.y, ground[index].min.z);
	glVertex3f(ground[index].max.x, ground[index].min.y, ground[index].min.z);

	//옆 앞
	glVertex3f(ground[index].max.x, ground[index].min.y, ground[index].max.z);
	glVertex3f(ground[index].max.x, ground[index].min.y, ground[index].min.z);
	glVertex3f(ground[index].max.x, ground[index].max.y, ground[index].min.z);
	glVertex3f(ground[index].max.x, ground[index].max.y, ground[index].max.z);

	//옆 y
	glVertex3f(ground[index].max.x, ground[index].max.y, ground[index].min.z);
	glVertex3f(ground[index].min.x, ground[index].max.y, ground[index].min.z);
	glVertex3f(ground[index].min.x, ground[index].max.y, ground[index].min.z);
	glVertex3f(ground[index].min.x, ground[index].max.y, ground[index].max.z);

	//옆뒤
	glVertex3f(ground[index].min.x, ground[index].max.y, ground[index].min.z);
	glVertex3f(ground[index].min.x, ground[index].min.y, ground[index].min.z);
	glVertex3f(ground[index].min.x, ground[index].min.y, ground[index].max.z);
	glVertex3f(ground[index].min.x, ground[index].max.y, ground[index].max.z);

	//윗
	glVertex3f(ground[index].min.x, ground[index].max.y, ground[index].max.z);
	glVertex3f(ground[index].min.x, ground[index].min.y, ground[index].max.z);
	glVertex3f(ground[index].max.x, ground[index].min.y, ground[index].max.z);
	glVertex3f(ground[index].max.x, ground[index].max.y, ground[index].max.z);
	glEnd();

}
void  Runcu1(int index, int yy) {
	glColor4fv(c_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, c_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, c_shn);
	float x, y, z, width, height;
	runcu[index].max.x = 5 - (index * 8);
	runcu[index].min.x = 4 - (index * 8);
	runcu[index].max.y = 1 + Delta7 + yy;
	runcu[index].min.y = -1 + Delta7 + yy;
	runcu[index].max.z = 1.5 + (index * 3);
	runcu[index].min.z = 0.5 + (index * 3);
	runcu[index].center.y = runcu[index].max.y - runcu[index].min.y;
	glBegin(GL_QUADS);

	//밑
	glVertex3f(runcu[index].min.x, runcu[index].max.y, runcu[index].min.z);
	glVertex3f(runcu[index].max.x, runcu[index].max.y, runcu[index].min.z);
	glVertex3f(runcu[index].max.x, runcu[index].min.y, runcu[index].min.z);
	glVertex3f(runcu[index].min.x, runcu[index].min.y, runcu[index].min.z);

	//옆x
	glVertex3f(runcu[index].min.x, runcu[index].min.y, runcu[index].max.z);
	glVertex3f(runcu[index].max.x, runcu[index].min.y, runcu[index].max.z);
	glVertex3f(runcu[index].min.x, runcu[index].min.y, runcu[index].min.z);
	glVertex3f(runcu[index].max.x, runcu[index].min.y, runcu[index].min.z);

	//옆 앞
	glVertex3f(runcu[index].max.x, runcu[index].min.y, runcu[index].max.z);
	glVertex3f(runcu[index].max.x, runcu[index].min.y, runcu[index].min.z);
	glVertex3f(runcu[index].max.x, runcu[index].max.y, runcu[index].min.z);
	glVertex3f(runcu[index].max.x, runcu[index].max.y, runcu[index].max.z);

	//옆 y
	glVertex3f(runcu[index].max.x, runcu[index].max.y, runcu[index].min.z);
	glVertex3f(runcu[index].min.x, runcu[index].max.y, runcu[index].min.z);
	glVertex3f(runcu[index].min.x, runcu[index].max.y, runcu[index].min.z);
	glVertex3f(runcu[index].min.x, runcu[index].max.y, runcu[index].max.z);

	//옆뒤
	glVertex3f(runcu[index].min.x, runcu[index].max.y, runcu[index].min.z);
	glVertex3f(runcu[index].min.x, runcu[index].min.y, runcu[index].min.z);
	glVertex3f(runcu[index].min.x, runcu[index].min.y, runcu[index].max.z);
	glVertex3f(runcu[index].min.x, runcu[index].max.y, runcu[index].max.z);

	//윗
	glVertex3f(runcu[index].min.x, runcu[index].max.y, runcu[index].max.z);
	glVertex3f(runcu[index].min.x, runcu[index].min.y, runcu[index].max.z);
	glVertex3f(runcu[index].max.x, runcu[index].min.y, runcu[index].max.z);
	glVertex3f(runcu[index].max.x, runcu[index].max.y, runcu[index].max.z);

	glEnd();

}
void  Runcube1(int index, int yy) {
	glColor4fv(b_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, b_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, b_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, b_shn);
	float x, y, z, width, height;
	runcube[index].max.x = 3 - (index * 8);
	runcube[index].min.x = -3 - (index * 8);
	runcube[index].max.y = 3 + Delta + yy;
	runcube[index].min.y = -3 + Delta + yy;
	runcube[index].max.z = 0.5 + (index * 3);
	runcube[index].min.z = -0.5 + (index * 3);
	runcube[index].center.y = runcube[index].max.y - runcube[index].min.y;
	glBegin(GL_QUADS);

	//밑
	glVertex3f(runcube[index].min.x, runcube[index].max.y, runcube[index].min.z);
	glVertex3f(runcube[index].max.x, runcube[index].max.y, runcube[index].min.z);
	glVertex3f(runcube[index].max.x, runcube[index].min.y, runcube[index].min.z);
	glVertex3f(runcube[index].min.x, runcube[index].min.y, runcube[index].min.z);

	//옆x
	glVertex3f(runcube[index].min.x, runcube[index].min.y, runcube[index].max.z);
	glVertex3f(runcube[index].max.x, runcube[index].min.y, runcube[index].max.z);
	glVertex3f(runcube[index].min.x, runcube[index].min.y, runcube[index].min.z);
	glVertex3f(runcube[index].max.x, runcube[index].min.y, runcube[index].min.z);

	//옆 앞
	glVertex3f(runcube[index].max.x, runcube[index].min.y, runcube[index].max.z);
	glVertex3f(runcube[index].max.x, runcube[index].min.y, runcube[index].min.z);
	glVertex3f(runcube[index].max.x, runcube[index].max.y, runcube[index].min.z);
	glVertex3f(runcube[index].max.x, runcube[index].max.y, runcube[index].max.z);

	//옆 y
	glVertex3f(runcube[index].max.x, runcube[index].max.y, runcube[index].min.z);
	glVertex3f(runcube[index].min.x, runcube[index].max.y, runcube[index].min.z);
	glVertex3f(runcube[index].min.x, runcube[index].max.y, runcube[index].min.z);
	glVertex3f(runcube[index].min.x, runcube[index].max.y, runcube[index].max.z);

	//옆뒤
	glVertex3f(runcube[index].min.x, runcube[index].max.y, runcube[index].min.z);
	glVertex3f(runcube[index].min.x, runcube[index].min.y, runcube[index].min.z);
	glVertex3f(runcube[index].min.x, runcube[index].min.y, runcube[index].max.z);
	glVertex3f(runcube[index].min.x, runcube[index].max.y, runcube[index].max.z);

	//윗
	glVertex3f(runcube[index].min.x, runcube[index].max.y, runcube[index].max.z);
	glVertex3f(runcube[index].min.x, runcube[index].min.y, runcube[index].max.z);
	glVertex3f(runcube[index].max.x, runcube[index].min.y, runcube[index].max.z);
	glVertex3f(runcube[index].max.x, runcube[index].max.y, runcube[index].max.z);

	glEnd();

}
void  Runcube2(int index, int yy) {
	glColor4fv(b_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, b_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, b_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, b_shn);
	float x, y, z, width, height;
	runcubee[index].max.x = 3 - (index * 8);
	runcubee[index].min.x = -3 - (index * 8);
	runcubee[index].max.y = 3 + Delta5 + yy;
	runcubee[index].min.y = -3 + Delta5 + yy;
	runcubee[index].max.z = 0.5 + (index * 3);
	runcubee[index].min.z = -0.5 + (index * 3);
	runcubee[index].center.y = runcubee[index].max.y - runcubee[index].min.y;
	glBegin(GL_QUADS);

	//밑
	glVertex3f(runcubee[index].min.x, runcubee[index].max.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].max.x, runcubee[index].max.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].max.x, runcubee[index].min.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].min.x, runcubee[index].min.y, runcubee[index].min.z);

	//옆x
	glVertex3f(runcubee[index].min.x, runcubee[index].min.y, runcubee[index].max.z);
	glVertex3f(runcubee[index].max.x, runcubee[index].min.y, runcubee[index].max.z);
	glVertex3f(runcubee[index].min.x, runcubee[index].min.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].max.x, runcubee[index].min.y, runcubee[index].min.z);

	//옆 앞
	glVertex3f(runcubee[index].max.x, runcubee[index].min.y, runcubee[index].max.z);
	glVertex3f(runcubee[index].max.x, runcubee[index].min.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].max.x, runcubee[index].max.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].max.x, runcubee[index].max.y, runcubee[index].max.z);

	//옆 y
	glVertex3f(runcubee[index].max.x, runcubee[index].max.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].min.x, runcubee[index].max.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].min.x, runcubee[index].max.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].min.x, runcubee[index].max.y, runcubee[index].max.z);
	//옆뒤
	glVertex3f(runcubee[index].min.x, runcubee[index].max.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].min.x, runcubee[index].min.y, runcubee[index].min.z);
	glVertex3f(runcubee[index].min.x, runcubee[index].min.y, runcubee[index].max.z);
	glVertex3f(runcubee[index].min.x, runcubee[index].max.y, runcubee[index].max.z);

	//윗
	glVertex3f(runcubee[index].min.x, runcubee[index].max.y, runcubee[index].max.z);
	glVertex3f(runcubee[index].min.x, runcubee[index].min.y, runcubee[index].max.z);
	glVertex3f(runcubee[index].max.x, runcubee[index].min.y, runcubee[index].max.z);
	glVertex3f(runcubee[index].max.x, runcubee[index].max.y, runcubee[index].max.z);

	glEnd();

}
void Undergr1(int index, int yy) {
	glColor4fv(q_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, q_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, q_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, q_shn);
	undergr[index].max.x = 3 - (index * 8);
	undergr[index].min.x = -3 - (index * 8);
	undergr[index].max.y = 3 + yy;
	undergr[index].min.y = -3 + yy;
	undergr[index].max.z = 0.5 + (index * 3) - Delta1;
	undergr[index].min.z = -0.5 + (index * 3) - Delta1;

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(undergr[index].min.x, undergr[index].max.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].max.x, undergr[index].max.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].max.x, undergr[index].min.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].min.x, undergr[index].min.y, undergr[index].min.z - Delta1);

	//옆x
	glVertex3f(undergr[index].min.x, undergr[index].min.y, undergr[index].max.z - Delta1);
	glVertex3f(undergr[index].max.x, undergr[index].min.y, undergr[index].max.z - Delta1);
	glVertex3f(undergr[index].min.x, undergr[index].min.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].max.x, undergr[index].min.y, undergr[index].min.z - Delta1);

	//옆 앞
	glVertex3f(undergr[index].max.x, undergr[index].min.y, undergr[index].max.z - Delta1);
	glVertex3f(undergr[index].max.x, undergr[index].min.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].max.x, undergr[index].max.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].max.x, undergr[index].max.y, undergr[index].max.z - Delta1);

	//옆 y
	glVertex3f(undergr[index].max.x, undergr[index].max.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].min.x, undergr[index].max.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].min.x, undergr[index].max.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].min.x, undergr[index].max.y, undergr[index].max.z - Delta1);
	//옆뒤
	glVertex3f(undergr[index].min.x, undergr[index].max.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].min.x, undergr[index].min.y, undergr[index].min.z - Delta1);
	glVertex3f(undergr[index].min.x, undergr[index].min.y, undergr[index].max.z - Delta1);
	glVertex3f(undergr[index].min.x, undergr[index].max.y, undergr[index].max.z - Delta1);

	//윗
	glVertex3f(undergr[index].min.x, undergr[index].max.y, undergr[index].max.z - Delta1);
	glVertex3f(undergr[index].min.x, undergr[index].min.y, undergr[index].max.z - Delta1);
	glVertex3f(undergr[index].max.x, undergr[index].min.y, undergr[index].max.z - Delta1);
	glVertex3f(undergr[index].max.x, undergr[index].max.y, undergr[index].max.z - Delta1);
	glEnd();
}
void Undergr2(int index, int yy) {
	glColor4fv(q_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, q_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, q_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, q_shn);
	undergrr[index].max.x = 3 - (index * 8);
	undergrr[index].min.x = -3 - (index * 8);
	undergrr[index].max.y = 3 + yy;
	undergrr[index].min.y = -3 + yy;
	undergrr[index].max.z = 0.5 + (index * 3) - Delta2;
	undergrr[index].min.z = -0.5 + (index * 3) - Delta2;

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(undergrr[index].min.x, undergrr[index].max.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].max.x, undergrr[index].max.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].max.x, undergrr[index].min.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].min.x, undergrr[index].min.y, undergrr[index].min.z - Delta2);

	//옆x
	glVertex3f(undergrr[index].min.x, undergrr[index].min.y, undergrr[index].max.z - Delta2);
	glVertex3f(undergrr[index].max.x, undergrr[index].min.y, undergrr[index].max.z - Delta2);
	glVertex3f(undergrr[index].min.x, undergrr[index].min.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].max.x, undergrr[index].min.y, undergrr[index].min.z - Delta2);

	//옆 앞
	glVertex3f(undergrr[index].max.x, undergrr[index].min.y, undergrr[index].max.z - Delta2);
	glVertex3f(undergrr[index].max.x, undergrr[index].min.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].max.x, undergrr[index].max.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].max.x, undergrr[index].max.y, undergrr[index].max.z - Delta2);

	//옆 y
	glVertex3f(undergrr[index].max.x, undergrr[index].max.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].min.x, undergrr[index].max.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].min.x, undergrr[index].max.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].min.x, undergrr[index].max.y, undergrr[index].max.z - Delta2);
	//옆뒤
	glVertex3f(undergrr[index].min.x, undergrr[index].max.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].min.x, undergrr[index].min.y, undergrr[index].min.z - Delta2);
	glVertex3f(undergrr[index].min.x, undergrr[index].min.y, undergrr[index].max.z - Delta2);
	glVertex3f(undergrr[index].min.x, undergrr[index].max.y, undergrr[index].max.z - Delta2);

	//윗
	glVertex3f(undergrr[index].min.x, undergrr[index].max.y, undergrr[index].max.z - Delta2);
	glVertex3f(undergrr[index].min.x, undergrr[index].min.y, undergrr[index].max.z - Delta2);
	glVertex3f(undergrr[index].max.x, undergrr[index].min.y, undergrr[index].max.z - Delta2);
	glVertex3f(undergrr[index].max.x, undergrr[index].max.y, undergrr[index].max.z - Delta2);
	glEnd();
}
void Undergr3(int index, int yy) {
	glColor4fv(q_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, q_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, q_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, q_shn);
	undergrrr[index].max.x = 3 - (index * 8);
	undergrrr[index].min.x = -3 - (index * 8);
	undergrrr[index].max.y = 3 + yy;
	undergrrr[index].min.y = -3 + yy;
	undergrrr[index].max.z = 0.5 + (index * 3) - Delta3;
	undergrrr[index].min.z = -0.5 + (index * 3) - Delta3;

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].max.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].max.x, undergrrr[index].max.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].max.x, undergrrr[index].min.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].min.y, undergrrr[index].min.z - Delta3);

	//옆x
	glVertex3f(undergrrr[index].min.x, undergrrr[index].min.y, undergrrr[index].max.z - Delta3);
	glVertex3f(undergrrr[index].max.x, undergrrr[index].min.y, undergrrr[index].max.z - Delta3);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].min.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].max.x, undergrrr[index].min.y, undergrrr[index].min.z - Delta3);

	//옆 앞
	glVertex3f(undergrrr[index].max.x, undergrrr[index].min.y, undergrrr[index].max.z - Delta3);
	glVertex3f(undergrrr[index].max.x, undergrrr[index].min.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].max.x, undergrrr[index].max.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].max.x, undergrrr[index].max.y, undergrrr[index].max.z - Delta3);

	//옆 y
	glVertex3f(undergrrr[index].max.x, undergrrr[index].max.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].max.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].max.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].max.y, undergrrr[index].max.z - Delta3);
	//옆뒤
	glVertex3f(undergrrr[index].min.x, undergrrr[index].max.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].min.y, undergrrr[index].min.z - Delta3);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].min.y, undergrrr[index].max.z - Delta3);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].max.y, undergrrr[index].max.z - Delta3);

	//윗
	glVertex3f(undergrrr[index].min.x, undergrrr[index].max.y, undergrrr[index].max.z - Delta3);
	glVertex3f(undergrrr[index].min.x, undergrrr[index].min.y, undergrrr[index].max.z - Delta3);
	glVertex3f(undergrrr[index].max.x, undergrrr[index].min.y, undergrrr[index].max.z - Delta3);
	glVertex3f(undergrrr[index].max.x, undergrrr[index].max.y, undergrrr[index].max.z - Delta3);
	glEnd();
}
void Undergr4(int index, int yy) {
	glColor4fv(q_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, q_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, q_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, q_shn);
	undergrrrr[index].max.x = 3 - (index * 8);
	undergrrrr[index].min.x = -3 - (index * 8);
	undergrrrr[index].max.y = 3 + yy;
	undergrrrr[index].min.y = -3 + yy;
	undergrrrr[index].max.z = 0.5 + (index * 3) - Delta4;
	undergrrrr[index].min.z = -0.5 + (index * 3) - Delta4;

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].max.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].max.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].min.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].min.y, undergrrrr[index].min.z - Delta4);

	//옆x
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].min.y, undergrrrr[index].max.z - Delta4);
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].min.y, undergrrrr[index].max.z - Delta4);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].min.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].min.y, undergrrrr[index].min.z - Delta4);

	//옆 앞
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].min.y, undergrrrr[index].max.z - Delta4);
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].min.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].max.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].max.y, undergrrrr[index].max.z - Delta4);

	//옆 y
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].max.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].max.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].max.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].max.y, undergrrrr[index].max.z - Delta4);
	//옆뒤
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].max.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].min.y, undergrrrr[index].min.z - Delta4);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].min.y, undergrrrr[index].max.z - Delta4);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].max.y, undergrrrr[index].max.z - Delta4);

	//윗
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].max.y, undergrrrr[index].max.z - Delta4);
	glVertex3f(undergrrrr[index].min.x, undergrrrr[index].min.y, undergrrrr[index].max.z - Delta4);
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].min.y, undergrrrr[index].max.z - Delta4);
	glVertex3f(undergrrrr[index].max.x, undergrrrr[index].max.y, undergrrrr[index].max.z - Delta4);
	glEnd();
}


void MyTimer(int Value)							//오브젝트가 델타값에따라 양옆으로 움직임
{
	Delta = Delta + vel;

	if ((Delta > 6 && vel > 0) || (Delta < -1 && vel < 0))
		vel *= -1;

	glutPostRedisplay();

	glutTimerFunc(5, MyTimer, 1); 

}
void MyTimer2(int Value)//
{
	Delta1 = Delta1 + vel1;

	if ((Delta1 > 10 && vel1 > 0) || (Delta1 < -1 && vel1 < 0)) {
		vel1 *= -1;
	}

	glutPostRedisplay();
	glutTimerFunc(1000, MyTimer2, 1); //2번 타이머
}
void MyTimer3(int Value)//
{
	Delta2 = Delta2 + vel2;

	if ((Delta2 > 20 && vel2 > 0) || (Delta2 < -20 && vel2 < 0)) {
		vel2 *= -1;
	}
	glutPostRedisplay();
}
void MyTimer4(int Value)
{
	Delta5 = Delta5 + vel5;

	if ((Delta5 > 1 && vel5 > 0) || (Delta5 < -6 && vel5 < 0))
		vel5 *= -1;

	glutPostRedisplay();
	glutTimerFunc(5, MyTimer4, 1); //2번 타이머
}
void MyTimer5(int Value)//
{
	Delta3 = Delta3 + vel3;


	if ((Delta3 > 20 && vel3 > 0) || (Delta3 < -20 && vel3 < 0)) {
		vel3 *= -1;
	}
	glutPostRedisplay();
}
void MyTimer6(int Value)//
{
	Delta4 = Delta4 + vel4;
	if ((Delta4 > 20 && vel4 > 0) || (Delta4 < -20 && vel4 < 0)) {
		vel4 *= -1;
	}
	glutPostRedisplay();
}
void MyTimer7(int Value)//
{
	Delta7 = Delta7 + vel7;

	if ((Delta7 > 6 && vel7 > 0) || (Delta7 < -6 && vel7 < 0))
		vel7 *= -1;


	glutPostRedisplay();
	glutTimerFunc(1, MyTimer7, 1); //2번 타이머



}

//카메라 세팅
void camera_setting(void) {

	glMatrixMode(GL_PROJECTION);	//렌즈
	glLoadIdentity();
	gluPerspective(70.0, 1.0, 1.0, 800.0);
	//	y축 기준 60도, 비율1:1, 1부터 500거리까지 보겟다

	glMatrixMode(GL_MODELVIEW);	//카메라 위치 //필드상에 놓는 역할
	glLoadIdentity();
	gluLookAt(
		LookAtx + bmx, LookAty + bmy, LookAtz + bmz,
		ballx, bally, ballz,//	을바라본다
		0.0, 0.0, 1.0);
}

void moveUp() {
	if (jc == 0)				//jc가 0일 경우는 점프 가능, 땅에 닿을 시 jc를 0으로 선언한다.
		bmz += 0.03;
	if (bmz >= cz + 5) {		//cz는 점프 뛴 순간의 위치
		mus = 0;
		jc = 1;
	}
}
void moveFront() {
	AngleX -= 0.5;
	bmx -= 0.01;
}
void moveBack() {
	AngleX += 0.5;
	bmx += 0.01;
}
void moveL() {
	AngleY += 0.5;
	bmy -= 0.01;
}
void moveR() {
	AngleY -= 0.5;
	bmy += 0.01;
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
	if (ch == 0) {
		bmz -= 0.015;
	}
}

void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);	//렌즈
	glLoadIdentity();
	gluPerspective(75.0, 1.0, 1.0, 500.0);
}

void groundcrash() {//충돌 감지 코드
	if (ground[0].min.x < ballx && ballx < ground[0].max.x && ground[0].min.y < bally && bally < ground[0].max.y &&   ground[0].min.z < ballz - 1 && ballz - 1 < ground[0].max.z) {
		ch = 1; cc = 0; jc = 0;
	}
	else if (ground[1].min.x < ballx && ballx < ground[1].max.x && ground[1].min.y < bally && bally < ground[1].max.y &&   ground[1].min.z < ballz - 1 && ballz - 1 < ground[1].max.z) {
		ch = 1; cc = 0; jc = 0;
	}
	else if (runcube[2].min.x < ballx && ballx < runcube[2].max.x && runcube[2].min.y < bally && bally < runcube[2].max.y &&   runcube[2].min.z < ballz - 1 && ballz - 1 < runcube[2].max.z) {

		ch = 1; cc = 0; jc = 0;
	}
	else if (ground[3].min.x < ballx && ballx < ground[3].max.x && ground[3].min.y < bally && bally < ground[3].max.y &&   ground[3].min.z < ballz - 1 && ballz - 1 < ground[3].max.z) {
		ch = 1; cc = 0; jc = 0;
	}
	else if (ground[4].min.x < ballx && ballx < ground[4].max.x && ground[4].min.y < bally && bally < ground[4].max.y &&   ground[4].min.z < ballz - 1 && ballz - 1 < ground[4].max.z) {
		ch = 1; cc = 0; jc = 0;
	}
	else if (ground[5].min.x < ballx && ballx < ground[5].max.x && ground[5].min.y < bally && bally < ground[5].max.y &&   ground[5].min.z < ballz - 1 && ballz - 1 < ground[5].max.z) {
		ch = 1; cc = 0; jc = 0;
	}
	else if (undergr[6].min.x < ballx && ballx < undergr[6].max.x && undergr[6].min.y < bally && bally < undergr[6].max.y &&  undergr[6].min.z < ballz - 1 && ballz - 1 < undergr[6].max.z) {
		ch = 1; cc = 0; jc = 0;
		glutTimerFunc(1000, MyTimer2, 1);
	}
	else if (ground[7].min.x < ballx && ballx < ground[7].max.x && ground[7].min.y < bally && bally < ground[7].max.y &&   ground[7].min.z < ballz - 1 && ballz - 1 < ground[7].max.z) {
		ch = 1; cc = 0; jc = 0;
	}
	else if (undergrr[8].min.x < ballx && ballx < undergrr[8].max.x && undergrr[8].min.y < bally && bally < undergrr[8].max.y &&  undergrr[8].min.z < ballz - 1 && ballz - 1 < undergrr[8].max.z) {
		ch = 1; cc = 0; jc = 0;
		glutTimerFunc(1000, MyTimer3, 1);
	}
	else if (undergrrr[9].min.x < ballx && ballx < undergrrr[9].max.x && undergrrr[9].min.y < bally && bally < undergrrr[9].max.y &&  undergrrr[9].min.z < ballz - 1 && ballz - 1 < undergrrr[9].max.z) {
		ch = 1; cc = 0; jc = 0;
		glutTimerFunc(1000, MyTimer5, 1);
	}
	else if (undergrrrr[10].min.x < ballx && ballx < undergrrrr[10].max.x && undergrrrr[10].min.y < bally && bally < undergrrrr[10].max.y &&  undergrrrr[10].min.z < ballz - 1 && ballz - 1 < undergrrrr[10].max.z) {
		ch = 1; cc = 0; jc = 0;
		glutTimerFunc(1000, MyTimer6, 1);
	}
	else if (runcube[11].min.x < ballx && ballx < runcube[11].max.x && runcube[11].min.y < bally && bally < runcube[11].max.y &&   runcube[11].min.z < ballz - 1 && ballz - 1 < runcube[11].max.z) {

		ch = 1; cc = 0; jc = 0;
	}
	else if (runcubee[12].min.x < ballx && ballx < runcubee[12].max.x && runcubee[12].min.y < bally && bally < runcubee[12].max.y &&   runcubee[12].min.z < ballz - 1 && ballz - 1 < runcubee[12].max.z) {

		ch = 1; cc = 0; jc = 0;
	}

	else if (runcube[13].min.x < ballx && ballx < runcube[13].max.x && runcube[13].min.y < bally && bally < runcube[13].max.y &&   runcube[13].min.z < ballz - 1 && ballz - 1 < runcube[13].max.z) {

		ch = 1; cc = 0; jc = 0;
	}
	else if (ground[14].min.x < ballx && ballx < ground[14].max.x && ground[14].min.y < bally && bally < ground[14].max.y &&   ground[14].min.z < ballz - 1 && ballz - 1 < ground[14].max.z) {
		ch = 1; cc = 0; jc = 0;
	}
	else if (ground[15].min.x < ballx && ballx < ground[15].max.x && ground[15].min.y < bally && bally < ground[15].max.y &&   ground[15].min.z < ballz - 1 && ballz - 1 < ground[15].max.z) {
		ch = 1; cc = 0; jc = 0;
	}
	else if (runcu[1].min.x <= ballx && ballx <= runcu[1].max.x && runcu[1].min.y <= bally && bally <= runcu[1].center.y  &&   runcu[1].min.z <= ballz - 1 && ballz - 1 <= runcu[1].max.z) {

		bmx += 3;
		bmy -= 3;
	}
	else if (runcu[1].min.x <= ballx && ballx <= runcu[1].max.x && runcu[1].center.y <= bally && bally <= runcu[1].max.y &&   runcu[1].min.z <= ballz - 1 && ballz - 1 <= runcu[1].max.z) {

		bmx += 3;
		bmy += 3;
	}
	else if (runcu[3].min.x <= ballx && ballx <= runcu[3].max.x && runcu[3].min.y <= bally && bally <= runcu[3].center.y  &&   runcu[3].min.z <= ballz - 1 && ballz - 1 <= runcu[3].max.z) {

		bmx += 3;
		bmy -= 3;
	}
	else if (runcu[3].min.x <= ballx && ballx <= runcu[3].max.x && runcu[3].center.y <= bally && bally <= runcu[3].max.y &&   runcu[3].min.z <= ballz - 1 && ballz - 1 <= runcu[3].max.z) {

		bmx += 3;
		bmy += 3;
	}
	else if (runcu[6].min.x <= ballx && ballx <= runcu[6].max.x && runcu[6].min.y <= bally && bally <= runcu[6].center.y  &&   runcu[6].min.z <= ballz - 1 && ballz - 1 <= runcu[6].max.z) {

		bmx += 3;
		bmy -= 3;
	}
	else if (runcu[6].min.x <= ballx && ballx <= runcu[6].max.x && runcu[6].center.y <= bally && bally <= runcu[6].max.y &&   runcu[6].min.z <= ballz - 1 && ballz - 1 <= runcu[6].max.z) {

		bmx += 3;
		bmy += 3;
	}
	else if (runcu[9].min.x <= ballx && ballx <= runcu[9].max.x && runcu[9].min.y <= bally && bally <= runcu[9].center.y  &&   runcu[9].min.z <= ballz - 1 && ballz - 1 <= runcu[9].max.z) {

		bmx += 3;
		bmy -= 3;
	}
	else if (runcu[9].min.x <= ballx && ballx <= runcu[9].max.x && runcu[9].center.y <= bally && bally <= runcu[9].max.y &&   runcu[9].min.z <= ballz - 1 && ballz - 1 <= runcu[9].max.z) {

		bmx += 3;
		bmy += 3;
	}
	else if (runcu[10].min.x <= ballx && ballx <= runcu[10].max.x && runcu[10].min.y <= bally && bally <= runcu[10].center.y  &&   runcu[10].min.z <= ballz - 1 && ballz - 1 <= runcu[10].max.z) {

		bmx += 3;
		bmy -= 3;
	}
	else if (runcu[10].min.x <= ballx && ballx <= runcu[10].max.x && runcu[10].center.y <= bally && bally <= runcu[10].max.y &&   runcu[10].min.z <= ballz - 1 && ballz - 1 <= runcu[10].max.z) {

		bmx += 3;
		bmy += 3;
	}
	else { ch = 0; }
}

void Rote() {
	if (mfs == 1) {
		glRotatef(AngleX, 0.0, 1.0, 0.0);
	}
	if (mbs == 1) {
		glRotatef(AngleX, 0.0, 1.0, 0.0);
	}
	if (mls == 1) {
		glRotatef(AngleY, 1.0, 0.0, 0.0);
	}
	if (mrs == 1) {
		glRotatef(AngleY, 1.0, 0.0, 0.0);
	}
}

void display(void) {
	glClearColor(0.7, 0.7, 0.7, 0.7);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera_setting();

	gravity();					//상시 적용되는 중력

	checkbutton();
	Runcube1(2, 0);
	ground1(1, 0);
	Runcu1(1, 0);
	ground1(0, 0);
	ground1(3, 2);
	Runcu1(3, 0);
	ground1(4, 2.5);
	ground1(5, 3);
	Undergr1(6, 3);
	Runcu1(6, 0);
	ground1(7, 9);
	Undergr2(8, 9);
	Undergr3(9, 9);
	Runcu1(9, 9);
	Undergr4(10, 9);
	Runcube1(11, 9);
	Runcu1(10, 9);
	Runcube2(12, 9);
	Runcube1(13, 9);
	ground1(14, 10);
	ground1(15, 10);
	groundcrash();

	ballx = bmx;				//ballx += bmx하면 오류
	bally = bmy;
	ballz = bmz;

	glPushMatrix();
	glTranslatef(ballx, bally, ballz);
	Rote();
	DrawShape();
	glPopMatrix();

	//glTranslatef(ballx + bmx, bally + bmy, ballz + bmz);

	glFlush();
	glutSwapBuffers();
}

void UpSpecial(int key, int x, int y) {			//키보드 릴리스 될때 실행
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
		mus = 0;
		break;
	}
}
void MySpecial(int key, int x, int y) {
	switch (key) {
	case	GLUT_KEY_UP:
		mfs = 1;					//값이 1이면 움직인다 UpSpecial의 경우는 놓으면 0으로 변환한다.
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
		if (mus == 0) {
			mus = 1;
		}
		if (cc == 0) {
			cz = bmz;
			cc = 1;
		}
		break;
	case	GLUT_KEY_F2:
		//상하좌우앞뒤 움직임 감지 초기화
		mus = 0;mfs = 0;mbs = 0;mls = 0;mrs = 0;
		//움직임 제한 변수 초기화
		jc = 0;cz = 0;cc = 0;
		//공 위치 초기화
		bmx = 0.0;bmy = 0.0;bmz = 3.0;
		break;
	}
	glutPostRedisplay();
}


int main(void) {
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
	glutTimerFunc(10, MyTimer, 1);//양쪽이동블록
	glutTimerFunc(10, MyTimer4, 1);
	glutTimerFunc(1, MyTimer7, 1);
	glutTimerFunc(20, MyTimer2, 1); //2번 타이머

	glutMainLoop();
	return 1;
}