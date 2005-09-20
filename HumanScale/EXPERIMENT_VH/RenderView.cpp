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
#include "RenderView.h"
#include "tex.h"      // primitive definition
#include "ball.h"

//MY ODBC
Cdirexec mydb;
CBall BBall;
SpidarInfo sp_info;
CMyTex render;
/////////////////////////////////////////////////////////
// check variables
static int full_mode =0;
static int chk_end =0;
static int chk_interp = 0;
static int animating = 0;
//////////////////////////////
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
//lighting & color definition
//static GLfloat light0pos[] = {0.0, 200.0, 200.0, 1.0};
static GLfloat light0pos[] = {80.0, 170.0, 200.0, 1.0};
static GLfloat light1pos[] = {200.0, 200.0, 0.0, 1.0};

static double ShadowMat[4][4];
/////////////////////////////////////////////////////////
using namespace std;
const float EYEOFFSET=0.8;

//SPIDAR-H
////////////////////////////
UINT SpidarUpdateTimer;
static float s_rad = 1;
////////////////////////////
///For experiment///////////////////////////////////////////////////////////////////
static int Test_Mode= 1; //Haptic 
static int VH_Mode= 1;
static int MultiPerson = 1; 
static int Crowd_Num= 3;
static int doing_Shadow = 0;
#define SOUND_PC 7
//2004 10 15 
//test mode = 0: no haptics 1: haptics 
//vh mode  0: only hand 1: full body	
#define NUM_OF_TEST 5
char testchar[10];
char buffer[10];

struct Experiment_data
{
	int Seq;
	int Exp_Mode[2][NUM_OF_TEST];
	
	Vec3f shotpos[2][NUM_OF_TEST];
	float shotvelocity[2][NUM_OF_TEST];
	int motion_id[2][NUM_OF_TEST];
	int success[2][NUM_OF_TEST];
	int start_time[2][NUM_OF_TEST];
    int elasped_time[2][NUM_OF_TEST];
};

Experiment_data exp_data;
///////////////////////////////////////////////////////////////////////////////////////
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
		mciSendString("set WAVEFILE time format tmsf notisfy", NULL, 0,0); //wait
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

	//shadow
	s_delta = 0;

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
	int counts[9]={10, 60, 6, 3, 6, 9, 3, 1, 3};
	//実験で２個追加
	MPI_Datatype type[9]={MPI_INT,MPI_FLOAT, MPI_FLOAT, MPI_FLOAT,MPI_FLOAT,MPI_INT, MPI_FLOAT, MPI_INT, MPI_FLOAT};

	MPI_Aint byte[9], extent;
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
//jeong add 2004/02/15
	MPI_Type_extent(MPI_INT,&extent);
	byte[7] = byte[6]+counts[6]*extent;
    MPI_Type_extent(MPI_FLOAT,&extent);
	byte[8] = byte[7]+counts[7]*extent;

	MPI_Type_struct(9,counts,byte,type,&stMove_Data);
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


