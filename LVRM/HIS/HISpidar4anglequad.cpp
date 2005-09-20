#include "HISpidar4anglequad.h"
#include "Device/DVDeviceManager.h"
#include <iostream>
#include <Base/Matrix.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

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
Vec3f HISpidar4anglequad::motorPosDef[][4] =	//	モータの取り付け位置(中心を原点とする)
	{
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
*/
//----------------------------------------------------------------------------

HISpidar4anglequad::HISpidar4anglequad(){
    for (int i=0;i<4;i++){
        tension[i] = 0;
        realtens[i] = 0;
//追加
		flag = true;
    }
}
HISpidar4anglequad::~HISpidar4anglequad(){SetMinForce();}

// links the HD to VD and RD
bool HISpidar4anglequad::Init(DVDeviceManager& dev){
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
	for (i=0; i<4; ++i){
		motor[i].counter_angle = (DVCounterBase*)dev.VPool().Rent("Counter");
		if (!motor[i].counter_angle) break;
		AddRealDeviceDependency(motor[i].counter_angle->RealDevice());
	}
	if (i<4) return false;
	InitMat();
//追加	
	Motor()[2].anglePerPulse *= -1;
	Motor()[3].anglePerPulse *= -1;	

	BeforeCalib();
	Calib();
	AfterCalib();
	return true;
}

bool HISpidar4anglequad::Init(DVDeviceManager& dev, Vec3f* motorPos, Vec3f* iPos, float vpn, float lpp, float app, float minF, float maxF){
	for(int i=0; i<4; i++){
		motor[i].pos = motorPos[i];
		motor[i].voltPerNewton = vpn;
		motor[i].lengthPerPulse = lpp;
		motor[i].anglePerPulse = app;
		motor[i].minForce = minF;
		motor[i].maxForce = maxF;
        motor[i].tLimit=motor[i].maxForce-motor[i].minForce;
		}
	for(int i=0; i<5; i++){
		/// posの初期化
		initPos[i] = iPos[i];
		pos[i] = initPos[i];
	}
	//データ取得用
	test_time = 0;
/*	memset(a1_data, 0, sizeof(a1_data));
	memset(a2_data, 0, sizeof(a2_data));
	memset(a3_data, 0, sizeof(a3_data));
	memset(a4_data, 0, sizeof(a4_data));
	memset(l1_data, 0, sizeof(l1_data));
	memset(l2_data, 0, sizeof(l2_data));
	memset(l3_data, 0, sizeof(l3_data));
	memset(l4_data, 0, sizeof(l4_data));

*/	
	return Init(dev);
}

bool HISpidar4anglequad::Calib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	
	Update();
	for(int i=0; i<4; i++) motor[i].Length((motor[i].pos-initPos[i]).norm());
	for(int i=0; i<4; i++) motor[i].Angle(atan2(initPos[i].Z()-motor[i].pos.Z(),initPos[i].X()-motor[i].pos.X()));

//	std::cout << motor[0].Angle() * (180.0/3.141592) << std::endl;
//	std::cout << initPos[0].Z()-motor[0].pos.Z() << std::endl;
//	std::cout << initPos[0].X()-motor[0].pos.X() << std::endl;
//	std::cout << atan((initPos[0].Z()-motor[0].pos.Z())/(initPos[0].X()-motor[0].pos.X())) * (180.0/3.141592) << std::endl;

	return true;
}

void HISpidar4anglequad::Update(){
	HIHapticDevice::Update();
	Pos_6dof();
		
/*
		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:	//Esc
				end_flag = 1;
				timeKillEvent(TimerId);
				timeEndPeriod(1);
				break;
			case  0x36:	//6
				Calib();
				break;
			}
		}

	
	x_data[test_time] = Xnew;
	y_data[test_time] = Ynew;
	z_data[test_time] = Znew;

	quatang_data[test_time] = 2*acos(quater.W())*(180/3.141592);
	quatvecX_data[test_time] = v_vec.X();
	quatvecY_data[test_time] = v_vec.Y();
	quatvecZ_data[test_time] = v_vec.Z();

	a1_data[test_time] = a1*(180/3.141592);
	a2_data[test_time] = a2*(180/3.141592);
	a3_data[test_time] = a3*(180/3.141592);
	a4_data[test_time] = a4*(180/3.141592);
	l1_data[test_time] = l1;
	l2_data[test_time] = l2;
	l3_data[test_time] = l3;
	l4_data[test_time] = l4;


//		std::cout << a1 << " " << a2 << " "  << a3 << " " << a4 << std::endl;

	if (test_time < sizeof(x_data) / sizeof(x_data[0])){
		test_time++;
	}
*/}

