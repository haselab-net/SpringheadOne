#ifndef PH_CONSTRAINT_H
#define PH_CONSTRAINT_H

#include <vector>
#include <Base/BaseUtility.h>
#include <Base/TinyVec.h>
#include <Base/TinyMat.h>
#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Physics/PHSolid.h>

namespace Spr{

	//////////////////////////////////////////////////////////////////////
	//PHConstrainedState
	
	class PHConstrainedState{
	public:
		class DesiredPosture{
		public:
			double		weight;
			Vec3d		position;
			Quaterniond orientation;
		};
		UTRef<PHSolid> solid;
		std::vector<DesiredPosture> desired;
		int nConstraints;

		PHConstrainedState():nConstraints(0){};
	};

	//////////////////////////////////////////////////////////////////////
	//PHConstraint
	
	class PHConstraint : public SGObject{
	public:
		UTRef<PHSolid>	solid[2];
		UTRef<PHConstrainedState> state[2];
		size_t idx[2];	//ConstrainedState��desired�z��ł̎����̃C���f�b�N�X

		///	���L���Ă���I�u�W�F�N�g�̐�
		virtual size_t NChildObjects();
		///	���L���Ă���I�u�W�F�N�g
		virtual SGObject* ChildObject(size_t i);
		///	�q�I�u�W�F�N�g�̒ǉ�
		virtual bool AddChildObject(SGObject* o, SGScene* s);
		///	�q�I�u�W�F�N�g�̍폜
		virtual bool DelChildObject(SGObject* o, SGScene* s);
		///	�q�ɂȂ肦��I�u�W�F�N�g�̌^���̔z��
		virtual const UTTypeInfo** ChildCandidates();
	};

	class PHPointConstraint : public PHConstraint{
	public:
		class Info{
			Vec3f pos_l, pos_r;
		};
		Info info;
	};
	/*
		template PointConstraint{
			Vector	positionL;
			Vector	positionR;
			[Solid]
		}
	*/

	class PHLineConstraint : public PHConstraint{
		Vec3f pos_l, pos_r;
		Vec3f dir_l, dir_r;
	};

	class PHPlaneConstraint : public PHConstraint{
		Vec3f pos_l, pos_r;
		Vec3f nor_l, nor_r;
	};

	class PHPararrelConstraint : public PHConstraint{
		Vec3f dir_l, dir_r;
	};

	class PHFixConstraint : public PHConstraint{
		Vec3f pos;
		Matrix3f rotation;
	};

	class PHMagnetConstraint : public PHConstraint{
		
	};

	class PHContactConstraint : public PHConstraint{

	};

	//////////////////////////////////////////////////////////////////////
	//PHConstraintEngine

	typedef std::vector<UTRef<PHConstrainedState> > PHConstrainedStates;
	typedef std::vector<UTRef<PHConstraint> > PHConstraints;
	
	class PHConstraintEngine : public SGBehaviorEngine
	{
		PHConstrainedStates states;
		PHConstraints cons;
	public:
		SGOBJECTDEF(PHConstraintEngine);
		int GetPriority() const {return SG_CONSTRAINT;}
		void Loaded(SGScene* scene);
		void Clear(SGScene* s){}
		PHConstraintEngine::PHConstraintEngine(){}

		virtual void Step(SGScene* s);
		void SolveConstraint();

		///	���L���Ă���I�u�W�F�N�g�̐�
		virtual size_t NChildObjects();
		///	���L���Ă���I�u�W�F�N�g
		virtual SGObject* ChildObject(size_t i);
		///	�q�I�u�W�F�N�g�̒ǉ�
		virtual bool AddChildObject(SGObject* o, SGScene* s);
		///	�q�I�u�W�F�N�g�̍폜
		virtual bool DelChildObject(SGObject* o, SGScene* s);
		///	�q�ɂȂ肦��I�u�W�F�N�g�̌^���̔z��
		virtual const UTTypeInfo** ChildCandidates();
	};

}

#endif