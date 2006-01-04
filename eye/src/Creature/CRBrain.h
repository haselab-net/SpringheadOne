// CRBrain.h: CRBrain �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef CRBRAIN_H
#define CRBRAIN_H

#include <SceneGraph/SGScene.h>
#include <Creature/CRPuppet.h>
#include <Graphics/GRRender.h>
#include <Physics/PHSolid.h>
#include <Base/Base.h>

#include <Creature/CRVisionMotionAnalysis.h>
#include <Creature/CREye.h>
#include <Creature/CRAttention.h>
#include <Creature/CRNeckController.h>
#include <Creature/CRInternalModel.h>
#include <Creature/CRActionPlanner2.h>

namespace Spr{

	class CRBrain{
	public:
		// �C���^�[�t�F�[�X
		void Load(SGScene* scene, CRPuppet* crPuppet, CRUser* crUser);
		void Step();
		void Draw(GRRender* render);
	};

	class CRBaseBrain: public CRBrain{
	public:
		// ��{�C���^�[�t�F�[�X
		void Load(SGScene* scene, CRPuppet* crPuppet, CRUser* crUser);
		void Step();
		void Draw(GRRender* render);

		// �g���C���^�[�t�F�[�X
		
		// �����o�ϐ�
		/// ���o�͊튯
		SGScene*  scene;
		CRPuppet* puppet;
		CRUser*   user;

		/// �������W���[��
		CRVisionMotionAnalysis crvMotionAnalysis; // ���o�n �����̉��
		CRAttention            crAttention;       // ���ӂ̋@�\
		CREye                  crEye;             // �ڂ̓���
		CRNeckController       crNeckController;  // �񁁓����̓���

		// �f�o�b�O�p
		PHSolid*  soLEye;
	};

	class CRSimulatingBrain: public CRBaseBrain{
	public:
		// ��{�C���^�[�t�F�[�X
		void Load(SGScene* scene, CRPuppet* crPuppet, CRUser* crUser);
		void Step();
		void Draw(GRRender* render);
		
		// �g���C���^�[�t�F�[�X
		void SetFilename(std::string filename){this->filename = filename;}
		void SetRender(GRRender* render){this->render = render;}
		CRActionPlanner* GetPlanner(){return &crPlanner;}
		void ToggleEyeMode();
		
		// �����o�ϐ�
		/// �������W���[��
		CRInternalModel  crInternalModel; // �\���p�̓������f��
		CRActionPlanner crPlanner; // �\���@�\

		/// InternalModel�̏������̂��߂ɕێ�
		std::string filename;
		GRRender*   render;
	};

} // namespace Spr

#endif // ifndef CRBRAIN_H

