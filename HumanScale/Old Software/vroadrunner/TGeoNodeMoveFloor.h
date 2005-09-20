// TGeoNodeMoveFloor.h: TGeoNodeMoveFloor �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGEONODEMOVEFLOOR_H__016B39B2_55CC_464B_BB57_8E4A15F9350A__INCLUDED_)
#define AFX_TGEONODEMOVEFLOOR_H__016B39B2_55CC_464B_BB57_8E4A15F9350A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoNode.h"
#include "GLView.h"

class TGeoNodeMoveFloor : public TGeoNode  
{
public:
	//�������̏ꏊ(�n�_�̈ʒu�j
	GLfloat x;
	GLfloat y;
	GLfloat z;
	
	//���̈ړ���
	GLfloat dist;
	
	//�������̈ړ������i�Г��̎��ԁj
	GLint T;
	GLint reverseT;
	
	//���̑��x�i�ړ��� / ���ԁj
	GLfloat vel;	
	
	//���̓��������i����T�Ŕ��]�j
	GLint floorDirection;

	//�����������ǂ���(�͂������Ŏ~�܂�)
	bool bMove;

	//���̏����ʒu����̈ړ���
	GLfloat mv;
	
//	//floorTime������T�ȏ�ɂȂ����瓮�����������]
	GLint floorTime;

	//Affinef afTrans;

public:
	int stopT;
	int floorNo;
	void Draw();
	void InitInstance(string name);
	TGeoNodeMoveFloor();
	virtual ~TGeoNodeMoveFloor();

};

#endif // !defined(AFX_TGEONODEMOVEFLOOR_H__016B39B2_55CC_464B_BB57_8E4A15F9350A__INCLUDED_)
