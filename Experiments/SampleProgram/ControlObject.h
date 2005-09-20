// ControlObject.h: ControlObject �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTROL_OBJECT_H
#define CONTROL_OBJECT_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>

#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>

#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>

#include <Base/TQuaternion.h>
#include <Base/Affine.h>

#include <vector>

#define Num 4

//ZMP�v�Z�ɕK�v�ȏd�͂̒萔
#define g_x 0
#define g_y 9.8
#define g_z 0

using namespace Spr;

class ControlObject  
{
public:
	ControlObject();
	virtual ~ControlObject();
	void Step(SGScene* scene,GRRender* render);
	void ReLoad();
	void Draw(GRRender* render);

	void FindObj(SGScene* scene);	//�\���b�h,�W���C���g�̎擾
	
	void OnKeyDown(UINT& nChar);

	bool bObj;						//�\���b�h���擾�������ۂ�
	PHSolid* soObj1;
	PHSolid* soObj2;
	PHJoint* joObj1;
	PHJoint* joObj2;

	void SetCenterMass();
	void SetScale();				//�I�u�W�F�N�g�̃X�P�[����ύX����B

	enum Color { red, blue, green }col;

	//int value[Color.max] = [ blue:6, green:2, red:5 ];
	int tete[3];

	//�`��̂��߂̊֐�
	Spr::GRMaterialData Mat(Spr::Vec4f c){
		return Spr::GRMaterialData(c, c, c,	c, 0.0f);
	}

	std::vector<int> test[Num];		// VH���T�ɕ����Ď��[

	void CalcZMP(SGScene* scene);
	void GetAccel(double dt);
	Spr::Vec3d vh_zmp;
	std::vector<Spr::Vec3d> Accel;
	std::vector<Spr::Vec3d> preVelocity;		//zmp�̌v�Z�ɕK�v��vh�̑��x
	Spr::Vec3d nowVelocity;
	bool first;									//GetAccel���Ăяo���ꂽ�̂����ڂ��ۂ��B

	std::vector<PHSolid*> solids;
	std::vector<PHJoint*> joints;

	bool antei;

};

#endif
