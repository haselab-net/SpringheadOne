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

/////////////////////////////////////////////////////////
#include "GLView.h"
#include "tex.h"      // primitive definition
#include "ball.h"
///////////////////
#include "Tokin.h"

//MY ODBC
Cdirexec mydb;
CBall BBall;
SpidarInfo sp_info;

// check variables
static int full_mode =0;
static int chk_end =0;
static int chk_interp = 0;
static int animating = 0;

CCyberGlove cglove(SERIAL1, B115200, USE_CYBERGLOVE);
cyberhand cfinger; 

Tokin tokin;
static GLfloat tokin_rot[3];
#define TokinUpdateRate	20 //12ms
UINT TokinUpdateTimer;
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
static double ft=0.0;

using namespace std;
const float EYEOFFSET=0.8;

//SPIDAR-H
////////////////////////////
UINT SpidarUpdateTimer;
static float s_rad = 4;
////////////////////////////
///For experiment///////////////////////////////////////////////////////////////////
static int Test_Mode= 2;	
char testchar[15];
char buffer[10];
//-1: stance pose , 0:one pattern, 1:various pattern 
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
		mciSendString("set WAVEFILE time format tmsf wait", NULL, 0,0);
   // mciSendString("stop WAVEFILE", NULL, 0,0);
   }
}

TGLView::TGLView(int n):nowMode(n)
{

	bDraw = true;
	stepHz = 0;
	
	float ini_initialx = 10;
	float ini_initialy = 2.0;
	float ini_initialz = -150;
	float ini_eyeheight = 8.0;
	afEye = Affinef(0,ini_eyeheight,0);
	
	viewMode = 0;
	
	afInitialBody = Affinef(ini_initialx,ini_initialy,ini_initialz);
	afBody = afInitialBody;

	afBody.LookAtGL(Vec3f(ini_initialx,ini_initialy,ini_initialz+1),
		Vec3f(ini_initialx,ini_initialy+1, ini_initialz)); //posz(-EZ), posy(EY) ini_initialz
}

bool TGLView::OnTimer(){
	Step();
	return true;
}

void TGLView::OnIdle()
{

  
}

void TGLView::shot_control()
{
     
	 BBall.chk_shoot = SHOOTED; 
	 BBall.chk_state = THROWING;
	 bHCollide = false;
	 BBall.shot_pos = BBall.new_pos;
	 BBall.ReturnHand.X() =  BBall.shot_pos.x;
	 BBall.ReturnHand.Y() =  BBall.shot_pos.y;
	 BBall.ReturnHand.Z() =  BBall.shot_pos.z;
	 t=0.0;
    
}
/////////////////////////////
void TGLView::init_spidar()
{
	//devMan.RPool().Register(new DRTokin3D);
    devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();
	//std::cout << devMan;
   
		//工作室のSPIDAR
	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)},
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};*/

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
	

		Vec3f t_pos[2];

		for(int i = 0; i < 2; i++){
	
			h_spidar[i].Update();
		//	if(h_spidar[i].Motor()[0].da){
		//		printf("");
		//	}
			if (BBall.chk_state == SHOOTED) 
				h_spidar[i].GetPos().Z() = 0.0;
			t_pos[i]  =h_spidar[i].GetPos() * convert_inch;
		}
		//setting ball
		sp_info.ball_pos.X() = BBall.new_pos.x;
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
	sp_info.user_hand[0].Z() = -80;
	sp_info.user_hand[1]  = sp_info.user_hand[0];
	
	sp_info.grp_pos[0]    = sp_info.user_hand[0];
	sp_info.grp_pos[1]    = sp_info.user_hand[0];
    
	//reset_spidar();
}
// initial ball setting
void TGLView::init_ball()
{
   
   t=0.0;
   dt=0.0;
   ft = 0.0;
   
   //ball reset
   BBall.Init();
   
   //mydb.vh[1].Anim.init();		
   reflex_v.x=0.0;
   reflex_v.y=0.0;
   reflex_v.z=0.0;
   //init 
   bHCollide = false;
   bVCollide = false;
}

