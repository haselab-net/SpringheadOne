//FOR VRML
#include "GeoNode.h"
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>
#include <GL/glu.h>		
#include <crtdbg.h>
#include <strstream>
#include <io.h>
#include <penalty.h>
#include <fstream>
#include <math.h>
#include <string.h>


////////////////////////////////////////////////////////
//MPI
#include "mpi/mpi.h"
MPI_Datatype stMove_Data;
int GRP_IDX[6] ={3, 3, 3, 4, 4, 2};
//MPI
/////////////////////////////////////////////////////////
//MY HEAD FILE
/////////////////////////////////////////////////////////
#include "GLView.h"
#include "ball.h"

//MY ODBC
CBall BBall;
/////////////////////////////////////////////////////////
// check variables
static int full_mode =0;
static int chk_end =0;
static int chk_interp = 0;
static int animating = 0;
//////////////////////////////
//lighting & color definition
static GLfloat light0pos[] = {0.0, 200.0, 200.0, 1.0};
static GLfloat light1pos[] = {200.0, 200.0, 0.0, 1.0};
////////////////////////////////////////////////////////////////
const GLfloat deltaRot = 5.0f;
const GLfloat nRange = 25.0f; 
const GLfloat nHeight = 45.0f; //45.0
const GLfloat delta_Pad = 0.5f;
const GLfloat convert_inch = 39.37;
//const GLfloat convert_inch = 39.37 * 3;	// 工作室SPIDAR用スケール調整
const GLfloat GnHeight = 2.5f; //
//JEONG
DWORD dwLastTime;
DWORD dwTime;