void TGLView::OnIdle()
{

	//Sleep(5);
	// キーボードの処理はここで行う
	//::OnKeyboard(press_key, 0, 0);
	
	int cur_frm =  mydb.vh.Anim.cur_frm;
	switch(press_key){
	case '0':
        doing_Shadow = !doing_Shadow;
		break;
	 case '1':
		Test_Mode = 0;
	    strcpy(testchar, "Test Mode: A");
		//_itoa(Test_Mode, buffer, 10);
		//strcat(testchar, buffer);
		exp_data.Seq = 0;
		break;
	 case '2':
		Test_Mode = 1;
		 strcpy(testchar, "Test Mode: B");
		//_itoa(Test_Mode, buffer, 10);
		//strcat(testchar, buffer);
		exp_data.Seq = 0;
		break;
	 case '3':
		VH_Mode = 0;
		 strcpy(testchar, "Test Mode: C");
		//_itoa(Test_Mode, buffer, 10);
		//strcat(testchar, buffer);
		break;
	case '4':
		VH_Mode = 1;
		 strcpy(testchar, "Test Mode: D");
		//_itoa(Test_Mode, buffer, 10);
		//strcat(testchar, buffer);
		break;
	 case 'w': //y
		  BBall.new_pos.y += delta_Pad;
		  //sp_info.ball_pos.Y() += delta_Pad;
		  printf( "hand y pos: %f ball y: %f \n", mydb.vh.MModel.LA_bone[3].position_global[cur_frm].y, BBall.new_pos.y);
		  break;
	 case 's': 
		  BBall.new_pos.y -= delta_Pad;
		  //sp_info.ball_pos.Y() -= delta_Pad;
		  printf( "hand y pos: %f ball y: %f \n", mydb.vh.MModel.LA_bone[3].position_global[cur_frm].y, BBall.new_pos.y);
		  break;
	 case 'a': //x
		  BBall.new_pos.x += delta_Pad;
		  printf( "hand x pos: %f ball x: %f \n", mydb.vh.MModel.LA_bone[3].position_global[cur_frm].x, BBall.new_pos.x);
		  break;
	 case 'd': 
		  BBall.new_pos.x -= delta_Pad;
		  printf( "hand x pos: %f ball x: %f \n", mydb.vh.MModel.LA_bone[3].position_global[cur_frm].x, BBall.new_pos.x);
		  break;
	 case 'A': //z
		  BBall.new_pos.z += delta_Pad;
		  printf( "hand z pos: %f ball z: %f \n", mydb.vh.MModel.LA_bone[3].position_global[cur_frm].z, BBall.new_pos.z);
		  break;
	 case 'D': 
		  BBall.new_pos.z -= delta_Pad;
		  printf( "hand z pos: %f ball z: %f \n", mydb.vh.MModel.LA_bone[3].position_global[cur_frm].z, BBall.new_pos.z);
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
    case 'm':
		MultiPerson = !MultiPerson;
		break;
	case 'i':
		init_ball();
		break;
	case ' ': 
		animating = !animating; //space
		printf( "cur frame: %d  hand y pos: %f ball x: %f ball y: %f ball z: %f \n", cur_frm, mydb.vh.MModel.LA_bone[3].position_global[cur_frm].y, BBall.new_pos.x, BBall.new_pos.y, BBall.new_pos.z);
	   	break;
	case '\t': //shooting
		shot_control();
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
	case 'p':
		if (myrank == 0) spidar_save();
		break;
	case '\x1b': //Esc
		if(cpNum == SOUND_PC)  PlaySound(NULL, NULL, 0);
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

/*
void comp_shotAngle(int cur_frame)
{
//jeong 08/15
	//it depends on our local coordinate
	// we rotated 90 degree at y axis. so x axis is directed at front.

// between shoulder and end effector, atan2(y, x) y/x is shotting angle
// y value is sensitive at + or - direction
//x is always + direction from shoulder point

//gamma = atan(x/z), atan2(x, z) between ball and sh
// 2004.08.31 
double s_width, s_height;
double rad_angle;
double ball_x, ball_z;
double sh_x, sh_z;

  s_width = vh[0].right_pos[cur_frame][0] - vh[0].right_sh[cur_frame][0];
  s_height = vh[0].right_pos[cur_frame][1] - vh[0].right_sh[cur_frame][1];

   rad_angle = atan2(s_height, s_width);
   shotting_angle = rad_angle* M_RAD_TO_DEG;

  sh_x = (vh[0].right_sh[cur_frame][0] + vh[0].left_sh[cur_frame][0])/2;
  sh_z = (vh[0].right_sh[cur_frame][2] + vh[0].left_sh[cur_frame][2])/2;

  ball_x = (vh[0].right_pos[cur_frame][0] + vh[0].left_pos[cur_frame][0])/2;
  ball_z = (vh[0].right_pos[cur_frame][2] + vh[0].left_pos[cur_frame][2])/2;

  s_height = sqrt((ball_z-sh_z)*(ball_z-sh_z));
  s_width = sqrt((ball_x-sh_x)*(ball_x-sh_x));
  rad_angle = atan2(s_width, s_height);
  gamma= 90-(rad_angle* M_RAD_TO_DEG);

}*/

void TGLView::shot_control()
{
     char buffer[10]="";
	 
	 //int extime;
	 //int flag;
	 int cnt;

	 exp_data.Seq++;
	 cnt = exp_data.Seq-1; //Test_Mode * 5 +
	 /*if (exp_data.Seq > 5 ) flag= MessageBox(NULL, "回数が終わりましたが、続きますか？", "ERROR", MB_YESNO);
	 if (flag == IDNO) animating = false;
	 else
	 {
		extime = timeGetTime();
		exp_data.elasped_time[cnt] = extime - exp_data.start_time[cnt];
	 }*/
  
	 exp_data.Exp_Mode[Test_Mode][cnt] = Test_Mode;
	 //if (exp_data.Seq ==  1 ) 
	 exp_data.start_time[Test_Mode][cnt] = timeGetTime();

	 BBall.chk_shoot = SHOOTED; 
	 BBall.chk_state = THROWING;
	 bHCollide = false;
	 BBall.shot_pos = BBall.new_pos;
	 t=0.0;
//start.
	  exp_data.shotpos[Test_Mode][cnt].X() = BBall.shot_pos.x;
	  exp_data.shotpos[Test_Mode][cnt].Y() = BBall.shot_pos.y;
	  exp_data.shotpos[Test_Mode][cnt].Z() = BBall.shot_pos.z;
	  //exp_data.shotvelocity[cnt]= sp_info.speed
    
}
/////////////////////////////
void TGLView::init_spidar()
{
	
    devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();
	//std::cout << devMan;
   
// 2003.10 NEW SETTING
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};

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
		if(dis[i] < BBall.radius + s_rad/2){
			t_force[i]     = -K * ((BBall.radius + s_rad/2 - dis[i])) * (sp_info.grp_pos[i] - sp_info.ball_pos).unit();
			t_force[i].Y() = -t_force[i].Y();	// 座標変換
			hold[i]  = 1;
			if(i == 0) holdHand = 0;
			else       holdHand = 1;
		}
		//force発生は、条件によって
	/*	if(bMinForce == 1) h_spidar[i].SetMinForce();
		else               h_spidar[i].SetForce(t_force[i]);*/
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
		sp_info.initBallPole.X() = BBall.radius + s_rad/2;
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
	
	if (BBall.new_pos.y <= BBall.RealHeight) 
		BBall.new_pos.y =BBall.RealHeight;
	//BBall.init_pos = BBall.new_pos;
	
	float dis   = (sp_info.grp_pos[!holdHand] - sp_info.ball_pos).norm(); 
	float t_dis = (sp_info.grp_pos[0] - sp_info.grp_pos[1]).norm();

	h_spidar[0].SetMinForce();
	h_spidar[1].SetMinForce();

	if(dis < BBall.radius + s_rad/2){
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
	
	if (BBall.new_pos.y <= BBall.RealHeight) 
		BBall.new_pos.y =BBall.RealHeight;

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
		if (Test_Mode == 1)
		{
			h_spidar[0].SetForce(t_force[0]);
		    h_spidar[1].SetForce(t_force[1]);
		}
	}

	for(int i = 1; i < LogNum; i++) sp_info.BallPosLog[i] = sp_info.BallPosLog[i-1];
	sp_info.BallPosLog[0] = sp_info.ball_pos;
	Vec3f ballSpeed = (sp_info.BallPosLog[0] - sp_info.BallPosLog[4]) * 1000 / SpidarUpdateRate;
    sp_info.speed   = ballSpeed.norm();

	if(sp_info.speed > BallSpeedThreshold && ballSpeed.Z() > BallSpeedThreshold * sin(M_PI / 4))
	{

		//BBall.shot_pos.x = sp_info.ball_pos.X();
		//BBall.shot_pos.y = sp_info.ball_pos.Y();
		//BBall.shot_pos.z = sp_info.ball_pos.Z();
        shot_control();

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
		sp_info.initBallPole.X() = BBall.radius + s_rad/2;
		sp_info.initBallPosition = convertPoleToEuclid(sp_info.initBallPole);
	}
}

/* If BallState is Fixed (Throwing or Falling) */
/*void TGLView::BallIsFixed(){
	Vec3f t_force[2];
	float dis[2];

	for(int i = 0; i < 2; i++){
		dis[i] = (sp_info.grp_pos[i] - sp_info.ball_pos).norm();
		if(dis[i] < BBall.radius + s_rad/2){
			t_force[i]     = -K * ((BBall.radius + s_rad/2 - dis[i])) * (sp_info.grp_pos[i] - sp_info.ball_pos).unit();
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
			t_pos[i]  =h_spidar[i].GetPos() * convert_scale;
		}
		//setting ball
		sp_info.ball_pos.X() = BBall.new_pos.x;
		//200410.20
		if (BBall.new_pos.y <= BBall.RealHeight) 
			BBall.new_pos.y =BBall.RealHeight;
		sp_info.ball_pos.Y() = BBall.new_pos.y;
		sp_info.ball_pos.Z() = BBall.new_pos.z;

		//spidarCheck();
		if(fabs(t_pos[0].X()) <= 100 && fabs(t_pos[1].X()) <= 100){
			for (int i = 0; i < 2; i++)
			{
				sp_info.grp_pos[i].X() = sp_info.user_hand[i].X() - t_pos[i].X();
				sp_info.grp_pos[i].Y() = sp_info.user_hand[i].Y() + t_pos[i].Y();
				sp_info.grp_pos[i].Z() = sp_info.user_hand[i].Z() - t_pos[i].Z(); //　x,z sign , inverse
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
			/*if(BBall.chk_shoot == REFLEXED){
				BBall.ReturnHand = (sp_info.grp_pos[0] + sp_info.grp_pos[1]) / 2;
				BBall.RetAngle(BBall.new_vel.y, off_v);
				BBall.chk_shoot = NOSHOOT;
				//time reset
				dt=0.0;
			}*/
		}
	}
}
// initial ball setting
void TGLView::init_paddle()
{
   paddle_v.x= 15;
   paddle_v.y=nRange+GnHeight; //nHeight-nRange; //0.88 m (2.8864) ~ 1m 1m=3.28feet ;30inch
   paddle_v.z=-115;//about 2.9 m 9.512 feet 115 inch
 
}
void TGLView::init_hand()
{
    //SPIDAR-H
	sp_info.user_hand[0].X() = 0.0;
	sp_info.user_hand[0].Y() = nRange;
	sp_info.user_hand[0].Z() = paddle_v.z;
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
   
   MyVector init_range;
   init_range.x = -5;
   init_range.y = nRange;
   init_range.z = -115;
   //ball reset
   BBall.Init(init_range, GnHeight);
   
   //mydb.vh.Anim.init();		
   reflex_v.x=0.0;
   reflex_v.y=0.0;
   reflex_v.z=0.0;
   //init 
   bHCollide = false;
   bVCollide = false;
}