//ball collision detect on shoulder
void TGLView::ballColliding(int cur_frm)
{
  float distance = JOINT_RAD*5+BBall.radius;
  MyVector l_pos, r_pos;

  l_pos = mydb.vh[1].Ret_Global(cur_frm, 140, 1); 
  r_pos = mydb.vh[1].Ret_Global(cur_frm, 130, 1);
  
  avg_epos.x=(l_pos.x+r_pos.x)/2;
  avg_epos.y=(l_pos.y+r_pos.y)/2;
  avg_epos.z=(l_pos.z+r_pos.z)/2;

  float left_rad = MyVectorDistance(BBall.new_pos, l_pos);
  float right_rad = MyVectorDistance(BBall.new_pos, r_pos);
  float t_rad = MyVectorDistance(BBall.new_pos, avg_epos);
 
  if (t_rad < distance || left_rad < distance || right_rad < distance)
  {
	  
		  bVCollide= true;  
	  	  BBall.chk_state = FALLING;
		  if (avg_epos.x <= BBall.new_pos.x)  transition_motion(LEFT_PHASE);
		  else transition_motion(RIGHT_PHASE);
		  //Play_wave("../vh[1]_data/floor_bounce.wav"); //sound/
		  //int off_frm = mydb.vh[1].Anim.Off_frm;
		  //reflect start point
		  //dt = 0.0; //time seeting 10/24
	      //off_v= mydb.vh[1].Ret_Global(off_frm, 100, 1);
	  
  }  

}

bool TGLView::bboxColliding()
{

	double dis=0.0;

	if (BBall.new_pos.x < mydb.vh[1].MModel.A_bone[0].bbox[0])
	    //dis = (BBall.new_pos.x - A_bone[0].center.x)*(BBall.new_pos.x - A_bone[0].center.x);
		dis = (BBall.new_pos.x - mydb.vh[1].MModel.A_bone[0].bbox[0])*(BBall.new_pos.x - mydb.vh[1].MModel.A_bone[0].bbox[0]);
    else if (BBall.new_pos.x > mydb.vh[1].MModel.A_bone[0].bbox[3]) 
		dis = (BBall.new_pos.x - mydb.vh[1].MModel.A_bone[0].bbox[3])*(BBall.new_pos.x - mydb.vh[1].MModel.A_bone[0].bbox[3]);
	
	if (BBall.new_pos.y < mydb.vh[1].MModel.A_bone[0].bbox[1])
		dis += (BBall.new_pos.y - mydb.vh[1].MModel.A_bone[0].bbox[1])*(BBall.new_pos.y - mydb.vh[1].MModel.A_bone[0].bbox[1]);
    else if (BBall.new_pos.y > mydb.vh[1].MModel.A_bone[0].bbox[4]) 
		dis += (BBall.new_pos.y - mydb.vh[1].MModel.A_bone[0].bbox[4])*(BBall.new_pos.y - mydb.vh[1].MModel.A_bone[0].bbox[4]);

	if (BBall.new_pos.z < mydb.vh[1].MModel.A_bone[0].bbox[2])
		dis += (BBall.new_pos.z - mydb.vh[1].MModel.A_bone[0].center.z)*(BBall.new_pos.z - mydb.vh[1].MModel.A_bone[0].center.z);
		//dis += (BBall.new_pos.z - A_bone[0].bbox[2])*(BBall.new_pos.z - A_bone[0].bbox[2]);
    else if (BBall.new_pos.z > mydb.vh[1].MModel.A_bone[0].bbox[5]) 
		//dis += (BBall.new_pos.z - A_bone[0].bbox[5])*(BBall.new_pos.z - A_bone[0].bbox[5]);
		dis += (BBall.new_pos.z - mydb.vh[1].MModel.A_bone[0].center.z)*(BBall.new_pos.z - mydb.vh[1].MModel.A_bone[0].center.z);

    double rad = BBall.radius * BBall.radius;
 
	return dis<=rad;

}

// after databse connection, table open
void TGLView::conn_motion(int status_type)
{
      
   //resetting
   m_StartTime = timeGetTime();
   mydb.vh[1].Anim.init();
    ///////////////////////////////////////////////////////////////////
   //various pattern

	transition_motion(status_type);

	mydb.vh[1].setup_endeffector(SKULL, mydb.vh[1].Anim.Frame_num);
    mydb.vh[1].setup_endeffector(LEFT_ARM, mydb.vh[1].Anim.Frame_num);
	mydb.vh[1].setup_endeffector(RIGHT_ARM, mydb.vh[1].Anim.Frame_num);
	
    //ELAPSED TIME   
   m_ElapsedTime = timeGetTime();
   std::cout << "access time: " << m_ElapsedTime- m_StartTime << "(ms)\n ";

}

