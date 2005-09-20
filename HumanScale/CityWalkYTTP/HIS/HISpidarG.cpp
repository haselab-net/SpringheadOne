#include <Base/Matrix.h>
#include "HISpidarG.h"
#include "Device/DVDeviceManager.h"
#include <iostream>

extern "C" int quadpr_(float*,int*,float*,float*,float*,int*,float*,char*,
                        char*,float*,float*,float*,float*,float*,int*,float*
                        ,int,int);

namespace LVRM {
//----------------------------------------------------------------------------
HISpidarG::~HISpidarG(){SetMinForce();}
bool HISpidarG::Init(DVDeviceManager& dev, const Vec3f& frame, float vpn, float lpp, float minF, float maxF){
	return Init(dev);
}
bool HISpidarG::Init(DVDeviceManager& dev){
	//	ドライバの取得
	int i;
	for(i=0; i<8; ++i){
		motor[i].da = (DVDaBase*)dev.VPool().Rent("D/A");
		if (!motor[i].da) break;
		AddRealDeviceDependency(motor[i].da->RealDevice());
	}
	if (i<8) return false;
	for(i=0; i<8; ++i){
		motor[i].counter = (DVCounterBase*)dev.VPool().Rent("Counter");
		if (!motor[i].counter) break;
		AddRealDeviceDependency(motor[i].counter->RealDevice());
	}
	SetMinForce();
	BeforeCalib();
	Calib();
	AfterCalib();
	return true;
}
bool HISpidarG::Calib(){
	//	ポインタを原点(中心)に置いて、キャリブレーションを行う
	for(int i=0; i<8; i++) motor[i].Length(motor[i].pos.size());
	return true;
}

void HISpidarG::Update(){
	float len[8];
	for(int i=0; i<8; i++) len[i]=motor[i].Length();
	Pos7dof();
}
#define TRUE	1
#define	FALSE   0 
		
//#define	PX	10.0f	//9.0f	//18.0f
//#define PY   10.0f	//9.5f	//19.5f
//#define PZ   10.0f	//9.8f	//19.5f
#define	const2	0.25
#define	const4  2
#define const21	0.5

#define PX	10.0f	//9.0f	//18.0f		//	x方向の辺の長さ/2
#define PY	10.0f	//9.5f	//19.5f		//	y方向の辺の長さ/2
#define PZ	10.0f	//9.8f	//19.5f		//	z方向の辺の長さ/2


HISpidarG::HISpidarG(){
	smoothnessWeight = 0.1f;					//	連続性の重み
	torqueWeight = 1.0f;						//	トルクの重み
	graspWeight = 1.0f;							//	把持力の重み
}   

bool HISpidarG::Pos7dof(){
	float	l1,l2,l3,l4,l5,l6,l7,l8;
	double  VL1,VL2,VL3,VL4,VL5,VL6,VL7,VL8;
	double	V0,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10,V11,V12,V13,V14,V15,V16,V17,V18;
	double	const1,const3,const5,const6,const7,const8,const9,const10,const11,const12,const13;
	double  const14,const15,const16,const17,const18,const19,const20;
	double  A4,A2,A0,B4,B2,B0,AA0,AA1,AA2,AA3,AA4,AA5,AA6,BB0,BB1,BB2,BB3,BB4,BB5;	
	double  A,B;
	double  cons01,consa2,consb2;
	Z=0;
	
	d2 = 10.5625f;	//12.25f;

	// Programed by Seahak
	// Adjust 18,April, 2001

	consa2 = PX*PX;	//PX,PY,PZ
	consb2 = PY*PY;	//PX,PY,PZ
    cons01 = d2+consa2+consb2;      //PX,PY,PZ	

	const6 =	PX;					//K11
	const5 =	2*PX;				//K12	
	const20 =	1/const5;			//K13
	const1 =	0.25*const20;		//K14
	const8 =    const5*const5;		//K15
	const7 =	const8*d2;			//K16
	//const7 =	const8*t;			//K16
	const3 =	d2+const5*const6;	//K17
	//const3 =	t+const5*const6;
	const14 =	2*PY;				//K21
	const10 =	4*PY;				//K22
	const17 =   PY*PY;				//K23
	const12 =	6*const17;			//K24
	const9 =	30*const17;			//K25
	const13 =	const10*const17;	//K26
	const18 =   10*const13;			//K27
	const16 =	const17*const17;	//K28
	const11 =	30*const16;			//K29
	const19 =	3*const17*const13;	//K30	
	const15 =	2*const17*const16;	//K31	
	BB5		= 	12*PZ;			
	AA6 	=	2;
	AA5 	=	0;

	l1 = motor[0].Length();
	l2 = motor[1].Length();
	l3 = motor[2].Length();
	l4 = motor[3].Length();
	l5 = motor[4].Length();
	l6 = motor[5].Length();
	l7 = motor[6].Length();
	l8 = motor[7].Length();
	VL1 =	l1*l1;
	VL2 =	l2*l2;
	VL3 =	l3*l3;
	VL4 =	l4*l4;
	VL5 =	l5*l5;
	VL6 =	l6*l6;
	VL7 =	l7*l7;
	VL8 =	l8*l8;
	
	
	X =		const1*(-VL1+VL2-VL3+VL4);
	Y =		const1*(-VL5-VL6+VL7+VL8);
	X1 =	const1*(-VL1+VL2+VL3-VL4);
	Y2 =	const1*(-VL5+VL6+VL7-VL8);
	
	V1 =	const2*(VL1+VL2+VL3+VL4);
	V2 =	const2*(VL1+VL2-VL3-VL4);
	V3 =	const2*(VL5+VL6+VL7+VL8);
	V4 =	const2*(VL5-VL6+VL7-VL8);
	
	//V0 =	X*X+Y*Y+const3;          //PX,PY
	V0 =	X*X+Y*Y+cons01;		     //PX,PY,PZ
	//V5 =	const4*X*X1-const5*Y; 	 //PX,PY
	V5 =	const4*X*X1-(2*PY*Y);     //PX,PY,PZ
	V6 =    const4*Y*Y2-const5*X;
	V7 =	V0-V1;
	V8 =	V0-V3;
	V9 =	const6*(V2-V5)-Y*V7;
	V10 =	const6*(V4-V6)-X*V8;
	
	A4 =	const4*V7+Y*Y;
	//A2 =	V7*V7-const7+const8*X1*X1-const4*Y*V9;					//PX,PY
	A2 =	V7*V7-(4*consb2*d2)+4*consb2*X1*X1-const4*Y*V9;			//PX,PY,PZ
	//A2 =	V7*V7-(4*consb2*t)+4*consb2*X1*X1-const4*Y*V9;
	A0 =	V9*V9;
	
	B4 =	const4*V8+X*X;
	B2 =	V8*V8-const7+const8*Y2*Y2-const4*X*V10;					//PX,PY and PX,PY,PZ same
	B0 =	V10*V10;
	
	V11 =	B4+A4;
	V12 =	B4-A4;
	V13 =	B2+A2;
	V14 =	B2-A2;
	V15 =	B0+A0;
	V16 =	B0-A0;
	
	AA4 =	const9+V11;
	AA3 =	const10*V12;
	AA2 =	const11+const12*V11+V13;
	AA1 =	const13*V12+const14*V14;
	AA0 =	const15+const16*V11+const17*V13+V15;
	
	BB4 =	V12;
	BB3 =	const18+const10*V11;
	BB2 =	const12*V12+V14;
	BB1 =	const19+const13*V11+const14*V13;
	BB0 =	const16*V12+const17*V14+V16;
	
	A = AA6/BB5;				
	B = (AA5-(A*BB4))/BB5;		
	AA4= AA4-A*BB3-B*BB4;	
	AA3 = AA3-A*BB2-B*BB3;	
	AA2 = AA2-A*BB1-B*BB2;	
	AA1 = AA1-A*BB0-B*BB1;	
	AA0 = AA0-B*BB0;			
	
	A = BB5/AA4;			
	B = (BB4-A*AA3)/AA4;	
	
	BB3 = BB3-A*AA2-B*AA3;	
	BB2 = BB2-A*AA1-B*AA2;	
	BB1 = BB1-A*AA0-B*AA1;	
	BB0 = BB0-B*AA0;		
	
	A = AA4/BB3;			
	B = (AA3-A*BB2)/BB3;	
	
	AA2=AA2-A*BB1-B*BB2;	
	AA1=AA1-A*BB0-B*BB1;	
	AA0=AA0-B*BB0;			
	
	A = BB3/AA2;			
	B = (BB2-A*AA1)/AA2;	
	BB1=BB1-A*AA0-B*AA1;	
	BB0=BB0-B*AA0;			
	
	Z = -BB0/BB1;			
	
	V17 =	Z-PZ;
	V18 =	Z+PZ;
	
	Y1 =	const20*(V7+V17*V17);
	//Y1 =	constb20*(V7+V17*V17);	PX,PY,PZ?
	X2 =	const20*(V8+V18*V18);
	
	Z1 =	(const21*(V2-V5)-Y*Y1)/V17;
	Z2 =	(const21*(V4-V6)-X*X2)/V18;
	
	//	TRACE("%3.3f %3.3f %3.3f %3.3f %3.3f %3.3f %3.3f %3.3f %3.3f\n",X,Y,Z,X1,Y1,Z1,X2,Y2,Z2);
	return true;
}

void HISpidarG::SetMinForce(){
	for(int i=0; i<8; ++i){
		motor[i].Force(motor[i].minForce);
	}
}

Vec3f HISpidarG::GetPos(){
	return Vec3f(X,Y,Z);
}
Matrix3f HISpidarG::GetRot(){
	Vec3f bar1(X1-X, Y1-Y, Z1-Z);
	Vec3f bar2(X2-X, Y2-Y, Z2-Z);
	Matrix3f rv;
	rv.Ez() = (bar1^bar2).unit();
	rv.Ex() = (bar1.unit()+bar2.unit()).unit();
	rv.Ey() = rv.Ez() ^ rv.Ex();
	return rv;
}
float HISpidarG::GetGap(){
	Vec3f d(X1-X2, Y1-Y2, Z1-Z2);
	return d.norm();
}

Vec3f HISpidarG::GetForce(){
	return FF.sub_vector(0,Vec3f());
}
Vec3f HISpidarG::GetTorque(){
	return FF.sub_vector(3,Vec3f());
}
float HISpidarG::GetGrasp(){
	return FF[6];
}
void HISpidarG::SetForce(const Vec3f& f, float w){
	F.sub_vector(0, f) = f;
	smoothnessWeight = w;
}
void HISpidarG::SetTorque(const Vec3f& t, float w){
	F.sub_vector(3, t) = t;
	torqueWeight = w;
}
void HISpidarG::SetGrasp(float g, float w){
	F[6] = g;
	graspWeight = w;
}
void HISpidarG::Force7dof(){
	float MA[8][7];
	float X_01 = X+X1;			//X+X1
	float Y_01 = Y+Y1;			//Y+Y1
	float Z_01 = Z+Z1;			//Z+Z1
	float X_10 = X-X1;			//X-X1
	float Y_10 = Y-Y1;			//Y-Y1
	float Z_10 = Z-Z1;			//Z-Z1

	float X_02 = X+X2;			//X+X2
	float Y_02 = Y+Y2;			//Y+Y2
	float Z_02 = Z+Z2;			//Z+Z2
	float X_20 = X-X2;			//X-X2
	float Y_20 = Y-Y2;			//Y-Y2
	float Z_20 = Z-Z2;			//Z-Z2

	float PX01 = PX - (X_01);
	float PY01 = PY - (Y_01);
	float PZ01 = PZ - (Z_01);
	float mPX01 = -PX - (X_01);
    float PX10 = PX - (X_10);
    float mPY10 = -PY - (Y_10); 
    float PZ10 = PZ - (Z_10);
	float mPX10 = -PX - (X_10);

	float PX02 = PX - (X_02);
	float PY02 = PY - (Y_02);
	float mPZ02 = -PZ - (Z_02);
	float mPX20 = -PX - (X_20);
	float PY20 = PY - (Y_20);
	float mPZ20 = -PZ - (Z_20);
	float mPY02 = -PY - (Y_02);
	float mPY20 = -PY - (Y_20);

  	float un[8]; //各糸の力ベクトルの大きさを設定

	un[0] = sqrt(PX01*PX01+PY01*PY01+PZ01*PZ01);
	un[1] = sqrt(mPX01*mPX01+PY01*PY01+PZ01*PZ01);
	un[2] = sqrt(PX10*PX10+mPY10*mPY10+PZ10*PZ10);
	un[3] = sqrt(mPX10*mPX10+mPY10*mPY10+PZ10*PZ10);
	un[4] = sqrt(PX02*PX02+PY02*PY02+mPZ02*mPZ02);
	un[5] = sqrt(mPX20*mPX20+PY20*PY20+mPZ20*mPZ20);
	un[6] = sqrt(PX02*PX02+mPY02*mPY02+mPZ02*mPZ02);
	un[7] = sqrt(mPX20*mPX20+mPY20*mPY20+mPZ20*mPZ20);
   
	float na,nb,nc,nn,nt,ni,nj,nk;
	// Coodinate of Grip position (Grip cordinate)
	na = Y2*Z1 -(Y1*Z2);	//-Y2*Z1 + Y1*Z2;	//法線ベクトルの　X　成分
	nb = X1*Z2 -(Z1*X2);	//-X1*Z2 + Z1*X2;	//法線ベクトルの　Y　成分
	nc = X2*Y1 -(X1*Y2);	//-X2*Y1 + X1*Y2;	//法線ベクトルの　Z　成分

	nn = na*na + nb*nb + nc*nc;				//法線ベクトルの大きさを
    nt = sqrt(nn);							//求めてntとする
	ni = na/nt;								//単位ベクトルのX成分
	nj = nb/nt;								//単位ベクトルのY成分
	nk = nc/nt;								//単位ベクトルのZ成分

	//各糸の姿勢ベクトルの単位ベクトルを求める

	MA[0][0] =  PX01/un[0];
	MA[0][1] =  PY01/un[0];
	MA[0][2] =  PZ01/un[0];
	MA[0][3] =  MA[0][2]*Y1-(MA[0][1]*Z1);			           
	MA[0][4] =  MA[0][0]*Z1-(MA[0][2]*X1);					
	MA[0][5] =  MA[0][1]*X1-(MA[0][0]*Y1);				
	MA[0][6] =  (MA[0][3]*ni + MA[0][4]*nj + MA[0][5]*nk);	

	MA[1][0] =  mPX01/un[1];
	MA[1][1] =  PY01/un[1];
	MA[1][2] =  PZ01/un[1];
	MA[1][3] =	MA[1][2]*Y1-(MA[1][1]*Z1);		
	MA[1][4] =	MA[1][0]*Z1-(MA[1][2]*X1);		
	MA[1][5] =	MA[1][1]*X1-(MA[1][0]*Y1);		
	MA[1][6] =  (MA[1][3]*ni+ MA[1][4]*nj + MA[1][5]*nk);

	MA[2][0] =  PX10/un[2];
	MA[2][1] =  mPY10/un[2];
	MA[2][2] =  PZ10/un[2];
	MA[2][3] =  Z1*MA[2][1]-(Y1*MA[2][2]);         
	MA[2][4] =  X1*MA[2][2]-(Z1*MA[2][0]);        
	MA[2][5] =  Y1*MA[2][0]-(X1*MA[2][1]);        
	MA[2][6] =  (MA[2][3]*ni + MA[2][4]*nj + MA[2][5]*nk);	

	MA[3][0] =  mPX10/un[3];
	MA[3][1] =  mPY10/un[3];
	MA[3][2] =  PZ10/un[3];
	MA[3][3] =  Z1*MA[3][1]-(Y1*MA[3][2]);      
	MA[3][4] =  X1*MA[3][2]-(Z1*MA[3][0]);     
	MA[3][5] =  Y1*MA[3][0]-(X1*MA[3][1]);      
	MA[3][6] =  (MA[3][3]*ni + MA[3][4]*nj + MA[3][5]*nk);	

	MA[4][0] =  PX02/un[4];
	MA[4][1] =  PY02/un[4];
	MA[4][2] =  mPZ02/un[4];
	MA[4][3] =  Y2*MA[4][2]-(Z2*MA[4][1]);         
	MA[4][4] =  Z2*MA[4][0]-(X2*MA[4][2]);        
	MA[4][5] =  X2*MA[4][1]-(Y2*MA[4][0]);        
	MA[4][6] =  -(MA[4][3]*ni + MA[4][4]*nj + MA[4][5]*nk);	

	MA[5][0] =  mPX20/un[5];
	MA[5][1] =  PY20/un[5];
	MA[5][2] =  mPZ20/un[5];
	MA[5][3] =  Z2*MA[5][1]-(Y2*MA[5][2]);             
	MA[5][4] =  X2*MA[5][2]-(Z2*MA[5][0]);              
	MA[5][5] =  Y2*MA[5][0]-(X2*MA[5][1]);             
	MA[5][6] =  -(MA[5][3]*ni + MA[5][4]*nj + MA[5][5]*nk);	

	MA[6][0] =  PX02/un[6];
	MA[6][1] =  mPY02/un[6];
	MA[6][2] =  mPZ02/un[6];
	MA[6][3] =  Y2*MA[6][2]-(Z2*MA[6][1]);           
	MA[6][4] =  Z2*MA[6][0]-(X2*MA[6][2]);          
	MA[6][5] =  X2*MA[6][1]-(Y2*MA[6][0]);           
	MA[6][6] = 	-(MA[6][3]*ni + MA[6][4]*nj + MA[6][5]*nk);

	MA[7][0] =  mPX20/un[7];
	MA[7][1] =  mPY20/un[7];
	MA[7][2] =  mPZ20/un[7];
	MA[7][3] =  Z2*MA[7][1]-(Y2*MA[7][2]);       
	MA[7][4] =  X2*MA[7][2]-(Z2*MA[7][0]);        
	MA[7][5] =  Y2*MA[7][0]-(X2*MA[7][1]);       
	MA[7][6] =  -(MA[7][3]*ni + MA[7][4]*nj + MA[7][5]*nk);	

	//Set coeffcients of quadratic programming 

	float qt[64],qr[64],qg[64];
	// Set coefficient of translation parameter
	qt[0] = MA[0][0]*MA[0][0] + MA[0][1]*MA[0][1] +MA[0][2]*MA[0][2]+smoothnessWeight;
	qt[9] = MA[1][0]*MA[1][0] + MA[1][1]*MA[1][1] +MA[1][2]*MA[1][2]+smoothnessWeight;
	qt[18] = MA[2][0]*MA[2][0] + MA[2][1]*MA[2][1] +MA[2][2]*MA[2][2]+smoothnessWeight;
	qt[27] = MA[3][0]*MA[3][0] + MA[3][1]*MA[3][1] +MA[3][2]*MA[3][2]+smoothnessWeight;
	qt[36] = MA[4][0]*MA[4][0] + MA[4][1]*MA[4][1] +MA[4][2]*MA[4][2]+smoothnessWeight;
	qt[45] = MA[5][0]*MA[5][0] + MA[5][1]*MA[5][1] +MA[5][2]*MA[5][2]+smoothnessWeight;
	qt[54] = MA[6][0]*MA[6][0] + MA[6][1]*MA[6][1] +MA[6][2]*MA[6][2]+smoothnessWeight;
	qt[63] = MA[7][0]*MA[7][0] + MA[7][1]*MA[7][1] +MA[7][2]*MA[7][2]+smoothnessWeight;
	
	// Set coefficient of rotation parameter
   	qr[0] = MA[0][3]*MA[0][3] + MA[0][4]*MA[0][4] +MA[0][5]*MA[0][5];
	qr[9] = MA[1][3]*MA[1][3] + MA[1][4]*MA[1][4] +MA[1][5]*MA[1][5];
	qr[18] = MA[2][3]*MA[2][3] + MA[2][4]*MA[2][4] +MA[2][5]*MA[2][5];
	qr[27] = MA[3][3]*MA[3][3] + MA[3][4]*MA[3][4] +MA[3][5]*MA[3][5];
	qr[36] = MA[4][3]*MA[4][3] + MA[4][4]*MA[4][4] +MA[4][5]*MA[4][5];
	qr[45] = MA[5][3]*MA[5][3] + MA[5][4]*MA[5][4] +MA[5][5]*MA[5][5];
	qr[54] = MA[6][3]*MA[6][3] + MA[6][4]*MA[6][4] +MA[6][5]*MA[6][5];
	qr[63] = MA[7][3]*MA[7][3] + MA[7][4]*MA[7][4] +MA[7][5]*MA[7][5];
	
	// Set coefficient of grasp parameter
	qg[0]  = MA[0][6]*MA[0][6];
	qg[9]  = MA[1][6]*MA[1][6];
	qg[18] = MA[2][6]*MA[2][6];
	qg[27] = MA[3][6]*MA[3][6];
	qg[36] = MA[4][6]*MA[4][6];
	qg[45] = MA[5][6]*MA[5][6];
	qg[54] = MA[6][6]*MA[6][6];
	qg[63] = MA[7][6]*MA[7][6];
	
	qt[1]  = qt[8]  = MA[0][0]*MA[1][0] + MA[0][1]*MA[1][1] +MA[0][2]*MA[1][2];
	qt[2]  = qt[16] = MA[0][0]*MA[2][0] + MA[0][1]*MA[2][1] +MA[0][2]*MA[2][2];
	qt[3]  = qt[24] = MA[0][0]*MA[3][0] + MA[0][1]*MA[3][1] +MA[0][2]*MA[3][2];
	qt[10] = qt[17] = MA[1][0]*MA[2][0] + MA[1][1]*MA[2][1] +MA[1][2]*MA[2][2];
	qt[4]  = qt[32] = MA[0][0]*MA[4][0] + MA[0][1]*MA[4][1] +MA[0][2]*MA[4][2];
	qt[11] = qt[25] = MA[1][0]*MA[3][0] + MA[1][1]*MA[3][1] +MA[1][2]*MA[3][2];
	qt[5]  = qt[40] = MA[0][0]*MA[5][0] + MA[0][1]*MA[5][1] +MA[0][2]*MA[5][2];
	qt[12] = qt[33] = MA[1][0]*MA[4][0] + MA[1][1]*MA[4][1] +MA[1][2]*MA[4][2];
	qt[26] = qt[19] = MA[2][0]*MA[3][0] + MA[2][1]*MA[3][1] +MA[2][2]*MA[3][2];
	qt[6]  = qt[48] = MA[0][0]*MA[6][0] + MA[0][1]*MA[6][1] +MA[0][2]*MA[6][2];
	qt[13] = qt[41] = MA[1][0]*MA[5][0] + MA[1][1]*MA[5][1] +MA[1][2]*MA[5][2];
	qt[20] = qt[34] = MA[2][0]*MA[4][0] + MA[2][1]*MA[4][1] +MA[2][2]*MA[4][2];
	qt[7]  = qt[56] = MA[0][0]*MA[7][0] + MA[0][1]*MA[7][1] +MA[0][2]*MA[7][2];
	qt[14] = qt[49] = MA[1][0]*MA[6][0] + MA[1][1]*MA[6][1] +MA[1][2]*MA[6][2];
	qt[21] = qt[42] = MA[2][0]*MA[5][0] + MA[2][1]*MA[5][1] +MA[2][2]*MA[5][2];
	qt[28] = qt[35] = MA[3][0]*MA[4][0] + MA[3][1]*MA[4][1] +MA[3][2]*MA[4][2];  
	qt[15] = qt[57] = MA[1][0]*MA[7][0] + MA[1][1]*MA[7][1] +MA[1][2]*MA[7][2];
	qt[22] = qt[50] = MA[2][0]*MA[6][0] + MA[2][1]*MA[6][1] +MA[2][2]*MA[6][2];
	qt[29] = qt[43] = MA[5][0]*MA[3][0] + MA[5][1]*MA[3][1] +MA[5][2]*MA[3][2];
	qt[23] = qt[58] = MA[2][0]*MA[7][0] + MA[2][1]*MA[7][1] +MA[2][2]*MA[7][2];  
	qt[30] = qt[51] = MA[6][0]*MA[3][0] + MA[6][1]*MA[3][1] +MA[6][2]*MA[3][2];
	qt[37] = qt[44] = MA[4][0]*MA[5][0] + MA[4][1]*MA[5][1] +MA[4][2]*MA[5][2];
	qt[31] = qt[59] = MA[7][0]*MA[3][0] + MA[7][1]*MA[3][1] +MA[7][2]*MA[3][2];
	qt[38] = qt[52] = MA[4][0]*MA[6][0] + MA[4][1]*MA[6][1] +MA[4][2]*MA[6][2];
	qt[39] = qt[60] = MA[4][0]*MA[7][0] + MA[4][1]*MA[7][1] +MA[4][2]*MA[7][2];
	qt[53] = qt[46] = MA[6][0]*MA[5][0] + MA[6][1]*MA[5][1] +MA[6][2]*MA[5][2];
	qt[47] = qt[61] = MA[5][0]*MA[7][0] + MA[5][1]*MA[7][1] +MA[5][2]*MA[7][2];
	qt[55] = qt[62] = MA[6][0]*MA[7][0] + MA[6][1]*MA[7][1] +MA[6][2]*MA[7][2];
	
	qr[1]  = qr[8]  = MA[0][3]*MA[1][3] + MA[0][4]*MA[1][4] +MA[0][5]*MA[1][5];
	qr[2]  = qr[16] = MA[0][3]*MA[2][3] + MA[0][4]*MA[2][4] +MA[0][5]*MA[2][5];
	qr[3]  = qr[24] = MA[0][3]*MA[3][3] + MA[0][4]*MA[3][4] +MA[0][5]*MA[3][5];
	qr[10] = qr[17] = MA[1][3]*MA[2][3] + MA[1][4]*MA[2][4] +MA[1][5]*MA[2][5];
	qr[4]  = qr[32] = MA[0][3]*MA[4][3] + MA[0][4]*MA[4][4] +MA[0][5]*MA[4][5];
	qr[11] = qr[25] = MA[1][3]*MA[3][3] + MA[1][4]*MA[3][4] +MA[1][5]*MA[3][5];
	qr[5]  = qr[40] = MA[0][3]*MA[5][3] + MA[0][4]*MA[5][4] +MA[0][5]*MA[5][5];
	qr[12] = qr[33] = MA[1][3]*MA[4][3] + MA[1][4]*MA[4][4] +MA[1][5]*MA[4][5];
	qr[26] = qr[19] = MA[2][3]*MA[3][3] + MA[2][4]*MA[3][4] +MA[2][5]*MA[3][5];
	qr[6]  = qr[48] = MA[0][3]*MA[6][3] + MA[0][4]*MA[6][4] +MA[0][5]*MA[6][5];
	qr[13] = qr[41] = MA[1][3]*MA[5][3] + MA[1][4]*MA[5][4] +MA[1][5]*MA[5][5];
	qr[20] = qr[34] = MA[2][3]*MA[4][3] + MA[2][4]*MA[4][4] +MA[2][5]*MA[4][5];
	qr[7]  = qr[56] = MA[0][3]*MA[7][3] + MA[0][4]*MA[7][4] +MA[0][5]*MA[7][5];
	qr[14] = qr[49] = MA[1][3]*MA[6][3] + MA[1][4]*MA[6][4] +MA[1][5]*MA[6][5];
	qr[21] = qr[42] = MA[2][3]*MA[5][3] + MA[2][4]*MA[5][4] +MA[2][5]*MA[5][5];
	qr[28] = qr[35] = MA[3][3]*MA[4][3] + MA[3][4]*MA[4][4] +MA[3][5]*MA[4][5];  
	qr[15] = qr[57] = MA[1][3]*MA[7][3] + MA[1][4]*MA[7][4] +MA[1][5]*MA[7][5];
	qr[22] = qr[50] = MA[2][3]*MA[6][3] + MA[2][4]*MA[6][4] +MA[2][5]*MA[6][5];
	qr[29] = qr[43] = MA[5][3]*MA[3][3] + MA[5][4]*MA[3][4] +MA[5][5]*MA[3][5];
	qr[23] = qr[58] = MA[2][3]*MA[7][3] + MA[2][4]*MA[7][4] +MA[2][5]*MA[7][5];  
	qr[30] = qr[51] = MA[6][3]*MA[3][3] + MA[6][4]*MA[3][4] +MA[6][5]*MA[3][5];
	qr[37] = qr[44] = MA[4][3]*MA[5][3] + MA[4][4]*MA[5][4] +MA[4][5]*MA[5][5];
	qr[31] = qr[59] = MA[7][3]*MA[3][3] + MA[7][4]*MA[3][4] +MA[7][5]*MA[3][5];
	qr[38] = qr[52] = MA[4][3]*MA[6][3] + MA[4][4]*MA[6][4] +MA[4][5]*MA[6][5];
	qr[39] = qr[60] = MA[4][3]*MA[7][3] + MA[4][4]*MA[7][4] +MA[4][5]*MA[7][5];
	qr[53] = qr[46] = MA[6][3]*MA[5][3] + MA[6][4]*MA[5][4] +MA[6][5]*MA[5][5];
	qr[47] = qr[61] = MA[5][3]*MA[7][3] + MA[5][4]*MA[7][4] +MA[5][5]*MA[7][5];
	qr[55] = qr[62] = MA[6][3]*MA[7][3] + MA[6][4]*MA[7][4] +MA[6][5]*MA[7][5];

	qg[1]  = qg[8]  = MA[0][6]*MA[1][6];
	qg[2]  = qg[16] = MA[0][6]*MA[2][6];
	qg[3]  = qg[24] = MA[0][6]*MA[3][6];
	qg[10] = qg[17] = MA[1][6]*MA[2][6];
	qg[4]  = qg[32] = MA[0][6]*MA[4][6];
	qg[11] = qg[25] = MA[1][6]*MA[3][6];
	qg[5]  = qg[40] = MA[0][6]*MA[5][6];
	qg[12] = qg[33] = MA[1][6]*MA[4][6];
	qg[26] = qg[19] = MA[2][6]*MA[3][6];
	qg[6]  = qg[48] = MA[0][6]*MA[6][6];
	qg[13] = qg[41] = MA[1][6]*MA[5][6];
	qg[20] = qg[34] = MA[2][6]*MA[4][6];
	qg[7]  = qg[56] = MA[0][6]*MA[7][6];
	qg[14] = qg[49] = MA[1][6]*MA[6][6];
	qg[21] = qg[42] = MA[2][6]*MA[5][6];
	qg[28] = qg[35] = MA[3][6]*MA[4][6];  
	qg[15] = qg[57] = MA[1][6]*MA[7][6];
	qg[22] = qg[50] = MA[2][6]*MA[6][6];
	qg[29] = qg[43] = MA[5][6]*MA[3][6];
	qg[23] = qg[58] = MA[2][6]*MA[7][6];  
	qg[30] = qg[51] = MA[6][6]*MA[3][6];
	qg[37] = qg[44] = MA[4][6]*MA[5][6];
	qg[31] = qg[59] = MA[7][6]*MA[3][6];
	qg[38] = qg[52] = MA[4][6]*MA[6][6];
	qg[39] = qg[60] = MA[4][6]*MA[7][6];
	qg[53] = qg[46] = MA[6][6]*MA[5][6];
	qg[47] = qg[61] = MA[5][6]*MA[7][6];
	qg[55] = qg[62] = MA[6][6]*MA[7][6];

	for (int num =0; num <64; num++){
		qt[num] = qt[num] + qr[num] + qg[num];
	}

	// Define cost variables
	float cost[8], cost1[8], cost2[8];
	
    // Set coefficient of translation parameter
    cost[0] = -2*(MA[0][0]*F[0] + MA[0][1]*F[1] +MA[0][2]*F[2]);
	cost[1] = -2*(MA[1][0]*F[0] + MA[1][1]*F[1] +MA[1][2]*F[2]);
	cost[2] = -2*(MA[2][0]*F[0] + MA[2][1]*F[1] +MA[2][2]*F[2]);
	cost[3] = -2*(MA[3][0]*F[0] + MA[3][1]*F[1] +MA[3][2]*F[2]);
	cost[4] = -2*(MA[4][0]*F[0] + MA[4][1]*F[1] +MA[4][2]*F[2]);
	cost[5] = -2*(MA[5][0]*F[0] + MA[5][1]*F[1] +MA[5][2]*F[2]);
	cost[6] = -2*(MA[6][0]*F[0] + MA[6][1]*F[1] +MA[6][2]*F[2]);
	cost[7] = -2*(MA[7][0]*F[0] + MA[7][1]*F[1] +MA[7][2]*F[2]);
	
	// Set coefficient of rotation parameter
	cost1[0] = -2*(MA[0][3]*F[3] + MA[0][4]*F[4] +MA[0][5]*F[5]);
	cost1[1] = -2*(MA[1][3]*F[3] + MA[1][4]*F[4] +MA[1][5]*F[5]);
	cost1[2] = -2*(MA[2][3]*F[3] + MA[2][4]*F[4] +MA[2][5]*F[5]);
	cost1[3] = -2*(MA[3][3]*F[3] + MA[3][4]*F[4] +MA[3][5]*F[5]);
	cost1[4] = -2*(MA[4][3]*F[3] + MA[4][4]*F[4] +MA[4][5]*F[5]);
	cost1[5] = -2*(MA[5][3]*F[3] + MA[5][4]*F[4] +MA[5][5]*F[5]);
	cost1[6] = -2*(MA[6][3]*F[3] + MA[6][4]*F[4] +MA[6][5]*F[5]);
	cost1[7] = -2*(MA[7][3]*F[3] + MA[7][4]*F[4] +MA[7][5]*F[5]);

	// Set coefficient of grasp parameter
	cost2[0] = -2*(MA[0][6]*F[6]);
	cost2[1] = -2*(MA[1][6]*F[6]);
	cost2[2] = -2*(MA[2][6]*F[6]);
	cost2[3] = -2*(MA[3][6]*F[6]);
	cost2[4] = -2*(MA[4][6]*F[6]);
	cost2[5] = -2*(MA[5][6]*F[6]);
	cost2[6] = -2*(MA[6][6]*F[6]);
	cost2[7] = -2*(MA[7][6]*F[6]);

	for (int num = 0; num <8; num++){
		cost[num] = cost[num] + cost1[num] + cost2[num];
	}

	// Left side of constraint
	float ca[64];
	for (int i = 1; i <63; i++){
		ca[i] = 0;
	}
	ca[0] = ca[9] = ca[18] = ca[27] = ca[36] = ca[45] = ca[54] = ca[63] = 1;

    // Right side of constraint
	float rhs[8];
	for (int j = 0; j<8; j++) rhs[j] = motor[j].Length();

	//Sign of condition
    int kt[8] = {1,1,1,1,1,1,1,1};

	//Parameters for quadratic programming funtion
	int input[14] = {8,8,8,8,0,720,1000,1,0,0,0,0,0,80};

	//Set tolerances
	float tol[2] = {0,0};

	//Character expression
	char title[] = "Quadratic problem";
	char pFile[] = "quad";

	//For output storage
	float obj[1],Tq[8],rc[8],dual[8],slk[8];
	int iout[2];
	float ws[720];
	int title_len = 0, pfile_len = 0;
    
    quadpr_(ca, kt, rhs, cost, qt, input, tol, title, pFile,
	 obj, Tq, rc, dual, slk, iout, ws, title_len, pfile_len);

	for(int i=0; i<8; i++){
		tension[i] = Tq[i];
	}

	// Check Tension
	for(int i = 0; i <8; i++){
		FF[i] = 0;
	}

	for(int i = 0; i < 7 ; i++){
		for (int j = 0; j < 8; j++){
			FF[i] += tension[j]*MA[j][i];
		}
	}

	// Display force
	DPF("%3.3f %3.3f %3.3f %3.3f %3.3f %3.3f %3.3f\n",FF[0],FF[1],FF[2],FF[3],FF[4],FF[5],FF[6]);
}

}	//	namespace LVRM
