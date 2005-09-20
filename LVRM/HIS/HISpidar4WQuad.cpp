/*

  Tangible Playroom�pISA-SPIDAR
  (8���[�^�����E4�`���l���g�p)

*/
#include "HISpidar4Wquad.h"
#include "Device/DVDeviceManager.h"
#include <iostream>
#include <Base/Matrix.h>

namespace LVRM {
extern "C" int quadpr_(float*,int*,float*,float*,float*,int*,float*,char*,
                        char*,float*,float*,float*,float*,float*,int*,float*
                        ,int,int);

/*
//	���[�^�̎��t���ʒu
//	���[�^�������̂Ɏ��t�����Ă���ꍇ
#define PX	0.265f	//	x�����̕ӂ̒���/2
#define PY	0.265f	//	y�����̕ӂ̒���/2
#define PZ	0.265f	//	z�����̕ӂ̒���/2
Vec3f HISpidar4Wquad::motorPosDef[][4] =	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
	{
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
*/
//----------------------------------------------------------------------------

HISpidar4Wquad::HISpidar4Wquad(){
	chNum = 4;//����SPIDAR��4ch���p��z�肵�Ă���
	chMax = 8;//define(SPIDAR4W_MAXMORTORS)����ϐ��Ɉڍs��...
    for (int i=0;i<SPIDAR4W_MAXMORTORS;i++){
        tension[i] = 0;
        realtens[i] = 0;
    }
}
HISpidar4Wquad::~HISpidar4Wquad(){SetMinForce();}

// links the HD to VD and RD
bool HISpidar4Wquad::Init(DVDeviceManager& dev){
	//	�h���C�o�̎擾
	int i;
	//���S�̂��߂Ƀ[��ch���珉�������Ă���
	for(i=0; i<SPIDAR4W_MAXMORTORS; ++i){
		motor[i].da = (DVDaBase*)dev.VPool().Rent("D/A");
		if (!motor[i].da) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<chMax) return false;
	for(i=0; i<SPIDAR4W_MAXMORTORS; ++i){
		motor[i].counter = (DVCounterBase*)dev.VPool().Rent("Counter");
		if (!motor[i].counter) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
		printf("DEBUG:Ct ch%d\n",i);
	}
	if (i<SPIDAR4W_MAXMORTORS) return false;
	InitMat();
	BeforeCalib();
	Calib();
	AfterCalib();
	return true;
}

bool HISpidar4Wquad::Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF){
	ch = (int)motorPos[8].X(); //1�ڂ̃��[�^ch
	printf("HISpidar4Wquad::Init - The 1st motor channel is %d.\n",ch);
//	for(int i=ch; i<chMax; i++){
	for(int i=0; i<SPIDAR4W_MAXMORTORS; i++){
		motor[i].pos = motorPos[i];
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
		motor[i].minForce = minF;
		motor[i].maxForce = maxF;
        motor[i].tLimit=motor[i].maxForce-motor[i].minForce;
//		std::cout << "motor[" << i << "].pos=" << motorPos[i] << std::endl;
        }
	return Init(dev);
}
bool HISpidar4Wquad::Calib(){
	//	�|�C���^�����_(���S)�ɒu���āA�L�����u���[�V�������s��
	// calibration sets the center of the reference frame at the current
	// position of the spidar (motor[i].pos.norm())
	Update();
	for(int i=0; i<SPIDAR4W_MAXMORTORS; i++) motor[i].Length(motor[i].pos.norm());
	return true;
}

void HISpidar4Wquad::Update(){
	HIHapticDevice::Update();
	float len[SPIDAR4W_MAXMORTORS];
	for(int i=0; i<SPIDAR4W_MAXMORTORS; i++) len[i]=motor[i].Length();
#ifdef __DEBUG
	printf("HISpidar4Wquad::Update()\n");
	for(int i=0;i<SPIDAR4W_MAXMORTORS;i++){
		printf("ch[%d] = %d\n",i,len[i]);
	}
#endif
	//�������炠��v�Z���@�Ƀx�[�X�`���l���w���������������
/*	pos = matPos * (
		Vec3f(	Square(len[0])-Square(len[1]),
		Square(len[1])-Square(len[2]),
		Square(len[2])-Square(len[3])	) + posSqrConst);
		*/
	pos = matPos * (
		Vec3f(	Square(len[ch+0])-Square(len[ch+1]),
		Square(len[ch+1])-Square(len[ch+2]),
		Square(len[ch+2])-Square(len[ch+3])	) + posSqrConst);

}