void TGLView::transition_motion(int s_type)
{

	switch(s_type)
	{
	case READY_PHASE:
		mydb.vh[1].Anim.Off_frm =1;
		mydb.vh[1].Anim.second_type = 0;
        mydb.vh[1].Anim.conv_rot.y = 0;
		mydb.vh[1].Anim.conv_trans.Reset();			
        
		mydb.vh[1].Anim.first_type = FREE_TALK;
		//rotation 
	  	mydb.vh[1].Anim.Frame_num = mydb.vh[1].dfmotion.total_frm[0];
		mydb.vh[1].Anim.Pre_frm = mydb.vh[1].dfmotion.total_frm[0]; 
		mydb.vh[1].move_motion(0,mydb.vh[1].Anim.Frame_num);
		break;
	case LEFT_PHASE:
		mydb.vh[1].Anim.first_type = TURN_LEFT;
		mydb.vh[1].Anim.second_type = 0;
		mydb.vh[1].Anim.conv_rot.y = 90;
        mydb.vh[1].Anim.conv_trans.x=-12;
		mydb.vh[1].Anim.conv_trans.y=-3;
		mydb.vh[1].Anim.conv_trans.z=-2;
		//setting convert(mydb.vh[1].dfmotion.d_motion[0].A_bone[0].trans.x)
		
		mydb.vh[1].Anim.Frame_num = mydb.vh[1].dfmotion.total_frm[1];
		mydb.vh[1].Anim.Pre_frm = mydb.vh[1].dfmotion.total_frm[1]; 
		mydb.vh[1].move_motion(1, mydb.vh[1].Anim.Frame_num);
		break;
	case RIGHT_PHASE:
		mydb.vh[1].Anim.first_type = TURN_RIGHT;
		mydb.vh[1].Anim.second_type = 0;
		mydb.vh[1].Anim.conv_rot.y = 90;
		mydb.vh[1].Anim.conv_trans.x=-12;			   			   			   
	    mydb.vh[1].Anim.conv_trans.y=-3;
		mydb.vh[1].Anim.conv_trans.z=-2;

		mydb.vh[1].Anim.Frame_num = mydb.vh[1].dfmotion.total_frm[2];
		mydb.vh[1].Anim.Pre_frm = mydb.vh[1].dfmotion.total_frm[2]; 
		mydb.vh[1].move_motion(2, mydb.vh[1].Anim.Frame_num);
	  
		break;
	} //switch case
  	


  // RESETTING //////////////////////////////
   BBall.chk_shoot = NOSHOOT;
   mydb.vh[1].Anim.cur_frm = 0;
   //////////////////////////////////////////
 
   /////////////////////////////////////////
}

void TGLView::drawHand(int f_idx)
{
 // thumb 1 joint 1
  glTranslatef(0.0, -1.6, 0.0);
  glPushMatrix();
      
	  glTranslatef(6.0f - 0.66666666f, 0.66666666f, 0.66666666f);
	  //glScalef(0.33333333f, 0.33333333f, 0.33333333f);
	  glRotatef(cfinger.finger[f_idx].y, 0.0f, 1.0f, 0.0f);
	  drawQuad(5.0, 1.0);
      glutSolidSphere(1.0, 15, 12); 
	  // thumb joint 2
	  glPushMatrix();
		
	    glTranslatef(6.0f, 0.0f, 0.0f);
		glRotatef(fabs(cfinger.finger[f_idx].y-cfinger.finger[f_idx+1].y), 0.0f, 1.0f, 0.0f);
		//glLine
		drawQuad(5.0, 1.0);
		glutSolidSphere(1.0, 15, 12); 
	  glPopMatrix(); // thumb 2
  glPopMatrix(); // pop off thumb modelview

}
  
