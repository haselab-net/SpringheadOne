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
#include "communication.h"

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
#include "tex.h"      // primitive definition
#include "ball.h"

//MY ODBC
Cdirexec mydb;
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

//ball parabola /collision detect variable
static MyVector avg_epos;
static MyVector paddle_v;
static MyVector reflex_v;
static MyVector off_v;
static double t=0.0;
static double dt=0.0;
//////////////////////////////////////////////////////
//SHADOW
static double ShadowMat[4][4];
static double ShadowMat2[4][4];				//shadow
//static double ShadowMat3[4][4];				//shadow

/////////////////////////////////////////////////////////
using namespace std;
const float EYEOFFSET=0.8;

//SLCamera
int MainAngleSide = 0;
int MainAngleVert = 0;
bool StateCamera = false;
bool StateChange = false;
//~SLCamera

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
	int counts[6]={10,60, 6, 3, 6, 3};
	
	MPI_Datatype type[6]={MPI_INT,MPI_FLOAT, MPI_FLOAT, MPI_FLOAT,MPI_FLOAT,MPI_FLOAT};

	MPI_Aint byte[6], extent;
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
    MPI_Type_extent(MPI_FLOAT,&extent);
	byte[5] = byte[4]+counts[4]*extent;

	MPI_Type_struct(6,counts,byte,type,&stMove_Data);
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

//SLCamera
//////////////////////////////////////////
bool TGLView::OnTimer2(){
	//slcamera.ShowData();
	//JEONG 10/21
	/*std::cout << mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].x << " " << BBall.new_pos.x <<std::endl;
	std::cout << mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].y << " " << BBall.new_pos.y <<std::endl;
	std::cout << mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].z << " " << BBall.new_pos.z <<std::endl;
	std::cout << "BallAngleY=" << BallAngleSide << "°" << std::endl;
	std::cout << "BallAngleX=" << BallAngleVert << "°" << std::endl; */
	
	return true;
}

//10/21
void TGLView::BallAngle(){
	BallDirection.X() = mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].x - BBall.new_pos.x;

	BallDirection.Y() = mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].y - BBall.new_pos.y;

	BallDirection.Z() = mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].y - BBall.new_pos.z;

	if(BallDirection.norm() != 0)    // 正規化
			BallDirection = BallDirection / BallDirection.norm();

	BallAngleSide = (int)((atan2(BallDirection.X(),BallDirection.Z())/PI1)*180);
	BallAngleVert = (int)((atan2(BallDirection.Y(),sqrt(BallDirection.X()*BallDirection.X()+
						BallDirection.Z()*BallDirection.Z()))/PI1)*180);


}

void TGLView::CheckEyeAngle(){
	if(MainAngleSide > 25)
	  MainAngleSide = 25;
	if(MainAngleSide < -25)
	  MainAngleSide = -25;
	if(MainAngleVert > 13)
	  MainAngleVert = 13*abs(MainAngleSide)*0.001;
	if(MainAngleVert < -13)
	  MainAngleVert = -13*abs(MainAngleSide)*0.001;
}

void TGLView::SelectAngle(){

	for(int i = 1; i < BallLogMax; i++)
		BallPosLog2[i] = BallPosLog2[i-1];
	BallPosLog2[0] = BBall.ball_spos;

	for(int i = 0; i < BallLogMax; i++){
		BallPosChange[i] = BallPosLog2[i] - BallPosLog2[i+1];
		BallPosChangeTotal += BallPosChange[i];
	}
	BallPosChangeTotal = BallPosChangeTotal/BallLogMax;
	if( BallPosChangeTotal.norm() > 0.2){
		MainAngleVert = BallAngleVert;
		MainAngleSide = BallAngleSide;
	}
	else{
		MainAngleVert = UserAngleVert;
		MainAngleSide = UserAngleSide;
	}
}

void TGLView::ChangeFace(){
	if(FlagFace == true)
		glBindTexture(GL_TEXTURE_2D, tex[10]);
	else
		glBindTexture(GL_TEXTURE_2D, tex[0]);
}


//~10/21
//////////////////////////////////////////