GLfloat green[] = {0.25, 1.0, 0.25, 1.0};
GLfloat red[] = {1.0, 0.25, 0.25, 1.0};
GLfloat blue[] = {0.25, 0.25, 1.0, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gray[] = {0.5, 0.5, 0.5, 1.0};
GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
GLfloat yellow[] = {1.0, 1.0, 0.25, 1.0};
GLfloat headcolor[] = {1.0, 0.8, 0.6, 1.0};
GLfloat wallcolor[] = {0.64, 0.47, 0.36, 1.0};
GLfloat shadow[] = { 0.0, 0.0, 0.0, 0.5   };
GLfloat bodycolor[] = {1.0, 0.5, 0.5, 1.0};
/////////////////////////////////////////////////////
//ball parabola /collision detect variable
static MyVector avg_epos;
static MyVector paddle_v;
static MyVector reflex_v;
static MyVector off_v;
static double t=0.0;
static double dt=0.0;
static double ft=0.0;
//////////////////////////////////////////////////////
//SHADOW
static double ShadowMat[4][4];
/////////////////////////////////////////////////////////
using namespace std;
const float EYEOFFSET=0.8;

//SPIDAR-H
////////////////////////////
UINT SpidarUpdateTimer;
static float s_rad = 1;
////////////////////////////
		
//SOUND
//2003.10.13
void TGLView::Play_wave(char *StrFile)
{
	char openfile[80];
	
	strcpy(openfile, "open ");
	strcat(openfile, StrFile);
	strcat(openfile, " alias WAVEFILE");
// At frist all device have to close
   mciSendString("close all", NULL, 0,0);
  if (mciSendString(openfile, NULL,0,NULL)!=0)
  {
	  ::MessageBox(NULL,"Open wave file Fail!","SOUND CHECK",MB_OK);
  }
  else
  {
		mciSendString("play WAVEFILE", NULL, 0,0);
		mciSendString("set WAVEFILE time format tmsf wait", NULL, 0,0);
   // mciSendString("stop WAVEFILE", NULL, 0,0);
   }
}

bool TGLView::LoadSettings()
{
	char* cname = getenv("COMPUTERNAME");
	char* p = cname;
	while(*p && !isdigit(*p)) p++;
	
	cpNum = -1;
		// 暫定コード
	cout << "Hostname = " << cname << endl;

	if (strlen(p)) cpNum = atoi(p);

	cout << "<cpNum> : "<< cpNum << endl;

	//MPI031021
	motionDatIndex = 0;
	bFirstReceive = true;

	//MPI031021
	if(myrank == 0){
		diffMotionDatFrm = 0;
	}

	if (cpNum == -1){
		return false;
	}

	//MPI031021
	// VPC00～VPC03はフレーム遅れなし
	if(cpNum < 4){
		diffMotionDatFrm = 0;
	}

	std::map<int,int> cpProj;
	cpProj[0]=0;
	cpProj[1]=1;
	cpProj[2]=2;
	cpProj[3]=3;
	cpProj[4]=0;
	cpProj[5]=1;
	cpProj[6]=2;
	cpProj[7]=3;

	cpProj[11]=8;
	cpProj[20]=9;
	cpProj[21]=10;
	cpProj[15]=11;
	cpProj[10]=12;
	cpProj[14]=13;
	cpProj[13]=14;
	cpProj[17]=15;
	
	cpProj[12]=16;
	cpProj[30]=17;
	cpProj[31]=18;
	cpProj[16]=19;
	cpProj[22]=9;
	cpProj[23]=10;
	cpProj[32]=17;
	cpProj[33]=18;

	int projNum = cpProj[cpNum];
	char filename[1024];
	std::ostrstream ostrfn(filename, sizeof(filename));
	//ostrfn << "..\\CameraParameter\\" << "sf" << projNum << '\0';
	ostrfn << "sf" << projNum << '\0';

	FILE *fp;
	int number;
	int set = 0;
	char ch[128];
   
    if((fp = fopen(filename, "r")) == NULL){
		std::cout << "Can not read or find file sf1" << std::endl;
 		return false;
   }
    fgets(ch,sizeof(ch),fp);

	std::istrstream cStrStrm(ch, sizeof(ch));

//	cout << ch <<endl;
	cStrStrm >> number >> setting[0] >> setting[1] >> setting[2] >> setting[3]
		>> setting[4] >> setting[5] >> setting[6];
	cout << number << endl;
        	for(int j=0;j<6;j++)
	cout << setting[j] << endl;
    fclose(fp);
//	stVision = StereoV(cpNum);
	return true;
}

TGLView::TGLView(int n):nowMode(n){

	bDraw = true;

	stepHz = 0;
	printStepHz = NULL;

	switch (nowMode){
	case 0:
		bSender = false;
		break;
	case 1:
		bSender = true;
		break;
	case 2:
		bSender = false;
		break;
	}

	// iniファイルからデータを読み込む
	char temp_str[100];
	char ini_file[100];
	GetCurrentDirectory( 100,ini_file );
	strcat( ini_file,"\\setting.ini" );

	GetPrivateProfileString("AVATOR","radius","0.5",temp_str,100,ini_file); // アパターの半径
	float ini_radius = atof( temp_str );
	GetPrivateProfileString("AVATOR","initialx","0",temp_str,100,ini_file); // x,y,z初期位置
	float ini_initialx = atof( temp_str );
	GetPrivateProfileString("AVATOR","initialy","0",temp_str,100,ini_file);
	float ini_initialy = atof( temp_str );
	GetPrivateProfileString("AVATOR","initialz","0",temp_str,100,ini_file);
	float ini_initialz = atof( temp_str );
	GetPrivateProfileString("AVATOR","eyeheight","0",temp_str,100,ini_file); //アバタ－の中心から目までの高さ
	float ini_eyeheight = atof( temp_str );
	afEye = Affinef(0,ini_eyeheight,0);
	GetPrivateProfileString("VIEWMODE","initialmode","0",temp_str,100,ini_file); // 初期の視点(0:アパタ 1:上から視点 2:鳥瞰視点)
	viewMode = atof( temp_str );
	GetPrivateProfileString("VIEWMODE","viewturn","0",temp_str,100,ini_file); // 視点の回転角度
	viewTurn = atof( temp_str );
	GetPrivateProfileString("TOPVIEW","topheight","6",temp_str,100,ini_file); // 初期の上から視点の高さ
	topHeight = atof( temp_str );
	GetPrivateProfileString("TOPVIEW","topshift","0.2",temp_str,100,ini_file); // 上から視点のアパターの中心からのずれ
	topShift = atof( temp_str );
	GetPrivateProfileString("BIRDSVIEW","birdsheight","5",temp_str,100,ini_file); // 初期の鳥瞰視点の高さ
	birdsHeight = atof( temp_str );
	GetPrivateProfileString("BIRDSVIEW","birdsradius","30",temp_str,100,ini_file); // 鳥瞰視点の角度
	birdsRadius = atof( temp_str );
	GetPrivateProfileString("BIRDSVIEW","birdsshift","1.1",temp_str,100,ini_file); // 鳥瞰視点のアパターの中心からのずれ
	birdsShift = atof( temp_str );
	//	アバターの設定
	radius = ini_radius;//1.0f;
	// アバターの手の設定
	hand_radius = ini_radius;//追加：赤羽

	afInitialBody = Affinef(ini_initialx,ini_initialy,ini_initialz);
	afBody = afInitialBody;

	afBody.LookAtGL(Vec3f(ini_initialx,ini_initialy,ini_initialz+1),
		Vec3f(ini_initialx,ini_initialy+1, ini_initialz)); //posz(-EZ), posy(EY) ini_initialz
	//  自分の設定
	VecMyself[0] = Vec3f(        0, 0.0,-radius/2);
	VecMyself[1] = Vec3f( radius/2, 0.0, radius/2);
	VecMyself[2] = Vec3f(-radius/2, 0.0, radius/2);

	//  固定カメラの設定：現在は使用していない
	bEye[0] = 0.0; bEye[1] = 10.0; bEye[2] = 0.0;
	bTarget[0] = 0.0; bTarget[1] = 0.0; bTarget[2] = 0.0;
	bUp[0] = 0.0; bUp[1] = 0.0; bUp[2] = -1.0;

	//	プロジェクタ位置データのロード
	if (LoadSettings()) SetParam();

	/////////////////////////////////////////////////////////////////////////////////////
	//MPI
	int counts[7]={10, 60, 6, 3, 6, 9, 3};
	
	MPI_Datatype type[7]={MPI_INT,MPI_FLOAT, MPI_FLOAT, MPI_FLOAT,MPI_FLOAT,MPI_INT, MPI_FLOAT};

	MPI_Aint byte[7], extent;
	byte[0] = 0;
	MPI_Type_extent(MPI_INT,&extent);
	byte[1] = counts[0]*extent;
	MPI_Type_extent(MPI_FLOAT,&extent);
	byte[2] = byte[1]+counts[1]*extent;
	//struct add JEONG
    MPI_Type_extent(MPI_FLOAT,&extent);
	byte[3] = byte[2]+counts[2]*extent;
    MPI_Type_extent(MPI_FLOAT,&extent);
	byte[4] = byte[3]+counts[3]*extent;
    MPI_Type_extent(MPI_INT,&extent);
	byte[5] = byte[4]+counts[4]*extent;
    MPI_Type_extent(MPI_FLOAT,&extent);
	byte[6] = byte[5]+counts[5]*extent;

	MPI_Type_struct(7,counts,byte,type,&stMove_Data);
	MPI_Type_commit(&stMove_Data);
   /////////////////////////////////////////////////////////////////////////////////////////
}

void TGLView::SetParam(){
	double angleA,angleB,angleC;
	angleB = asin(setting[1]);
	angleA = asin(setting[0]/cos(angleB));
	angleC = setting[5]/180 * M_PI;
	Affinef Rzx,Rzy,Rxy;
	Rzx.Ex() = Vec3f(cos(angleA),0,-sin(angleA));
	Rzx.Ey() = Vec3f(0,1,0);
	Rzx.Ez() = Vec3f(sin(angleA),0,cos(angleA));
	Rzy.Ex() = Vec3f(1,0,0);
	Rzy.Ey() = Vec3f(0,cos(angleB),-sin(angleB));
	Rzy.Ez() = Vec3f(0,sin(angleB),cos(angleB));
	Rxy.Ex() = Vec3f(cos(angleC),sin(angleC),0);
	Rxy.Ey() = Vec3f(-sin(angleC),cos(angleC),0);
	Rxy.Ez() = Vec3f(0,0,1);
//	cout << angleB ;
	R = Rzx*Rzy*Rxy;
	fov_v = setting[3] * 2;
}

bool TGLView::OnTimer(){
	Step();
	return true;
}

void TGLView::OnIdle(){
	//Sleep(5);
	// キーボードの処理はここで行う
	//::OnKeyboard(press_key, 0, 0);
	switch(press_key){
	 case 'w': //y
		  sp_info.ball_pos.Y() += delta_Pad;
		  
		  break;
	 case 's': 
		  //BBall.new_pos.y -= delta_Pad;
		  sp_info.ball_pos.Y() -= delta_Pad;
		  
		  break;
	 case 'a': //x
		  BBall.new_pos.x += delta_Pad;
		  
		  break;
	 case 'd': 
		  BBall.new_pos.x -= delta_Pad;
		  
		  break;
	 case 'A': //z
		  BBall.new_pos.z += delta_Pad;
		  
		  break;
	 case 'D': 
		  BBall.new_pos.z -= delta_Pad;
		  
		  break;
	case 'b':
		bMinForce = !bMinForce;
		break;
	case 'B':
		BBall.chk_state = FALLING;
		bHCollide = false;
		ft=0.0;
		BBall.Fall_pos = sp_info.grp_pos[1];
		break;
	case 'c':
		reset_spidar();
		init_ball();
		break;
	case 'f':
		full_mode =!full_mode;
		if(full_mode) glutFullScreen();
		else   glutReshapeWindow(400,300);
	   break;
	case 'i':
		//afBody = afInitialBody;
		//afBody.LookAtGL(Vec3f(afInitialBody.PosX(),afInitialBody.PosY(),afInitialBody.PosZ()+1),
		//	Vec3f(afInitialBody.PosX(),afInitialBody.PosY()+1,afInitialBody.PosZ()));
		init_ball();
		break;
	case '\t':
      BBall.chk_shoot = SHOOTED; 
	  BBall.chk_state = THROWING;
	  bHCollide = false;
	  	//BBall.shot_pos.x = sp_info.ball_pos.X();
		//BBall.shot_pos.y = sp_info.ball_pos.Y();
		//BBall.shot_pos.z = sp_info.ball_pos.Z();
		//BBall.shot_pos = BBall.new_pos;
		//printf("spidar x y z: %f, %f, %f  \n", BBall.shot_pos.x, BBall.shot_pos.y, BBall.shot_pos.z);
	  break;
	case 'h':
		if(printStepHz == NULL){
			printFileNameTime = timeGetTime();			
			printStepHz = new std::ofstream(_itoa(printFileNameTime,printFileName,10));
			PrintHz(*printStepHz);
		}else{
			PrintHz(*printStepHz);
			delete printStepHz;
		}
		break;
	case '\x1b': //Esc
		//SPIDAR-H timer
		bMinForce = true;

		timeKillEvent(SpidarUpdateTimer);
		timeEndPeriod(1);
		Exit();
		break;

	default:
		break;
	}

	press_key = 0;	
}
/////////////////////////////
void TGLView::init_spidar()
{
	
    devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();
	//std::cout << devMan;
   
		//工作室のSPIDAR
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)},
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};
	
	// VM室Big-SM_PIDAR用　モータの取り付け座標（ターンテーブルの中心から床上110cmが原点）	
	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.75f, 2.00f, 1.12f),	Vec3f( 1.65f, 2.17f,-1.34f), Vec3f(-1.65f,-0.9f,-1.44f), Vec3f( 1.65f,-0.99f, 0.50f)},
		{Vec3f( 1.58f, 2.00f, 1.12f),	Vec3f(-1.65f, 2.18f,-1.34f), Vec3f( 1.65f,-0.98f,-1.44f), Vec3f(-1.65f,-0.98f, 0.50f)}
	};*/

	// 2003.10 NEW SETTING
	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};*/

	bMinForce = true;
	holdHand  = false;
	ballCount = 0;
	for(int i=0; i<2; i++)
	{
	   h_spidar[i].Init(devMan, motorPos[i], MAXON, LPP, 0.5f, 20.0f); 
	   h_spidar[i].Calib();
	}

}
void TGLView::reset_spidar(){
	for(int i = 0; i < 2; i++) h_spidar[i].Calib();
	init_ball();
}