//ball collision detect
void TGLView::ballColliding(int cur_frm)
{
  float distance = JOINT_RAD+BBall.radius*10;
  MyVector l_pos, r_pos;

  l_pos = mydb.vh.MModel.LA_bone[3].position_global[cur_frm];
  r_pos = mydb.vh.MModel.RA_bone[3].position_global[cur_frm];
  //mydb.vh.Ret_Global(cur_frm, 130, 3);
  //r_pos = //mydb.vh.Ret_Global(cur_frm, 140, 3);
  
  avg_epos.x=(l_pos.x+r_pos.x)/2;
  avg_epos.y=(l_pos.y+r_pos.y)/2;
  avg_epos.z=(l_pos.z+r_pos.z)/2;

  float left_rad = MyVectorDistance(BBall.new_pos, l_pos);
  float right_rad = MyVectorDistance(BBall.new_pos, r_pos);
  float t_rad = MyVectorDistance(BBall.new_pos, avg_epos);
 
  if (t_rad < distance || left_rad < distance || right_rad < distance)
  {
	  if(mydb.vh.Anim.Off_frm > 1) 
	  {
		  bVCollide= true; // 
		  BBall.chk_state = CATCHING;
	
		  int off_frm = mydb.vh.Anim.Off_frm;
		  //reflect start point
		  dt = 0.0; //time seeting 2003．10/24
	      off_v= mydb.vh.Ret_Global(off_frm, 100, 3);
	
	  	  if (cpNum == SOUND_PC) 
		  {
			  Play_wave("sound/floor_bounce.wav"); //sound/
			  //少し遅いので、今回はやめる　2004.10.21
			  // PlaySound("sound/Noise.wav", NULL, SND_ASYNC|SND_LOOP);
		 }
	  }
  }  

}

bool TGLView::bboxColliding()
{

	double dis=0.0;

	if (BBall.new_pos.x < mydb.vh.MModel.A_bone[0].bbox[0])
	    //dis = (BBall.new_pos.x - A_bone[0].center.x)*(BBall.new_pos.x - A_bone[0].center.x);
		dis = (BBall.new_pos.x - mydb.vh.MModel.A_bone[0].bbox[0])*(BBall.new_pos.x - mydb.vh.MModel.A_bone[0].bbox[0]);
    else if (BBall.new_pos.x > mydb.vh.MModel.A_bone[0].bbox[3]) 
		dis = (BBall.new_pos.x - mydb.vh.MModel.A_bone[0].bbox[3])*(BBall.new_pos.x - mydb.vh.MModel.A_bone[0].bbox[3]);
	
	if (BBall.new_pos.y < mydb.vh.MModel.A_bone[0].bbox[1])
		dis += (BBall.new_pos.y - mydb.vh.MModel.A_bone[0].bbox[1])*(BBall.new_pos.y - mydb.vh.MModel.A_bone[0].bbox[1]);
    else if (BBall.new_pos.y > mydb.vh.MModel.A_bone[0].bbox[4]) 
		dis += (BBall.new_pos.y - mydb.vh.MModel.A_bone[0].bbox[4])*(BBall.new_pos.y - mydb.vh.MModel.A_bone[0].bbox[4]);

	if (BBall.new_pos.z < mydb.vh.MModel.A_bone[0].bbox[2])
		dis += (BBall.new_pos.z - mydb.vh.MModel.A_bone[0].center.z)*(BBall.new_pos.z - mydb.vh.MModel.A_bone[0].center.z);
		//dis += (BBall.new_pos.z - A_bone[0].bbox[2])*(BBall.new_pos.z - A_bone[0].bbox[2]);
    else if (BBall.new_pos.z > mydb.vh.MModel.A_bone[0].bbox[5]) 
		//dis += (BBall.new_pos.z - A_bone[0].bbox[5])*(BBall.new_pos.z - A_bone[0].bbox[5]);
		dis += (BBall.new_pos.z - mydb.vh.MModel.A_bone[0].center.z)*(BBall.new_pos.z - mydb.vh.MModel.A_bone[0].center.z);

    double rad = BBall.radius * BBall.radius;
 
	return dis<=rad;

}

// after databse connection, table open
void TGLView::conn_motion(int status_type)
{
   char ssql[80];
   int maxrows;
   
   MyVector temp_v;
   mydb.vh.Anim.init();
   
   mydb.vh.Anim.Key_time = BBall.WhatTime(-1.5, -5.0, 5.0); //distance => frame
   temp_v= BBall.Parabola(BBall.angle, mydb.vh.Anim.Key_time);
   mydb.vh.Anim.Key_frm = mydb.vh.Anim.Key_time/mydb.vh.Anim.unit_Time;
//key_frame ; db内でKEY//////////////////////////////////////////////
   mydb.param.pos[0]=temp_v.x;
   mydb.param.pos[1]=temp_v.y;
   mydb.param.pos[2]=temp_v.z;
   mydb.param.key_frm = mydb.vh.Anim.Key_frm;

  // mydb.param.key_type = status_type;
//////////////////////////////////////////////////////////////////////
//varous pattern
     //resetting
   m_StartTime = timeGetTime();

	strcpy(ssql, mydb.make_sql(mydb.vh.Anim.Key_frm, mydb.param.pos));
	maxrows =mydb.sqlexec(ssql); //if it has catch motion, check 
	if (maxrows > 0) mydb.vh.Anim.S_status = CATCHSUCCESS;
	else mydb.vh.Anim.S_status = CATCHFAIL;

		 //ELAPSED TIME   
   m_ElapsedTime = timeGetTime();
   printf( "VMSQL access time: %d, \t%u (ms)\n", mydb.vh.Anim.first_type, m_ElapsedTime-m_StartTime );

	transition_motion(status_type);
	mydb.vh.setup_endeffector(SKULL, mydb.vh.Anim.Frame_num); 

}