void TGLView::OnIdle(){
	//Sleep(5);
	// キーボードの処理はここで行う
	//::OnKeyboard(press_key, 0, 0);
	switch(press_key){
	 case 'w': //y
		  sp_info.grp_pos[0].Y() +=delta_Pad;
		  sp_info.grp_pos[1].Y() +=delta_Pad;
		  //BBall.init_pos.y = (sp_info.grp_pos[0].Y() + sp_info.grp_pos[1].Y())/2;
		  printf( "hand y pos: %f ball y: %f \n", sp_info.grp_pos[0].Y(), BBall.init_pos.y);
		  break;
	 case 's': 
		  sp_info.grp_pos[0].Y() -=delta_Pad;
		  sp_info.grp_pos[1].Y() -=delta_Pad;
		  //BBall.init_pos.y = (sp_info.grp_pos[0].Y() + sp_info.grp_pos[1].Y())/2;
		  printf( "hand y pos: %f ball y: %f \n", sp_info.grp_pos[0].Y(), BBall.init_pos.y);
		  break;
	 case 'a': //x
		  sp_info.grp_pos[0].X() +=delta_Pad;
		  sp_info.grp_pos[1].X() +=delta_Pad;
		  //BBall.init_pos.x = (sp_info.grp_pos[0].X() + sp_info.grp_pos[1].X())/2;
		  printf( "hand x pos: %f ball x: %f \n", sp_info.grp_pos[0].X(), BBall.init_pos.x);
		  break;
	 case 'd': 
		  sp_info.grp_pos[0].X() -=delta_Pad;
		  sp_info.grp_pos[1].X() -=delta_Pad;
		  //BBall.init_pos.x = (sp_info.grp_pos[0].X() + sp_info.grp_pos[1].X())/2;
		  printf( "hand x pos: %f ball x: %f \n", sp_info.grp_pos[0].X(), BBall.init_pos.x);
		  break;
	 case 'A': //z
		  sp_info.grp_pos[0].Z() +=delta_Pad;
		  sp_info.grp_pos[1].Z() +=delta_Pad;
		  //BBall.init_pos.z = (sp_info.grp_pos[0].Z() + sp_info.grp_pos[1].Z())/2;
		  printf( "hand z pos: %f ball z: %f \n", sp_info.grp_pos[0].Z(), BBall.init_pos.z);
		  break;
	 case 'D': 
		  sp_info.grp_pos[0].Z() -=delta_Pad;
		  sp_info.grp_pos[1].Z() -=delta_Pad;
		  //BBall.init_pos.z = (sp_info.grp_pos[0].Z() + sp_info.grp_pos[1].Z())/2;
		  break;
	case 'b':
		bMinForce = !bMinForce;
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
		afBody = afInitialBody;
		afBody.LookAtGL(Vec3f(afInitialBody.PosX(),afInitialBody.PosY(),afInitialBody.PosZ()+1),
			Vec3f(afInitialBody.PosX(),afInitialBody.PosY()+1,afInitialBody.PosZ()));
		break;
	case ' ': 
		animating = !animating; //space
	   	break;
	case '\t':
      BBall.chk_shoot = SHOOTED; 
	  BBall.chk_state = THROWING;
	  bHCollide = false;
	  break;
	case 'z':
	  mydb.vh.Anim.distZ -= 5;
	  break;
    case 'Z':
	  mydb.vh.Anim.distZ += 5;
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
		timeKillEvent(SpidarUpdateTimer);
		timeEndPeriod(1);
		Exit();
		break;

	//-----SLCamera
	case 'k':
		//slcamera.SwitchAngle();
		StateChange = !StateChange;
		//slcamera.OnOffChange = !slcamera.OnOffChange;
		break;

	case 'K':
		MainAngleSide = 0;
		MainAngleVert = 0;
		break;
	//10/21
	case 'j':
		UserAngleSide = UserAngleSide - 5;
		//BBall.new_pos.x = BBall.new_pos.x + 0.4;
		break;

	case 'l':
		UserAngleSide = UserAngleSide + 5;
		break;

	case 'o':
		UserAngleVert = UserAngleVert - 5;
		break;

	case 'm':
		UserAngleVert = UserAngleVert + 5;
		break;

	case 'J':
		cout << "UserAngleSide =";
		cin >> UserAngleSide;
		break;

	case 'L':
		cout << "UserAngleVert =";
		cin >> UserAngleVert;
		break;

	case 'O': //SLCameraのキャリブレーション
		if(StateCamera == true)
			slcamera.CallibSLC();
		break;
	//~10/21

	//-----~SLCamera
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
	std::cout << devMan;
   
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

	/*// 2003.10 NEW SETTING
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
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
void TGLView::reset_spidar()
{

	for(int i=0;i<2;i++)
	{
		h_spidar[i].Calib();
	}
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
	// FREE state is PAUSE or RETURNING or FALLING
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

	// FREE → HOLDING
	if(hold[0] & hold[1]){
		bHCollide  = true;
		BBall.chk_state  = HOLDING;
		//hold
		sp_info.VBradius = (sp_info.grp_pos[0] - sp_info.grp_pos[1]).norm() / 2;
		sp_info.initInclination  = convertEuclidToPole(sp_info.grp_pos[0] - sp_info.grp_pos[1]);
		sp_info.initBallPole     = convertEuclidToPole(sp_info.ball_pos - (sp_info.grp_pos[0] + sp_info.grp_pos[1]) / 2);
	}
	// FREE → HALFHOLD
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
	// SETTING BALL POSITION
	BBall.new_pos.x = sp_info.ball_pos.X();
	BBall.new_pos.y = sp_info.ball_pos.Y();
	BBall.new_pos.z = sp_info.ball_pos.Z();
	
	float dis   = (sp_info.grp_pos[!holdHand] - sp_info.ball_pos).norm(); 
	float t_dis = (sp_info.grp_pos[0] - sp_info.grp_pos[1]).norm();

	h_spidar[0].SetMinForce();
	h_spidar[1].SetMinForce();

	// HALFHOLD → HOLDING
	if(dis < BBall.radius + s_rad){
		BBall.chk_state  = HOLDING;
		bHCollide = true;
		sp_info.VBradius         = t_dis / 2;
		sp_info.initInclination  = convertEuclidToPole(sp_info.grp_pos[0] - sp_info.grp_pos[1]);
		sp_info.initBallPole     = convertEuclidToPole(sp_info.ball_pos - (sp_info.grp_pos[0] + sp_info.grp_pos[1]) / 2);
	}
	// HALFHOLD → FALLING
	/*else if(ballCount > 200){
		BBall.chk_state = FALLING;
		bHCollide = false;
		ballCount = 0;
	}
	ballCount++;*/
}

/* If BallState is Holiding */
void TGLView::BallIsHolding(){
	Vec3f changeInclination, nowBallPole;
	Vec3f t_force[2];
	bool hold = 0;

	changeInclination = convertEuclidToPole(sp_info.grp_pos[0] - sp_info.grp_pos[1]) - sp_info.initInclination;
	nowBallPole.X()   = sp_info.initBallPole.X();
	nowBallPole.Y()   = sp_info.initBallPole.Y() + changeInclination.Y();
	nowBallPole.Z()   = sp_info.initBallPole.Z() + changeInclination.Z();

	sp_info.ball_pos  = convertPoleToEuclid(nowBallPole) + (sp_info.grp_pos[0] + sp_info.grp_pos[1]) / 2;
	// SETTING NEW BALL POSITION
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
	Vec3f ballSpeed = (sp_info.BallPosLog[0] - sp_info.BallPosLog[1]) * 1000 / SpidarUpdateRate;
    sp_info.speed   = ballSpeed.norm();

	// 投げ方・旧バージョン
	if(!hold){
		if(sp_info.speed > BallSpeedThreshold && ballSpeed.Z() > BallSpeedThreshold * sin(M_PI / 4))
		{
			// HOLDING → THROWING
			BBall.chk_shoot = SHOOTED; 
			BBall.chk_state = THROWING;
			bHCollide = false;
			ballCount = 0;
			BBall.init_pos.x = sp_info.ball_pos.X();
            BBall.init_pos.y = sp_info.ball_pos.Y();
			BBall.init_pos.z = sp_info.ball_pos.Z();
		}
		else
		{
			// HOLDING → FALLING
			//BBall.chk_state = FALLING;
			//bHCollide = false;
			//ballCount = 0;

			// HOLDING → HALFHOLD
			BBall.chk_state   = HALFHOLD;
			holdHand = 0;
			if((sp_info.grp_pos[0] - sp_info.ball_pos).norm() < (sp_info.grp_pos[1] - sp_info.ball_pos).norm()) holdHand = 1;
			ballCount = 0;
			sp_info.initBallPole     = convertEuclidToPole(sp_info.ball_pos - sp_info.grp_pos[holdHand]);
			sp_info.initBallPole.X() = BBall.radius + s_rad;
			sp_info.initBallPosition = convertPoleToEuclid(sp_info.initBallPole);
		}
	}

	// 投げ方・新バージョン
	/*if(sp_info.speed > BallSpeedThreshold && ballSpeed.Z() > BallSpeedThreshold * sin(M_PI / 4)){
		if(sp_info.ball_pos.Z() - sp_info.grp_pos[0].Z() > 0 && sp_info.ball_pos.Z() - sp_info.grp_pos[1].Z() > 0){
			// HOLDING → THROWING
			BBall.chk_shoot = SHOOTED; 
			BBall.chk_state = THROWING;
			bHCollide = false;
			ballCount = 0;
		}
	}
	else if(!hold){
		// HOLDING → HALFHOLD
		BBall.chk_state = HALFHOLD;
		holdHand = 0;
		if((sp_info.grp_pos[0] - sp_info.ball_pos).norm() < (sp_info.grp_pos[1] - sp_info.ball_pos).norm()) holdHand = 1;
		ballCount = 0;
		sp_info.initBallPole     = convertEuclidToPole(sp_info.ball_pos - sp_info.grp_pos[holdHand]);
		sp_info.initBallPole.X() = BBall.radius + s_rad;
		sp_info.initBallPosition = convertPoleToEuclid(sp_info.initBallPole);
	}*/
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
	Vec3f t_pos[2];

    for(int i = 0; i < 2; i++){
		h_spidar[i].Update();
		t_pos[i]  = h_spidar[i].GetPos() * convert_inch;
	}
	//setting ball
	sp_info.ball_pos.X() = BBall.new_pos.x;
	sp_info.ball_pos.Y() = BBall.new_pos.y;
	sp_info.ball_pos.Z() = BBall.new_pos.z;

	//spidarCheck();
	if(fabs(t_pos[0].X()) <= 100 && fabs(t_pos[1].X()) <= 100){
		for (int j = 0; j < 2; j++)
		{
			sp_info.grp_pos[j].X() = sp_info.user_hand[j].X() - t_pos[j].X();
			sp_info.grp_pos[j].Y() = sp_info.user_hand[j].Y() + t_pos[j].Y();
			sp_info.grp_pos[j].Z() = sp_info.user_hand[j].Z() - t_pos[j].Z(); //　x,z sign , inverse
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
	}

}
// initial ball setting
void TGLView::init_paddle()
{
   
   paddle_v.x= 15;
   paddle_v.y=nRange-GnHeight; //nHeight-nRange; //0.88 m (2.8864) ~ 1m 1m=3.28feet ;30inch
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
	
   //sp_info.grp_pos[0].X() = sp_info.grp_pos[0].X()-BBall.radius;
   //sp_info.grp_pos[1].X() = sp_info.grp_pos[1].X()-BBall.radius*2;
   reset_spidar();
}
// initial ball setting
void TGLView::init_ball()
{
   
   t=0.0;
   dt=0.0;
   
   //ball reset
   BBall.Init();
   
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
  float distance = JOINT_RAD+BBall.radius;
  MyVector l_pos, r_pos;

  l_pos = mydb.vh.Ret_Global(cur_frm, 130);
  r_pos = mydb.vh.Ret_Global(cur_frm, 140);
  
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
		  Play_wave("sound/floor_bounce.wav");
		  int off_frm = mydb.vh.Anim.Off_frm;
		  //reflect start point
	      off_v= mydb.vh.Ret_Global(off_frm, 100);
	  }
  }  

}

