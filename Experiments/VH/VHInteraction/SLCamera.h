// SLCamera.h: SLCamera �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_)
#define AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SceneGraph/SGScene.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
//#include <Base/TMatrix.h>
#include <Base/Affine.h>
#include <Creature/CRHuman.h>
#include <Creature/CRUser.h>
#include "CySLC.h"

namespace Spr{;

class SLCamera{
	class TMarker{
	public:
		Spr::Vec3f pos;	// �}�[�J�[�ʒu
		Spr::Vec3f vel;	// �}�[�J�[���x
		int state;		// �}�[�J�[���(0:����, 1:�ŐV, 2:�Â�)
		int match;		// �}�b�`���x�� (-1:���� 0,1,...:���x���ԍ�)
		std::vector<float> dist;	// �擾�������x���ʒu�ƒ��O�̃}�[�J�[�ʒu�Ƃ̋���
	
		TMarker(){
			pos = Spr::Vec3f(0,0,0);
			vel = Spr::Vec3f(0,0,0);
			state = 0;
			match = -1;
		};
	};

protected:
	HANDLE lCamera;
	int labelNum;           //���x�����O�J�����Ŏ擾���郉�x���̐�
	int lastLabelNum;
	enum {labelNumMax = 64};//�ꉞ64�ɂ��Ă��邪���Ȃ����Ă��\��Ȃ��̂ł́D
	enum {LABEL_NUM = 3};
	SLCOBJECT labelPos[labelNumMax]; //���x���̏����i�[����

public:
	#define MaxMarkerNum 3			// �g���b�N���O�}�[�J�[�̐�

	bool bStep;
	bool bCalib;
	Spr::Vec3f tmpLabelPos[labelNumMax];	// ���x���ʒu�̉��ۑ�
	TMarker marker[MaxMarkerNum];			// �}�[�J�[
	int match[labelNumMax];					// ���x���̃}�[�J�[�Ƃ̑Ή�
	int markerNum;							// �擾�������x����(�L���ȃ}�[�J�[��)

	Affinef T;	// ���W�ϊ��s��

	SLCamera();
	virtual ~SLCamera();
	bool InitSLC();
	void CloseSLCamera();

	void Init();
	void SetInitMarkerPos(Spr::Vec3f* soPos);
	bool Draw(Spr::GRRender* render);

	void Calib(SLCOBJECT label);
	Spr::Vec3f Convert(SLCOBJECT label);
	void SaveLabelPos();
	void SaveDistance();
	void IncorrectRecognitionCheck();
	//void SearchNeighborhood();

	float CalcTotalCost(int m, float cost, float min);
	void Matching();
	void CaseOfErasure();

	bool Step(Spr::SGScene* scene);

	void SetUserPos(CRUser* user); 
};

}

#endif // !defined(AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_)