void TGLView::transition_motion(int s_type)
{
 char ssql[80];

  
	switch(s_type)
	{
	case READY_PHASE:
		mydb.vh.Anim.Off_frm =1;
		mydb.vh.Anim.second_type = 0;
	
		mydb.vh.Anim.first_type = FREE_MOVEMENT;
		mydb.vh.Anim.S_status = NOEVENT;
		
        //mydb.vh.Anim.conv_rot.y = 0;
		//mydb.vh.Anim.conv_trans.Reset();			
        //rotation 
	  	mydb.vh.Anim.Frame_num = mydb.vh.dfmotion.total_frm[0];
		mydb.vh.Anim.Pre_frm = mydb.vh.dfmotion.total_frm[0]; 
		mydb.vh.move_motion(0,mydb.vh.Anim.Frame_num);
		
		break;
	case CATCH_PHASE:
		//Data searched	
		  if (mydb.vh.Anim.S_status == CATCHSUCCESS)
		  {
				strcpy(ssql, mydb.make_sql(CATCH_PHASE));
				mydb.sqlexec(1, ssql);
				strcpy(ssql, mydb.make_sql(THROW_PHASE));
				mydb.sqlexec(mydb.vh.Anim.Pre_frm, ssql);
	
			  // first frame 
			  // mydb.vh.MModel.Com_Interp(tt, tt-1);
				mydb.vh.setup_endeffector(LEFT_ARM, mydb.vh.Anim.Frame_num);
				mydb.vh.setup_endeffector(RIGHT_ARM, mydb.vh.Anim.Frame_num);
		   }
		break;
	case MISS_PHASE:
		mydb.vh.Anim.Off_frm =1;
		mydb.vh.Anim.second_type = 0;

		if (mydb.vh.Anim.S_status == CATCHMISSING)
		{
			if (BBall.Fall_pos.X() < -10 ) mydb.vh.Anim.first_type = CATCHMISS_LEFT; 
			else mydb.vh.Anim.first_type = CATCHMISS_RIGHT; 
			
			mydb.vh.Anim.S_status = NOEVENT;
           if (mydb.vh.Anim.first_type == CATCHMISS_LEFT)
		   {
		   	 	mydb.vh.Anim.Frame_num = mydb.vh.dfmotion.total_frm[1];
				mydb.vh.Anim.Pre_frm = mydb.vh.dfmotion.total_frm[1]; 
				mydb.vh.move_motion(1,mydb.vh.Anim.Frame_num);
		   }else {
			    mydb.vh.Anim.Frame_num = mydb.vh.dfmotion.total_frm[2];
				mydb.vh.Anim.Pre_frm = mydb.vh.dfmotion.total_frm[2]; 
				mydb.vh.move_motion(2,mydb.vh.Anim.Frame_num);
		   }
		}
		
	break;
	}

  // RESETTING //////////////////////////////
   BBall.chk_shoot = NOSHOOT;
   mydb.vh.Anim.cur_frm = 0;
 /////////////////////////////////////////////


/*   /////////For experiment///////////////
    int cnt = Test_Mode* 5 + exp_data.Seq-1;
    if( mydb.vh.Anim.first_type != FREE_MOVEMENT)
	{
		exp_data.motion_id[cnt]= mydb.vh.Anim.first_type;
        exp_data.success[cnt] = mydb.vh.Anim.S_status;
	}*/
    ////////////////////////////////////
}

void TGLView::drawjoint(int cur_frm, MyBone sk_bone)
{

// Drawing joint ball/join
 	if (sk_bone.id == Hips){ //root translate
        glTranslatef(sk_bone.trans[cur_frm].x, sk_bone.trans[cur_frm].y, sk_bone.trans[cur_frm].z);
	}
	else{ // all joint except for root
	//
      render.Draw_Ellipse(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z, sk_bone.scale);
      glTranslatef(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z);
    }
	
	glRotatef(sk_bone.rot[cur_frm].z, 0.0, 0.0, 1.0);  //z
	glRotatef(sk_bone.rot[cur_frm].x, 1.0, 0.0, 0.0);  //x
	glRotatef(sk_bone.rot[cur_frm].y, 0.0, 1.0, 0.0);  //y

	if (MultiPerson)
	  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);//AMBIENT_AND
	else
	{
	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bodycolor);//AMBIENT_AND
      glMaterialfv(GL_FRONT, GL_SPECULAR, red);
      glMaterialf(GL_FRONT, GL_SHININESS, 1.5);
	}

	 if(sk_bone.id == Head)
	 {
		 //DrawPlayMode(mydb.vh.Anim.S_status);
		   
			glEnable(GL_LIGHTING);
		    glutSolidSphere(HEAD_RAD, 15, 12); 
			glPushMatrix();
		      render.mySolidCylinder(1.5, 1.5f, 3.0f, 0.5, 32);
		  	glPopMatrix();
		
	 }
	 else 
	 {
	     glDisable(GL_TEXTURE_2D);
		 glutSolidSphere(JOINT_RAD, 15, 12); // joint ball drawing
		
		if (sk_bone.id == RightAnkle || sk_bone.id == LeftAnkle ) // foot box drawing
		{
			   glPushMatrix();
				glTranslatef(0, 0, -1.5);
				glRotatef(-90, 0 ,1,0); 
				render.drawQuad(6.0, 1.0);
			   glPopMatrix();
		 }
		 else if (sk_bone.id== LeftWrist || sk_bone.id== RightWrist ) //hand end_site drawing
		 {
			 MyVector m_size(0.75, 0.0, 0.75);
			  glPushMatrix();
				glScalef(1.0, 1.0, 2.0);
				render.Draw_Ellipse(0, -3.14, 0, m_size);
			  glPopMatrix();
		 }
	  }

}
// all body drawing
void TGLView::drawBody_other(int idx, int cur_frame)
{
glPushMatrix();
	
     drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].A_bone[0]); //1
    
		glPushMatrix();
			drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].LL_bone[0]); //2
			drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].LL_bone[1]); //3
			drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].LL_bone[2]); //4
		glPopMatrix();

		glPushMatrix();
			drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].RL_bone[0]); //5
			drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].RL_bone[1]); //6
			drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].RL_bone[2]); //7
		glPopMatrix();

	    glPushMatrix();
			drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].A_bone[1]); //8
			drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].A_bone[2]); //9
	
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].LA_bone[0]); //10
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].LA_bone[1]);//11
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].LA_bone[2]);  //12
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].LA_bone[3]);  //13
			glPopMatrix();
		
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].RA_bone[0]);  //14
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].RA_bone[1]);  //15
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].RA_bone[2]);   //16
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].RA_bone[3]);   //17
		    glPopMatrix();

			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].H_bone[0]);  //18
				drawjoint(cur_frame, mydb.vh.dfmotion.d_motion[idx].H_bone[1]);  //19
			glPopMatrix();

	glPopMatrix();

glPopMatrix();

}

// all body drawing
void TGLView::drawBody(int cur_frame)
{
glPushMatrix();
	
     drawjoint(cur_frame, mydb.vh.MModel.A_bone[0]); //1
    
		glPushMatrix();
			drawjoint(cur_frame, mydb.vh.MModel.LL_bone[0]); //2
			drawjoint(cur_frame, mydb.vh.MModel.LL_bone[1]); //3
			drawjoint(cur_frame, mydb.vh.MModel.LL_bone[2]); //4
		glPopMatrix();

		glPushMatrix();
			drawjoint(cur_frame, mydb.vh.MModel.RL_bone[0]); //5
			drawjoint(cur_frame, mydb.vh.MModel.RL_bone[1]); //6
			drawjoint(cur_frame, mydb.vh.MModel.RL_bone[2]); //7
		glPopMatrix();

	    glPushMatrix();
			drawjoint(cur_frame, mydb.vh.MModel.A_bone[1]); //8
			drawjoint(cur_frame, mydb.vh.MModel.A_bone[2]); //9
	
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.MModel.LA_bone[0]); //10
				drawjoint(cur_frame, mydb.vh.MModel.LA_bone[1]);//11
				drawjoint(cur_frame, mydb.vh.MModel.LA_bone[2]);  //12
				drawjoint(cur_frame, mydb.vh.MModel.LA_bone[3]);  //13
			glPopMatrix();
		
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.MModel.RA_bone[0]);  //14
				drawjoint(cur_frame, mydb.vh.MModel.RA_bone[1]);  //15
				drawjoint(cur_frame, mydb.vh.MModel.RA_bone[2]);   //16
				drawjoint(cur_frame, mydb.vh.MModel.RA_bone[3]);   //17
		    glPopMatrix();

			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.MModel.H_bone[0]);  //18
				drawjoint(cur_frame, mydb.vh.MModel.H_bone[1]);  //19
			glPopMatrix();

	glPopMatrix();