void TGLView::drawjoint(int cur_frm, MyBone sk_bone)
{
  int i;
// Drawing joint ball/join
 	if (sk_bone.id == Hips){ //root translate
        glTranslatef(sk_bone.trans[cur_frm].x, sk_bone.trans[cur_frm].y, sk_bone.trans[cur_frm].z);
	}
	else{ // all joint except for root
	//
      Draw_Ellipse(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z, sk_bone.scale);
      glTranslatef(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z);
    }
	
	glRotatef(sk_bone.rot[cur_frm].z, 0.0, 0.0, 1.0);  //z
	glRotatef(sk_bone.rot[cur_frm].x, 1.0, 0.0, 0.0);  //x
	glRotatef(sk_bone.rot[cur_frm].y, 0.0, 1.0, 0.0);  //y
	
	 glMaterialfv(GL_FRONT, GL_DIFFUSE, bodycolor);//AMBIENT_AND
     //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
     //glMaterialf(GL_FRONT, GL_SHININESS, 10);

	 if(sk_bone.id == Head)
	 {
		 //DrawPlayMode(mydb.vh[1].Anim.S_status);

			glEnable(GL_LIGHTING);
		    glutSolidSphere(HEAD_RAD, 15, 12); 
			/*glPushMatrix();
			glTranslatef(0, -40, 0);
			
		
          	for(TGeoNodes::iterator it = allNodes[0].begin(); it != allNodes[0].end(); ++it)
			{
					glScalef(25, 25, 25);
					(*it)->Draw();
			}
		      //mySolidCylinder(1.5, 1.5f, 3.0f, 0.5, 32);
		  	glPopMatrix();*/
	 }
	 else 
	 {
	     glDisable(GL_TEXTURE_2D);
		 glutSolidSphere(JOINT_RAD, 15, 12); // joint ball drawing
		
		if (sk_bone.id == RightAnkle || sk_bone.id == LeftAnkle ) // foot box drawing
		{
			  glPushMatrix();
			 
				 if (sk_bone.id== LeftAnkle) {
					 i=3;
					 glTranslatef(-2.5, -1.5, 1);
				 }
				 else {
					 i=4;
					 glTranslatef(2.5, -1.5, 1);
				 }
			 		
          		for(TGeoNodes::iterator it = allNodes[i].begin(); it != allNodes[i].end(); ++it)
				{
						glScalef(25, 25, 25);
						(*it)->Draw();
				}
				//	glTranslatef(0, 0, -1.5);
				//	glRotatef(-90, 0 ,1,0); 
				//	drawQuad(6.0, 1.0);
			   glPopMatrix();
		 }
		 else if (sk_bone.id== LeftWrist || sk_bone.id== RightWrist ) //hand end_site drawing
		 {
			 //MyVector m_size(0.75, 0.0, 0.75);
			 // glPushMatrix();
			//	glScalef(1.0, 1.0, 2.0);
			//	Draw_Ellipse(0, -3.14, 0, m_size);
			//  glPopMatrix();
		 if (sk_bone.id== LeftWrist) {
				 i=1;
				 glTranslatef(-5, -22, 0);
			 }
             else {
				 i=2;
				 glTranslatef(5, -22, 0);
			 }
			//glMaterialfv(GL_FRONT, GL_AMBIENT, wallcolor); 	
          	for(TGeoNodes::iterator it = allNodes[i].begin(); it != allNodes[i].end(); ++it)
			{
					glScalef(25, 25, 25);
					(*it)->Draw();
			}
		 }
	  }

}

// all body drawingTGLView::
void TGLView::drawBody(int cur_frame, int which_model)
{
glPushMatrix();
     glTranslatef(mydb.vh[which_model].Anim.conv_trans.x, mydb.vh[which_model].Anim.conv_trans.y, mydb.vh[which_model].Anim.conv_trans.z);
	 glRotatef(mydb.vh[which_model].Anim.conv_rot.y, 0,1,0); 
     drawjoint(cur_frame, mydb.vh[which_model].MModel.A_bone[0]); //1
    
		glPushMatrix();
			drawjoint(cur_frame, mydb.vh[which_model].MModel.LL_bone[0]); //2
			drawjoint(cur_frame, mydb.vh[which_model].MModel.LL_bone[1]); //3
			drawjoint(cur_frame, mydb.vh[which_model].MModel.LL_bone[2]); //4
		glPopMatrix();

		glPushMatrix();
			drawjoint(cur_frame, mydb.vh[which_model].MModel.RL_bone[0]); //5
			drawjoint(cur_frame, mydb.vh[which_model].MModel.RL_bone[1]); //6
			drawjoint(cur_frame, mydb.vh[which_model].MModel.RL_bone[2]); //7
		glPopMatrix();

	    glPushMatrix();
			drawjoint(cur_frame, mydb.vh[which_model].MModel.A_bone[1]); //8
			drawjoint(cur_frame, mydb.vh[which_model].MModel.A_bone[2]); //9
	
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh[which_model].MModel.LA_bone[0]); //10
				drawjoint(cur_frame, mydb.vh[which_model].MModel.LA_bone[1]);//11
				drawjoint(cur_frame, mydb.vh[which_model].MModel.LA_bone[2]);  //12
				drawjoint(cur_frame, mydb.vh[which_model].MModel.LA_bone[3]);  //13
			glPopMatrix();
		
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh[which_model].MModel.RA_bone[0]);  //14
				drawjoint(cur_frame, mydb.vh[which_model].MModel.RA_bone[1]);  //15
				drawjoint(cur_frame, mydb.vh[which_model].MModel.RA_bone[2]);   //16
				drawjoint(cur_frame, mydb.vh[which_model].MModel.RA_bone[3]);   //17
		    glPopMatrix();

			glPushMatrix();
				drawjoint(cur_frame, mydb.vh[which_model].MModel.H_bone[0]);  //18
				drawjoint(cur_frame, mydb.vh[which_model].MModel.H_bone[1]);  //19
			glPopMatrix();

	glPopMatrix();