// after databse connection, table open
void TGLView::conn_motion(int init_type, double k_time)
{
   double pos[3];
   MyVector temp_v;
  
   temp_v= BBall.Parabola(BBall.angle, k_time);

   pos[0]=temp_v.x;
   pos[1]=temp_v.y;
   pos[2]=temp_v.z;

   m_StartTime = timeGetTime();
    //compare ball pos to hand pos
   //frame_num =0;
   mydb.vh.Anim.init();
   mydb.vh.Anim.Key_frm = k_time/mydb.vh.Anim.unit_Time;

if (init_type !=0 )
{
	   int row_cnt = mydb.select_motion(init_type, mydb.vh.Anim.Key_frm, (double*) pos);
	//Data searched	
	   if (row_cnt != 0)
	   {
		   int sel = rand() % mydb.vh.Anim.Throw_num;
		   mydb.vh.Anim.throw_type = mydb.sel_throw[sel][0];
		   mydb.vh.Anim.Off_frm= mydb.sel_throw[sel][1];

		   int tt=mydb.load_basic(mydb.vh.Anim.throw_type, mydb.vh.Anim.catch_type);
		   mydb.vh.Anim.Off_frm = mydb.vh.Anim.Off_frm +tt; 
		   mydb.vh.Anim.Pre_frm =tt;
           
		   mydb.load_motion(0,mydb.vh.Anim.catch_type);
		   mydb.load_motion(mydb.vh.Anim.Pre_frm, mydb.vh.Anim.throw_type);
		  // frist frame 
		   mydb.vh.Com_Interp(tt);
		   mydb.vh.Anim.S_status = 1;

		   mydb.vh.setup_endeffector(LEFT_ARM, mydb.vh.Anim.Frame_num);
		   mydb.vh.setup_endeffector(RIGHT_ARM, mydb.vh.Anim.Frame_num);
	   }
	   else  // data no found 
	   {
		   mydb.vh.Anim.Off_frm =1;
		   mydb.vh.Anim.catch_type = mydb.vh.Anim.Init_code;
		   int tt=mydb.load_basic(0, mydb.vh.Anim.catch_type); // waiting 
		   mydb.vh.Anim.Pre_frm =tt;
		   mydb.load_motion(0,mydb.vh.Anim.catch_type);
		   mydb.vh.Anim.S_status = 2;  // 
	   }
}
else
{  //init type = 0
	mydb.vh.Anim.Off_frm =1;
	mydb.vh.Anim.catch_type = mydb.vh.Anim.Init_code;
    int tt=mydb.load_basic(0, mydb.vh.Anim.catch_type); // waiting 
	mydb.vh.Anim.Pre_frm =tt;
	mydb.load_motion(0,mydb.vh.Anim.catch_type);
	mydb.vh.Anim.S_status = 0;  
}

//JEONG - SLCamera 10/21
	mydb.vh.setup_endeffector(SKULL, mydb.vh.Anim.Frame_num);
//JEONG ~SLCamera

 //ELAPSED TIME   
   m_ElapsedTime = timeGetTime();
   printf( "\t%u (ms)\n", m_ElapsedTime-m_StartTime );
   // shooting reset
   BBall.chk_shoot = NOSHOOT;
   mydb.vh.Anim.cur_frm = 0;
}

