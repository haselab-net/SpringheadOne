// MoveFloor.h: CMoveFloor �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEFLOOR_H__91F5DF09_A733_47F4_834D_BFD580470BF5__INCLUDED_)
#define AFX_MOVEFLOOR_H__91F5DF09_A733_47F4_834D_BFD580470BF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<GL/glut.h>
#include<stdio.h>

namespace vroadrunner{

#define F_SIZE 6
#define V_SIZE 8
#define E_SIZE 12

class CMoveFloor  
{
public:
	GLfloat vertex[V_SIZE][3];
	int static edge[][2];
	int static face[][4];
	GLdouble static normal[][3];

	//�������̏ꏊ(�n�_�̈ʒu�j
	GLfloat x;
	GLfloat y;
	GLfloat z;

	//�������̑傫��
	GLfloat w;
	GLfloat d;
	GLfloat h;
	
	//���̈ړ���
	GLfloat mx;
	GLfloat my;
	GLfloat mz;
	
	//�������̈ړ������i�Г��̎��ԁj
	GLint T;

	//���������]�܂ł̎���(�Г�)
	GLint reverseT;

	//���̑��x�i�ړ��� / ���ԁj
	GLfloat vx;
	GLfloat vy;
	GLfloat vz;	
	
	//���̓��������i����T�Ŕ��]�j
	GLint floorDirection;
	
	//�����������ǂ���(�͂������Ŏ~�܂�)
	bool bMove;

	//floorTime������T�ȏ�ɂȂ����瓮�����������]
	GLint floorTime;
	
public:
	int stopT;
	void OnDraw();
	void OnDraw(GLfloat *x,GLfloat *y,GLfloat *z, bool *b);
	void InitInstance(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d);
	void InitInstance(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d, GLfloat mx, GLfloat my, GLfloat mz, GLfloat T);
	void SetVertex();
	void Judge(GLfloat *bx, GLfloat *by, GLfloat *bz, bool *bGravity);
	CMoveFloor();
	//CMoveFloor(GLfloat w,GLfloat h, GLfloat d);
	virtual ~CMoveFloor();

private:
};

}

#endif // !defined(AFX_MOVEFLOOR_H__91F5DF09_A733_47F4_834D_BFD580470BF5__INCLUDED_)