glPopMatrix();

}

void TGLView::Processing_shadow(Affinef afCam)
{
		////////////////////////////shadowの処理////////////////////////////////
	
	//光源からシーンを描画する為の設定を行う
	Shadow->makeSceneWithShadow1();

		//光源からシーンを描画
		renderScene();
	
	
	//描画結果をテクスチャに保存し、viewportを元に戻す
	Shadow->makeSceneWithShadow2();
	
		//射影行列、モデルビュー行列なども元に戻す
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(eyeView[0], eyeView[1], eyeView[2], eyeView[3]);

	    glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
	
			
		//afCamは以前定義しているので、Loadしさえすればよい
		glLoadMatrixf(afCam);

		//glLightfv(GL_LIGHT0, GL_POSITION, light0pos);

	
	//テクスチャステージを１に設定し、ｇｌの拡張命令などの設定をし、
	//テクスチャ行列を生成している
	Shadow->makeSceneWithShadow3();
		//以下の２つの生かすと、明らかに結果がおかしくなる
		//glTranslatef(0, -nHeight, mydb.vh.Anim.camera.z);
		//glRotatef(mydb.vh.Anim.camera.y, 0.0f, 1.0f, 0.f);


		//MODULATEは乗算、DECALはテクスチャの色が反映されない、REPLACEはテクスチャ色のみが出力される。だと思った。 
		//glActiveTextureARB(GL_TEXTURE0_ARB);
		
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		//glMatrixMode(GL_MODELVIEW);
		
		//その設定のまま、シーンを描画
		renderScene();

	//後処理。テクスチャ行列や２次元テクスチャの無効化をする
	Shadow->makeSceneWithShadow4();
	
	/////////////////////////////////shadowの処理/////////////////////////////////////
}

void TGLView::renderScene(void)
{

  glPushMatrix();
     glTranslatef(0, -nHeight, mydb.vh.Anim.camera.z);
	 glRotatef(mydb.vh.Anim.camera.y, 0.0f, 1.0f, 0.0f);
	 
    //glEnable(GL_LIGHTING);
	 
  //hip -5 
	if (VH_Mode == 1)
       drawBody(mydb.vh.Anim.cur_frm);
	else if (VH_Mode == 0)
	   drawHand(mydb.vh.Anim.cur_frm);

	 if (MultiPerson)
	 {
	   glPushMatrix();
         glTranslatef(-100, 0, 100);
	     drawBody_other(Crowd_Num, mydb.vh.dfmotion.cur_frm[3]);
		glPopMatrix();

		glPushMatrix();
         glTranslatef(-80, 0, 100);
         drawBody_other(Crowd_Num+1, mydb.vh.dfmotion.cur_frm[4]);
		glPopMatrix();

		glPushMatrix();
         glTranslatef(100, 0, 200);
		 glRotatef(90, 0.0f, 1.0f, 0.0f);
         drawBody_other(Crowd_Num+2, mydb.vh.dfmotion.cur_frm[5]);
		glPopMatrix();
	 } 

	//床の描画　by武山さん
    /*  glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	      glBindTexture(GL_TEXTURE_2D, render.tex[0]); //tex[1]
		  glEnable(GL_TEXTURE_2D);

	      glScalef(100.0, 1.0, 100.0); 
		  glTranslatef(-2, GnHeight, -0.5);
		  glBegin(GL_POLYGON);
		  glTexCoord2f(-150.0f, 0.5f);
		  glVertex3d(-150.0, 1.0, -150.0);
		  glTexCoord2f(0.5f, 150.0f);
		  glVertex3d(-150.0, 1.0, 150.0);
		  glTexCoord2f(150.0f, 0.5f);
		  glVertex3d(150.0, 1.0, 150.0);
		  glTexCoord2f(0.5f, -150.0f);
		  glVertex3d(150.0, 1.0, -150.0);
		  glEnd();
		  glDisable(GL_TEXTURE_2D);
	  glPopMatrix();*/

	  //SPIDAR-H HAND Position
		glPushMatrix(); //left
	   	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(sp_info.grp_pos[0].X(),  sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z());
	 		if (BBall.chk_state == HOLDING || BBall.chk_state == HALFHOLD ) 
				glutWireSphere(s_rad, 15, 12);
			else
				glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

       glPushMatrix(); //right
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(sp_info.grp_pos[1].X(),  sp_info.grp_pos[1].Y(), sp_info.grp_pos[1].Z());
	 		if (BBall.chk_state == HOLDING || BBall.chk_state == HALFHOLD ) 
				glutWireSphere(s_rad, 15, 12);
			else
				glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

	   /*DrawTextXY(200.0, 10, 30.0, 0.1,testchar);
	   _itoa( exp_data.Seq, buffer, 10);
	   DrawTextXY(250.0, 10, 30.0, 0.1, buffer);*/

	   //shadow
	//ＶＨのテクスチャの設定をするために、
	//ここでテクスチャ０を有効にする必要あり
	//ちなみに、「何も指定していない時は、全てテクスチャステージ０の設定として扱われる」
	//みたいな、便利な仕様にはなっていないっぽい。
	//引数はただの整数で大丈夫っぽい
	//glActiveTextureARB(0);
  if (doing_Shadow)	glActiveTextureARB(GL_TEXTURE0_ARB);
	//vrml start 
    drawBall();
//VRML END
	//shadow
	//ＶＨのテクスチャ設定が終わったので、テクスチャ１に設定を戻す。
	//glActiveTextureARB(GL_TEXTURE1_ARB);

	if (doing_Shadow) Shadow->renderLight();

	glPopMatrix();

}