void TGLView::drawjoint(int cur_frm, MyBone sk_bone)
{
	//SLCamera 10/21
	SelectAngle();
	//~10/21

// Drawing joint ball/join
 	if (sk_bone.id == Hips){ //root translate
        glTranslatef(sk_bone.trans[cur_frm].x, sk_bone.trans[cur_frm].y, sk_bone.trans[cur_frm].z);
	}
	else{ // all joint except for root
	//
      Draw_Ellipse(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z, sk_bone.scale);
      glTranslatef(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z);
    }
	//SLCamera
	//10/21
	//　胸の回転
	//if(sk_bone.id == chest1 && myrank != 0 && ( StateCamera == true || StateChange == true))
	if(sk_bone.id == chest1 && ( myrank != 0 || StateCamera == true || StateChange == true))
	//~10/21
	{
		glRotatef(sk_bone.rot[cur_frm].x /*+ MainAngleVert/2*/, 1.0, 0.0, 0.0);  //x
		glRotatef(sk_bone.rot[cur_frm].y /*+ MainAngleSide/2*/, 0.0, 1.0, 0.0);  //y
	}
	else 
	{
		glRotatef(sk_bone.rot[cur_frm].z, 0.0, 0.0, 1.0);  //z
		glRotatef(sk_bone.rot[cur_frm].x, 1.0, 0.0, 0.0);  //x
		glRotatef(sk_bone.rot[cur_frm].y, 0.0, 1.0, 0.0);  //y
			// if joint is selected, its color will be change
	}

	//頭の回転
    if (sk_bone.id == Head && ( StateCamera == true || StateChange == true ))
	{
    	glRotatef(MainAngleVert/3, 1.0, 0.0, 0.0);  //x
		glRotatef(MainAngleSide/3, 0.0, 1.0, 0.0);  //y
	}
	//~SLCamera
	
	 glMaterialfv(GL_FRONT, GL_DIFFUSE, headcolor);
     //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
     //glMaterialf(GL_FRONT, GL_SHININESS, 20);

	 if(sk_bone.id == Head){
		glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		   //SLCamera 10/21
		  ChangeFace();
		  //~SLCamera 
		  glEnable(GL_TEXTURE_2D);

		  glRotatef(90, 0.0f, 1.0f, 0.0f);
		  glRotatef(85, 1.0f, 0.0f, 0.0f);
		  drawSphere(HEAD_RAD);	
	  
		  //SLCamera 10/21
	      //目の描画
		  glPushMatrix();
			  //目の回転:JEONG 10/21 updated
			  if( StateCamera == true || StateChange == true ){

				  CheckEyeAngle(); //目の回転の制限

				  glRotatef(MainAngleVert, 0.0f, 1.0f, 0.0f);
				  glRotatef(-MainAngleSide, 0.0f, 0.0f, 1.0f);
			  }
		     glBindTexture(GL_TEXTURE_2D, tex[7]);
			 glEnable(GL_TEXTURE_2D);
			  //右目
			  glPushMatrix();
				glTranslatef(-2.5, -1.8, -0.7);
				drawSphere(EYE_RAD);	
			  glPopMatrix();

			  //左目
			  glPushMatrix();
				glTranslatef(-2.5, 1.0, -0.7);
				drawSphere(EYE_RAD);	
			  glPopMatrix();
			  //~SLCamera 10/21
		  glPopMatrix(); ///目の描画終わり

		glPopMatrix(); //頭のの描画終わり

	 }
	 else {
	     glDisable(GL_TEXTURE_2D);
		 glutSolidSphere(JOINT_RAD, 15, 12); // joint ball drawing
		
		if (sk_bone.id == RightAnkle || sk_bone.id == LeftAnkle ) // foot box drawing
		{
			   glPushMatrix();
				glTranslatef(0, 0, -1.5);
				glRotatef(-90, 0 ,1,0); 
				drawQuad(4.0, 1.0);
			   glPopMatrix();
		 }
		 else if (sk_bone.id== LeftWrist || sk_bone.id== RightWrist ) //hand end_site drawing
		 {
			 MyVector size(0.75, 1, 0.75);
			  glPushMatrix();
				glScalef(1.0, 1.0, 2.0);
				Draw_Ellipse(0, -3.14, 0, size);
			  glPopMatrix();
		 }
	  }
}

