#pragma once
#ifndef PHWATERCONTACT_H
#define PHWATERCONTACT_H

#include "PHSolid.h"
#include <SceneGraph/SGScene.h>

namespace Spr{;


/**	���̂Ɨ��̂Ƃ̐ڐG�͌v�Z�G���W��.
	����ł�PHWater 1 �� PHSolid ��
	�������PHSolid���m�̐ڐG��PHContactEngine�ł���Ă�������
 */
class PHContactEngine:public SGBehaviorEngine, CDCollisionListener{
public:
	///	����Ώۃt���[�����Ƃ̃f�[�^
	class FrameRecord:public CDUserRecord{
	public:
		///@name	UpdateChache�ōX�V����ϐ�
		//@{
		/**	�Ō�ɐڐG�������� = �L���b�V���p�����[�^���Ō�ɍX�V���������D
			�ڐG���ɁC�L���b�V�����X�V����̂ŁCcount �����݂̎����Ɠ��������
			�Փ˂��N�������Ƃ��Ӗ�����D	*/
		unsigned count;
		//	�t���[��(����)�P�ʂ̃p�����[�^
		Vec3f cog, vel, angVel;			///<	�d�S�C���x�C�p���x
		Vec3f pos, lastPos;				///<	�ʒu�C�Ō�̈ʒu
		Quaternionf ori, lastOri;		///<	�����C�O��̌���
		//@}
		void UpdateCache(int c);		///<	�L���b�V���ϐ������̂Ȃǂ������Ă���D
		PHSolid* solid;					///<	����Ώۍ���
		FrameRecord(){ count = -1; }	///<	s
	};
	///	����Ώۃt���[���΂��Ƃ̃f�[�^
	class FramePairRecord:public CDUserRecord{
	public:
		Vec3f cocog;					///<	2���̂̏d�S�̒��_(��Όn)
		Vec3f reflexForce;				///<	�R��
		Vec3f reflexTorque;				///<	�R�͂ɂ��g���N(cocog�n)
		Vec3f frictionForce;			///<	���C��
		Vec3f frictionTorque;			///<	���C�͂ɂ��g���N(cocog�n)
		///@name	��Όn�ł̓ǂݏo��
		//@{
		///	�R��
		Vec3f GetReflexForce(){ return reflexForce; }
		///	�R�͂̃g���N
		Vec3f GetReflexTorque(){ return reflexTorque + (cocog^reflexForce); }
		///	���C��
		Vec3f GetFrictionForce(){ return frictionForce; }
		///	���C�͂̃g���N
		Vec3f GetFrictionTorque(){ return frictionTorque + (cocog^frictionForce); }
		//@}
		
		///	�f�[�^�̃N���A�D
		void Clear(){
			reflexForce = reflexTorque = frictionForce = frictionTorque = Vec3f();
		}
		///	�͂̍ő�l�𐧖񂷂�D
		void LimitForces(){
			bool b = false;
			b |= LimitForce(reflexForce);
			b |= LimitForce(frictionForce);
			b |= LimitForce(reflexTorque);
			b |= LimitForce(frictionTorque);
			if (b){
				reflexForce = frictionForce = reflexTorque = frictionTorque = Vec3f();
			}
		}
	protected:
		bool LimitForce(Vec3f& f){
			float sq = f.square();
			const float MF = 200000;
			if (!(sq < MF*MF)){
				return true;
			}
			return false;
		}
	};
	///	�ʌ`��΂��Ƃ̃f�[�^
	class ConvexPairRecord:public CDUserRecord{
	public:
		///@name	�R�͂̌v�Z
		//@{
		float area;							///<	�������̖ʐ�
		Vec3f reflexSpringForce;			///<	�΂˂ɂ��R��
		Vec3f reflexDamperForce;			///<	�_���p�[�ɂ��R��
		Vec3f reflexSpringTorque;			///<	�΂˂ɂ��R�g���N(commonPoint�n)
		Vec3f reflexDamperTorque;			///<	�_���p�[�ɂ��R�g���N(commonPoint�n)
		Vec3f reflexForcePoint;				///<	�R�͂̍�p�_(commonPoint�n)
		//@}