//////////////////// SPIDAR 動作確認 ////////////////////
void TGLView::spidarCheck(){
		// SPIDAR の座標表示
		for(int j = 0; j < 2; j++){
			printf("%d: (x,y,z) = (%f, %f, %f)\n", j, h_spidar[j].GetPos().X(), h_spidar[j].GetPos().Y(), h_spidar[j].GetPos().Z());
		}
		printf("\n");

		// モーターの糸の長さ表示
		/*for(int j = 0; j < 2; j++){
			for(int i = 0; i < 4; i++){
				std::cout << i + 4 * j << ": " << h_spidar[j].Motor()[i].Length()  << " ";
			}
			printf("\n");
		}
		printf("\n");*/
}

Vec3f TGLView::convertEuclidToPole(Vec3f euclid){
	Vec3f pole;	// X：半径、Y：θ、Z：φ とする。
	pole.X() = sqrt(pow(euclid.X(), 2) + pow(euclid.Y(), 2) + pow(euclid.Z(), 2));
	pole.Y() = atan2(euclid.Z(), euclid.X());
	pole.Z() = atan2(euclid.Y(), sqrt(pow(euclid.X(), 2) + pow(euclid.Z(), 2)));
	return pole;
}

Vec3f TGLView::convertPoleToEuclid(Vec3f pole){
	Vec3f euclid;	// X：半径、Y：θ、Z：φ とする。
	euclid.X() = pole.X() * cos(pole.Y()) * cos(pole.Z());
	euclid.Y() = pole.X() * sin(pole.Z());
	euclid.Z() = pole.X() * sin(pole.Y()) * cos(pole.Z());
	return euclid;
}
/* If BallState is Free */
void TGLView::BallIsFree(){
	Vec3f t_force[2];
	float dis[2];
	bool  hold[2];

	for(int i = 0; i < 2; i++){
		hold[i] = 0;
		dis[i] = (sp_info.grp_pos[i] - sp_info.ball_pos).norm();
		if(dis[i] < BBall.radius + s_rad){
			t_force[i]     = -K * ((BBall.radius + s_rad - dis[i])) * (sp_info.grp_pos[i] - sp_info.ball_pos).unit();
			t_force[i].Y() = -t_force[i].Y();	// 座標変換
			hold[i]  = 1;
			if(i == 0) holdHand = 0;
			else       holdHand = 1;
		}
		if(bMinForce == 1) h_spidar[i].SetMinForce();
		else               h_spidar[i].SetForce(t_force[i]);
	}

	if(hold[0] & hold[1]){
		bHCollide  = true;
		BBall.chk_state  = HOLDING;
		//hold
		sp_info.VBradius = (sp_info.grp_pos[0] - sp_info.grp_pos[1]).norm() / 2;
		sp_info.initInclination  = convertEuclidToPole(sp_info.grp_pos[0] - sp_info.grp_pos[1]);
		sp_info.initBallPole     = convertEuclidToPole(sp_info.ball_pos - (sp_info.grp_pos[0] + sp_info.grp_pos[1]) / 2);
		for(int i = 0; i < LogNum; i++) sp_info.BallPosLog[i] = sp_info.ball_pos;
	}
	else if(hold[0] || hold[1]){
		bHCollide  = true;
		BBall.chk_state   = HALFHOLD;
        //JEONG
		ballCount = 0;
		sp_info.initBallPole = convertEuclidToPole(sp_info.ball_pos - sp_info.grp_pos[holdHand]);
		sp_info.initBallPole.X() = BBall.radius + s_rad;
		sp_info.initBallPosition = convertPoleToEuclid(sp_info.initBallPole);
	}
}

