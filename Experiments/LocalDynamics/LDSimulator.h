#ifndef LDSIMULATOR_H
#define LDSIMULATOR_H

#include <FrameWork/FWAppD3D.h>
#include <FrameWork/FWPointer.h>
#include <Physics/PHContactEngine.h>
#include <SceneGraph/SGScene.h>
#include <vector>
#include "LDRenderer.h"
#include <Device/DRUsb20Sh4.h>
#include "Matrix6.h"
#include "Vector6.h"

namespace Spr{;
class LDSimulator:public FWAppD3D, CDCollisionListener{
//	FWAppD3D* pApp;
	PHContactEngine::FrameRecord* pFrameRecord;			// �ŋߖT���̂�FrameRecord
	PHContactEngine::FrameRecord* pPrevFrameRecord;		// ���O��FrameRecord
	PHContactEngine::FrameRecord* pDSFrameRecord;		// DummySphere��FrameRecord
	PHContactEngine::FrameRecord* pPointerFrameRecord;	// �͊o�|�C���^��FrameRecord
	PHContactEngine::FrameRecord* pPointer2FrameRecord;	// �͊o�|�C���^��FrameRecord

	DRUsb20Sh4* ihc;
	LDRenderer* pLDRenderer;


	float k, b;						// ���̂̂΂ˌW���ƃ_���p�W��
	double mass_inv;				// ���̂̎��ʂ̋t��

	Vec6d velocities;				// ���̂̑��x�E�p���x������6�����x�N�g��
	Vec3d pos, normal;				// ���̂̈ʒu�A�@���x�N�g��
	Vec3d prevPos;					// ���O�̈ʒu
	Vec3d center;					// ���̂̎��ʒ��S
	Vec3d f_external;				// ���̂ɂ�����O��
	Vec3d torque_ex;				// ���̂̃g���N
	Vec3d p;						// ���ʒ��S�����p�_�ւ̃x�N�g��
	Vec3d impulse;					// ���̂��󂯂��͐�
	Vec6d displacement;				// ���̂̈ړ��ʁi�ʒu�E��]�j

	Matrix6d Iinv;					// �����s��̋t�s��
	Matrix3d rotation;				// ��]�s��(���̍��W�n->�O���[�o�����W�n)
	Matrix3d matP;					// �O�ς̍s��\��
	
	Vec3d pointer;					// �͊o�|�C���^�̈ʒu

	float simuDT;
	bool bChanged;					// ���M���镨�̂��ς�������ǂ����̃`�F�b�N
	bool calc_ld;
	bool bExperiment;

	std::vector<Vec3d> vNormals;
	std::vector<Vec3d> vClosestPoints;
	std::vector<PHContactEngine::FrameRecord*> vFrameRecords;

protected:
	int listenerPos;				///<	���X�i�̈ʒu
	
public:
	FWPointer6D* device;

	LDSimulator();
	~LDSimulator();

	//�p�����[�^������
	void ResetParameters();
	//����̍X�V�����i�V�~�����[�^�j
	void Step();
	//���[�J���G���A�̕����X�V
	void IntegrateLocalDynamics();
	//���[�J���G���A�̐ݒ�
	void DefineLocalArea();
	//�ŋߖT�_�̒T���ivector�v�f�̓Y������Ԃ�)
	int FindClosestPoint();
	//LocalDynamics�v���Z�X�ɏ�񑗐M
	void Send();
	//LocalDynamics�v���Z�X�������M
	void Receive();
	//IHC���g�����ꍇ�̒ʐM�p�֐�
	void IHCSend();
	void IHCReceive();
	/*�Փ˔��胊�X�i�p�֐�
	  �Փ˕��̂̍ŋߖT�_��FrameRecord���x�N�^�[�ɓo�^  */
	void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	// Listener�ǉ�
	void AddListener();
	void Load(std::string fn);
	bool OnKeyDown(unsigned int key);

	bool GetCalcLD() { return calc_ld; }
	void SetCalcLD(bool  b) { calc_ld = b;}
	void SetBExperiment(bool b){ bExperiment = b; }
	void SetSimuDT(float t) { simuDT = t; }
//	void SetFWApp(FWAppD3D* p){ pApp = p; }
	void SetLDRenderer(LDRenderer* p){ pLDRenderer = p; }
	void FindDSFrameRecord();
	void SetIHC(DRUsb20Sh4* i){ ihc = i; }
};

} // Spr

#endif //LDSIMULATOR_H