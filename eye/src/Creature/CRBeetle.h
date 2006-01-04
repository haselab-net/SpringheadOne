#pragma once

#include <Physics/PHSolid.h>
#include <Physics/PHJointPid.h>
#include <Physics/PHContactEngine.h>

namespace Spr{;
//�J�u�g���V�R���g���[��
//�����F���̃��R�r�A��
/*
	(�Ҋ֐�Yaw, �Ҋ֐�Pitch, �G�֐�) = (q1, q2, q3)
	(�ڒ�, ����) = (l1, l2)
	J = {
		-c1(l1c2 + l2c(2-3))	 s1(l1s2 + l2s(2-3))	-l2s1s(2-3)
		-s1(l1c2 + l2c(2-3))	-c1(l1s2 + l2s(2-3))	 l2c1s(2-3)
		0							l1c2 + l2c(2-3)		-l2c(2-3)
	}
	det(J) = -l1l2(l1c2 + l2c(2-3))s3
	ci := cos(qi), si := sin(qi)
*/
enum {LEFT, RIGHT};
enum {FRONT, MIDDLE, BACK};
enum {YAW, PITCH, KNEE, HAND};
enum CRBeetleState{STOP, NEUTRAL, FORWARD, BACKWARD, LEFTTURN, RIGHTTURN};

typedef float FLOAT;
typedef Vec3f VECTOR;
DEF_RECORD(XBeetle,{
	GUID Guid(){ return WBGuid("9EFE25A4-2BBF-4395-B6A7-2084207D9211"); }
	FLOAT	fPValue;
	FLOAT	fIValue;
	FLOAT	fDValue;					//PID�W��
	FLOAT	fStride;					//����
	FLOAT	fTurnAngle;					//���񎞂̋r�̐U��p
	FLOAT	fClearance;					//�r�����グ��
	FLOAT	fSpeed;						//���H�ł̑���̑��x
	FLOAT	fSwingBackRate;				//���H���x/���H���x
	VECTOR	toe[3];						//	����ʒu
	DWORD	bUseAi;						//AI���g�����ǂ���
});


class CRBeetle:public SGBehaviorEngine, public XBeetle{
SGOBJECTDEF(CRBeetle);
public:
	class Leg{
		Spr::Vec3f	Error, ErrorInt, ErrorDiff;
	public:
		//�݌v�p�����[�^
		float		fLength[2];			//�ڂ����̒���
		float		fTilt;				//�`���g�p
		Spr::Vec3f	v3BasePosition;		//�x�[�X�ʒu�i�{�f�B�t���[���j
		Spr::Vec3f	v3ToePosition;		//�j���[�g�������̂ܐ�ʒu�i�{�f�B�t���[���j
		Spr::Vec3f	v3DesiredPosition;	//�ܐ�ڕW�ʒu
		float		fDirection;			//�{�f�B���S����ܐ�ւ̕��ʊp
		float		fDistance;			//�{�f�B���S����ܐ�ւ̋���

		CRBeetle*		pBody;				//���̂ւ̎Q��
		Spr::PHJoint1D* pJoint[4];			//�֐�
		Spr::PHJointPid* pidJoint[4];		//�֐�
		
		void Reset();
		void SetPosition(const Spr::Vec3f& pos);
		void Step(double dt);
		Leg(){}
		~Leg(){}
	};

	bool	bReady;

	//��A���A�r
	Spr::PHJoint1D *pjntNeck[2];
	Spr::PHJointPid *pidNeck[2];
	Spr::PHJoint1D *pjntTail;
	Spr::PHJointPid *pidTail;
	Leg Legs[2][3];
	UTRef<PHContactEngine> contactEngine;

	//���s����
	CRBeetleState	nState;
	int		nNeutralStep;
	float	fCounter;

	//	AI
	Spr::UTRef<Spr::PHSolid> beetle;
	std::vector< UTRef<Spr::PHSolid> > attractors;
	std::vector< UTRef<Spr::PHContactEngine::FramePairRecord> > fpRecords;
	
	///	�h�L�������g����̃��[�h���C�Q�Ƃ������������ƂŌĂяo�����D
	virtual void Loaded(SGScene* scene);
	void SetState(CRBeetleState);
	void Step(SGScene* s);
	void AiStep();
	void CheckForce();
	CRBeetle(void);
	~CRBeetle(void);
};

class CRBeetleLoader : public FIObjectLoader<CRBeetle>
{
public:
	CRBeetleLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(VECTOR);
		db->REG_RECORD_PROTO(XBeetle);
	}
	bool LoadData(FILoadScene* ctx, CRBeetle* b){
		ctx->docs.Top()->GetWholeData((XBeetle&)*b);
		return true;
	}
};

class CRBeetleSaver : public FIObjectSaver<CRBeetle>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, CRBeetle* b){
		doc->SetWholeData((XBeetle&)*b);
	}
};

DEF_REGISTER_BOTH(CRBeetle);


}	//	end namespace Spr