glPopMatrix();

}

void TGLView::OnDraw()
{
	///////////////////////////////////////////////////////////////////////
	//ステンシルバッファをクリアしなきゃ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    gluLookAt(0.0, 2.0, mydb.vh[1].Anim.distZ, 0.0, 1.0,0.0, 0.0, 1.0, 0.0);
//camera setting
/*
	static Affinef R180(Rad(180),'y');
	Affinef afProjector = afBody * R180 * R * R180;
	Affinef afCam = (afEye * afProjector).inv();
	glLoadMatrixf(afCam);
*/
  glPushMatrix();
     glTranslatef(0, -nHeight, mydb.vh[1].Anim.camera.z);
	 glRotatef(mydb.vh[1].Anim.camera.y, 0.0f, 1.0f, 0.0f);
	 
 	 glEnable(GL_LIGHTING);
     //hip -5 
	 glMaterialfv(GL_FRONT, GL_AMBIENT, wallcolor);
	
	 glPushMatrix();
		 glRotatef(-90, 0.0f, 1.0f, 0.0f);
		 drawBody(mydb.vh[1].Anim.cur_frm, 1);
		 drawBody(mydb.vh[0].Anim.cur_frm, 0);
     glPopMatrix(); 
	//床の描画　by武山さん
      glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_AMBIENT, gray);
	      glBindTexture(GL_TEXTURE_2D, tex[0]); //tex[1]
		  glEnable(GL_TEXTURE_2D);

	      //glScalef(10.0, 1.0, 10.0); 
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
	  glPopMatrix();
	  /*
	  	//ball display
	  	for(TGeoNodes::iterator it = allNodes[0].begin(); it != allNodes[0].end(); ++it)
		{
					glPushMatrix();
						glTranslatef(BBall.new_pos.x, BBall.new_pos.y, BBall.new_pos.z);
						glRotatef(nowBallPole.Y()*180/M_PI, 0.0, -1.0, 0.0);
						glRotatef(nowBallPole.Z()*180/M_PI, -cos(nowBallPole.Y()), 0.0, -sin(nowBallPole.Y()));
						(*it)->Draw();
					glPopMatrix();
		}
*/

	    glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
		 glTranslatef(BBall.new_pos.x+5, BBall.new_pos.y, BBall.new_pos.z);
	     glRotatef(tokin_rot[2], 0.0f, 0.0f, 1.0f); //Z yaw
		 //glRotatef(tokin_rot[1], 0.0f, 1.0f, 0.0f); //Y
		 glRotatef(tokin_rot[0], 1.0f, 0.0f, 0.0f); //X
	     //glutSolidTeapot(5.0);
		 mySolidCylinder(1.0, 1.0, 1.0, 3.5, 32);
		glPopMatrix();
       
		glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
		 glTranslatef(BBall.new_pos.x+15, BBall.new_pos.y, BBall.new_pos.z);
	     glRotatef(tokin_rot[1], 0.0f, 1.0f, 0.0f); //Y
		 mySolidCylinder(1.0, 1.0, 1.0, 3.5, 32);
		glPopMatrix();

		glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
		 glTranslatef(BBall.new_pos.x+25, BBall.new_pos.y, BBall.new_pos.z);
	     glRotatef(tokin_rot[0], 1.0f, 0.0f, 0.0f); //Y
		 mySolidCylinder(1.0, 1.0, 1.0, 3.5, 32);
		glPopMatrix();
		//draw_hand
		glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
			//glTranslatef(sp_info.user_hand[0].X(), sp_info.user_hand[0].Y(),sp_info.user_hand[0].Z());
			glTranslatef(BBall.new_pos.x, BBall.new_pos.y, BBall.new_pos.z);
			glRotatef(cfinger.wristj.y, 0.0f, 1.0f, 0.0f); //y pitch 16
			glRotatef(cfinger.wristj.z, 0.0f, 0.0f, 1.0f);	//z sayu yaw 17	
			//glScalef(1.0, 1.0, 0.6);
			//glutSolidSphere(2.0, 15, 12); 

			//glTranslatef(2.5, 0.0, 0.0);
			//glRotatef(cfinger.finger[15].y, 0.0f, 1.0f, 0.0f);
			//glScalef(1.0, 0.6, 1.0);
			drawQuad(5.0, 1.0);
			//glutSolidSphere(2.0, 15, 12); 
