#ifndef PH_JOINT_H
#define PH_JOINT_H


#include <Base/BaseUtility.h>
#include <Base/TinyVec.h>
#include <Base/TinyMat.h>
#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Physics/PHSolid.h>
#include <WinBasis/WBPreciseTimer.h>
#include <float.h>

/**	@file �R�����A�����̌n
	����F
	Featherstone�̃A���S���Y���𗘗p���ăc���[��ɘA�����ꂽ���̂�
	�^�����V�~�����[�g����N���X

	�J���ӔC�ҁF�c��E�� tazaki@cyb.mei.titech.ac.jp

	�Q�l�����F"Impulse-based Dynamic Simulation of Rigid Body Systems" Brian Mirtich 1996
*/
namespace Spr{;

/**@name	Featherstone's method �Ŏg����"spatial matrix/vector"
	��3��(�́C�p���x�C�p�����x�Ȃ�)������킵�C
	��3��(�g���N�C���x�C�����x)������킷�D(�Ǝv��) by hase
	�͂ƃg���N�ŉ�]�ƕ��i���t�Ȃ̂Œ��ӁD
*/
//@{
typedef PTM::TVector<6, double> SpVec6d;
typedef PTM::TSubVector<3, SpVec6d::desc> SpSubVec3d;
typedef PTM::TMatrixCol<6, 6, double> SpMatrix6d;
typedef PTM::TSubMatrixCol<3, 3, SpMatrix6d::desc> SpSubMatrix3d;
//@}

typedef Vec3f Vector;
typedef Matrix3f Matrix3x3;
DEF_RECORD(XJointBase, {
	GUID Guid(){ return WBGuid("23F6D545-8987-4aa6-BBE1-2FE32C096D5A"); }	
	Matrix3x3	m3fRotationParent;
	Vector		v3fPositionParent;
	Matrix3x3	m3fRotationChild;
	Vector		v3fPositionChild;
});

///	�֐߂̊�{�N���X�D�c���[�\�������DPHJointEngine���c���[�����D
class PHJointBase : public UTTreeNode<PHJointBase>, public SGObject{
public:
	typedef std::vector<UTRef<PHJointBase> > array_type;
	friend class PHJointEngine;
	friend class PHJointClearForce;
	friend class PHJointLoader;
	friend class CRHuman;
public:
	SGOBJECTDEFABST(PHJointBase);
	UTRef<PHSolid> solid;	//	�qSolid�D�֐߂͐eSolid�ƎqSolid���Ȃ�
	UTRef<SGFrame> frame;	//	���[�g�m�[�h���Œ�̏ꍇ��Frame
protected:
	enum IntType{
		SYMPLETIC,
		ANALYTIC,
	}intType;
	/**	@name �t�@�C�����烍�[�h�����ϐ�
		�֐߃t���[���̐���
			�֐߂̈ʒu�ƌX����\�����邽�߂̃t���[���B
			�֐߃t���[�����_�͊֐ߎ��̈ʒu��\���B
			��]�֐�(TYPE_HINGE)�̏ꍇ�A
				�t���[���̂y������]���̌�����\���B
			m3fRotationParent =: pRj
			m3fRotationChild  =: cRj
			fPosition =: q
			�Ƃ����ƁA
			�q�m�[�h����e�m�[�h�ւ̉�]�ϊ��́A
				pRc = pRj * Rot(q, 'z') * cRj.trans()
			�����֐�(TYPE_SLIDER)�̏ꍇ�A
				�t���[���̂y�����������̌�����\���B	*/
	//@{
	Vec3f		v3fPositionParent;			///<	�e���̂̃t���[�����猩���֐߈ʒu
	Matrix3f	m3fRotationParent;			///<	�e���̂̃t���[�����猩���֐ߎp��
	Vec3f		v3fPositionChild;			///<	�q���̂̃t���[�����猩���֐߈ʒu
	Matrix3f	m3fRotationChild;			///<	�q���̂̃t���[�����猩���֐ߎp��
	//@}
	
	/**	@name ��ԕϐ��D
		�ȉ��̃R�����g�ŁC
		- Fc := child frame
		- Fp := parent frame
		- Fj := joint frame
		���Ӗ�����
	*/
	//@{
	Matrix3d		R;					///<	orientation matrix
	Quaterniond		quat;				///<	orientation quaternion
	Vec3d			p;					///<	position vector
	Matrix3d		pRj, cRj, pRc, cRp;	///<	rotation matrix(Fp to Fj, Fj to Fc, Fp to Fc, Fc to Fp)
	Vec3d			prc, prj, crj;		///<	radius vector(Fp to Fc in Fc, Fp to Fj in Fp, Fc to Fj in Fc)
	Vec3d			v, v_abs;			///<	velocity in Fc/world coord.
	Vec3d			w, w_abs;			///<	angular velocity in Fc/world coord.
	Vec3d			pvc, pwc;			///<	[angular]velocity relative to Fp in Fc coord. 
	double			m;					///<	mass
	Matrix3d		I;					///<	inertia matrix
	SpMatrix6d		Ii;					///<	spatial isolated inertia
	SpMatrix6d		Ia;					///<	spatial articulated inertia
	SpVec6d			Za;					///<	zero accelaration force in Fc coord.
	SpVec6d			c;					///<	Coriolis vector in Fc coord.
	SpVec6d			a;					///<	spatial accelaration in Fc coord.
	//@}
	