/* If BallState is HalfHoliding */
void TGLView::BallIsHalfHolding(){
	Vec3f nowBallPole;

	sp_info.ball_pos  = sp_info.initBallPosition + sp_info.grp_pos[holdHand];
	//SETTING BALL POSITION
	BBall.new_pos.x = sp_info.ball_pos.X();
	BBall.new_pos.y = sp_info.ball_pos.Y();
	BBall.new_pos.z = sp_info.ball_pos.Z();
	//BBall.init_pos = BBall.new_pos;
	
	float dis   = (sp_info.grp_pos[!holdHand] - sp_info.ball_pos).norm(); 
	float t_dis = (sp_info.grp_pos[0] - sp_info.grp_pos[1]).norm();

	h_spidar[0].SetMinForce();
	h_spidar[1].SetMinForce();

	if(dis < BBall.radius + s_rad){
		BBall.chk_state  = HOLDING;
		bHCollide = true;
		sp_info.VBradius         = t_dis / 2;
		sp_info.initInclination  = convertEuclidToPole(sp_info.grp_pos[0] - sp_info.grp_pos[1]);
		sp_info.initBallPole     = convertEuclidToPole(sp_info.ball_pos - (sp_info.grp_pos[0] + sp_info.grp_pos[1]) / 2);
		for(int i = 0; i < LogNum; i++) sp_info.BallPosLog[i] = sp_info.ball_pos;
	}
	else if(ballCount > 500){
		BBall.chk_state = FALLING;
		//falling pos
		BBall.Fall_pos = sp_info.ball_pos;
		bHCollide = false;
		ballCount = 0;
		//time setting
		ft=0.0;
	}
	ballCount++;
}

