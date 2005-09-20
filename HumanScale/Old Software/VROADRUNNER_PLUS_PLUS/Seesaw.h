// Seesaw.h: CSeesaw クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEESAW_H__09341BB2_9896_4D1F_B908_B8F8858CC5CB__INCLUDED_)
#define AFX_SEESAW_H__09341BB2_9896_4D1F_B908_B8F8858CC5CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <GL/glut.h>

#include <stdio.h>

namespace vroadrunner{

#define F_SIZE 6
#define V_SIZE 8
#define E_SIZE 12

class CSeesaw
{
public:
	GLdouble vertex[V_SIZE][3];
	int static edge[][2];
	int static face[][4];
	GLdouble static normal[][3];

	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;

	GLdouble w;
	GLdouble d;
	GLdouble h;

	GLdouble x;
	GLdouble y;
	GLdouble z;

	int r; //シーソーの傾き角

public:
	void SetMaterial();
	void OnDraw(GLdouble x,GLdouble y,GLdouble z);
	void InitInstance(GLdouble w,GLdouble h,GLdouble d);
	void InitInstance(GLdouble x,GLdouble y,GLdouble z,GLdouble w,GLdouble h,GLdouble d);
	void OnDraw();
	void SetVertex();
	CSeesaw();
	CSeesaw(GLdouble w,GLdouble h, GLdouble d);
	virtual ~CSeesaw();

private:
};

}

#endif // !defined(AFX_SEESAW_H__09341BB2_9896_4D1F_B908_B8F8858CC5CB__INCLUDED_)
