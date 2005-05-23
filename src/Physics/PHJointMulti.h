#ifndef PH_JOINTMULTI_H
#define PH_JOINTMULTI_H

#include <Physics/PHJoint.h>

namespace Spr{;

///	�����R�x�̊֐߂̊�{�N���X
template <int NDOF>
class PHJointMulti:public PHJointBase{
public:
	typedef PTM::TVector<NDOF, double> AVec;
	AVec torque;							///<	�g���N
	AVec accel;								///<	�֐߉����x
	AVec delta_position;					///<	�ψʂ̕ϕ�
	AVec velocity;							///<	���x
	void AddTorque(AVec t){ torque+=t; }	///<	�g���N��ǉ�
	void SetTorque(AVec t){ torque=t; }		///<	�g���N��ݒ�
	AVec GetTorque(){ return torque; }		///<	�g���N���擾
	AVec GetVelocity(){ return velocity; }	///<	�֐ߑ��x���擾
	///	�֐߂̎��R�x
	virtual int GetJointDof(){ return NDOF; }
	///	�֐ߑ��x�̎擾
	virtual double GetJointVelocity(int i){ return velocity[i]; }
	///	�֐߉����x�̎擾
	virtual double GetJointAccel(int i){ return accel[i]; }
	///	�֐߃g���N�̎擾
	virtual double GetJointTorque(int i){ return torque[i]; }
	///	�֐߃g���N�̐ݒ�
	virtual void SetJointTorque(double v, int i){ torque[i] = v; }
	///	�֐߃g���N��������
	virtual void AddJointTorque(double v, int i){ torque[i] += v; }
	///	articulated inertia & ZA-force
	void CompArticulatedInertia(double dt){
		//	�q�W���C���g��Ia,Za�̌v�Z�D
		PHJointBase::CompArticulatedInertia(dt);
		Iss = (S_tr * Ia * S).inv();
		//�e�m�[�h��Za,Ia�Ɏ�����Za,Ia��ςݏグ��
		//Ia
		OfParent(&PHJointMulti::Ia) += 
			pXc_Mat_cXp( Ia - (Ia * S*Iss*S_tr * Ia) );
		//Za
		OfParent(&PHJointMulti::Za) += 
			pXc_Vec( Z_plus_Ia_c + Ia*S*Iss*(torque - S_tr*Z_plus_Ia_c) );
	}
	/**	�h���N���X��Integrate() ���Ăяo���֐��D
		�h���N���X��Integrate()�̗�F
		void Integrate(double dt){
			PreIntegrate(dt);
			//	�����ŁCdelta_position ����C�֐߂̎p�����v�Z�D
			PropagateState();
			PHJointBase::Integrate(dt);
		}
	*/
	void PreIntegrate(SGScene* scene){
		double dt = scene->GetTimeStep();
		a_p = cXp_Vec(OfParent(&PHJointMulti::a));

		if (intType == SYMPLETIC){
			//	x(dt) = x(0) + dt*v(0)/m
			//	v(dt) = v(0) + dt*f(dt)
			//�����x���v�Z
			accel = Iss*(torque - S_tr*(Z_plus_Ia_c + Ia*a_p));
			//���x��ϕ�
			velocity += accel * scene->GetTimeStep();
			//�ʒu��ϕ����鏀��
			delta_position = velocity * scene->GetTimeStep();
		}else{
			//�����x���v�Z
			accel = Iss*(torque - S_tr*(Z_plus_Ia_c + Ia*a_p));
			//�ʒu��ϕ����鏀��
			delta_position = (velocity + 0.5 * accel * dt) * dt;
			//���x��ϕ�
			velocity += accel * dt;
		}
		velocity *= scene->GetVelocityLossPerStep();
		//�d�S����̉����x(�q�m�[�h�̐ϕ��Ŏg�p����)
		a = a_p + c + S*accel;
		if (velocity.norm() > 2*M_PI*10){
			DSTR << "Joint " << GetName() << " has velocity of " << velocity << std::endl;
			DebugBreak();
		}
	}
	void CalcAccel(double dt){				///<	���̃W���C���g�̉����x�v�Z�D�ڍׂ͊�{�N���X�̃R�����g���Q�ƁD
		GetParent()->CalcAccel(dt);	//	�e�̉����x���v�Z
		a_p = cXp_Vec(OfParent(&PHJointMulti::a));
		//�����x���v�Z
		accel = Iss*(torque - S_tr*(Z_plus_Ia_c + Ia*a_p));
		//�d�S����̉����x(�q�m�[�h�̐ϕ��Ŏg�p����)
		a = a_p + c + S*accel;
	}
protected:
	typedef PTM::TMatrixCol<6, NDOF, double>	SMat;
	typedef PTM::TMatrixCol<NDOF, 6, double>	SMatTr;
	SMat S;									///<	spatial joint axis in Fc coord.
	SMatTr S_tr;							///<	S' S�̓Ǝ��̓]�n.
	PTM::TMatrixCol<NDOF, NDOF, double> Iss;///<	S'IaS �̃L���b�V��
	virtual void Reset(){
		accel.clear();
		torque.clear();
		PHJointBase::Reset();
	}
/*
	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
*/	
	virtual void ClearTorqueRecursive(){
		PHJointBase::ClearTorqueRecursive();
		torque.clear();
	}
};


/**	�{�[���W���C���g�C���͎q���̑��ɌŒ肳���D*/
class PHJointBall:public PHJointMulti<3>{
public:
	SGOBJECTDEF(PHJointBall);
	Quaterniond position;					///<	�֐߂̌���
	Vec3d center;			//	����̒��S
	double minDot;			//	���͈�
	double minTwist;			//	�Ђ˂�̉��͈�
	double maxTwist;			//	�Ђ˂�̉��͈�

	PHJointBall();
	///	�֐߈ʒu�̎擾
	virtual double GetJointPosition(int i){ return position.rotation()[i]; }

	virtual void Integrate(SGScene* scene);
	virtual void CompJointAxis();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompCoriolisAccel();
};

/**	���j�o�[�T���W���C���g�C���͎q���̑��ɌŒ肳��� x���� y���������D*/
class PHJointUniversal:public PHJointMulti<2>{
public:
	SGOBJECTDEF(PHJointUniversal);
	Vec2d position;					///<	�֐߂̌���
	///	�֐߈ʒu�̎擾
	virtual double GetJointPosition(int i){ return position[i]; }

	virtual void Integrate(SGScene* scene);
	virtual void CompJointAxis();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompCoriolisAccel();

	//�L���b�V���ϐ�	X���CY������̉�]�s��
	Matrix3f rotX, rotY;
};

}
#endif