/* If BallState is Holiding */
void TGLView::BallIsHolding(){
	Vec3f changeInclination;
	Vec3f t_force[2];
	bool hold = 0;

	changeInclination = convertEuclidToPole(sp_info.grp_pos[0] - sp_info.grp_pos[1]) - sp_info.initInclination;
	nowBallPole.X()   = sp_info.initBallPole.X();
	nowBallPole.Y()   = sp_info.initBallPole.Y() + changeInclination.Y();
	nowBallPole.Z()   = sp_info.initBallPole.Z() + changeInclination.Z();

	sp_info.ball_pos  = convertPoleToEuclid(nowBallPole) + (sp_info.grp_pos[0] + sp_info.grp_pos[1]) / 2;
	//SETTING NEW BALL POSITION
	BBall.new_pos.x = sp_info.ball_pos.X();
	BBall.new_pos.y = sp_info.ball_pos.Y();
	BBall.new_pos.z = sp_info.ball_pos.Z();
	
	float t_dis = (sp_info.grp_pos[0] - sp_info.grp_pos[1]).norm();

	if(t_dis < 2 * sp_info.VBradius){
		t_force[0]     = -K * ((2 * sp_info.VBradius - t_dis) / 2) * (sp_info.grp_pos[0] - sp_info.grp_pos[1]).unit();
		t_force[0].Y() = -t_force[0].Y();	// 座標変換
		t_force[1]     = -t_force[0];
		hold = 1;
	}
	if(bMinForce == 1){
		h_spidar[0].SetMinForce();
		h_spidar[1].SetMinForce();
	}
	else{
		h_spidar[0].SetForce(t_force[0]);
		h_spidar[1].SetForce(t_force[1]);
	}

	for(int i = 1; i < LogNum; i++) sp_info.BallPosLog[i] = sp_info.BallPosLog[i-1];
	sp_info.BallPosLog[0] = sp_info.ball_pos;
	Vec3f ballSpeed = (sp_info.BallPosLog[0] - sp_info.BallPosLog[4]) * 1000 / SpidarUpdateRate;
    sp_info.speed   = ballSpeed.norm();

	if(sp_info.speed > BallSpeedThreshold && ballSpeed.Z() > BallSpeedThreshold * sin(M_PI / 4)){
		BBall.chk_shoot = SHOOTED; 
		BBall.chk_state = THROWING;

		//BBall.shot_pos.x = sp_info.ball_pos.X();
		//BBall.shot_pos.y = sp_info.ball_pos.Y();
		//BBall.shot_pos.z = sp_info.ball_pos.Z();
		BBall.shot_pos = BBall.new_pos;
		bHCollide = false;
		//time resetting 10/24
		t=0.0;
	}
	else if(!hold){
		BBall.chk_state   = HALFHOLD;
        //JEONG
		ballCount = 0;
		holdHand  = 0;
		if((sp_info.ball_pos - sp_info.grp_pos[0]).norm() > (sp_info.ball_pos - sp_info.grp_pos[1]).norm()){
			holdHand = 1;
		}
		sp_info.initBallPole = convertEuclidToPole(sp_info.ball_pos - sp_info.grp_pos[holdHand]);
		sp_info.initBallPole.X() = BBall.radius + s_rad;
		sp_info.initBallPosition = convertPoleToEuclid(sp_info.initBallPole);
	}
}