	//	s���܂܂Ȃ��ϐ��̃L���b�V���D
	Matrix3d	rcross, rpcross, rcross_cRp, rpcross_pRc;
	SpVec6d		a_p, Ia_c, Z_plus_Ia_c;
public:
	///	�R���X�g���N�^
	PHJointBase();
	///	���[�h��̏������D�ċA�D
	void Loaded(SGScene* scene);
	///	�w�肵��PHSolid�����Ɏ��m�[�h����������
	PHJointBase* Search(PHSolid*);
	///	�e���̂̃t���[�����猩���֐ߎp��
	Affinef GetPostureFromParent(){ Affinef rv; rv.Pos()=v3fPositionParent; rv.Rot()=m3fRotationParent; return rv; }
	///	�q���̂̃t���[�����猩���֐ߎp��
	Affinef GetPostureFromChild(){ Affinef rv; rv.Pos()=v3fPositionChild; rv.Rot()=m3fRotationChild; return rv; }
	///	�q���̂̊p�����x(World�n)
	Vec3f GetSolidAngularAccel(){ return solid->GetRotation() * a.sub_vector(0, Vec3f()); }
	///	�q���̂̉����x(World�n)
	Vec3f GetSolidAccel(){ return solid->GetRotation() * a.sub_vector(3, Vec3f()); }
	///	�q���̂̊p�����x(World�n)
	Vec3f GetSolidAngularVelocity(){ return w_abs; }
	///	�q���̂̉����x(World�n)
	Vec3f GetSolidVelocity(){ return v_abs; }
	/// ��΍��W�ł̊֐߂̌���
	Matrix3d GetOrientation(){return R;}
	
	///	�֐߂̎��R�x
	virtual int GetJointDof()=0;
	///	�֐߈ʒu�̎擾
	virtual double GetJointPosition(int i)=0;
	///	�֐ߑ��x�̎擾
	virtual double GetJointVelocity(int i)=0;
	///	�֐߉����x�̎擾
	virtual double GetJointAccel(int i)=0;
	///	�֐߃g���N�̎擾
	virtual double GetJointTorque(int i)=0;
	///	�֐߃g���N�̐ݒ�
	virtual void SetJointTorque(double v, int i)=0;
	///	�֐߃g���N��������
	virtual void AddJointTorque(double v, int i)=0;

	///@name	Featherstone's algorithm
	//@{
	///	�R���I���̗͂ɂ������x�̌v�Z
	void CompCoriolisAccelRecursive(double dt);	
	///	articulated inertia & ZA-force
	virtual void CompArticulatedInertia(double dt);
	///	�ϕ�
	virtual void Integrate(double dt);
	/**	���̃m�[�h�̉����x���v�Z�������ꍇ�ɌĂԁD
		�e�m�[�h�̉����x�͕���p�ŋ��܂�D�q�m�[�h�ɂ��Ă͌v�Z���Ȃ��D
		
		�Ăяo�������D
		1. root->CalcCoriolisAccelRecusive(dt);
		2. �S�W���C���g�ɂ��āCPHJointBase::torque �ɒl�𒼐ڑ������D
		3. root->CalcArticulatedInertia(dt);
		4. CalcAccel(dt)
		5. �W���C���g�� a ��������x��ǂݏo��(���W�n�ɒ���)
		6. 2-5���g���N�p�^�[����ς��Ȃ���Ăяo���D
		
		Compute accelaration of the child solid of this joint.
		For partial use of Featherstone's algorithm.
	*/
	virtual void CalcAccel(double dt)=0;
	//@}

	///	���L���Ă���I�u�W�F�N�g�̐�
	virtual size_t NChildObjects();
	///	���L���Ă���I�u�W�F�N�g
	virtual SGObject* ChildObject(size_t i);
	///	�Q�Ƃ��Ă���I�u�W�F�N�g�̐�
	virtual size_t NReferenceObjects();
	///	�Q�Ƃ��Ă���I�u�W�F�N�g
	virtual SGObject* ReferenceObject(size_t i);
	///	�q�I�u�W�F�N�g�̒ǉ�
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	�q�I�u�W�F�N�g�̍폜
	virtual bool DelChildObject(SGObject* o, SGScene* s);
	///	�q�ɂȂ肦��I�u�W�F�N�g�̌^���̔z��
	virtual const UTTypeInfo** ChildCandidates();