// all body drawing
void TGLView::drawBody(int cur_frame)
{
glPushMatrix();
	
     drawjoint(cur_frame, mydb.vh.A_bone[0]); //0
    
		glPushMatrix();
			drawjoint(cur_frame, mydb.vh.LL_bone[0]); 
			drawjoint(cur_frame, mydb.vh.LL_bone[1]); 
			drawjoint(cur_frame, mydb.vh.LL_bone[2]); 
		glPopMatrix();

		glPushMatrix();
			drawjoint(cur_frame, mydb.vh.RL_bone[0]);
			drawjoint(cur_frame, mydb.vh.RL_bone[1]);
			drawjoint(cur_frame, mydb.vh.RL_bone[2]);
		glPopMatrix();

	    glPushMatrix();
			drawjoint(cur_frame, mydb.vh.A_bone[1]);
			drawjoint(cur_frame, mydb.vh.A_bone[2]);  //8
	
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.LA_bone[0]); //9
				drawjoint(cur_frame, mydb.vh.LA_bone[1]);//10
				drawjoint(cur_frame, mydb.vh.LA_bone[2]);  //11
				drawjoint(cur_frame, mydb.vh.LA_bone[3]);  //12
			glPopMatrix();
		
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.RA_bone[0]);  //13
				drawjoint(cur_frame, mydb.vh.RA_bone[1]);  //14
				drawjoint(cur_frame, mydb.vh.RA_bone[2]);   //15
				drawjoint(cur_frame, mydb.vh.RA_bone[3]);   //16
		    glPopMatrix();

			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.H_bone[0]);  //17
				drawjoint(cur_frame, mydb.vh.H_bone[1]);  //18
			glPopMatrix();

	glPopMatrix();

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

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//shadow
	///////////////////////////////////////////////////////////////////////
	//ステンシルバッファをクリアしなきゃ
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT  | GL_STENCIL_BUFFER_BIT);	//shadow

    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
   // gluLookAt(0.0, 2.0, mydb.vh.Anim.distZ, 0.0, 1.0,0.0, 0.0,1.0, 0.0);
	static Affinef R180(Rad(180),'y');
	
	Affinef afProjector = afBody * R180 * R * R180;
	
	Affinef afCam = (afEye * afProjector).inv();
	glLoadMatrixf(afCam);

  glPushMatrix();
     glTranslatef(0, -nHeight, -30);
	 glRotatef(mydb.vh.Anim.camera.x, 1.0f, 0.0f, 0.0f);
	 glRotatef(mydb.vh.Anim.camera.y, 0.0f, 1.0f, 0.0f);
	 glRotatef(mydb.vh.Anim.camera.z, 0.0f, 0.0f, 1.0f);
	 
    glEnable(GL_LIGHTING);
	 
  //hip -5 
      drawBody(mydb.vh.Anim.cur_frm);
	  
	//shadow  
	//床の領域にステンシル値１を付ける
	glEnable(GL_STENCIL_TEST);							//shadow
	glStencilFunc( GL_ALWAYS, 1, ~0);					//shadow
	glStencilOp(GL_KEEP,GL_KEEP ,GL_REPLACE);			//shadow
	//床の描画
	glPushMatrix();										//shadow
	      glBindTexture(GL_TEXTURE_2D, tex[1]);			//shadow
		  glEnable(GL_TEXTURE_2D);						//shadow
	      glScalef(100.0, 1.0, 100.0);					//shadow
		  glTranslatef(-2, GnHeight, -0.5);				//shadow
	      drawQuad(4, 1);								//shadow
		  glDisable(GL_TEXTURE_2D);						//shadow
	glPopMatrix();

	//箱の領域にステンシル値３を付ける
	glStencilFunc( GL_ALWAYS, 3, ~0);					//shadow
	glStencilOp(GL_KEEP,GL_KEEP ,GL_REPLACE);			//shadow
	//箱の描画
	glPushMatrix();										//shadow
	      glBindTexture(GL_TEXTURE_2D, tex[6]);			//shadow
		  glEnable(GL_TEXTURE_2D);						//shadow
		  glTranslatef(paddle_v.x, paddle_v.y/2, paddle_v.z);//shadow
		  glRotatef(90, 1.0, 0.0, 0.0);						//shadow
	      drawQuad(20, 7);									//shadow	
		  glDisable(GL_TEXTURE_2D);							//shadow
	glPopMatrix();											//shadow

	glDisable(GL_STENCIL_TEST);							//shadow

	//カラー・デプスバッファマスクをセットする
	//これで以下の内容のピクセルの色の値は、書き込まれない。
	glColorMask(0,0,0,0);								//shadow
	glDepthMask(0);										//shadow
	  
	//影をつけたい領域にステンシル値２を付ける
  	glEnable(GL_STENCIL_TEST);							//shadow
	glStencilFunc( GL_EQUAL, 1, ~0);					//shadow
	//ステンシル値が１のピクセルのステンシル値をインクリメント
	glStencilOp(GL_KEEP,GL_KEEP ,GL_INCR);				//shadow
	glDisable(GL_DEPTH_TEST);							//shadow

	//VHの影部分にステンシル値２を付ける
	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
			//glMultMatrixf((GLfloat *)ShadowMat);		//shadow
			glMultMatrixd(ShadowMat[0]);				//shadow
	        drawBody(mydb.vh.Anim.cur_frm);				//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow

	//ボールの影部分にステンシル値２を付ける
	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
			//glMultMatrixf((GLfloat *)ShadowMat);		//shadow
			glMultMatrixd(ShadowMat[0]);				//shadow
	        BBall.Draw();								//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow

	//同じく、影をつける領域に２をつける
	glStencilFunc( GL_EQUAL, 3, ~0);					//shadow
	//ステンシル値が３のピクセルのステンシル値をデクリメント
	glStencilOp(GL_KEEP,GL_KEEP ,GL_DECR);				//shadow

	//ボールのステンシル値２付け(対ボックスのtop面用)
	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
	        glMultMatrixd(ShadowMat2[0]);				//shadow
			BBall.Draw();								//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow

	/*
	//対ボックスの右側側面用
	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
	        glMultMatrixd(ShadowMat3[0]);				//shadow
			BBall.Draw();								//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow
*/

	glEnable(GL_DEPTH_TEST);	
	//ビットマスクを解除
	glColorMask(1,1,1,1);								//shadow
	glDepthMask(1);										//shadow

	//影をつける。ステンシル値が２の所だけが描画される
	glStencilFunc( GL_EQUAL, 2, ~0 );							//shadow
	glEnable(GL_BLEND);											//shadow
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		//shadow	
		glDisable(GL_DEPTH_TEST);								//shadow
			glColor4f(0.1f, 0.1f, 0.1f, 0.5f);					//shadow
			DrawFloor();										//shadow
			//glColor4f(0.1f, 0.1f, 0.1f, 0.5f);					//shadow
			//DrawFloor2();										//shadow
		glEnable(GL_DEPTH_TEST);								//shadow
	glDisable(GL_BLEND);										//shadow
	glDisable(GL_STENCIL_TEST);									//shadow