/* If BallState is Fixed (Throwing or Falling) */
/*void TGLView::BallIsFixed(){
	Vec3f t_force[2];
	float dis[2];

	for(int i = 0; i < 2; i++){
		dis[i] = (sp_info.grp_pos[i] - sp_info.ball_pos).norm();
		if(dis[i] < BBall.radius + s_rad){
			t_force[i]     = -K * ((BBall.radius + s_rad - dis[i])) * (sp_info.grp_pos[i] - sp_info.ball_pos).unit();
			t_force[i].Y() = -t_force[i].Y();	// 座標変換
		}
		if(bMinForce == 1) h_spidar[i].SetMinForce();
		else               h_spidar[i].SetForce(t_force[i]);
	}

	//if(ballCount > 200) BBall.chk_state = FALLING;//BBall.chk_state = FREE;
	ballCount++;
}*/

void TGLView::update_spidar()
{
	
	if(myrank == 0)
	{
		Vec3f t_pos[2];

		for(int i = 0; i < 2; i++){
	
			h_spidar[i].Update();
		//	if(h_spidar[i].Motor()[0].da){
		//		printf("");
		//	}
			if (BBall.chk_state == SHOOTED) 
				h_spidar[i].GetPos().Z() = 0.0;
			t_pos[i]  =h_spidar[i].GetPos() * convert_inch*2;
		}
		//setting ball
		sp_info.ball_pos.X() = BBall.new_pos.x;
		sp_info.ball_pos.Y() = BBall.new_pos.y;
		sp_info.ball_pos.Z() = BBall.new_pos.z;

		//spidarCheck();
		if(fabs(t_pos[0].X()) <= 100 && fabs(t_pos[1].X()) <= 100){
			for (int i = 0; i < 2; i++)
			{
				sp_info.grp_pos[i].X() = sp_info.user_hand[i].X() + t_pos[i].X();  //update 0211
				sp_info.grp_pos[i].Y() = sp_info.user_hand[i].Y() + t_pos[i].Y();
				sp_info.grp_pos[i].Z() = sp_info.user_hand[i].Z() + t_pos[i].Z(); //　x,z sign , inverse
			}
				
			if(BBall.chk_state == HALFHOLD){
				BallIsHalfHolding();
			}
			else if(BBall.chk_state == HOLDING){
				BallIsHolding();
			}
			else if(BBall.chk_state == PAUSE || BBall.chk_state == RETURNING || BBall.chk_state == FALLING){
				BallIsFree();
			}
			else
			{
				for (int i = 0; i < 2; i++){
					h_spidar[i].SetMinForce();
				}
			}
			if(BBall.chk_shoot == REFLEXED){
				BBall.ReturnHand = (sp_info.grp_pos[0] + sp_info.grp_pos[1]) / 2;
				BBall.RetAngle(BBall.new_vel.y, off_v);
				BBall.chk_shoot = NOSHOOT;
				//time reset
				dt=0.0;
			}
		}
	}
}

void TGLView::init_hand()
{
    //SPIDAR-H
	sp_info.user_hand[0].X() = 0.0;
	sp_info.user_hand[0].Y() = 120.0;
	sp_info.user_hand[0].Z() =0.0;
	sp_info.user_hand[1]  = sp_info.user_hand[0];
	
	sp_info.grp_pos[0]    = sp_info.user_hand[0];
	sp_info.grp_pos[1]    = sp_info.user_hand[0];
    
	reset_spidar();
}
// initial ball setting
void TGLView::init_ball()
{
   
   t=0.0;
   dt=0.0;
   ft = 0.0;
   
   //ball reset
   BBall.Init();
   
   reflex_v.x=0.0;
   reflex_v.y=0.0;
   reflex_v.z=0.0;
   //init 
   bHCollide = false;
   bVCollide = false;
}