/*
			glPushMatrix();
			     glTranslatef(-1.0, 1.0, 3.0);
			     glRotatef(cfinger.finger[1].y, 0.0f, 1.0f, 0.0f);
	             //glRotatef(0.01239*(cfinger.finger[1].y-35), 0.0f, 1.0f, 0.0f);
	             drawQuad(5.0, 1.0);
				 glutSolidSphere(1.0, 15, 12); 
             //glPopMatrix(); // pop off t	
	  
	                glPushMatrix();
			        //glTranslatef(-3.0, 2.0, -3.0);
			        glTranslatef(5.0, 0.0, 0.0);
			        glRotatef(fabs(cfinger.finger[1].y-cfinger.finger[2].y), 0.0f, 1.0f, 0.0f);
					drawQuad(5.0, 1.0);
					glutSolidSphere(1.0, 15, 12); 
			glPopMatrix(); // thumb 2
			glPopMatrix(); // pop off t	
			*/
			
			glTranslatef(-4.0, 3.0, 0.0);
			glRotatef(100, 1.0f, 0.0f, 0.0f);	
			glRotatef(45, 0.0f, 0.0f, 1.0f);	
			for(int k=1;k<5;k++)
			{
				int j=1+3*k;
				
				//glRotatef(cfinger.finger[j-1].y-100, 1.0f, 0.0f, 0.0f);
				drawHand(j);
			}
  
		glPopMatrix();// wrist*/

//ytakeyama end
/*int tempflag = 0;

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
						glRotatef(nowBallPole.Y()*180/M_PI, 0.0, -1.0, 0.0);
						glRotatef(nowBallPole.Z()*180/M_PI, -cos(nowBallPole.Y()), 0.0, -sin(nowBallPole.Y()));
						(*it)->Draw();
					glPopMatrix();
					tempflag = 1;
				}
				break;
			case 1:
				//lefthand display
				glPushMatrix();
				    glTranslatef(sp_info.grp_pos[0].X(),  sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z());
					glRotatef(180, 0.0, 1.0, 0.0);
					(*it)->Draw();
				glPopMatrix();
			break;
			case 2:
				//right display
				glPushMatrix();
				    glTranslatef(sp_info.grp_pos[1].X(),  sp_info.grp_pos[1].Y(), sp_info.grp_pos[1].Z());
					glRotatef(180, 0.0, -1.0, 0.0);
					(*it)->Draw();
				glPopMatrix();
			break;
			}
		}
}
*/
//shadow 
//	DrawShadow();
glPopMatrix();

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


void TGLView::OnSize(int w, int h)
{
  GLfloat aspect;

  if(h == 0) { h=1;}

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  aspect =(float) w/h;
  //for VRML of Ytakeyama 10000以上
  gluPerspective(30.0, aspect, 1.0, 2000);

  glMatrixMode(GL_MODELVIEW);

}