//shadow



// ball display
		//BBall.Draw();//ytakeyama comment out
		//ytakeyama ボールと空を描きます
		int tempflag = 0;
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			if(tempflag){
				(*it)->Draw();
			}else{
				glPushMatrix();
					glTranslatef(BBall.new_pos.x, BBall.new_pos.y, BBall.new_pos.z);
					(*it)->Draw();
				glPopMatrix();
				tempflag = 1;
			}
		}
		//ytakeyama end
//shadow
       //BBall.Shadow_ball(ShadowMat);
		glPushMatrix(); //left
	   	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(sp_info.grp_pos[0].X(),  sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z());
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

       glPushMatrix(); //right
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(sp_info.grp_pos[1].X(),  sp_info.grp_pos[1].Y(), sp_info.grp_pos[1].Z());
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();
	
	   //箱の描画
	   /*
	   glPushMatrix();
	      glBindTexture(GL_TEXTURE_2D, tex[6]);
		  glEnable(GL_TEXTURE_2D);
		  glTranslatef(paddle_v.x, paddle_v.y/2, paddle_v.z);
		  glRotatef(90, 1.0, 0.0, 0.0); 
	      drawQuad(20, 7); 
		  glDisable(GL_TEXTURE_2D);
	 
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
  gluPerspective(fov_v, aspect, 1.0, 1000);

  glMatrixMode(GL_MODELVIEW);

}
void TGLView::OnJoystick(unsigned int button, int x, int y, int z){
	std::cout << "B" << button << " x:" << x << " y:" << y << " z:" << z << std::endl;
}
void TGLView::OnSpecialKey(int key, int x, int y) 
{
  if (key == GLUT_KEY_LEFT)    { mydb.vh.Anim.camera.y -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { mydb.vh.Anim.camera.y += deltaRot; }
  if (key == GLUT_KEY_UP)      { mydb.vh.Anim.camera.x -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { mydb.vh.Anim.camera.x += deltaRot; }
  glutPostRedisplay();
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	//MPI
	press_key = key;
	
}

void TGLView::OnMouse(Button button, bool on, int x, int y)
{
//	mouseButton = button;
//	mouseX = x;	mouseY = y;
/*
	if(!on){
		mouseButton = NONE;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case LEFT:
		if(on)
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	case MIDDLE:
		if(on)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	}*/
}