	void SaveX(XJointBase& x) const;
	void LoadX(const XJointBase& x);

	//	�֐ߊp��-�΁`�΂ɐ�������֐�
	virtual void LimitAngle(double& d){}
protected:
	///	�h���N���X����{�N���X�^�I�u�W�F�N�g�̃����o�ɃA�N�Z�X���邽�߂̎�i
	template <class T> T& OfParent(T PHJointBase::* member){
		return GetParent()->*member;
	}

	inline SpMatrix6d pXc_Mat_cXp(SpMatrix6d& m);
	inline SpVec6d	pXc_Vec(SpVec6d& v);
	inline SpVec6d cXp_Vec(SpVec6d& v);

	virtual void CompJointAxis()=0;
	virtual void CompRelativePosition()=0;
	virtual void CompRelativeVelocity()=0;
	virtual void CompCoriolisAccel()=0;
	void UpdateSolid();
	void UpdateJointPosture();
	void PropagateState();				///<	�ʒu�E���x�̓`�d�i��ċA�֐��j	
	virtual void ClearTorqueRecursive();
	/**	�p���Ȃǂ̍Đݒ�	��ċA
		��{�I�ȏ�ԗ�(PHJointState�̓��e)��Loaded��LoadState�ŃZ�b�g���ꂽ��ɁA
		�]���ϐ����v�Z���邽�߂̊֐�	*/
	virtual void Reset();
	virtual void LoadState(const SGBehaviorStates& states);
	virtual void SaveState(SGBehaviorStates& states) const;
};

class PHJointRoot:public PHJointBase{
public:
	SGOBJECTDEF(PHJointRoot);
	virtual void Reset();
	virtual void CalcAccel(double dt);
	virtual void CompJointAxis(){}
	virtual void CompRelativePosition(){}
	virtual void CompRelativeVelocity(){}
	virtual void CompCoriolisAccel(){}
	virtual void Integrate(double dt);
	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;	
protected:
	virtual int GetJointDof(){ return 0; }
	virtual double GetJointPosition(int i){ return 0; }
	virtual double GetJointVelocity(int i){ return 0; }
	virtual double GetJointAccel(int i){ return 0; }
	virtual double GetJointTorque(int i){ return 0; }
	virtual void SetJointTorque(double v, int i){}
	virtual void AddJointTorque(double v, int i){}
};


/**	PHJointEngine
	�֐߃G���W���D1��Articulated Body�ɑΉ�����D
*/
class PHJointEngine	: public SGBehaviorEngine{
public:
	SGOBJECTDEF(PHJointEngine);
	typedef PHJointBase::array_type array_type;		///<	�W���C���g�̔z��
	UTRef<PHJointBase> root;						///<	���[�g�m�[�h�D�֐߂ł͂Ȃ��D
	typedef std::set< UTRef<PHSolid> > PHSolidSet;	///<	���̂̃Z�b�g�D
	PHSolidSet solids;

	//	Featherstone�@�ɂ����鎞�Ԃ̌v��
	WBPreciseTimer timer;
	

	int GetPriority() const {return SGBP_JOINTENGINE;}
	void Step (SGScene* s);
	void Loaded(SGScene* scene);
	void Clear(SGScene* s){}
	PHJointEngine::PHJointEngine(){}

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
	///	�I�u�W�F�N�g���q���Ɏ����Ă��邩�ǂ����D
	bool Has(SGObject*);

	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;

