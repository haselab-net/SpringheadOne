#ifndef HAPTICRENDER_H
#define HAPTICRENDER_H


#include <Base/Affine.h>
#include <HIS/HIHapticDevice.h>
#include "../../ihc/src/IHCBase/IHUsbPacket.h"

using namespace Spr;
#define USE_IHC

class HapticRender{
	float forceScale, posScale;
	Vec3f dSpring, lastSpring;
public:
	typedef IHPacketPlane::PlaneState PlaneState;

	UTRef<HIForceDevice6D> device;
	volatile PlaneState planeState;
	typedef PTM::TVector<3, volatile float> VVec3f;
	VVec3f offset;
	VVec3f force;			//	�񎦂����͐�
	VVec3f pointer;			//	�f�o�C�X�̈ʒu
	VVec3f pos;				//	���݂̒񎦖ʂ̈ʒu
	VVec3f normal;			//	���݂̒񎦖ʂ̖@���̌���
	VVec3f newPos;			//	�󂯎�����񎦖ʂ̈ʒu
	VVec3f newNormal;		//	�󂯎�����񎦖ʂ̖@���̌���
	VVec3f proxy;			//	�v���L�V�̈ʒu
	float hapticDt;			//	�͊o���[�v��dt
	float simulationDt;		//	�V�~�����[�V������dt
	float hapticTime;

	HapticRender();
	virtual ~HapticRender();
	///	�ʂ��Z�b�g
	void SetPlane(Vec3f pos, Vec3f normal, PlaneState stat, float ps, float fs, Vec3f offset);
	///	�͐ς��󂯎��
	Vec3f GetForce();
	///	
	void HapticStep();

};

#endif
