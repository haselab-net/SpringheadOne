#include "HIS.h"
#pragma hdrstop
#include "HISpidar4.h"

namespace Spr {
/*
//	���[�^�̎��t���ʒu
//	���[�^�������̂Ɏ��t�����Ă���ꍇ
#define PX	0.265f	//	x�����̕ӂ̒���/2
#define PY	0.265f	//	y�����̕ӂ̒���/2
#define PZ	0.265f	//	z�����̕ӂ̒���/2
Vec3f HISpidar4::motorPosDef[][4] =	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
	{
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
*/
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidar4, HIForceDevice3D);

HISpidar4::HISpidar4(){
    for (int i=0;i<4;i++){
        tension[i] = 0;
    }
}
HISpidar4::~HISpidar4(){SetMinForce();}

// links the HD to VD and RD
bool HISpidar4::Init(DVDeviceManager& dev){
	//	�h���C�o�̎擾
	int i;
	for(i=0; i<4; ++i){
		if (!dev.VPool().Rent(motor[i].da)) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<4) return false;
	for(i=0; i<4; ++i){
		if (!dev.VPool().Rent(motor[i].counter)) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
	}
	if (i<4) return false;
	InitMat();
	BeforeCalib();
	Calib();
	AfterCalib();
	bGood = true;
	return true;
}
bool HISpidar4::Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF){
	for(int i=0; i<4; i++){
		motor[i].pos = motorPos[i];
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
		motor[i].minForce = minF;
		motor[i].maxForce = maxF;
        }
	return Init(dev);
}
bool HISpidar4::Calib(){
	//	�|�C���^�����_(���S)�ɒu���āA�L�����u���[�V�������s��
	// calibration sets the center of the reference frame at the current
	// position of the spidar (motor[i].pos.norm())
	Update(0.001f);
	for(int i=0; i<4; i++) motor[i].SetLength(motor[i].pos.norm());
	return true;
}
void HISpidar4::Update(float dt){
	HIForceDevice3D::Update(dt);
	float len[4];
	for(int i=0; i<4; i++) len[i]=motor[i].GetLength();
//	TRACE("len = %1.3f, %1.3f, %1.3f, %1.3f\n", len[0], len[1], len[2], len[3]);
	// test debug
	/*
	for(int i=0;i<4;i++){
		printf("%d",len[i]);
	}
	*/
	//printf("\n");
	// test debug

	pos = matPos * (
		Vec3f(	Square(len[0])-Square(len[1]),
		Square(len[1])-Square(len[2]),
		Square(len[2])-Square(len[3])	) + posSqrConst);
}

/*	2���v��@�ɂ�钣�͌v�Z	*/
void HISpidar4::SetForce(const Vec3f& v3force, float eff, float cont){
	//	���̕����x�N�g�������߂�B
    for (int i=0; i<4; i++){
        phi[i] = (motor[i].pos-pos).unit();		//	���͂̒P�ʃx�N�g��		tension direction
	}
	/*	�ړI�֐�
		  |f-��t|^2 + cont*|t-t'|^2 + eff*t^2
		= (��^2+eff+cont)*t^2 + (-2*��*f -2*cont*t')*t + f^2+cont*t'^2
		���ŏ��ɂ��钣��t�����߂�D	*/
    TQuadProgram<float, 4> qp;
    //	�ړI�֐��̂Q���W���s��
    for(int i=0;i<4;i++) qp.matQ[i][i]=phi[i]*phi[i]+eff+cont;
    qp.matQ[0][1]=qp.matQ[1][0]=phi[0]*phi[1];
    qp.matQ[0][2]=qp.matQ[2][0]=phi[0]*phi[2];
    qp.matQ[0][3]=qp.matQ[3][0]=phi[0]*phi[3];
    qp.matQ[1][2]=qp.matQ[2][1]=phi[1]*phi[2];
    qp.matQ[1][3]=qp.matQ[3][1]=phi[1]*phi[3];
    qp.matQ[2][3]=qp.matQ[3][2]=phi[2]*phi[3];

    //	�ړI�֐��̂P���W���x�N�g��
    for(int i=0;i<4;i++) qp.vecC[i]= phi[i]*v3force + cont*tension[i];
	//	�ŏ����́E�ő咣�͂̐ݒ�
	Vec4f minF, maxF;
	for(int i=0; i<4; ++i){
		minF[i] = motor[i].minForce;
		maxF[i] = motor[i].maxForce;
	}
	qp.Init(minF, maxF);
	qp.Solve();
    for(int i=0;i<4;i++) {
        tension[i]=qp.vecX[i];							//���́�x[]
    }
    for (int i=0;i<4;i++){
        motor[i].SetForce(tension[i]);
    }
}
Vec3f HISpidar4::GetForce(){
    int i;
	Vec3f f;
    for (i=0;i<4;i++) f=f+tension[i]*phi[i];
    return f;
}

void HISpidar4::SetMinForce(){
	for(int i=0; i<4; i++) motor[i].SetForce(motor[i].minForce);
}
void HISpidar4::InitMat(){
	matPos = Matrix3f(
		motor[1].pos-motor[0].pos,
		motor[2].pos-motor[1].pos,
		motor[3].pos-motor[2].pos).trans() * 2;
	matPos = matPos.inv();

	posSqrConst = Vec3f(motor[1].pos.square()-motor[0].pos.square(),
		motor[2].pos.square()-motor[1].pos.square(),
		motor[3].pos.square()-motor[2].pos.square());
}

}	//	namespace Spr
