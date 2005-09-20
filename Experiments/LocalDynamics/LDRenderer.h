#ifndef LDRENDERER_H
#define LDRENDERER_H

//#define USE_IHC
//#define ADJUST_POS

#ifdef USE_IHC
#include <Base/Affine.h>
#include <HIS/HISpidar4.h>
#else
#include <WinBasis/WinBasis.h>
#include <Framework/FWPointer.h>
#endif

#include <HIS/HIHapticDevice.h>
#include "Matrix6.h"
#include "Vector6.h"
#include "../../ihc/src/IHCBase/IHUsbPacket.h"

namespace Spr {;

class LDRenderer{
	float k, b;                      // ���̂̂΂ˌW���ƃ_���p�W��
	float x, prevX;                  // �|�C���^�̐N���ʂƈ��O�̐N����

	Vec3f torque_ex;                 // ���̂̃g���N
	Vec3f f_external;                // �O�͏��
	Vec3f pos, prevPos;              // ���̂̍ŋߖT�_�̈ʒu�ƈ��O�̈ʒu
	Vec3f currentPos;                // ���݂̈ʒu�iLD�v�Z�Ȃ��p�j
	Vec3f center;                    // ���̂̎��ʒ��S
	Vec3f normal, prevNormal;        // �Փ˂̖@���ƈ��O�̖@��
	Vec3f currentNormal;             // ���݂̖@���iLD�v�Z�Ȃ��p�j

	Matrix6f Iinv;
	Vec6f    velocities;
	Vec6f    displacement;

	Vec3f impulse;
	Vec3f force;                     // �񎦗�
	Vec3f pointer, prevPointer;      // �|�C���^�̌��݈ʒu�ƈ��O�̈ʒu
	float ldDT, simuDT;              // ���ԊԊu(rendering & simulator)

	bool calc_ld;                    // �Ǐ������̌v�Z���s�����ǂ���
	bool bCalc;
	bool bExperiment;				// �����p�t���O�iLD�Ȃ��̎��̏ꍇ�킯�p�j

	float posScale;
	float forceScale;
	
	Quaternionf qtOffset;
	Quaternionf qtOffsetInv;
	Vec3f       v3Offset;

#ifndef USE_IHC
	WBCriticalSection cs;            // Critical Section (Simulator�Ƃ̓����j
#endif

public:

#ifndef USE_IHC
	FWPointer6D* device;
#else
//	UTRef<HIForceDevice6D> device;
	HISpidar4* device;
#endif

	LDRenderer();
	~LDRenderer();
	//����̍X�V����
	void Step();
	// �O��̕ϐ����L�^
	void RegisterPreviousParameters();
	// �͊o�񎦃v���Z�X
	void LDHapticRendering();
	// �񎦗͂̌v�Z
	void CalcForce();
	// �Ǐ��I���͊w�̌v�Z�v���Z�X
	void LDLocalDynamics();
	// �Ǐ��I���͊w���v�Z���Ȃ��ꍇ�i��r�����p�j
	void LocalUpdate();
	// Simulator����󂯎��ʒu�̏C��
	void AdjustPos();
#ifndef USE_IHC
	// Enter Critical Section
	void CSEnter(){ cs.Enter(); }
	// Leave Critical Section
	void CSLeave(){ cs.Leave(); }
#endif

	Vec3f GetImpulse()		{ return impulse;  }
	Vec3f GetPos()			{ return pos;        }
	Vec3f GetNormal()		{ return normal;     }
	Vec3f GetPointer()		{ return pointer;    }
	Vec3f GetForce()		{ return force;      }
	Vec6f GetVelocities()	{ return velocities; }
	Vec6f GetDisplacement()	{ return displacement; }
	float GetLDDT()			{ return ldDT;       }

	void SetSimuDT(float t){ simuDT = t; }
	void SetLDDT(float t)  { ldDT   = t; }
	void SetK (float x)    { k      = x; }
	void SetB (float x)    { b      = x; }
 
	void SetPos       (const Vec3f& p) { pos        = p; }
	void SetNormal    (const Vec3f& n) { normal     = n; }
	void SetCenter    (const Vec3f& c) { center     = c; }
	void SetExTorque  (const Vec3f& t) { torque_ex  = t; }
	void SetExForce   (const Vec3f& f) { f_external = f; }
	void SetVelocities(const Vec6f& v) { velocities = v; }

	void SetCurrentPos(){currentPos = prevPos; }

	void SetIinv    (const Matrix6f& i) { Iinv = i; }

	void SetParameters(IHPacketLD packet);
	void ResetParameters(){ impulse.clear(); 
						displacement.clear(); }

	bool GetCalcLD()       { return calc_ld;  }
	void SetCalcLD(bool b) { calc_ld = b; } 
	void SetBCalc (bool b) { bCalc   = b; }
	void SetBExperiment(bool b) { bExperiment = b; }

	void SetPointer(Vec3f p){ pointer = p; }
};

} // namespase Spr

#endif //LDRENDERER_H