void TGLView::OnMouseMove(bool on, int x, int y)
{
	switch(mouseButton){
	case LEFT:
		Invalidate();
		break;		
	case MIDDLE:
		Invalidate();
		break;
	case RIGHT:
		Invalidate();
		break;
	}
	mouseX = x;	mouseY = y;
}

void CALLBACK SpidarUpdateProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD)
{
	((TGLView*)dwUser)->update_spidar();

}

void TGLView::OnOpen(void)
{

//SLCameraの初期化
 StateCamera = slcamera.InitSLC();
 FlagFace = false;

  //10/21
 BallAngleSide = 0;
 BallAngleVert = 0;
 UserAngleSide = 0;
 UserAngleVert = 0;
 MainAngleSide = 0;
 MainAngleVert = 0;

 for(int i=0; i <= BallLogMax; i++){
	 BallPosChange[BallLogMax] = Vec3f(0.0, 0.0, 0.0);
	 BallPosLog2[BallLogMax] = Vec3f(0.0, 0.0, 0.0);
 }
 BallPosChangeTotal = Vec3f(0.0, 0.0, 0.0);

 //~10/21
  //~SLCamera

	//	GLの初期化
 GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
 tQuaternion nGround(0, GnHeight, 0, 0);
 MyVector vLight(-1, 50, 0); 
 //shadow
 //ｙは箱のtop面の高さ。箱のtop平面の定義
 float y = 16.5 + paddle_v.y/2;							//shadow
 tQuaternion nGround2(0, 1, 0, -y);						//shadow
 //ｘは箱の右側側面の位置。箱の右側側面の定義
 /*
 float x = 20 + paddle_v.x;								//shadow
 tQuaternion nGround3(1, 0, 0, -x);						//shadow
*/

//vLightが二つ？jeong
//vLight２つは意味なかったです。消しました　ogawa
	
 glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
 glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
 glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
// glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);	//shadow
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
	CalcShadowMat(ShadowMat, nGround, vLight);
	CalcShadowMat(ShadowMat2, nGround2, vLight);			//shadow
	//CalcShadowMat(ShadowMat2, nGround3, vLight);			//shadow

	// MYDB Init connection
   int ret = mydb.sqlconn();
   if (ret == 1)
   {
	   init_paddle();
	   init_hand();
	   init_ball();	
    
	 // compute database search condition
	   mydb.vh.Anim.Key_time = BBall.WhatTime(-1.5); //distance => frame
	   mydb.load_model(1);//1

	   mydb.vh.Anim.Throw_num=mydb.random_throw();
	   conn_motion(0, mydb.vh.Anim.Key_time);
   }
   
   Load_Texture();
//MPI
   //MPI031021
   //なぜかここで止まってしまうので、とりあえず、コメントアウト
   //PI_Barrier(MPI_COMM_WORLD);
//~MPI
   if(myrank == 0){
		glutReshapeWindow(400,300);
   }else{
		glutFullScreen();
   }
   
   //SPIDAR-H
   init_spidar();
  //timer setting	
	timeBeginPeriod(1);
	SpidarUpdateTimer = timeSetEvent(SpidarUpdateRate, 1, SpidarUpdateProc, (DWORD)this, TIME_PERIODIC);

	SetTimer(33, &TGLView::OnTimer); //Step()を呼び出す周期 33

	//SLCamera
	//SetTimer(2, &TGLView::OnTimer2);
	//~SLCamera

	dwTime = timeGetTime(); //GetTickCount();
}