//**********************コレスキー法？***************************//
void HISpidar4anglequad::CholeskyFacToGetSolution(TMatrix<9,9,float> Q ,TVector<9,float> s)
{
	int i,j,k;
	int num=9;
	
	//reduction  foreward
	Q[0][0]=sqrt(Q[0][0]);
	s[0]/=Q[0][0];
	for(i=1;i<num;i++) {
		Q[0][i]/=Q[0][0];
	}	
	
	for(i=1;i<num;i++){
		for(k=0;k<i;k++){
			Q[i][i] -= Q[k][i]*Q[k][i];
			s[i]-=Q[k][i]*s[k];
		}
		Q[i][i]=sqrt(Q[i][i]);
		for(j=i+1;j<num;j++){
			for (k=0;k<i;k++){
				Q[i][j]-=Q[k][i]*Q[k][j];
			}
			Q[i][j]/=Q[i][i];
		}
		s[i]/=Q[i][i];
	}

	// backwark substitution 
    for(i=num-1;i>=0;i--){
		for(j=i+1;j<num;j++){			
			s[i] -= Q[i][j] * s[j];
        }
		s[i] /= Q[i][i];
    }
    
	deltaX=s[0]  , deltaY=s[1]  , deltaZ=s[2];
	deltaX1=s[3] , deltaY1=s[4] , deltaZ1=s[5];
	deltaX2=s[6] , deltaY2=s[7] , deltaZ2=s[8];

}


