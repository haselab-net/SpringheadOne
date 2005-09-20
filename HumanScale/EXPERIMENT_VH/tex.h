#ifndef _CMyTex_H
#define _CMyTex_H

#include <GL/glaux.h>
#include <GL/glut.h>
#include "mymath.h"

//used inside glutIdle() to keep constant frame rate
#define	GRAVITY 10.0      //13.5
#define STATIC_FRIC 0.87  //99
//JEONG 10/21
#define TEX_NUM 3 //11

//JEONG
static DWORD dwLastTime;
static DWORD dwTime;

static GLfloat green[] = {0.25, 1.0, 0.25, 1.0};
static GLfloat red[] = {1.0, 0.25, 0.25, 1.0};
static GLfloat blue[] = {0.25, 0.25, 1.0, 1.0};
static GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat gray[] = {0.5, 0.5, 0.5, 1.0};
static GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
static GLfloat yellow[] = {1.0, 1.0, 0.25, 1.0};
static GLfloat headcolor[] = {1.0, 0.8, 0.6, 1.0};
static GLfloat wallcolor[] = {0.64, 0.47, 0.36, 1.0};
static GLfloat shadow[] = { 0.0, 0.0, 0.0, 0.5   };
static GLfloat bodycolor[] = {1.0, 0.5, 0.5, 1.0};

class CMyTex  
{
public:
	CMyTex();
	virtual ~CMyTex();

	AUX_RGBImageRec *texRec[TEX_NUM];
    GLuint tex[TEX_NUM];

	AUX_RGBImageRec* LoadBMPFile(char *filename);
	void Load_Texture();
	void drawBox();
	void drawQuad(double width, double height);
	void drawSphere(float rad);
	void Draw_Ellipse(float x, float y, float z, MyVector scale);
	void myglSolidFrustum(short Nxy,float ratio);//â~êçë‰
	void mySolidCylinder(float x, float y, float z, float r, int n);
	float Passed(float oldTime);

};


#endif 