void HISpidar4Wquad::SetForce(const Vec3f& f, float eff, float cont){
	/*	2���v��@�ɂ�钣�͌v�Z
		���������ȒP�ɂ��邽�߁C�ŏ����� < ���� < �ő咣�� �ł͂Ȃ�
		0 < t=����-�ŏ����� < �ő咣��-�ŏ�����
		�Ƃ��āCt�����߂Ă���C���ۂɏo�͂��钣�͂����߂�D	*/
	Vec3f v3force = f;							//	�ړI�̗�
    Vec3f v3fmin=Vec3f(0,0,0);
    for (int i=0; i<chNum; i++){
        phi[i] = (motor[i+ch].pos-pos).unit();		//	���͂̒P�ʃx�N�g��		tension direction
        v3fmin=v3fmin+motor[i+ch].minForce*phi[i];	//	�ŏ����͂����o����	"least power"
    }
    v3force = v3force-v3fmin;					//	���͂���ŏ����͂ɂ��͂�����

	/*	�ړI�֐�
		  |f-��t|^2 + cont*|t-t'|^2 + eff*t^2
		= (��^2+eff+cont)*t^2 + (-2*��*f -2*cont*t')*t + f^2+cont*t'^2
		���ŏ��ɂ��钣��t�����߂�D	*/
    float quad[16];								//	�ړI�֐��̂Q���W���s��
    for(int i=0;i<4;i++) quad[4*i+i]=phi[i]*phi[i]+eff+cont;
    quad[1]=quad[4]=phi[0]*phi[1];
    quad[2]=quad[8]=phi[0]*phi[2];
    quad[3]=quad[12]=phi[0]*phi[3];
    quad[6]=quad[9]=phi[1]*phi[2];
    quad[7]=quad[13]=phi[1]*phi[3];
    quad[11]=quad[14]=phi[2]*phi[3];

    float cost[4];								//	�ړI�֐��̂P���W���x�N�g��
    for(int i=0;i<4;i++) cost[i]=-2*phi[i]*v3force -2*cont*tension[ch+i];

    float a[16];								//	�������̍��ӁA�P�ʍs��
    a[0]=a[5]=a[10]=a[15]=1;
    a[1]=a[2]=a[3]=a[4]=a[6]=a[7]=a[8]=a[9]=a[11]=a[12]=a[13]=a[14]=0;

    float rhs[4];								//	�������̉E�ӁA�x�N�g��

    for(int i=0;i<4;i++) rhs[i] = motor[ch+i].maxForce - motor[ch+i].minForce;

    int kt[4]={1,1,1,1};						//	�������̕s�����A<=�Ȃ�P
    int input[14]={4,4,4,4,0,240,1000,1,0,0,0,0,0,80};	//	�e�ݒ�ϐ�
    float tol[2]={0,0};							//	���e�덷

    char title[]="quadratic ploblem";
    char pfile[]="savefile";

    float obj[1],x[4],rc[4],dual[4],slk[4];        //output��auxiliary storage
    int iout[2];                                   //output
    float ws[240];                                 //work space
    int title_len=0, pfile_len=0;

    quadpr_(a, kt, rhs, cost, quad, input, tol, title, pfile,
	 obj, x, rc, dual, slk, iout, ws,title_len,pfile_len);

    for(int i=0;i<4;i++) {
        tension[ch+i]=x[i];							//���́�x[]
    }
    for (int i=0;i<4;i++){
		realtens[ch+i] = tension[ch+i] + motor[ch+i].minForce;
	    motor[ch+i].Force(realtens[ch+i]);
    }
}

Vec3f HISpidar4Wquad::GetForce(){
    int i;
	Vec3f f;
    for (i=0;i<SPIDAR4W_MAXMORTORS;i++) f=f+tension[i]*phi[i];
    return f;
}

void HISpidar4Wquad::SetMinForce(){
//	for(int i=0; i<SPIDAR4W_MAXMORTORS; i++) motor[i].Force(motor[i].minForce);
	for(int i=0; i<SPIDAR4W_MAXMORTORS; i++) {
		if ((ch<=i)&&(i<=chMax)) {
			//�g���`���l�������o��
			motor[i].Force(motor[i].minForce);
			//���Ƃ̓[����ݒ�
		} else {
			motor[i].Force(0);
			motor[i].da->Voltage(0);
			printf("ch[%d] - SetMinForce as Zero.\n",i);
		}
	}
}

void HISpidar4Wquad::InitMat()
{
	matPos = 2*Matrix3f(
		motor[ch+1].pos-motor[ch+0].pos,
		motor[ch+2].pos-motor[ch+1].pos,
		motor[ch+3].pos-motor[ch+2].pos).trans();
	matPos = matPos.inv();

	posSqrConst = Vec3f(motor[ch+1].pos.square()-motor[ch+0].pos.square(),
		motor[ch+2].pos.square()-motor[ch+1].pos.square(),
		motor[ch+3].pos.square()-motor[ch+2].pos.square());

}
}	//	namespace LVRM
