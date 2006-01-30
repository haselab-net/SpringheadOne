// CRAttention.h: CRAttention �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRATTENTION_H
#define CRATTENTION_H

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <SceneGraph/SGScene.h>
#include <SceneGraph/SGFrame.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PhSolid.h>
#include <Physics/PhJoint.h>
#include <Physics/PhJointPid.h>
#include <Base/Affine.h>
#include <HIS/HIMouse.h>
#include <vector>
#include <Creature/CREye.h>
#include <Creature/CRNeckController.h>
#include <fstream>

namespace Spr{;

class CRPuppet;

// ���ӂ̃N���X
class CRAttention{
public:
	CRAttention();
	virtual ~CRAttention();

public:
//---------�@��{�C���^�t�F�[�X�@---------//
	void Step();
	void Load(SGScene* scene,CRPuppet* crPuppet, CREye* crEye,CRNeckController* crNeckController);
	virtual void OnKeyDown(UINT &nChar);
	virtual void Draw(GRRender* render);
	virtual void Init();

//------------�@�g���C���^�t�F�[�X�@-----------//
	/// �����_�̐ݒ�
	void SetAttentionPoint(Vec3f position, float ammount);
	void SetAttentionSolid(PHSolid* solid, float ammount);
	void StartExperiment();

//-----------------�@�����@----------------//
	void CalcMaxAttentionPoint();

//-----------------�@�����o�ϐ��@----------------//
	// �֘A���W���[��
	SGScene* scene; // �ėp
	CRPuppet* crPuppet; // ���o�͗p
	CREye* crEye; // �o�͗p
	CRNeckController* crNeckController; // �o�͗p

	// ���ӊ��N�Ώۂ̃��X�g
	class AttentionInfo{
	public:
		AttentionInfo(){ammount = 0.0f; position = Vec3f(0.0f, 0.0f, 0.0f);}
		AttentionInfo(float a, Vec3f p): ammount(a), position(p) {}
		float ammount;
		Vec3f position;
	};
	typedef std::map<PHSolid*,AttentionInfo>            AttentionList;
	typedef std::map<PHSolid*,AttentionInfo>::iterator  AttentionListIter;
	typedef std::pair<PHSolid*,AttentionInfo>           AttentionListItem;
	AttentionList attentionList;

	float    maxAttentionAmmount;
	PHSolid* maxAttentionSolid;
	Vec3f    maxAttentionPoint;
	bool bFoundAttention;
	bool bHeadControl;
	bool bChangeAttention;
	bool bActiveAttention;
	DWORD attentionChangeTime;

	// ����
	bool bExperimentMode;
	std::ofstream ofs_head;
	std::ofstream ofs_hedg;
	std::ofstream ofs_leye;
	std::ofstream ofs_reye;
	std::ofstream ofs_gaze;
	std::ofstream ofs_eyes;
	std::ofstream ofs_info;
	std::vector<int>   timinglist;
	std::vector<Vec3f> poslist;
	int counter;
	DWORD startTiming;
	
};

}		//	end namespace Spr
#endif 
