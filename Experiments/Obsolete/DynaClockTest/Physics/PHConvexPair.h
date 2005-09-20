#ifndef PHINTERSECTION_H
#define PHINTERSECTION_H
#include <Collision/CDFramePair.h>
#include <Collision/CDDetectorImp.h>
#include <Physics/PHSolid.h>

namespace Spr{;

class PHConvexPair:public CDConvexPairWithNormal, public CDContactAnalysis{
public:
	//	�R�͂̌v�Z
	float area;							///<	�������̖ʐ�
	Vec3f reflexSpringForce;			///<	�΂˂ɂ��R��
	Vec3f reflexDamperForce;			///<	�_���p�[�ɂ��R��
	Vec3f reflexSpringTorque;			///<	�΂˂ɂ��R�g���N
	Vec3f reflexDamperTorque;			///<	�_���p�[�ɂ��R�g���N
	Vec3f reflexForcePoint;				///<	�R�͂̍�p�_
	
	//	���C�̌v�Z
	Vec3f transFrictionBase[2];			///<	���i�Î~���C�p�o�l�̒[�_
	Vec3f rotFricSpring;				///<	��]�Î~���C�p�o�l�̐L��
	Quaternionf lastOri01;
	Vec3f dynaFric;						///<	�����C��
	Vec3f dynaFricMom;					///<	�����C�͂̃��[�����g(���_���)
	Vec3f frictionForce;				///<	���C��
	Vec3f frictionTorque;				///<	���C�g���N(���C�͂̍�p�_�����_�̂Ƃ���)

	//	�W��
	float rk, rb, fk, fb;

	/**	������������������R�͂����߂�D
		�y�i���e�B�Ƃ��̃��[�����g�C���Α��x�Ƃ��̃��[�����g�����߂�D	*/
	void CalcReflexForce(Vec3f* cog, Vec3f* vel, Vec3f* angVel, float k, float b);
	///	�O�p�`1����������y�i���e�B�Ƃ��̃��[�����g�C���Α��x�Ƃ��̃��[�����g�����߂�D
	void CalcTriangleReflexForce(Vec3f p, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel, float k, float b);
	///	���C�͂����߂�D	���̂Ƃ��듮���C�̍ő�l������������ŃR�}���񂹂Ȃ��D
	void CalcFriction(Quaternionf* ori, Vec3f* pos, Vec3f* cog, Vec3f* vel, Vec3f* angVel, float k, float b);

	/**	������������������R�͂����߂�D
		�y�i���e�B�Ƃ��̃��[�����g�C���Α��x�Ƃ��̃��[�����g�����߂�D
		���͂Ɩ��C�͂̂΂ˁE�_���p�W����ʁX�ɕ������D�@*/
	void CalcReflexForce(Vec3f* cog, Vec3f* vel, Vec3f* angVel, float rk, float rb, float fk, float fb);
	/**	�O�p�`1����������y�i���e�B�Ƃ��̃��[�����g�C���Α��x�Ƃ��̃��[�����g�����߂�D
		���͂Ɩ��C�͂̂΂ˁE�_���p�W����ʁX�ɕ������D	*/
	void CalcTriangleReflexForce(Vec3f p, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel, float rk, float rb, float fk, float fb);
};
class PHGeometryPair:public CDGeometryPair{
};

};
#endif
