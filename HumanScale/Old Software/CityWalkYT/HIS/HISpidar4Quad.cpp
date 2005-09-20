#include "HISpidar4quad.h"
#include "Device/DVDeviceManager.h"
#include <iostream>
#include <Base/Matrix.h>

namespace LVRM {
extern "C" int quadpr_(float*,int*,float*,float*,float*,int*,float*,char*,
                        char*,float*,float*,float*,float*,float*,int*,float*
                        ,int,int);
/*
//	モータの取り付け位置
//	モータが直方体に取り付けられている場合
#define PX	0.265f	//	x方向の辺の長さ/2
#define PY	0.265f	//	y方向の辺の長さ/2
#define PZ	0.265f	//	z方向の辺の長さ/2
Vec3f HISpidar4quad::motorPosDef[][4] =	//	モータの取り付け位置(中心を原点とする)
	{
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
*/
//----------------------------------------------------------------------------

HISpidar4quad::HISpidar4quad(){
    for (int i=0;i<4;i++){
        tension[i] = 0;
        realtens[i] = 0;
    }
}
HISpidar4quad::~HISpidar4quad(){SetMinForce();}

// links the HD to VD and RD
bool HISpidar4quad::Init(DVDeviceManager& dev){
	//	ドライバの取得
	int i;
	for(i=0; i<4; ++i){
		motor[i].da = (DVDaBase*)dev.VPool().Rent("D/A");
		if (!motor[i].da) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<4) return false;
	for(i=0; i<4; ++i){
		motor[i].counter = (DVCounterBase*)dev.VPool().Rent("Counter");
		if (!motor[i].counter) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
	}
	if (i<4) return false;
	InitMat();
	BeforeCalib();
	Calib();
	AfterCalib();
	return true;
}
bool HISpidar4quad::Init(DVDeviceManager& dev, Vec3f* motorPos, float vpn, float lpp, float minF, float maxF){
	for(int i=0; i<4; i++){
		motor[i].pos = motorPos[i];
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
		motor[i].minForce = minF;
		motor[i].maxForce = maxF;
        motor[i].tLimit=motor[i].maxForce-motor[i].minForce;
        }
	return Init(dev);
}
bool HISpidar4quad::Calib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	// calibration sets the center of the reference frame at the current
	// position of the spidar (motor[i].pos.norm())
	Update();
	for(int i=0; i<4; i++) motor[i].Length(motor[i].pos.norm());
	return true;
}
void HISpidar4quad::Update(){
	HIHapticDevice::Update();
	float len[4];
	for(int i=0; i<4; i++) len[i]=motor[i].Length();
//	TRACE("len = %1.3f, %1.3f, %1.3f, %1.3f\n", len[0], len[1], len[2], len[3]);
	pos = matPos * (
		Vec3f(	Square(len[0])-Square(len[1]),
		Square(len[1])-Square(len[2]),
		Square(len[2])-Square(len[3])	) + posSqrConst);
}

void HISpidar4quad::SetForce(const Vec3f& f, float eff, float cont){
	/*	2次計画法による張力計算
		条件式を簡単にするため，最小張力 < 張力 < 最大張力 ではなく
		0 < t=張力-最小張力 < 最大張力-最小張力
		として，tを求めてから，実際に出力する張力を求める．	*/
	Vec3f v3force = f;							//	目的の力
    Vec3f v3fmin=Vec3f(0,0,0);
    for (int i=0; i<4; i++){
        phi[i] = (motor[i].pos-pos).unit();		//	張力の単位ベクトル		tension direction
        v3fmin=v3fmin+motor[i].minForce*phi[i];	//	最小張力が作り出す力	"least power"
    }
    v3force = v3force-v3fmin;					//	入力から最小張力による力を引く

	/*	目的関数
		  |f-φt|^2 + cont*|t-t'|^2 + eff*t^2
		= (φ^2+eff+cont)*t^2 + (-2*φ*f -2*cont*t')*t + f^2+cont*t'^2
		を最小にする張力tを求める．	*/
    float quad[16];								//	目的関数の２次係数行列
    for(int i=0;i<4;i++) quad[4*i+i]=phi[i]*phi[i]+eff+cont;
    quad[1]=quad[4]=phi[0]*phi[1];
    quad[2]=quad[8]=phi[0]*phi[2];
    quad[3]=quad[12]=phi[0]*phi[3];
    quad[6]=quad[9]=phi[1]*phi[2];
    quad[7]=quad[13]=phi[1]*phi[3];
    quad[11]=quad[14]=phi[2]*phi[3];

    float cost[4];								//	目的関数の１次係数ベクトル
    for(int i=0;i<4;i++) cost[i]=-2*phi[i]*v3force -2*cont*tension[i];

    float a[16];								//	条件式の左辺、単位行列
    a[0]=a[5]=a[10]=a[15]=1;
    a[1]=a[2]=a[3]=a[4]=a[6]=a[7]=a[8]=a[9]=a[11]=a[12]=a[13]=a[14]=0;

    float rhs[4];								//	条件式の右辺、ベクトル
    for(int i=0;i<4;i++) rhs[i] = motor[i].maxForce - motor[i].minForce;

    int kt[4]={1,1,1,1};						//	条件式の不等号、<=なら１
    int input[14]={4,4,4,4,0,240,1000,1,0,0,0,0,0,80};	//	各設定変数
    float tol[2]={0,0};							//	許容誤差

    char title[]="quadratic ploblem";
    char pfile[]="savefile";

    float obj[1],x[4],rc[4],dual[4],slk[4];        //output＆auxiliary storage
    int iout[2];                                   //output
    float ws[240];                                 //work space
    int title_len=0, pfile_len=0;

    quadpr_(a, kt, rhs, cost, quad, input, tol, title, pfile,
	 obj, x, rc, dual, slk, iout, ws,title_len,pfile_len);

    for(int i=0;i<4;i++) {
        tension[i]=x[i];							//張力＝x[]
    }
    for (int i=0;i<4;i++){
		realtens[i] = tension[i] + motor[i].minForce;
        motor[i].Force(realtens[i]);
    }
}
Vec3f HISpidar4quad::GetForce(){
    int i;
	Vec3f f;
    for (i=0;i<4;i++) f=f+tension[i]*phi[i];
    return f;
}

void HISpidar4quad::SetMinForce(){
	for(int i=0; i<4; i++) motor[i].Force(motor[i].minForce);
}

void HISpidar4quad::InitMat()
{
	matPos = 2*Matrix3f(
		motor[1].pos-motor[0].pos,
		motor[2].pos-motor[1].pos,
		motor[3].pos-motor[2].pos).trans();
	matPos = matPos.inv();

	posSqrConst = Vec3f(motor[1].pos.square()-motor[0].pos.square(),
		motor[2].pos.square()-motor[1].pos.square(),
		motor[3].pos.square()-motor[2].pos.square());
}
}	//	namespace LVRM