		///@name���C�̌v�Z
		//@{
		Vec3f transFrictionBase[2];			///<	���i�Î~���C�p�o�l�̒[�_(���[�J���t���[���n)
		float rotSpring;					///<	��]�΂˂̐L��(���W�A��)
		Vec3f dynaFric;						///<	�����C��
		Vec3f dynaFricMom;					///<	�����C�͂̃��[�����g(commonPoint�n)
		Vec3f frictionForce;				///<	���C��
		Vec3f frictionTorque;				///<	���C�g���N(���C�͂̍�p�_ �� (reflexForcePoint+commonPoint)�n)
		enum FrictionState{ STATIC, DYNAMIC };
		FrictionState frictionState;		///<	���C�̏��
		//@}
		///@name	��΍��W�n�ł̓ǂݏo��
		//@{
		///	�o�l�ɂ��R�͂̍�p�_
		Vec3f GetReflexForcePoint(CDConvexPair* cp){ return reflexForcePoint + cp->commonPoint;}
		///	�o�l�ɂ��R��
		Vec3f GetReflexSpringForce(){ return reflexSpringForce;}
		///	�_���p�ɂ��R��
		Vec3f GetReflexDamperForce(){ return reflexDamperForce;}
		///	�R��
		Vec3f GetReflexForce(){ return reflexSpringForce + reflexDamperForce;}

		///	���C��
		Vec3f GetFrictionForce(){ return frictionForce; }
		///	���C�g���N
		Vec3f GetFrictionTorque(CDConvexPair* cp){ return frictionTorque + (GetReflexForcePoint(cp)^frictionForce); }
		//@}
	};
private:
	bool firstLoadedCall;					///<	Loaded�ŕ����񏉊��������Ȃ��悤�ɂ��邽�߂̃t���O
	float area;								///<	�Փ˕����̖ʐ�
	float convertedMass;					///<	���Z����
	float reflexSpring;						///<	�o�l�v��
	float reflexDamper;						///<	�_���p�v��
	float frictionSpring;					///<	���C�̃o�l�W��
	float frictionDamper;					///<	���C�̃_���p�W��
	float staticFriction;					///<	�Î~���C�W��
	float dynamicFriction;					///<	�����C�W��
protected:
	int listenerPos;						///<	CDContactEngine�̒��ł̃��X�i�̈ʒu
	int frameRecordPos;						///<	CDContactEngine�̒��ł̃t���[���P�ʂ̃��[�U���R�[�h�̈ʒu
	int framePairRecordPos;					///<	CDContactEngine�̒��ł̃t���[���ΒP�ʂ̃��[�U���R�[�h�̈ʒu
	int convexPairRecordPos;				///<	CDContactEngine�̒��ł̓ʌ`��ΒP�ʂ̃��[�U���R�[�h�̈ʒu
	std::vector< std::pair<DWORD, DWORD> > inactiveList;
											///<	��A�N�e�B�u�ȃy�A�ꗗ(���[�h���ɂ����g�p)