//******************* 逐次計算を行う関数 *********************//
void HISpidar4anglequad::SetMat( double x,double y,double  z,double x1,double y1,double z1,double x2,double y2,double z2)
{
    int     i, j, k;
	double	denom1, denom2, denom3, denom4;		//　角度の項の分母

	denom1 = (x+x1-motor[0].pos.X())*(x+x1-motor[0].pos.X()) + (z+z1-motor[0].pos.Z())*(z+z1-motor[0].pos.Z());
	denom2 = (x-x1-motor[1].pos.X())*(x-x1-motor[1].pos.X()) + (z-z1-motor[1].pos.Z())*(z-z1-motor[1].pos.Z());
	denom3 = (x+x2-motor[2].pos.X())*(x+x2-motor[2].pos.X()) + (z+z2-motor[2].pos.Z())*(z+z2-motor[2].pos.Z());
	denom4 = (x-x2-motor[3].pos.X())*(x-x2-motor[3].pos.X()) + (z-z2-motor[3].pos.Z())*(z-z2-motor[3].pos.Z());
    
	matA[0][0]=x+x1-motor[0].pos.X();		matA[0][1]=y+y1-motor[0].pos.Y();		matA[0][2]=z+z1-motor[0].pos.Z();
    matA[0][3]=x+x1-motor[0].pos.X();		matA[0][4]=y+y1-motor[0].pos.Y();		matA[0][5]=z+z1-motor[0].pos.Z();
	matA[0][6]=0;			matA[0][7]=0;			matA[0][8]=0;
   
    matA[1][0]=x-x1-motor[1].pos.X();		matA[1][1]=y-y1-motor[1].pos.Y();		matA[1][2]=z-z1-motor[1].pos.Z();
    matA[1][3]=-(x-x1-motor[1].pos.X());	matA[1][4]=-(y-y1-motor[1].pos.Y());	matA[1][5]=-(z-z1-motor[1].pos.Z());
	matA[1][6]=0;			matA[1][7]=0;			matA[1][8]=0;
    
    matA[2][0]=x+x2-motor[2].pos.X();		matA[2][1]=y+y2-motor[2].pos.Y();		matA[2][2]=z+z2-motor[2].pos.Z();
    matA[2][3]=0;			matA[2][4]=0;			matA[2][5]=0;
	matA[2][6]=x+x2-motor[2].pos.X();		matA[2][7]=y+y2-motor[2].pos.Y();		matA[2][8]=z+z2-motor[2].pos.Z();
    
	matA[3][0]=x-x2-motor[3].pos.X();		matA[3][1]=y-y2-motor[3].pos.Y();		matA[3][2]=z-z2-motor[3].pos.Z();
    matA[3][3]=0;			matA[3][4]=0;			matA[3][5]=0;
	matA[3][6]=-(x-x2-motor[3].pos.X());	matA[3][7]=-(y-y2-motor[3].pos.Y());	matA[3][8]=-(z-z2-motor[3].pos.Z());

    matA[4][0]=-(z+z1-motor[0].pos.Z())/denom1;		matA[4][1]=0;	matA[4][2]=(x+x1-motor[0].pos.X())/denom1;
    matA[4][3]=-(z+z1-motor[0].pos.Z())/denom1;		matA[4][4]=0;	matA[4][5]=(x+x1-motor[0].pos.X())/denom1;
	matA[4][6]=0;			matA[4][7]=0;			matA[4][8]=0;

    matA[5][0]=-(z-z1-motor[1].pos.Z())/denom2;		matA[5][1]=0;	matA[5][2]=(x-x1-motor[1].pos.X())/denom2;
    matA[5][3]=(z-z1-motor[1].pos.Z())/denom2;		matA[5][4]=0;	matA[5][5]=-(x-x1-motor[1].pos.X())/denom2;
	matA[5][6]=0;			matA[5][7]=0;			matA[5][8]=0;
  
    matA[6][0]=-(z+z2-motor[2].pos.Z())/denom3;		matA[6][1]=0;	matA[6][2]=(x+x2-motor[2].pos.X())/denom3;
    matA[6][3]=0;			matA[6][4]=0;			matA[6][5]=0;
	matA[6][6]=-(z+z2-motor[2].pos.Z())/denom3;		matA[6][7]=0;	matA[6][8]=(x+x2-motor[2].pos.X())/denom3;
    
	matA[7][0]=-(z-z2-motor[3].pos.Z())/denom4;		matA[7][1]=0;	matA[7][2]=(x-x2-motor[3].pos.X())/denom4;
    matA[7][3]=0;			matA[7][4]=0;			matA[7][5]=0;
	matA[7][6]=(z-z2-motor[3].pos.Z())/denom4;		matA[7][7]=0;	matA[7][8]=-(x-x2-motor[3].pos.X())/denom4;

    matA[8][0]=0;			matA[8][1]=0;			matA[8][2]=0;
    matA[8][3]=x1;			matA[8][4]=y1;			matA[8][5]=z1;
	matA[8][6]=0;			matA[8][7]=0;			matA[8][8]=0;

    matA[9][0]=0;			matA[9][1]=0;			matA[9][2]=0;
    matA[9][3]=0;			matA[9][4]=0;			matA[9][5]=0;
	matA[9][6]=x2;			matA[9][7]=y2;			matA[9][8]=z2;

    matA[10][0]=0;			matA[10][1]=0;			matA[10][2]=0;
    matA[10][3]=-(x1-x2);	matA[10][4]=-(y1-y2);	matA[10][5]=-(z1-z2);
	matA[10][6]=x1-x2;		matA[10][7]=y1-y2;		matA[10][8]=z1-z2;
   	
	for( i=0;i<9;i++)
	{
	   for( j=0;j<9;j++)
	   {
		ATA[i][j]=0;
		for( k=0;k<11;k++)
		    ATA[i][j]+=matA[k][i]*matA[k][j];
       }
    }
	for( i=0;i<9;i++) ATA[i][i]+=sigma;
	
	//  VecL[i] is the measured length of this time minus the computed legth of last time  
    VecL[0]=0.5*(l1-l1old);
    VecL[1]=0.5*(l2-l2old);
	VecL[2]=0.5*(l3-l3old);
	VecL[3]=0.5*(l4-l4old);
	VecL[4]=(a1-a1old);
	VecL[5]=(a2-a2old);
	VecL[6]=(a3-a3old);
	VecL[7]=(a4-a4old);
    VecL[8]=0;//(d1*d1-(x1*x1+y1*y1+z1*z1))*0.5;//0;
	VecL[9]=0;//(d1*d1-(x2*x2+y2*y2+z2*z2))*0.5;//0;
	VecL[10]=0;//(d3*d3-((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2)))*0.5;//0;
    
	for(i=0;i<9;i++){
		ATVecL[i]=0;
		for(k=0;k<11;k++)
			ATVecL[i]+=matA[k][i]*VecL[k];
	}

}

