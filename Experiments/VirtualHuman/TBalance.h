// TBalance.h: TBalance �N���X�̃C���^�[�t�F�C�X
//
//	VH�̃o�����X�ɕK�v�Ȍv�Z�`������Ă����B
//	
//
//////////////////////////////////////////////////////////////////////

#ifndef TBALANCE_H
#define TBALANCE_H

#include <Physics/PHHuman.h>

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include <vector>

#include "THuman.h"
#include "CollisionArea.h"
//#include "QuadricPR.h"

//ZMP�v�Z�ɕK�v�ȏd�͂̒萔
#define g_x 0
#define g_y 9.8
#define g_z 0

class TBalance  
{
private:

	//�񎟌v��@
	//QuadricPR quadPR;

	//ZMP�̌v�Z
	Spr::Vec3d vh_zmp;
	Spr::Vec3d preVelocityTest;					//zmp�̌v�Z�ɕK�v��vh�̑��x
	Spr::Vec3d nowVelocityTest;

	std::vector<Spr::Vec3d> preVelocity;		//zmp�̌v�Z�ɕK�v��vh�̑��x
	Spr::Vec3d nowVelocity;
	bool first;									//GetAccel���Ăяo���ꂽ�̂����ڂ��ۂ��B

	std::vector<Spr::Vec3d> Accel;				//���鎞�Ԃ�VH�̊e�����N�̉����x������

	//ZMP�̑���̈�`�F�b�N
	Spr::Vec3f footPoint[8];			//�����̈�̒[�_
	Spr::Vec3d footLCenter,footRCenter;	//���E�̑��̒��S�̈ʒu
	Spr::Vec3f bBoxMax,bBoxMin;

public:
	TBalance();
	virtual ~TBalance();
	
	//�@�o�����X���`�F�b�N�ɕK�v�Ȃ��̂�S�ČĂ�ł����B
	void CheckBalance(Spr::GRRender* render, Spr::PHHuman* phHuman, double dt);

	void CalcZmp(Spr::PHHuman* phHuman, double dt);			//VH��ZMP���v�Z����
	void GetAccel(Spr::PHHuman* phHuman, double dt);			//ZMP�v�Z�ɕK�v��VH�̊e�����N�̉����x���v�Z����B
	bool DrawZmp(Spr::PHHuman* phHuman, Spr::GRRender* render);		//ZMP�̕`��
	Spr::Vec3d GetZmp();									//ZMP���擾
	bool ZmpIncludeCheck();					//ZMP������̈���ɂ��邩�Ď� 
	bool PosIncludeTriangle(Spr::Vec3f t_a, Spr::Vec3f t_b, Spr::Vec3f t_c, Spr::Vec3f point);
	
	//����̈�i���Ƃ̐ڐG�̈�j�̌v�Z�N���X
	CollisionArea collisionArea;

	void GetAccelTest(THuman& human, double dt);
	void CalcZmpTest(THuman& human, double dt);
	
	//�@�g���ĂȂ�
	void CalcFootPoint(THuman& human);				//�����̈�̌v�Z
	Spr::Vec3f* GetFootPoint();						//�����̈�̒[�_�̎擾

	void Reload();									//Reload���ꂽ���̊֐�

};
#endif