	///
	template <class CT> void EnumJoint(CT& ct){
		typedef void (PHJointEngine::*Func)(PHJointBase* b, CT& ct);
		Func func = &PHJointEngine::EnumJointFunc;
		root->MemberTraverse(this, func, ct);
	}
protected:
	template <class CT> void EnumJointFunc(PHJointBase* p, CT& ct){
		CT::value_type v = DCASTP(CT::value_type, p);
		if (v) ct.push_back(v);
	}
};

/**	�X�e�b�v�̍ŏ���Joint�̃g���N���N���A����D	*/
class PHJointClearForce:public SGBehaviorEngine{
	SGOBJECTDEF(PHJointClearForce);
public:
	UTRef<PHJointEngine> je;
	///	�N���A����
	virtual void Step(SGScene* s);
	virtual int GetPriority() const { return SGBP_CLEARFORCE; }
};

//----------------------------------------------------------------------------
//	����

///	�v�f�A�N�Z�X
inline SpSubVec3d& svitem(SpVec6d& v, int i){
	return *(SpSubVec3d*)((double*)&v + i*3);
}
inline SpSubMatrix3d& smitem(SpMatrix6d& m, int i, int j){
	return m.sub_matrix(i * 3, j * 3, PTM::TMatDim<3, 3>());
}
///	�Ǝ��̓��ϒ�`(v1^T * v2)
inline double svdot(const SpVec6d& v1, const SpVec6d& v2){
	return 
		v1[0] * v2[3] + v1[1] * v2[4] + v1[2] * v2[5] + 
		v1[3] * v2[0] + v1[4] * v2[1] + v1[5] * v2[2];
}

///	v1 * v2^T �œ�����s��
inline Matrix3d mat(const Vec3d& v1, const Vec3d& v2){
	return Matrix3d(
		v1[0] * v2[0], v1[0] * v2[1], v1[0] * v2[2],
		v1[1] * v2[0], v1[1] * v2[1], v1[1] * v2[2],
		v1[2] * v2[0], v1[2] * v2[1], v1[2] * v2[2]);
}
inline SpMatrix6d svmat(const SpVec6d& v1, const SpVec6d& v2){
    PTM::TVecDim<3> dim3;
	const SpSubVec3d& v11 = v1.sub_vector(0, dim3), v12 = v1.sub_vector(3, PTM::TVecDim<3>());
	const SpSubVec3d& v21 = v2.sub_vector(0, dim3), v22 = v2.sub_vector(3, PTM::TVecDim<3>());
	SpMatrix6d y;
	y.sub_matrix(0, 0, PTM::TMatDim<3, 3>()) = mat(v11, v22);
	y.sub_matrix(0, 3, PTM::TMatDim<3, 3>()) = mat(v11, v21);
	y.sub_matrix(3, 0, PTM::TMatDim<3, 3>()) = mat(v12, v22);
	y.sub_matrix(3, 3, PTM::TMatDim<3, 3>()) = mat(v12, v21);
	return y;
}
inline void LimitCycle(float& t){
	const float M_PIf = (float)M_PI;
	if (!_finite(t) || t > 1e6 || t < -1e6){
		assert(0);
		if (t>0) t = M_PIf;
		else t= - M_PIf;
		return;
	}
	int times;
	if (t > M_PIf){
		times = int((t+M_PIf) / (2*M_PIf));
	}else if (t < -M_PIf){
		times = int((t-M_PIf) / (2*M_PIf));
	}else{
		return;
	}
	t -= (float)(times*(2*M_PIf));
	assert(-M_PIf <= t &&  t < M_PIf);
}
inline void LimitCycle(double& t){
	if (!_finite(t) || t > 1e6 || t < -1e6){
		if (t>0) t = (double)M_PI;
		else t= - (double)M_PI;
		return;
	}
	int times;
	if (t > M_PI){
		times = int((t+M_PI) / (2*M_PI));
	}else if (t < -M_PI){
		times = int((t-M_PI) / (2*M_PI));
	}else{
		return;
	}
	t -= (double)(times*(2*M_PI));
	if (t < -M_PI || M_PI <= t){
		DSTR << t << std::endl;
		assert(0);
	}
}

SpMatrix6d PHJointBase::pXc_Mat_cXp(SpMatrix6d& m){
	static Matrix3d pRc_m11_cRp, pRc_m12_cRp, pRc_m21_cRp, pRc_m22_cRp, tmp;
	pRc_m11_cRp = pRc * smitem(m, 0, 0) * cRp;
	pRc_m12_cRp = pRc * smitem(m, 0, 1) * cRp;
	pRc_m21_cRp = pRc * smitem(m, 1, 0) * cRp;
	pRc_m22_cRp = pRc * smitem(m, 1, 1) * cRp;
	tmp = pRc_m11_cRp - pRc * smitem(m, 0, 1) * rcross_cRp;
	SpMatrix6d	y;
	smitem(y, 0, 0) = tmp;
	smitem(y, 0, 1) = pRc_m12_cRp;
	smitem(y, 1, 0) = rpcross * tmp + pRc_m21_cRp - pRc * smitem(m, 1, 1) * rcross_cRp;
	smitem(y, 1, 1) = rpcross * pRc_m12_cRp + pRc_m22_cRp;
	return y;
}

SpVec6d	PHJointBase::pXc_Vec(SpVec6d& v){
	Vec3d pRc_v1 = pRc * svitem(v, 0);
	SpVec6d y;
	svitem(y, 0) = pRc_v1;
	svitem(y, 1) = rpcross * pRc_v1 + pRc * svitem(v, 1);
	return y;
}

SpVec6d PHJointBase::cXp_Vec(SpVec6d& v){
	Vec3d cRp_v1 = cRp * svitem(v, 0);
	SpVec6d y;
	svitem(y, 0) = cRp_v1;
	svitem(y, 1) = -rcross * cRp_v1 + cRp * svitem(v, 1);
	return y;
}


}

#endif