//*********************位置を呼ぶ関数(誤差を収束させる)************************//
bool HISpidar4anglequad::Pos_6dof()
{
sigma = 0.0025f;
int step = 0;
if(flag == true){
	X1 = initPos[0].X();
	Y1 = initPos[0].Y();
	Z1 = initPos[0].Z();
	X2 = initPos[2].X();
	Y2 = initPos[2].Y();
	Z2 = initPos[2].Z();
	X  = initPos[4].X();
	Y  = initPos[4].Y();
	Z  = initPos[4].Z();

	//calibration時の糸の長さと角度を初期値として与える
	l1old = (motor[0].pos-initPos[0]).norm();
	l2old = (motor[1].pos-initPos[1]).norm();
	l3old = (motor[2].pos-initPos[2]).norm();
	l4old = (motor[3].pos-initPos[3]).norm();
	a1old = atan2(initPos[0].Z()-motor[0].pos.Z(),initPos[0].X()-motor[0].pos.X());
	a2old = atan2(initPos[1].Z()-motor[1].pos.Z(),initPos[1].X()-motor[1].pos.X());
	a3old = atan2(initPos[2].Z()-motor[2].pos.Z(),initPos[2].X()-motor[2].pos.X());
	a4old = atan2(initPos[3].Z()-motor[3].pos.Z(),initPos[3].X()-motor[3].pos.X());

	//l1oldなどはすべて二乗で扱っている
	l1old = l1old*l1old;
	l2old = l2old*l2old;
	l3old = l3old*l3old;
	l4old = l4old*l4old;

	d1 = initPos[0].norm();
	d2 = initPos[2].norm();
	d3 = (initPos[0]-initPos[2]).norm();

	flag = false;


}
/******エンコーダの値から長さ、角度を計算******/	
		l1 = motor[0].Length();
		l2 = motor[1].Length();
		l3 = motor[2].Length();
		l4 = motor[3].Length();
		a1 = motor[0].Angle();
		a2 = motor[1].Angle();
		a3 = motor[2].Angle();
		a4 = motor[3].Angle();

		l1 = l1*l1;
		l2 = l2*l2;
		l3 = l3*l3;
		l4 = l4*l4;

		do{
			step++;

			SetMat(X,Y,Z,X1,Y1,Z1,X2,Y2,Z2); 
			CholeskyFacToGetSolution(ATA,ATVecL);

			Xnew=X+deltaX ; Ynew=Y+deltaY ; Znew=Z+deltaZ ;
			//float size1,size2;
			//size1=sqrt((X1+deltaX1)*(X1+deltaX1)+(Y1+deltaY1)*(Y1+deltaY1)+(Z1+deltaZ1)*(Z1+deltaZ1));
			//size2=sqrt((X2+deltaX2)*(X2+deltaX2)+(Y2+deltaY2)*(Y2+deltaY2)+(Z2+deltaZ2)*(Z2+deltaZ2));			
		
//ここは単位ベクトル化に直しているの？////////////////

///0.06020797という定数は手元位置の中心から手元位置の周辺位置の距離
			X1new=X1+deltaX1; Y1new=Y1+deltaY1; Z1new=Z1+deltaZ1;
			X2new=X2+deltaX2; Y2new=Y2+deltaY2; Z2new=Z2+deltaZ2;
			float size1,size2;
			size1=sqrt(X1new*X1new+Y1new*Y1new+Z1new*Z1new);
			size2=sqrt(X2new*X2new+Y2new*Y2new+Z2new*Z2new);
			X1new=X1new*d1/size1;Y1new=Y1new*d1/size1;Z1new=Z1new*d1/size1;
			X2new=X2new*d2/size2;Y2new=Y2new*d2/size2;Z2new=Z2new*d2/size2;
///////////////////////////////////////////////////////
			//次のループのためにそれぞれを保存
			X=Xnew; Y=Ynew; Z=Znew;
			X1=X1new; Y1=Y1new; Z1=Z1new;
			X2=X2new; Y2=Y2new; Z2=Z2new;
			

			//Saving the new legth for next iterative step
			l1new = (Xnew+X1new-motor[0].pos.X())*(Xnew+X1new-motor[0].pos.X())+(Ynew+Y1new-motor[0].pos.Y())*(Ynew+Y1new-motor[0].pos.Y())+(Znew+Z1new-motor[0].pos.Z())*(Znew+Z1new-motor[0].pos.Z());
			l2new = (Xnew-X1new-motor[1].pos.X())*(Xnew-X1new-motor[1].pos.X())+(Ynew-Y1new-motor[1].pos.Y())*(Ynew-Y1new-motor[1].pos.Y())+(Znew-Z1new-motor[1].pos.Z())*(Znew-Z1new-motor[1].pos.Z());
			l3new = (Xnew+X2new-motor[2].pos.X())*(Xnew+X2new-motor[2].pos.X())+(Ynew+Y2new-motor[2].pos.Y())*(Ynew+Y2new-motor[2].pos.Y())+(Znew+Z2new-motor[2].pos.Z())*(Znew+Z2new-motor[2].pos.Z());
			l4new = (Xnew-X2new-motor[3].pos.X())*(Xnew-X2new-motor[3].pos.X())+(Ynew-Y2new-motor[3].pos.Y())*(Ynew-Y2new-motor[3].pos.Y())+(Znew-Z2new-motor[3].pos.Z())*(Znew-Z2new-motor[3].pos.Z());
			a1new = atan2(Znew+Z1new-motor[0].pos.Z(),Xnew+X1new-motor[0].pos.X());
			a2new = atan2(Znew-Z1new-motor[1].pos.Z(),Xnew-X1new-motor[1].pos.X());
			a3new = atan2(Znew+Z2new-motor[2].pos.Z(),Xnew+X2new-motor[2].pos.X());
			a4new = atan2(Znew-Z2new-motor[3].pos.Z(),Xnew-X2new-motor[3].pos.X());


			l1old = l1new;
			l2old = l2new;
			l3old = l3new;
			l4old = l4new;
			a1old = a1new;
			a2old = a2new;
			a3old = a3new;
			a4old = a4new;

			//  Precision control by the error for abs of measured length  minus computed length       
			//errorLen=(abs(l1-l1old)+abs(l2-l2old)+abs(l3-l3old)+abs(l4-l4old)+abs(l5-l5old)+abs(l6-l6old)+abs(l7-l7old)+abs(l8-l8old));
			//   time  period must be computed  by getting the elapsed system time ,using function timeGetTime()
		}
		while(step < 2);
		
		pos[0] = Vec3f(Xnew+X1new, Ynew+Y1new, Znew+Z1new);
		pos[1] = Vec3f(Xnew-X1new, Ynew-Y1new, Znew-Z1new);
		pos[2] = Vec3f(Xnew+X2new, Ynew+Y2new, Znew+Z2new);
		pos[3] = Vec3f(Xnew-X2new, Ynew-Y2new, Znew-Z2new);
		pos[4] = Vec3f(Xnew, Ynew, Znew);

//回転のためのローカル座標系の軸を算出

///vec[0]=x軸，vec[1]=y軸，vec[2]=z軸
		vec[0] = Vec3f(-X1new-X2new,-Y1new-Y2new,-Z1new-Z2new).unit();
		vec[1] = Vec3f(-Z2new*Y1new + Z1new*Y2new,-X2new*Z1new + X1new*Z2new,-Y2new*X1new + Y1new*X2new).unit();
		vec[2] = Vec3f(X1new-X2new,Y1new-Y2new,Z1new-Z2new).unit();

		rot.Ex()=vec[0];
		rot.Ey()=vec[1];
		rot.Ez()=vec[2];

		quater.from_matrix(rot);
//Quaternionのベクトル部分を正規化
		v_vec=quater.V().unit();
		rotation = 2*acos(quater.W())*(180/3.141592);

	return true;
}

void HISpidar4anglequad::SetForce(const Vec3f& f, float eff, float cont){
	/*	2次計画法による張力計算
		条件式を簡単にするため，最小張力 < 張力 < 最大張力 ではなく
		0 < t=張力-最小張力 < 最大張力-最小張力
		として，tを求めてから，実際に出力する張力を求める．	*/
	Vec3f v3force = f;							//	目的の力
    Vec3f v3fmin=Vec3f(0,0,0);
    for (int i=0; i<4; i++){
        phi[i] = (motor[i].pos-pos[5]).unit();		//	張力の単位ベクトル		tension direction
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
Vec3f HISpidar4anglequad::GetForce(){
    int i;
	Vec3f f;
    for (i=0;i<4;i++) f=f+tension[i]*phi[i];
    return f;
}

void HISpidar4anglequad::SetMinForce(){
	for(int i=0; i<4; i++) motor[i].Force(motor[i].minForce);
}

void HISpidar4anglequad::InitMat()
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