void TGLView::OnSpecialKey(int key, int x, int y) 
{
  if (key == GLUT_KEY_LEFT)    { mydb.vh[1].Anim.camera.y -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { mydb.vh[1].Anim.camera.y += deltaRot; }
  if (key == GLUT_KEY_UP)      { mydb.vh[1].Anim.camera.z  -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { mydb.vh[1].Anim.camera.z  += deltaRot; }
  glutPostRedisplay();
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	press_key = key;
	//Sleep(5);
	// キーボードの処理はここで行う
	//::OnKeyboard(press_key, 0, 0);
	int cur_frm =  mydb.vh[1].Anim.cur_frm;
	switch(press_key){
	 case 'w': //y
		  BBall.new_pos.y += delta_Pad;
		  //sp_info.ball_pos.Y() += delta_Pad;
		  break;
	 case 's': 
		  BBall.new_pos.y -= delta_Pad;
		  //sp_info.ball_pos.Y() -= delta_Pad;
		  break;
	 case 'a': //x
		  BBall.new_pos.x += delta_Pad;
		  break;
	 case 'd': 
		  BBall.new_pos.x -= delta_Pad;
		  break;
	 case 'Z': //z
		  sp_info.user_hand[0].Z() += delta_Pad;
		  break;
	 case 'z': 
		  sp_info.user_hand[0].Z() -= delta_Pad;
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
		init_ball();
		break;
	case 'n':
		BBall.angle += 5;
		break;
	case 'm':
		BBall.angle -= 5;
		break;
	case ' ': 
		animating = !animating; //space
		printf( "cur frame: %d  hand y pos: %f ball x: %f ball y: %f ball z: %f \n", cur_frm, mydb.vh[1].MModel.LA_bone[3].position_global[cur_frm].y, BBall.new_pos.x, BBall.new_pos.y, BBall.new_pos.z);
	   	break;
	case '\t': //shooting
		shot_control();
	    break;
    case 'l':
       transition_motion(LEFT_PHASE);
		break;
	case 'r':
        transition_motion(RIGHT_PHASE);
		break;
	case 'p':
		spidar_save();
		break;
	case '\x1b': //Esc
		//SPIDAR-H timer
		bMinForce = true;

		timeKillEvent(SpidarUpdateTimer);
		timeKillEvent(TokinUpdateTimer);
		timeEndPeriod(1);
		Exit();
		break;

	default:
		break;
	}

	//press_key = 0;	
	
}

void CALLBACK SpidarUpdateProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD)
{
	//((TGLView*)dwUser)->update_spidar();
	
}

void CALLBACK TokinUpdateProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD)
{

	tokin.GetDeviceData();

	tokin_rot[2]= RadiansToDegrees(tokin.angle[2]);
	tokin_rot[1]= RadiansToDegrees(tokin.angle[1]);
	tokin_rot[0]= RadiansToDegrees(tokin.angle[0]);
		
//		if (USE_CYBERGLOVE) glove_update();
		// tokin_rot[2]= RadiansToDegrees(tokin.GetEuler().Z());
		// tokin_rot[1]= RadiansToDegrees(tokin.GetEuler().Y());
		// tokin_rot[0]= RadiansToDegrees(tokin.GetEuler().X());
   //std::cout << "angle z=" << tokin_rot[2] << "°" << std::endl;
   //std::cout << "angle y=" << tokin_rot[1] << "°" << std::endl;


}

void TGLView::glove_init(void)
{
	 //cyberglove
  //#ifdef USE_CYBERGLOVE
    CCyberGlove cglove(SERIAL1, B115200, USE_CYBERGLOVE);
  //#endif

}
void TGLView::glove_update(void)
{

if (cglove.GetNSensors() != 0)
{
	cglove.UpdateData();

	for (int i=0; i<cglove.GetNSensors(); i++)
	{
		cfinger.finger[i].y=cglove.GetData(i);
		if(i==16) cfinger.wristj.y=cglove.GetData(i); //pitch y
		if(i==17) cfinger.wristj.z=cglove.GetData(i); //yaw: z

	}
	//std::cout << "finger y=" << cfinger.finger[1].y << "°" << std::endl;
}

}

void TGLView::init_db(void)
{

char ssql[50];
		// MYDB Init connection
   int ret = mydb.sqlconn();
   if (ret == 1)
   {
	   init_paddle();
	   init_hand();
	   init_ball();	
    
	 // compute database search condition
	   mydb.load_model(1);//1
	   for (int i=0; i<3;i++) 
		   mydb.load_motion(i);
       //vh[1] hand Zaxis position 2003 10 26
	   conn_motion(READY_PHASE);
	   //another model
	   mydb.load_model(3);//3 jeong
       strcpy(ssql, mydb.make_sql(PARTNER_TALK, 0));	
	   int maxrows = mydb.sqlexec(ssql, 0);
   }
   
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
 glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
 glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

 //GLfloat light0pos[] = {10.0, 1.0, 5.0, 1.0};			//shadow
 glLightfv(GL_LIGHT0, GL_POSITION, light0pos);			//shadow
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHTING);
	
  glClearColor(0.75, 0.75, 1.0, 0.0); //sky color
  //glClearColor(0.0, 0.0, 0.0, 0.0); //black
  glEnable(GL_DEPTH_TEST); //hidden surface removed

 
  glFrontFace(GL_CCW); //no counterclockwise
  glEnable(GL_CULL_FACE); 
  glCullFace(GL_BACK);
 
  //init motion
   init_db();
   Load_Texture();
  
   glutReshapeWindow(400,300);