void TGLView::Ball_Control(int cur_frm)
{

		if (bVCollide) 
		{
			BBall.new_pos = mydb.vh.Ret_Global(cur_frm, 100);
		}
		else if (bHCollide) 
		//else
		{   //HOLDING or HALFHOLD
			//BBall.new_pos =BBall.init_pos;
			BBall.new_pos.x =sp_info.ball_pos.X();
			BBall.new_pos.y =sp_info.ball_pos.Y();
			BBall.new_pos.z =sp_info.ball_pos.Z();
		}

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
			  mydb.vh.Anim.cur_frm++;

			  if ( bVCollide && BBall.chk_shoot != REFLEXED && mydb.vh.Anim.Off_frm != 1 && mydb.vh.Anim.cur_frm == mydb.vh.Anim.Off_frm)
			  { 
				  BBall.chk_shoot = REFLEXED;
				  BBall.chk_state = RETURNING; //start move ball
				  bVCollide = false;
				  //NOCOLLISION;
			  }
			  if (!bHCollide && BBall.chk_state == THROWING)
			  {
				  BBall.new_pos = BBall.Parabola(BBall.angle, t);
				  if (t>=1 && BBall.minY <= GnHeight || BBall.new_pos.z > 50) 
					 init_ball();
				 t +=mydb.vh.Anim.unit_Time;
			  }
		
			  if(!bVCollide && BBall.chk_state == RETURNING)
			  {
				 BBall.new_pos = BBall.Reflect(dt, off_v);
				 if (BBall.minZ < BBall.init_pos.z) 
					 init_ball();
				 dt +=(mydb.vh.Anim.unit_Time*2);
			  }
			
			 if (BBall.chk_shoot == SHOOTED ) 
			 {
				 conn_motion(1, mydb.vh.Anim.Key_time); 
				 // new
				 BBall.chk_shoot = NOSHOOT;
				 bHCollide = false;
				 //bVCollide = false;
				 Play_wave("sound/ball_shoot.wav");
			 	 FlagFace = true;
				 //~SLCamera
			 }
			 if (BBall.chk_shoot == NOSHOOT && mydb.vh.Anim.cur_frm >= mydb.vh.Anim.Frame_num ) 
			 {
                 //if any event isn't 
				 init_ball();
				 //~SLCamera
				 FlagFace = false;
				 //MPI communication 
                 //End_Interp(cur_frm-1); // 0~ frame_num-1
				 conn_motion(0, mydb.vh.Anim.Key_time); 
				 mydb.vh.Com_Interp(mydb.vh.Anim.cur_frm);
			  }
		 //virtual human hand and ball collision
		 if (!bVCollide && BBall.chk_state == THROWING && mydb.vh.Anim.cur_frm >= mydb.vh.Anim.Key_frm-5) 
			 ballColliding(mydb.vh.Anim.cur_frm);
		 //setting
		 Ball_Control(mydb.vh.Anim.cur_frm);

		//SLCamera
		 //10/21
		 BallAngle();
		 if( StateCamera == true){
			 slcamera.Step();
			//slcamera.ShowData();
		 	 UserAngleSide = -slcamera.AdjustAngleSide();
			 UserAngleVert = -slcamera.AdjustAngleVert();
		 }
		 //~10/21
		//~SLCamera
		
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
	if(cur_frm == 1)
	{
		//apply share struct from server struct
		M_DAT.Anim_Set[0] = mydb.vh.Anim.catch_type;
		M_DAT.Anim_Set[1] = mydb.vh.Anim.throw_type;
		M_DAT.Anim_Set[2] = mydb.vh.Anim.Frame_num;
		M_DAT.Anim_Set[3] = mydb.vh.Anim.Key_frm;
		M_DAT.Anim_Set[4] = mydb.vh.Anim.Off_frm;
		M_DAT.Anim_Set[5] = mydb.vh.Anim.Pre_frm;
		M_DAT.Anim_Set[6] = mydb.vh.Anim.S_status;
 	}
	M_DAT.Anim_Set[7] = mydb.vh.Anim.cur_frm;
	M_DAT.Cur_Bone[0] = mydb.vh.A_bone[0].trans[cur_frm].x;
    M_DAT.Cur_Bone[1] = mydb.vh.A_bone[0].trans[cur_frm].y;
    M_DAT.Cur_Bone[2] = mydb.vh.A_bone[0].trans[cur_frm].z;
    idx=3;
	for(int i=0;i<6;i++)
	{
		int grp_id = i*10+100;
		int Max_idx= GRP_IDX[i];
		for(int j=0;j<Max_idx;j++)
		{
			int kk = 3*j;
			ret=mydb.vh.Ret_Array(grp_id, j, cur_frm);
            M_DAT.Cur_Bone[idx+kk] =ret.x;
            M_DAT.Cur_Bone[idx+kk+1] =ret.y;
            M_DAT.Cur_Bone[idx+kk+2] =ret.z;
		}
		idx = idx + 3*Max_idx;
	}

	M_DAT.Ball_Pos[0] = BBall.new_pos.x;
	M_DAT.Ball_Pos[1] = BBall.new_pos.y;
	M_DAT.Ball_Pos[2] = BBall.new_pos.z;
	
		//SLCamera
	M_DAT.Temp_Rot[0] = MainAngleSide;
	M_DAT.Temp_Rot[1] = MainAngleVert;
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
		mydb.vh.Anim.catch_type = M_DAT.Anim_Set[0];
		mydb.vh.Anim.throw_type = M_DAT.Anim_Set[1];
		mydb.vh.Anim.Frame_num = M_DAT.Anim_Set[2];
		mydb.vh.Anim.Key_frm = M_DAT.Anim_Set[3];
		mydb.vh.Anim.Off_frm = M_DAT.Anim_Set[4];
		mydb.vh.Anim.Pre_frm = M_DAT.Anim_Set[5];
		mydb.vh.Anim.S_status = M_DAT.Anim_Set[6];
	}

	mydb.vh.A_bone[0].trans[mydb.vh.Anim.cur_frm].x = M_DAT.Cur_Bone[0];
    mydb.vh.A_bone[0].trans[mydb.vh.Anim.cur_frm].y = M_DAT.Cur_Bone[1];
    mydb.vh.A_bone[0].trans[mydb.vh.Anim.cur_frm].z = M_DAT.Cur_Bone[2];

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
		//SLCamera
	MainAngleSide = M_DAT.Temp_Rot[0];
	MainAngleVert = M_DAT.Temp_Rot[1];
}

///////////////////////////////	shadow	////////////////////////
//床の描画
void TGLView::DrawFloor(void){								//shadow
	glDisable(GL_LIGHTING);									//shadow
	glBegin(GL_QUADS);										//shadow
		//面なので、左周りに定義しないと、表として扱われない。	//shadow
		glVertex3f( 100, 2.5, 100);							//shadow
		glVertex3f( 100, 2.5,-100);							//shadow
		glVertex3f(-100, 2.5,-100);							//shadow
		glVertex3f(-100, 2.5, 100);							//shadow
	glEnd();												//shadow
	glEnable(GL_LIGHTING);									//shadow
}															//shadow

void TGLView::DrawFloor2(void){								//shadow
	//ｙは箱のtop面の高さ
	float y=0;												//shadow
	y = 16.5 + paddle_v.y/2;								//shadow
	glDisable(GL_LIGHTING);									//shadow
	glBegin(GL_QUADS);										//shadow
		//面なので、左周りに定義しないと、表として扱われない。	//shadow
		glVertex3f( 100, y, 100);							//shadow
		glVertex3f( 100, y,-100);							//shadow
		glVertex3f(-100, y,-100);							//shadow
		glVertex3f(-100, y, 100);							//shadow
	glEnd();												//shadow
	glEnable(GL_LIGHTING);									//shadow
}															//shadow