void TGLView::OnDraw()
{
    //MPI
	// キーボード入力を送信
	MPI_Bcast(&press_key, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//次のフレームを送信
	//if(myrank == 0) apply_send();
	MPI_Bcast(&M_DAT, 1, stMove_Data, 0, MPI_COMM_WORLD);
	if(myrank != 0) apply_receive();
	///////////////////////////////////////////////////////////////////////
	//ステンシルバッファをクリアしなきゃ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();

	static Affinef R180(Rad(180),'y');
	
	Affinef afProjector = afBody * R180 * R * R180;
	
	Affinef afCam = (afEye * afProjector).inv();
	glLoadMatrixf(afCam);

  glPushMatrix();
     //camera
     glTranslatef(0, -nHeight, vCamera.Z());
	 glRotatef(vCamera.Y(), 0.0, 1.0, 0.0);

	 glEnable(GL_LIGHTING);

	//BBall.Draw();//ytakeyama comment out
	//ytakeyama ボールと空を描きます
		int tempflag = 0;
for(int i=0; i<3; i++)
{
		for(TGeoNodes::iterator it = allNodes[i].begin(); it != allNodes[i].end(); ++it)
		{
			switch(i)
			{
			case 0:

				if(tempflag){
					(*it)->Draw();
				}else{
					//ball display
					glPushMatrix();
						glTranslatef(BBall.new_pos.x, BBall.new_pos.y, BBall.new_pos.z);
					//	glRotatef(nowBallPole.Y()*180/M_PI, 0.0, -1.0, 0.0);
					//	glRotatef(nowBallPole.Z()*180/M_PI, -cos(nowBallPole.Y()), 0.0, -sin(nowBallPole.Y()));
							(*it)->Draw();
						glutWireSphere(50, 15, 12);
					glPopMatrix();
					tempflag = 1;
				}
				break;
			case 1:
				//lefthand display
				glPushMatrix();
				    glTranslatef(sp_info.grp_pos[0].X(),  sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z());
					(*it)->Draw();
				glPopMatrix();
			break;
			case 2:
				//lefthand display
				glPushMatrix();
				    glTranslatef(sp_info.grp_pos[1].X(),  sp_info.grp_pos[1].Y(), sp_info.grp_pos[1].Z());
					(*it)->Draw();
				glPopMatrix();
			break;
			}
		}
}
//SPIDAR-H HAND Position
/*		glPushMatrix(); //left
	   	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(sp_info.grp_pos[0].X(),  sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z());
	 		if (BBall.chk_state == HOLDING || BBall.chk_state == HALFHOLD ) 
				glutWireSphere(s_rad, 15, 12);
			else
				glutSolidSphere(s_rad, 15, 12);

       glPushMatrix(); //right
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(sp_info.grp_pos[1].X(),  sp_info.grp_pos[1].Y(), sp_info.grp_pos[1].Z());
	 		if (BBall.chk_state == HOLDING || BBall.chk_state == HALFHOLD ) 
				glutWireSphere(s_rad, 15, 12);
			else
				glutSolidSphere(s_rad, 15, 12);
		glPopMatrix();
*/
	glPopMatrix();

//MPI
	MPI_Barrier(MPI_COMM_WORLD);

glutSwapBuffers();		// Flip()を含む

}

void TGLView::OnSize(int w, int h)
{
  GLfloat aspect;

  if(h == 0) { h=1;}

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  aspect =(float) w/h;
  //for VRML of Ytakeyama 10000以上
  gluPerspective(fov_v, aspect, 1.0, 10000);

  glMatrixMode(GL_MODELVIEW);

}

void TGLView::OnSpecialKey(int key, int x, int y) 
{
  if (key == GLUT_KEY_LEFT)    { vCamera.Y() -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { vCamera.Y() += deltaRot; }
  if (key == GLUT_KEY_UP)      { vCamera.Z() -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { vCamera.Z() += deltaRot; }
  glutPostRedisplay();
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	//MPI
	press_key = key;
	
}

void CALLBACK SpidarUpdateProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD)
{
	((TGLView*)dwUser)->update_spidar();

}

void TGLView::OnOpen(void)
{

  //InitCamera();	
	//	GLの初期化
 GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
 tQuaternion nGround1(0, 1, 0, -(GnHeight+2));
 MyVector vLight(-30, 50, -50);							//shadow
 
 glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
 glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
 glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
// glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
 glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

 GLfloat light0pos[] = {10.0, 1.0, 5.0, 1.0};			//shadow
 glLightfv(GL_LIGHT0, GL_POSITION, light0pos);			//shadow
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHTING);
	
  //glClearColor(0.75, 0.75, 1.0, 0.0); sky color
  glClearColor(0.0, 0.0, 0.0, 0.0); //black
  glEnable(GL_DEPTH_TEST); //hidden surface removed

    ////////////////////////////////////////////////////////////////////////
  //shadow関係の初期化
  glDepthFunc(GL_LEQUAL);								//shadow
  //深度バッファのクリア値の設定
  glClearDepth(1.0);								//shadow
  //ステンシルバッファクリア値の設定
  glClearStencil(0);								//shadow
  ////////////////////////////////////////////////////////////////////////////
 
  glFrontFace(GL_CCW); //no counterclockwise
  glEnable(GL_CULL_FACE); 
  glCullFace(GL_BACK);
 
//generate the shadow matrix
	CalcShadowMat(ShadowMat, nGround1, vLight);

	init_hand();
	init_ball();	
	
	vCamera.X() = 0.0;
	vCamera.Y() = 180.0;
	vCamera.Z() = 50.0;

//MPI
   //MPI031021
   //なぜかここで止まってしまうので、とりあえず、コメントアウト
   //PI_Barrier(MPI_COMM_WORLD);
//~MPI
   if(myrank == 0){
		glutReshapeWindow(400,300);
   }else{
		glutFullScreen();
        glutSetCursor(GLUT_CURSOR_NONE);
   }
   
   //SPIDAR-H
   init_spidar();
  //timer setting	
	timeBeginPeriod(1);
	SpidarUpdateTimer = timeSetEvent(SpidarUpdateRate, 1, SpidarUpdateProc, (DWORD)this, TIME_PERIODIC);
	SetTimer(33, &TGLView::OnTimer); //Step()を呼び出す周期 33
	dwTime = timeGetTime(); //GetTickCount();

}

void TGLView::Ball_Control(int cur_frm)
{
	//BBall.new_pos =BBall.init_pos;
	BBall.new_pos.x =sp_info.ball_pos.X();
	BBall.new_pos.y =sp_info.ball_pos.Y();
	BBall.new_pos.z =sp_info.ball_pos.Z();
}

void TGLView::Step()
{
	stepHz ++;
    //time distance 計測
	dwLastTime = dwTime;
	dwTime = timeGetTime();
	float pass = float(dwTime - dwLastTime) / 1000;

	//MPI
	if(myrank == 0)
	{
		 if (animating) 
		  {
			  //mydb.vh.Anim.cur_frm++;
			 //Jeong10/24 
			   
			 if (BBall.chk_shoot == SHOOTED ) 
			 {
			
				 BBall.chk_shoot = NOSHOOT;
				 bHCollide = false;
				 t=0.0; //resetting time
			
			 }
		 //setting
		 //Ball_Control(mydb.vh.Anim.cur_frm);
		} //animating
	}  //server only 実行
 
	if (bDraw) Invalidate();			//	描画要求
}

void TGLView::PrintHz(std::ostream& os) const{
	os << stepHz << "," << timeGetTime() << std::endl;
}

void TGLView::apply_send(int cur_frm)
{
	//MPI
	int idx=0;
	MyVector ret;

	M_DAT.Ball_Pos[0] = BBall.new_pos.x;
	M_DAT.Ball_Pos[1] = BBall.new_pos.y;
	M_DAT.Ball_Pos[2] = BBall.new_pos.z;
	
	//SPIDAR-H JEONG 10/23
	
    M_DAT.Hand_Pos[0] = sp_info.grp_pos[0].X();
	M_DAT.Hand_Pos[1] = sp_info.grp_pos[0].Y();
	M_DAT.Hand_Pos[2] = sp_info.grp_pos[0].Z();

	M_DAT.Hand_Pos[3] = sp_info.grp_pos[1].X();
	M_DAT.Hand_Pos[4] = sp_info.grp_pos[1].Y();
	M_DAT.Hand_Pos[5] = sp_info.grp_pos[1].Z();
	
	M_DAT.RotBall[0] = nowBallPole.X();
	M_DAT.RotBall[1] = nowBallPole.Y();
	M_DAT.RotBall[2] = nowBallPole.Z();

}
void TGLView::apply_receive()
{
   	//MPI031021
	//============ BEGIN ===============

	//初回のみ適当なモーションデータで配列を埋めておく
	if(bFirstReceive){
		for(int i=0;i<MOTION_DAT_BUF_SIZE;i++){
			motionDatBuf[i] = M_DAT;
		}
		bFirstReceive = false;
	}

//Ball pos 
	BBall.new_pos.x = M_DAT.Ball_Pos[0];
	BBall.new_pos.y = M_DAT.Ball_Pos[1];
	BBall.new_pos.z = M_DAT.Ball_Pos[2];
	
	//SPIDAR-H JEONG 10/23
	sp_info.grp_pos[0].X() = M_DAT.Hand_Pos[0];
	sp_info.grp_pos[0].Y() = M_DAT.Hand_Pos[1];
	sp_info.grp_pos[0].Z() = M_DAT.Hand_Pos[2];

	sp_info.grp_pos[1].X() = M_DAT.Hand_Pos[3];
	sp_info.grp_pos[1].Y() = M_DAT.Hand_Pos[4];
	sp_info.grp_pos[1].Z() = M_DAT.Hand_Pos[5];
		
	 nowBallPole.X() = M_DAT.RotBall[0];
     nowBallPole.Y() = M_DAT.RotBall[1];
	 nowBallPole.Z() = M_DAT.RotBall[2];
}