void TGLView::OnDraw()
{
    //MPI
	// キーボード入力を送信
	MPI_Bcast(&press_key, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//次のフレームを送信
	if(myrank == 0) apply_send(mydb.vh.Anim.cur_frm);
	MPI_Bcast(&M_DAT, 1, stMove_Data, 0, MPI_COMM_WORLD);
	if(myrank != 0) apply_receive();
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();

//camera setting
	static Affinef R180(Rad(180),'y');
	
	Affinef afProjector = afBody * R180 * R * R180;
	Affinef afCam = (afEye * afProjector).inv();

	glLoadMatrixf(afCam);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
  
	if (doing_Shadow)
	{
	//Processing shadow
		Processing_shadow(afCam);
	//Rendering Scene()
	} else renderScene();

//MPI
	MPI_Barrier(MPI_COMM_WORLD);

glutSwapBuffers();		// Flip()を含む

}

void TGLView::DrawTextXY(double x,double y,double z,double scale,char *s)
{
   int i;

   glPushMatrix();
       //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	   glTranslatef(x,y,z);
	   glRotatef(180, 0.0, 1.0, 0.0);
	   glScalef(scale,scale,scale);
	   for (i=0;i<strlen(s);i++)
		  glutStrokeCharacter(GLUT_STROKE_ROMAN,s[i]);
   glPopMatrix();
}

void TGLView::drawBall()
{

//ytakeyama end
int tempflag = 0;

		for(TGeoNodes::iterator it = allNodes[0].begin(); it != allNodes[0].end(); ++it)
		{
			if(tempflag){
					(*it)->Draw();
			}else{
					//ball display
					glPushMatrix();
						glTranslatef(BBall.new_pos.x, BBall.new_pos.y, BBall.new_pos.z);
						glRotatef(nowBallPole.Y()*180/M_PI, 0.0, -1.0, 0.0);
						glRotatef(nowBallPole.Z()*180/M_PI, -cos(nowBallPole.Y()), 0.0, -sin(nowBallPole.Y()));
						(*it)->Draw();
					glPopMatrix();
					tempflag = 1;
				}
		}

}

void TGLView::drawHand(int cur_frm)
{

	    glEnable(GL_LIGHTING);
	    glEnable(GL_TEXTURE_2D);
	
		glPushMatrix();
		      glTranslatef(mydb.vh.MModel.A_bone[0].trans[cur_frm].x, mydb.vh.MModel.A_bone[0].trans[cur_frm].y, mydb.vh.MModel.A_bone[0].trans[cur_frm].z);
			  glScalef(5.0, 5.0, 5.0);
			  render.drawBox(); 
	    glPopMatrix();

		for(TGeoNodes::iterator it = allNodes[1].begin(); it != allNodes[1].end(); ++it)
		{	
				//lefthand display
			glPushMatrix();
			glTranslatef(mydb.vh.MModel.LA_bone[3].position_global[cur_frm].x, mydb.vh.MModel.LA_bone[3].position_global[cur_frm].y, mydb.vh.MModel.LA_bone[3].position_global[cur_frm].z);
			  //glTranslatef(sp_info.grp_pos[0].X(),  sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z());
			//glRotatef(180, 0.0, 1.0, 0.0);
			(*it)->Draw();
			glPopMatrix();
		}
		for(TGeoNodes::iterator it = allNodes[2].begin(); it != allNodes[2].end(); ++it)
		{
			//right display
			glPushMatrix();
			    //glTranslatef(sp_info.grp_pos[1].X(),  sp_info.grp_pos[1].Y(), sp_info.grp_pos[1].Z());
				glTranslatef(mydb.vh.MModel.RA_bone[3].position_global[cur_frm].x, mydb.vh.MModel.RA_bone[3].position_global[cur_frm].y, mydb.vh.MModel.RA_bone[3].position_global[cur_frm].z);
			//	glRotatef(180, 0.0, 1.0, 0.0);
				(*it)->Draw();
			glPopMatrix();
		}

}
////////////////////////////////////////////////
void TGLView::OnSize(int w, int h)
{
  GLfloat aspect;

  if(h == 0) { h=1;}

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  aspect =(float) w/h;
  //for VRML of Ytakeyama 10000以上
  //gluPerspective(fov_v, aspect, 1.0, 10000);

  ////////////////////////////////shadow//////////////////////////
  //shadow　視錘台がfloat型の４要素配列で欲しいので、変更しています。ogawa
  eyeView[0] = 80.0f;
  eyeView[1] = aspect;
  eyeView[2] = 1.0f;
  //eyeView[3] = 10000.0f;
  eyeView[3] = 1500.0f;

  winWidth = w;
  winHeight = h;

  gluPerspective(eyeView[0], eyeView[1], eyeView[2], eyeView[3]);
  Shadow->setWindowSize(&winWidth, &winHeight);
 ////////////////////////////////shadow//////////////////////////

  glMatrixMode(GL_MODELVIEW);

}

void TGLView::OnSpecialKey(int key, int x, int y) 
{
  if (key == GLUT_KEY_LEFT)    { mydb.vh.Anim.camera.y -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { mydb.vh.Anim.camera.y += deltaRot; }
  if (key == GLUT_KEY_UP)      { mydb.vh.Anim.camera.z  -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { mydb.vh.Anim.camera.z  += deltaRot; }
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
void TGLView::init_db(void)
{

		// MYDB Init connection
   int ret = mydb.sqlconn();
   if (ret == 1)
   {
	   init_paddle();
	   init_hand();
	   init_ball();	
    
	 // compute database search condition
	   mydb.load_model(1,3);//1
	   for (int i=0; i<3;i++)  mydb.load_motion(i);
	   //vh hand Zaxis position
	   transition_motion(READY_PHASE);

	 if (MultiPerson)
	 {
      //if model is same, it has to load in once,,,but anyway execute
	   for (int j=3; j<8;j++)  mydb.load_model(j,1);
	   for (int i=3; i<8;i++)  mydb.load_motion(i);
	 }	 
	   
   }
   
}

void TGLView::OnOpen(void)
{

  //InitCamera();	
	//	GLの初期化
 GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
 
 glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
 glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
 glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
 glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 // glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
 
 glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

//by ogawa
 //グローバル変数でもlight0posという４要素配列がなぜか宣言されていました。
 //どちらか１つしか必要ないので、こちらのlight0posを消しておきます。
 
 //GLfloat light0pos[] = {100.0, 1.0, 5.0, 1.0};	//shadow
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHTING);
 glLightfv(GL_LIGHT0, GL_POSITION, light0pos);		//shadow


  glClearColor(0.75, 0.75, 1.0, 0.0); //sky color
  //glClearColor(0.0, 0.0, 0.0, 0.0); //black
  glEnable(GL_DEPTH_TEST); //hidden surface removed

    ///////////////////////////////↓　shadow関係の初期化　↓/////////////////////////////
  glClearDepth(1.0);
   
  //後にOnSize()で、eyeViewの値は更新される
  eyeView[0] = 0;
  eyeView[1] = 0;
  eyeView[2] = 0;
  eyeView[3] = 0;
  
  //GLの拡張命令を使うために、以下を行う必要あり
 // SetupGL1_4();
 // SetupARBExtensions();
 // SetupWGLExtensions();

  //Shadowクラスのオブジェクトを生成
  //光源の位置が定義されている、４要素の配列をコピー
  Shadow = new CMySoftShadowMap;
  //jeong mainで初期化したので
  //Shadow->InitMultiTextures();

  Shadow->copyLightPos(light0pos);
  //Shadow->setWindowSize(&winWidth, &winHeight);
  
  Shadow->copyEyeView(eyeView);

  /////////////////////////////////↑　　　shadow　　　↑///////////////////////////////////////

 
  glFrontFace(GL_CCW); //no counterclockwise
  glEnable(GL_CULL_FACE); 
  glCullFace(GL_BACK);
 

  //2004.10.20
   init_ball();
// MYDB Init connection
   init_db();    
    
   render.Load_Texture();

   deltaRot = 5.0f;
   nRange = 25.0f; 
   nHeight = 45.0f; //45.0
	delta_Pad = 0.5f;
	GnHeight = 2.5f; //
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
 
   //2004.10.20 updated
   if (cpNum == SOUND_PC)
	PlaySound("sound/Bass.wav", NULL, SND_ASYNC|SND_LOOP); //Noise

   if (myrank == 0) 
	   //SPIDAR-H
		init_spidar();

  //timer setting	
	timeBeginPeriod(1);
	SpidarUpdateTimer = timeSetEvent(SpidarUpdateRate, 1, SpidarUpdateProc, (DWORD)this, TIME_PERIODIC);
	
	SetTimer(33, &TGLView::OnTimer); //Step()を呼び出す周期 33
  	dwTime = timeGetTime(); //GetTickCount();
//experiment
	exp_data.Seq = 0;	

}
/*
//---------------------------------------------------------------------------
void TGLView::::CheckBgm(void)//ループ演奏のチェック
{
char buf[256];
mciSendString("status bgm mode",buf,sizeof(buf),NULL);
AnsiString as1 = buf;
if(as1 == "stopped")
  {
   PlayBgm();
  }
}
*/
void TGLView::Ball_Control(int cur_frm)
{

		if (bVCollide) 
		{
			BBall.new_pos = mydb.vh.Ret_Global(cur_frm, 100, 3); //index =3
			
		}
		else if (bHCollide) 
		//else
		{   //HOLDING or HALFHOLD
			//BBall.new_pos =BBall.init_pos;
			BBall.new_pos.x =sp_info.ball_pos.X();
			BBall.new_pos.y =sp_info.ball_pos.Y();
			BBall.new_pos.z =sp_info.ball_pos.Z();
		}

		if (BBall.new_pos.y <= BBall.RealHeight) 
			BBall.new_pos.y =BBall.RealHeight;

}
void TGLView::Step()
{
	stepHz ++;
    //time distance 計測
	dwLastTime = dwTime;
	dwTime = timeGetTime();
	float pass = float(dwTime - dwLastTime) / 1000;
	 
	if (MultiPerson)
	{
		 if (animating) 
		 {
			for (int j=3; j<6;j++) 
			  {
				  mydb.vh.dfmotion.cur_frm[j]++;
				  if (mydb.vh.dfmotion.cur_frm[j] >= mydb.vh.dfmotion.total_frm[j]) mydb.vh.dfmotion.cur_frm[j] =0;
			  }
		 }
	}
	//MPI
	if(myrank == 0)
	{
		 if (animating) 
		  {
			  mydb.vh.Anim.cur_frm++;

			  if ( bVCollide && BBall.chk_shoot != REFLEXED && mydb.vh.Anim.Off_frm != 1 && mydb.vh.Anim.cur_frm == mydb.vh.Anim.Off_frm)
			  { 
				  BBall.chk_shoot = REFLEXED;
				  BBall.chk_state = RETURNING; //start move ball
				  bVCollide = false;
				  BBall.ReturnHand = (sp_info.grp_pos[0] + sp_info.grp_pos[1]) / 2;
				  BBall.RetAngle(BBall.new_vel.y, off_v);
				  BBall.chk_shoot = NOSHOOT;
				  //time reset
				  dt=0.0;
			  }
			//Jeong10/24 
			  switch(BBall.chk_state)
			  {
				case THROWING:
			   //!bHCollide &&
			  	  BBall.new_pos = BBall.Parabola(BBall.angle, t);
				  if (t>=1 && BBall.minY <= GnHeight || BBall.new_pos.z > 100){
					 init_ball();
				  }
				     t +=mydb.vh.Anim.unit_Time;
				break;
				case FALLING:
			  
		      	  BBall.new_pos = BBall.Fall_Roll(BBall.new_vel.y,ft);
				   if(BBall.bounce <.1)
				   {
				       //init_ball();
					   BBall.chk_state = PAUSE;
				   }
				   ft +=((double)mydb.vh.Anim.unit_Time/2);
			  	break;
			   case RETURNING:	
			  //!bVCollide && 
				if( BBall.chk_shoot == REFLEXED && mydb.vh.Anim.cur_frm > mydb.vh.Anim.Off_frm)
						BBall.chk_shoot = NOSHOOT;

				  BBall.new_pos = BBall.Reflect(dt, off_v);
				 if (BBall.minZ < -200 || BBall.minY <= GnHeight) 
					 init_ball();
				 dt +=(mydb.vh.Anim.unit_Time); //*2
			 	break;
			  case REPULSE: 
	 			if ( BBall.minY < GnHeight) 
				{
				   BBall.new_pos.y = BBall.radius *2;
				   BBall.chk_state = PAUSE;
				   if (BBall.minZ > 0) init_ball();
				}
				else
				{
					dt +=mydb.vh.Anim.unit_Time; 
					BBall.new_pos = BBall.init_pos + BBall.new_vel*dt;
					BBall.new_pos.y= BBall.init_pos.y +(BBall.new_vel.y*dt-0.5*g*dt*dt);
					BBall.minY =BBall.new_pos.y - BBall.radius;
				}
			    break;
			  }
			 if (BBall.chk_shoot == SHOOTED ) 
			 {
				 conn_motion(CATCH_PHASE); 
				 BBall.chk_shoot = NOSHOOT;
				 bHCollide = false;
				 //bVCollide = false;
				 t=0.0; //resetting time
				 if (cpNum == SOUND_PC) 
				    Play_wave("sound/ball_shoot.wav");
				  
			 	 
			 }
			 if (BBall.chk_shoot == NOSHOOT && mydb.vh.Anim.cur_frm >= mydb.vh.Anim.Frame_num ) 
			 {
                 //if any event isn't 
				 //init_ball();
				 //MPI communication 
                 //End_Interp(cur_frm-1); // 0~ frame_num-1
				 //10/26
				 mydb.vh.Anim.init();
				 transition_motion(READY_PHASE); 
			  }
		 //virtual human hand and ball collision
		 if (!bVCollide && BBall.chk_state == THROWING && mydb.vh.Anim.cur_frm >= mydb.vh.Anim.Key_frm-5) 
		 {
			 ballColliding(mydb.vh.Anim.cur_frm);
			 if (mydb.vh.Anim.Off_frm > 1 && mydb.vh.Anim.cur_frm >= mydb.vh.Anim.Key_frm+15&& !bVCollide)
			 {
				 mydb.vh.Anim.S_status = CATCHMISSING;
				 mydb.vh.Anim.init();
				 transition_motion(MISS_PHASE); 
				 if (cpNum == SOUND_PC) Play_wave("sound/Aho.wav");
			 }
		 }

		if (!bVCollide && BBall.chk_state != REPULSE && BBall.chk_shoot != REFLEXED)
			{
		 	 bool chk_bbox=bboxColliding();
			 if (chk_bbox) 
			 {
				 BBall.chk_state = REPULSE;
				 BBall.collision_repulse();
				 BBall.init_pos = BBall.new_pos;
				 t=0.0;
				 dt=0.0;
			 }
		 }
		 //setting
		 Ball_Control(mydb.vh.Anim.cur_frm);
		
	 } //animating
	}  //server only 実行
 
	if (bDraw) Invalidate();			//	描画要求
}

void TGLView::PrintHz(std::ostream& os) const{
	os << stepHz << "," << timeGetTime() << std::endl;
}

//save text file
void TGLView::spidar_save()
{
	FILE *stream;
    char f_name[256];
	char buffer[10];
   
	//int ex_time = GetDateFormat(LOCALE_SYSTEM_DEFAULT, NULL,DATE_SHORTDATE,NULL, "yyMMdd", buffer, sizeof(buffer));
    int extime = GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, NULL, "HHmmss", buffer, sizeof(buffer));
	strcpy(f_name, "./data/");
	strcat(f_name, buffer);
	strcat(f_name, ".txt");

   /* text mode */
   if( (stream = fopen( f_name, "w+t" )) != NULL )
   {
	for (int i = 0; i < 2; i++ )
	{
      for (int j = 0; j < NUM_OF_TEST; j++ )
	  {
		 fprintf(stream, "%d, %f, %f, %f, %f", exp_data.Exp_Mode[i][j], exp_data.shotpos[i][j].X(), exp_data.shotpos[i][j].Y(), exp_data.shotpos[i][j].Z());
		 fprintf(stream, "%d, %d, %d, %d\n", exp_data.shotvelocity[i][j], exp_data.motion_id[i][j], exp_data.success[i][j], exp_data.start_time[i][j],exp_data.elasped_time[i][j]);
	  }
	 }
      //printf( "world_postion text file is saved! \n" );
     fclose( stream );
   }
   else   printf( "Open of file is failed! \n" );

   text_clear();

}

void TGLView::text_clear()
{
	  for (int i = 0; i < 2; i++ )
	  {
		 for (int j = 0; j < NUM_OF_TEST; j++ )
		{
		  exp_data.shotpos[i][j].X() = 0.0;
		  exp_data.shotpos[i][j].Y() = 0.0;
		  exp_data.shotpos[i][j].Z() = 0.0;
		  exp_data.shotvelocity[i][j] = 0;
		  exp_data.motion_id[i][j] = 0;
		  exp_data.success[i][j] = 0; 
		  exp_data.start_time[i][j] = 0.0;
		  exp_data.elasped_time[i][j] =0.0;
		  }
	  }
}
void TGLView::apply_send(int cur_frm)
{
	//MPI
	int idx=0;
	MyVector ret;
	if(cur_frm == 1)
	{
		//apply share struct from server struct
		M_DAT.Anim_Set[0] = mydb.vh.Anim.first_type;
		M_DAT.Anim_Set[1] = mydb.vh.Anim.second_type;
		M_DAT.Anim_Set[2] = mydb.vh.Anim.Frame_num;
		M_DAT.Anim_Set[3] = mydb.vh.Anim.Key_frm;
		M_DAT.Anim_Set[4] = mydb.vh.Anim.Off_frm;
		M_DAT.Anim_Set[5] = mydb.vh.Anim.Pre_frm;
		M_DAT.Anim_Set[6] = mydb.vh.Anim.S_status;
 	}
	M_DAT.Anim_Set[7] = mydb.vh.Anim.cur_frm;
	M_DAT.Cur_Bone[0] = mydb.vh.MModel.A_bone[0].trans[cur_frm].x;
    M_DAT.Cur_Bone[1] = mydb.vh.MModel.A_bone[0].trans[cur_frm].y;
    M_DAT.Cur_Bone[2] = mydb.vh.MModel.A_bone[0].trans[cur_frm].z;
    idx=3;
	for(int i=0;i<6;i++)
	{
		int grp_id = i*10+100;
		int Max_idx= GRP_IDX[i];
		for(int j=0;j<Max_idx;j++)
		{
			int kk = 3*j;
			ret=mydb.vh.Ret_Array(cur_frm, grp_id, j);
            M_DAT.Cur_Bone[idx+kk] =ret.x;
            M_DAT.Cur_Bone[idx+kk+1] =ret.y;
            M_DAT.Cur_Bone[idx+kk+2] =ret.z;
		}
		idx = idx + 3*Max_idx;
	}

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
//add 2004/02/15
	M_DAT.mode = MultiPerson;
    M_DAT.vcamera[0] = mydb.vh.Anim.camera.x;
	M_DAT.vcamera[1] = mydb.vh.Anim.camera.y;
	M_DAT.vcamera[2] = mydb.vh.Anim.camera.z;
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

    mydb.vh.Anim.cur_frm = M_DAT.Anim_Set[7];
    if(mydb.vh.Anim.cur_frm == 1)
	{
   		//apply my struct from shared structure
		mydb.vh.Anim.first_type = M_DAT.Anim_Set[0];
		mydb.vh.Anim.second_type = M_DAT.Anim_Set[1];
		mydb.vh.Anim.Frame_num = M_DAT.Anim_Set[2];
		mydb.vh.Anim.Key_frm = M_DAT.Anim_Set[3];
		mydb.vh.Anim.Off_frm = M_DAT.Anim_Set[4];
		mydb.vh.Anim.Pre_frm = M_DAT.Anim_Set[5];
		mydb.vh.Anim.S_status = M_DAT.Anim_Set[6];
	}

	mydb.vh.MModel.A_bone[0].trans[mydb.vh.Anim.cur_frm].x = M_DAT.Cur_Bone[0];
    mydb.vh.MModel.A_bone[0].trans[mydb.vh.Anim.cur_frm].y = M_DAT.Cur_Bone[1];
    mydb.vh.MModel.A_bone[0].trans[mydb.vh.Anim.cur_frm].z = M_DAT.Cur_Bone[2];

    int idx=3;
	MyVector apply_vec;
	for(int i=0;i<6;i++)
	{
		int grp_id = i*10+100;
		int Max_idx= GRP_IDX[i];
		for(int j=0;j<Max_idx;j++)
		{
			int kk = 3*j;
			apply_vec.x = M_DAT.Cur_Bone[idx+kk];
            apply_vec.y = M_DAT.Cur_Bone[idx+kk+1];
            apply_vec.z = M_DAT.Cur_Bone[idx+kk+2];
			mydb.vh.input_motion(apply_vec, grp_id, j, mydb.vh.Anim.cur_frm);
		}
		idx = idx + 3*Max_idx;
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

	//fprintf(stderr, "spidar pos:%f, %f, %f \n",sp_info.grp_pos[0].X(), sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z()); 
		
	 nowBallPole.X() = M_DAT.RotBall[0];
     nowBallPole.Y() = M_DAT.RotBall[1];
	 nowBallPole.Z() = M_DAT.RotBall[2];

	 //add 2004/02/15
	 MultiPerson = M_DAT.mode;
     mydb.vh.Anim.camera.x = M_DAT.vcamera[0];
	 mydb.vh.Anim.camera.y = M_DAT.vcamera[1];
	 mydb.vh.Anim.camera.z = M_DAT.vcamera[2];

}