//glutFullScreen();
   glutSetCursor(GLUT_CURSOR_NONE);

   //tokin use
    tokin.Init_tokin();
	tokin.SetStdData();
	tokin.Poll();
	tokin.GetDeviceData();

   //SPIDAR-H
   init_spidar();
  //timer setting	
	timeBeginPeriod(1);
	SpidarUpdateTimer = timeSetEvent(SpidarUpdateRate, 1, SpidarUpdateProc, (DWORD)this, TIME_PERIODIC);
//tokin
	TokinUpdateTimer = timeSetEvent(TokinUpdateRate, 1, TokinUpdateProc, (DWORD)this, TIME_PERIODIC);
	SetTimer(33, &TGLView::OnTimer); //Step()を呼び出す周期 33
	dwTime = timeGetTime(); //GetTickCount();

}

void TGLView::Ball_Control(int cur_frm)
{
/*
		if (bVCollide) 
		{
			BBall.new_pos = mydb.vh[1].Ret_Global(cur_frm, 100,1);
		}
		else if (bHCollide) 
		//else
		{   //HOLDING or HALFHOLD
			//BBall.new_pos =BBall.init_pos;
			BBall.new_pos.x =sp_info.ball_pos.X();
			BBall.new_pos.y =sp_info.ball_pos.Y();
		BBall.new_pos.z =sp_info.ball_pos.Z();

		}
*/
			if(BBall.chk_state == THROWING)
			{
			 
			  	  BBall.new_pos = BBall.Parabola(BBall.angle, t);
				  if (!bVCollide) ballColliding(cur_frm);
				  if (t>=1 && BBall.minY <= GnHeight || BBall.new_pos.z > 100){
					 init_ball();
				  }
			     t +=mydb.vh[1].Anim.unit_Time;
			}

			if (bVCollide && BBall.chk_state == FALLING) 
			{
				BBall.new_pos = BBall.Fall_Roll(15,ft);
			
				if(BBall.bounce <.1)
				{
				       init_ball();
					   BBall.chk_state = PAUSE;
					   bVCollide = FALSE;
				 }
				 ft +=(double)mydb.vh[1].Anim.unit_Time;
			  	
			}

}
void TGLView::Step()
{
	stepHz ++;
    //time distance 計測
	dwLastTime = dwTime;
	dwTime = timeGetTime();
	float pass = float(dwTime - dwLastTime) / 1000;


	 if (animating) 
	 {
		 mydb.vh[1].Anim.cur_frm++;
		 if (mydb.vh[1].Anim.cur_frm >= mydb.vh[1].Anim.Frame_num)
		 {
			 transition_motion(READY_PHASE);
		 }
		 // another model
		 mydb.vh[0].Anim.cur_frm++;
         if (mydb.vh[0].Anim.cur_frm >= mydb.vh[0].Anim.Frame_num)
		 {
			 mydb.vh[0].Anim.cur_frm=0;
		 }
		 //setting
		 Ball_Control(mydb.vh[1].Anim.cur_frm);
	
		
	 } //animating
 
	if (bDraw) Invalidate();			//	描画要求
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
      for (int i = 0; i < 15; i++ )
	  {
		 //fprintf(stream, "%d, %f, %f, %f, %f, %d, %d, %d, %d\n", exp_data.Exp_Mode[i], exp_data.shotpos[i].X(), exp_data.shotpos[i].Y(), exp_data.shotpos[i].Z(), exp_data.shotvelocity[i], exp_data.motion_id[i], exp_data.success[i], exp_data.start_time[i],exp_data.elasped_time[i]);
	  }
      //printf( "world_postion text file is saved! \n" );
      fclose( stream );
   }
   else   printf( "Open of file is failed! \n" );

}

