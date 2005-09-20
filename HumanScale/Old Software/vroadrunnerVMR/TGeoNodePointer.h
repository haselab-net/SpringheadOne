// TGeoNodePointer.h: TGeoNodePointer �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGEONODEPOINTER_H__5D26FE86_D268_4D67_8863_91CAEE101F70__INCLUDED_)
#define AFX_TGEONODEPOINTER_H__5D26FE86_D268_4D67_8863_91CAEE101F70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoNode.h"
#define ALGORITHM2

class TGeoNodePointer : public TGeoNode  
{
public:
	bool isDraw;
	bool isAttack;
	float g;
	float dy;
	float dz;
#ifdef ALGORITHM2
	int count; //����Draw�����s���ꂽ���̃J�E���g
	int upcount; //�㏸�I���܂ł̃J�E���g
	int downcount; //���~�J�n�܂ł̃J�E���g
#endif

public:
	bool IsDraw();
	float radius;
	float GetRadius();
	void Draw();
	void Step(); // Step�̎��ɌĂԁB�O�i�Ȃ�
	bool Attack( Affinef &_afStart ); // �ł�
	bool Attack( Affinef &afCam,Affinef &afLaser );
	Affinef GetPosition();
	void Hide();
	TGeoNodePointer();
	virtual ~TGeoNodePointer();

};

#endif // !defined(AFX_TGEONODEPOINTER_H__5D26FE86_D268_4D67_8863_91CAEE101F70__INCLUDED_)