	UTRef<CDCollisionEngine> collisionEngine;///<	�Փ˔���G���W��
public:	
	SGOBJECTDEF(PHContactEngine);
	std::vector< std::vector<DWORD> > inactiveListBackups;
											//<		�Z�[�u�ɔ����Ċo���Ă���
	PHSolids solids;						///<	�y�i���e�B�@�̑Ώۂ̍���
	size_t nSolidsInitilized;				///<	solids�̂����ǂ��܂ł��������ς݂��������D
	bool bDraw;								///<	�`��t���O
	///
	PHContactEngine();
	CDCollisionEngine* GetCollisionEngine(){ return collisionEngine; }
	///	�q�I�u�W�F�N�g�̒ǉ�
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	int GetListenerPos(){ return listenerPos; }
	///	FrameRecord��CDCollisionEngine�̒��ł̈ʒu
	int GetFrameRecordPos(){ return frameRecordPos; }
	///	FramePairRecord��CDCollisionEngine�̒��ł̈ʒu
	int GetFramePairRecordPos(){ return framePairRecordPos; }
	///	ConvexPairRecord��CDCollisionEngine�̒��ł̈ʒu
	int GetConvexPairRecordPos(){ return convexPairRecordPos; }
	///	FrameRecord�̎擾
	FrameRecord* GetFrameRecord(SGFrame* fr){
		return (FrameRecord*)collisionEngine->GetFrameRecord(collisionEngine->GetFrameID(fr), frameRecordPos);
	}
	/**	FramePairRecord�̎擾
		FramePairRecord�̓t���[���̑΂��Ƃ�1�D
		(fr1,fr2)��(fr2,fr1)�œǂ񂾏ꍇ�C�ǂ��炩�Е���NULL��Ԃ��D
		(fr1,fr2)�Œl������ꂽ�Ȃ�΁CFramePairRecord��f1�ɉ�������͂����D
	*/
	FramePairRecord* GetFramePairRecord(SGFrame* fr1, SGFrame* fr2){
		CDFramePairWithRecord* fp = collisionEngine->GetFramePair(fr1, fr2);
		if (fp) return UTRef<FramePairRecord>(fp->records[framePairRecordPos]);
		return NULL;
	}
	///	�A�N�e�B�u�łȂ��t���[���΂�ǉ�
	void AddInactive(int f1, int f2);

	///	���[�h�������̏����CCollisionEngine �ɃR�[���o�b�N��o�^����DCollisionEngine���Ȃ���΍��D���[�U���Ăяo�����Ƃ͂Ȃ��D
	virtual void Loaded(SGScene* scene);
	///	�����������ALoaded���Ă�
	virtual void Init(SGScene* scene);
	///	�ڐG�͂̕`��
	void Render(GRRender* r, SGScene* s);
protected:
	///
	int GetPriority() const { return SGBP_CONTACTENGINE; }
	///	���Ԃ� dt �i�߂�D(�������Ȃ��CListener �� Collide �����ׂĂ̏���������D)
	virtual void Step(SGScene* s){}
	///	solids���N���A����D
	virtual void Clear(SGScene* s);
	///	�t���[�����m���Փ˂��Ă���ꍇ�C�ڐG��͂̑O�ɌĂяo�����D�R�͌v�Z�̏������s���D
	void Before(SGScene* scene, CDFramePairWithRecord* fr);
	/**	�t���[�����m���Փ˂��Ă���ꍇ�C�ڐG��͂̒���ɓʌ`��΂��ƂɌĂяo�����D
		�ڐG�ʐςƍR�͂̌v�Z���s���D*/
	virtual void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	/**	�t���[�����m���Փ˂��Ă���ꍇ�C�Փˉ�͂��I����Ă���t���[���΂ɂ�1�x�Ăяo�����D
		�R�͂̒����C���C�͌v�Z���s��	*/
	void After(SGScene* scene, CDFramePairWithRecord* fr);
	///	�R�͌v�Z
	void CalcReflexForce(PHContactEngine::FrameRecord** fr, CDConvexPairWithRecord* cp,
		PHContactEngine::ConvexPairRecord* rec, CDContactAnalysis* analyzer);
	///	3�p�`�P�ʂ̍R�͌v�Z
	void CalcTriangleReflexForce(CDConvexPairWithRecord* cp, PHContactEngine::ConvexPairRecord* rec,
		Vec3f p0, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel);
	void CalcFriction(PHContactEngine::FrameRecord** fr, PHContactEngine::FramePairRecord* fpr, CDConvexPairWithNormal* cp,
		PHContactEngine::ConvexPairRecord* rec);
	void DrawForce(CDConvexPair& cp, PHContactEngine::ConvexPairRecord& cpr, Affinef afw, GRRender* render, SGScene* scene);

	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
};


}
#endif

