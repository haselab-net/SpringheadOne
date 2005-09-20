// SLCamera.h: SLCamera �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_)
#define AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include "CySLC.h"

#define PI 3.14159265358979
#define MarkerNumMax 10

class SLCamera{

struct TMarker{
	Spr::Vec3f pos;	// �}�[�J�[�ʒu
	Spr::Vec3f vel;	// �}�[�J�[���x
//	int state;		// �}�[�J�[���
//	int match;		// �}�b�`���x�� (-1:���� 0,1,...:���x���ԍ�)
//	float dist[MarkerNumMax];	// ���x���Ƃ̋���
};

protected:
	HANDLE lCamera;
	int labelNum;           //���x�����O�J�����Ŏ擾���郉�x���̐�
	int lastLabelNum;
	enum {labelNumMax = 64};//�ꉞ64�ɂ��Ă��邪���Ȃ����Ă��\��Ȃ��̂ł́D
	enum {LABEL_NUM = 3};
	SLCOBJECT labelPos[labelNumMax]; //���x���̏����i�[����

public:
	TMarker marker[MarkerNumMax];	// �}�[�J�[
	int markerNum;
//	int minIndex[labelNumMax];
//	int maxLabelNum;
//	float minCost;

	SLCamera();
	virtual ~SLCamera();
	bool InitSLC();
	void CloseSLCamera();

	bool Draw(Spr::GRRender* render);
	Spr::Vec3f Convert(SLCOBJECT label);
//	void Matching(int lnum);
//	float CalcTotalCost(int lnum, int l, float cost);
//	void SetPosition(float dt);
	void SetMarkerPos();
	bool Step(float dt);
};

#endif // !defined(AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_)
