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
//MPI
#include "mpi/mpi.h"
//MY HEAD FILE
/////////////////////////////////////////////////////////
#include "GLView.h"
#include "odbcexec.h"  // database open/close
#include "tex.h"       // primitive definition
#include "Ball.h" 
/////////////////////////////////////////////////////////
// check variables
static int full_mode =0;
static int chk_detect = NOCOLLISION;
static int chk_end =0;
static int chk_interp = 0;
static int animating = 0;

CBall BBall;
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
static GLfloat ShadowMat[4][4];
static GLfloat s_rad = 1;
/////////////////////////////////////////////////////////
using namespace std;
const float EYEOFFSET=0.8;

//SLCamera
int MainAngleSide = 0;
int MainAngleVert = 0;

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

bool TGLView::LoadSettings(){
	char* cname = getenv("COMPUTERNAME");
	char* p = cname;
	while(*p && !isdigit(*p)) p++;
	
	cpNum = -1;
		// 暫定コード
	cout << "Hostname = " << cname << endl;

	if (strlen(p)) cpNum = atoi(p);
	cout << "<cpNum> : "<< cpNum << endl;
	if (cpNum == -1) return false;

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
//TGLView::TGLView(bool b):bSender(b){
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
//	afBody = Affinef(0, 0, 10);
//	afBody.LookAtGL(Vec3f(0, 0, 0)/*視点の位置*/, Vec3f(0,17,0)/*カメラの上方向*/);
	
	// アバターの手の設定
	hand_radius = ini_radius;//追加：赤羽
//	afLHand = Affinef(-5,0,-5); //追加：赤羽
//	afRHand = Affinef(5,0,-5); //追加：赤羽

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

	//MPI
	int counts[5]={10,60, 6, 3, 3};
	
	MPI_Datatype type[5]={MPI_INT,MPI_FLOAT, MPI_FLOAT, MPI_FLOAT,MPI_FLOAT};

	MPI_Aint byte[5], extent;
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

	MPI_Type_struct(5,counts,byte,type,&stMove_Data);
	MPI_Type_commit(&stMove_Data);

	/*
	//	通信の初期化
	cmdSocket.Register(new CmdEye(this));
	if (bSender) cmdSocket.Init(10100, 10000);
	else cmdSocket.Init(10000, 10100);
	*/
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
bool TGLView::OnTimer2(){
	slcamera.ShowData();
	return true;
}

void TGLView::OnIdle(){
	//Sleep(5);
	// キーボードの処理はここで行う
	//::OnKeyboard(press_key, 0, 0);
	switch(press_key){
	 case 'w': //y
		  t_hand.grp_pos0.y +=delta_Pad;
		  t_hand.grp_pos2.y +=delta_Pad;
		  BBall.p0.y = (t_hand.grp_pos0.y + t_hand.grp_pos2.y)/2;
		  printf( "hand y pos: %f ball y: %f \n", t_hand.grp_pos0.y, BBall.p0.y);
		  break;
	 case 's': 
		  t_hand.grp_pos0.y -=delta_Pad;
		  t_hand.grp_pos2.y -=delta_Pad;
		  BBall.p0.y = (t_hand.grp_pos0.y + t_hand.grp_pos2.y)/2;
		  printf( "hand y pos: %f ball y: %f \n", t_hand.grp_pos0.y, BBall.p0.y);
		  break;
	 case 'a': //x
		  t_hand.grp_pos0.x +=delta_Pad;
		  t_hand.grp_pos2.x +=delta_Pad;
		  BBall.p0.x = (t_hand.grp_pos0.x + t_hand.grp_pos2.x)/2;
		  printf( "hand x pos: %f ball x: %f \n", t_hand.grp_pos0.x, BBall.p0.x);
		  break;
	 case 'd': 
		  t_hand.grp_pos0.x -=delta_Pad;
		  t_hand.grp_pos2.x -=delta_Pad;
		  BBall.p0.x = (t_hand.grp_pos0.x + t_hand.grp_pos2.x)/2;
		  printf( "hand x pos: %f ball x: %f \n", t_hand.grp_pos0.x, BBall.p0.x);
		  break;
	 case 'A': //z
		  t_hand.grp_pos0.z +=delta_Pad;
		  t_hand.grp_pos2.z +=delta_Pad;
		  BBall.p0.z = (t_hand.grp_pos0.z + t_hand.grp_pos2.z)/2;
		  printf( "hand z pos: %f ball z: %f \n", t_hand.grp_pos0.z, BBall.p0.z);
		  break;
	 case 'D': 
		  t_hand.grp_pos0.z -=delta_Pad;
		  t_hand.grp_pos2.z -=delta_Pad;
		  BBall.p0.z = (t_hand.grp_pos0.z + t_hand.grp_pos2.z)/2;
		  break;
	case 'b':
		glutPostRedisplay();
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
	  BBall.chk_move = MOVING;
	  chk_detect = NOCOLLISION;
	  break;
	case 'z':
	  Anim.distZ -= 5;
	  break;
    case 'Z':
	  Anim.distZ += 5;
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
//	std::cout << devMan;

	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	for(int i=0; i<2; i++)
	{
	   h_spidar[i].Init(devMan, motorPos[i], MAXON, LPP, 0.5f, 20.0f); 
	   h_spidar[i].Calib();
	}

}
void TGLView::reset_spidar()
{
	  /*
	BBall.dev_pos.x= Ball.p0.x/INCH_MITER; //1m = 39.37 inch 
    float du_radius =(Ball.radius/INCH_MITER);
	spidar.Calib();
	sp_pos.X()= -(ball_gl.Pos().X()+spidar.GetPos().X())+du_radius; 
    sp_pos.Y()= ball_gl.Pos().Y()+spidar.GetPos().Y();
    sp_pos.Z()= ball_gl.Pos().Z()+spidar.GetPos().Z();

	spidar2.Calib();
	sp_pos1.X()= -(ball_gl.Pos().X()+spidar2.GetPos().X())+du_radius; 
    sp_pos1.Y()= ball_gl.Pos().Y()+spidar2.GetPos().Y();
    sp_pos1.Z()= ball_gl.Pos().Z()+spidar2.GetPos().Z();*/

	for(int i=0;i<2;i++)
	{
		h_spidar[i].Calib();
	}
	//init_ball();

}
void TGLView::update_spidar()
{

     Vec3f t_force[2];
     Vec3f t_pos[2];

    for(int i=0; i<2;i++)
	{
	  t_force[i] = Vec3f(0, 0, 0);
	
	  h_spidar[i].Update();
      t_pos[i] = h_spidar[i].GetPos()*INCH_MITER;
	}

    if (fabs(t_pos[0].X()) <= 100 && fabs(t_pos[1].X()) <= 100 )
	 {
		 //printf( "spidar.x: %f, y: %f  z%f \n", ss.x, ss.y, ss.z);
		 t_hand.grp_pos0.x = t_hand.user_hand0.x - t_pos[0].X();
		 t_hand.grp_pos0.y = t_hand.user_hand0.y + t_pos[0].Y();
		 t_hand.grp_pos0.z = t_hand.user_hand0.z - t_pos[0].Z(); //x,z sign , inverse
         
		 t_hand.grp_pos2.x = t_hand.user_hand2.x - t_pos[1].X();
		 t_hand.grp_pos2.y = t_hand.user_hand2.y + t_pos[1].Y();
		 t_hand.grp_pos2.z = t_hand.user_hand2.z - t_pos[1].Z();
		
		 //printf( "hand pos.x: %f, y: %f  z%f \n", grp_pos0.x, grp_pos0.y, grp_pos0.z);
	
  		 double dis = MyVectorDistance(t_hand.grp_pos0, BBall.p0); 
		 double dis2 =MyVectorDistance(t_hand.grp_pos2, BBall.p0) ; 
	 	 double t_dis = MyVectorDistance(t_hand.grp_pos0, t_hand.grp_pos2);
	     printf( "dis: %f,  dis2: %f  hand_dis: %f\n",dis, dis2, t_dis);
		if (dis-BBall.radius < s_rad)
		{
			t_force[0].X() = -K * ((BBall.radius+s_rad - dis)/INCH_MITER); 
		}
		h_spidar[0].SetForce(t_force[0]);
		
		if (dis2-BBall.radius < s_rad)
		{
		   t_force[1].X() = -K * ((BBall.radius+s_rad - dis2)/INCH_MITER); 
	
		}
		h_spidar[1].SetForce(t_force[1]);
        t_hand.h_force =t_force[1].X();

		if (chk_detect == NOCOLLISION && BBall.chk_move != MOVING) handColliding();
	          	
//b_force,x != force.X()
		if (BBall.chk_shoot!=SHOOTED && abs(t_force[0].X()) >= 0.5 && abs(t_force[1].X()) >= 0.5)
		 {
			     BBall.b_force.x = t_force[0].X();
		 		 BBall.dev_pos.x = (t_hand.grp_pos0.x+t_hand.grp_pos2.x)/2; 
				 BBall.dev_pos.y = (t_hand.grp_pos0.y+t_hand.grp_pos2.y)/2;
				 BBall.dev_pos.z = (t_hand.grp_pos0.z+t_hand.grp_pos2.z)/2; 
				
				 BBall.p0 = BBall.dev_pos;
		  	 //printf( "force: %f,  ball pos: x%f  y%f %i\n",force.X(), Ball.dev_pos.x, Ball.dev_pos.y, chk_hand);
		 }
		 if (BBall.chk_move !=MOVING && BBall.chk_shoot !=SHOOTED && t_hand.h_force <= 0.1 && t_hand.hand_dis > 20 && BBall.p0.y <= 60) 
		 {
				BBall.chk_shoot = SHOOTED;
				BBall.chk_move= MOVING;

				BBall.p0 = BBall.dev_pos;
				//printf( "force: %f, ball shooting pos: x:%f  y:%f z:%f\n",t_hand.h_force, Ball.p0.x, Ball.p0.y, Ball.p0.z);
		 }
	 }	 //<=100

}
// initial ball setting
void TGLView::init_paddle()
{
   
   paddle_v.x=-15;
   paddle_v.y=nRange; //nHeight-nRange; //0.88 m (2.8864) ~ 1m 1m=3.28feet ;30inch
   paddle_v.z=-115;//about 2.9 m 9.512 feet 115 inch
 
}
void TGLView::init_hand()
{
   t_hand.grp_pos0.x = 16;
   t_hand.grp_pos0.y = nRange;
   t_hand.grp_pos0.z = paddle_v.z;
   t_hand.grp_pos0.x = t_hand.grp_pos0.x-BBall.radius;
  
   t_hand.grp_pos2 = t_hand.grp_pos0;
   t_hand.grp_pos2.x = t_hand.grp_pos2.x-BBall.radius*2;
      
   //t_hand.user_hand2 =t_hand.user_hand0;
   //t_hand.grp_pos0 =t_hand.user_hand0;
   //t_hand.grp_pos2 =t_hand.user_hand0;
   reset_spidar();
}
// initial ball setting
void TGLView::init_ball()
{
   
   t=0.0;
   dt=0.0;
   
   BBall.Init();
   //Anim.init();		

   reflex_v.x=0.0;
   reflex_v.y=0.0;
   reflex_v.z=0.0;
   
   chk_detect = NOCOLLISION;
      
}

//ball collision detect
void TGLView::ballColliding(int cur_frm)
{
  float distance = JOINT_RAD+BBall.radius;
  MyVector l_pos, r_pos;

  l_pos.x=LA_bone[3].position_global[cur_frm].x;
  l_pos.y=LA_bone[3].position_global[cur_frm].y;
  l_pos.z=LA_bone[3].position_global[cur_frm].z;

  r_pos.x = RA_bone[3].position_global[cur_frm].x;
  r_pos.y = RA_bone[3].position_global[cur_frm].y;
  r_pos.z = RA_bone[3].position_global[cur_frm].z;

  avg_epos.x=(l_pos.x+r_pos.x)/2;
  avg_epos.y=(l_pos.y+r_pos.y)/2;
  avg_epos.z=(l_pos.z+r_pos.z)/2;

  float left_rad = MyVectorDistance(BBall.p1, l_pos);
  float right_rad = MyVectorDistance(BBall.p1, r_pos);
  float t_rad = MyVectorDistance(BBall.p1, avg_epos);
 
  if (t_rad < distance || left_rad < distance || right_rad < distance)
  {
	  if(Anim.Off_frm > 1) 
	  {
		  chk_detect= V_COLLISION; // if data is searched, chk_detect = 1
		  BBall.chk_move = PAUSE;
		  Play_wave("sound/floor_bounce.wav");
		  int off_frm = Anim.Off_frm;
		  //reflect start point
	      off_v.x= (LA_bone[3].position_global[off_frm].x+RA_bone[3].position_global[off_frm].x)/2;
	      off_v.y= (LA_bone[3].position_global[off_frm].y+RA_bone[3].position_global[off_frm].y)/2;
	      off_v.z= (LA_bone[3].position_global[off_frm].z+RA_bone[3].position_global[off_frm].z)/2;
	  }
	  
  }
  else
  { 
	  chk_detect = NOCOLLISION;
	 //if (pre_frm != 0 && off_frm >1 )  frame_num = pre_frm;
  }
}

//hand collision detect
void TGLView::handColliding()
{
	double t_rad = BBall.radius + s_rad;
	double l_dis = MyVectorDistance(t_hand.grp_pos0, BBall.p0); 
    double r_dis = MyVectorDistance(t_hand.grp_pos2, BBall.p0); 
 
	if (t_rad >= l_dis && t_rad >= r_dis)
	{
	 	chk_detect = H_COLLISION; 
		BBall.chk_move = PAUSE;
	}
	else 
	{
		chk_detect = NOCOLLISION;
		BBall.chk_move = MOVING;
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
   Anim.Key_frm = k_time/Anim.unit_Time;
   int ret= select_motion(init_type, Anim.Key_frm, (double*) pos);
  
   m_ElapsedTime = timeGetTime();
   printf( "\t%u (ms)\n", m_ElapsedTime-m_StartTime );
 
  if (ret !=0 ) // data searched
  {
	  setup_endeffector(LEFT_ARM, Anim.Frame_num);
	  setup_endeffector(RIGHT_ARM, Anim.Frame_num);
  }
    BBall.chk_shoot = NOSHOOT;
    Anim.cur_frm = 0;
}
// Drawing joint ball/join
void drawjoint(int cur_frm, MyBone sk_bone)
{
  
	if (sk_bone.id == Hips){ //root translate
        glTranslatef(sk_bone.trans[cur_frm].x, sk_bone.trans[cur_frm].y, sk_bone.trans[cur_frm].z);
	}
	else{ // all joint except for root
	//
      Draw_Ellipse(sk_bone.b_trans.x, sk_bone.b_trans.y,sk_bone.b_trans.z);
      glTranslatef(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z);
    }

	glRotatef(sk_bone.rot[cur_frm].z, 0.0, 0.0, 1.0);  //z
	glRotatef(sk_bone.rot[cur_frm].x, 1.0, 0.0, 0.0);  //x
	glRotatef(sk_bone.rot[cur_frm].y, 0.0, 1.0, 0.0);  //y

	if(sk_bone.id == chest1){
		//SLCamera
		//glRotatef(sk_bone.rot[cur_frm].z, 0.0, 0.0, 1.0);  //z
		glRotatef(MainAngleVert/2, 1.0, 0.0, 0.0);  //x
		glRotatef(MainAngleSide/2, 0.0, 1.0, 0.0);  //y
		//~SLCamera
	}

	// if joint is selected, its color will be change
    if (sk_bone.id == Head) {

			//SLCamera
	//	glRotatef(sk_bone.rot[cur_frm].z, 0.0, 0.0, 1.0);  //z
		glRotatef(MainAngleVert/3, 1.0, 0.0, 0.0);  //x
		glRotatef(MainAngleSide/3, 0.0, 1.0, 0.0);  //y
		//~SLCamera

	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, headcolor);
	}
	else 
	 glMaterialfv(GL_FRONT, GL_DIFFUSE, headcolor);
	
     glMaterialfv(GL_FRONT, GL_SPECULAR, white);
     glMaterialf(GL_FRONT, GL_SHININESS, 20);

	 if(sk_bone.id == Head){
		glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		  
		  glBindTexture(GL_TEXTURE_2D, tex[0]);

		  glEnable(GL_TEXTURE_2D);
		  glRotatef(90, 0.0f, 1.0f, 0.0f);
		  glRotatef(85, 1.0f, 0.0f, 0.0f);

		  drawSphere(BBall.radius/2);	
		glPopMatrix();

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
			  glPushMatrix();
				glScalef(1.0, 1.0, 2.0);
				Draw_Ellipse(0, -3.14, 0);
			  glPopMatrix();
		 }
	  }
}

// all body drawing
void TGLView::drawBody(int cur_frame)
{
glPushMatrix();
	
     drawjoint(cur_frame, A_bone[0]); //0
    
		glPushMatrix();
			drawjoint(cur_frame, LL_bone[0]); 
			drawjoint(cur_frame, LL_bone[1]); 
			drawjoint(cur_frame, LL_bone[2]); 
		glPopMatrix();

		glPushMatrix();
			drawjoint(cur_frame, RL_bone[0]);
			drawjoint(cur_frame, RL_bone[1]);
			drawjoint(cur_frame, RL_bone[2]);
		glPopMatrix();

	    glPushMatrix();
			drawjoint(cur_frame, A_bone[1]);
			drawjoint(cur_frame, A_bone[2]);  //8
	
			glPushMatrix();
				drawjoint(cur_frame, LA_bone[0]); //9
				drawjoint(cur_frame, LA_bone[1]);//10
				drawjoint(cur_frame, LA_bone[2]);  //11
				drawjoint(cur_frame, LA_bone[3]);  //12
			glPopMatrix();
		
			glPushMatrix();
				drawjoint(cur_frame, RA_bone[0]);  //13
				drawjoint(cur_frame, RA_bone[1]);  //14
				drawjoint(cur_frame, RA_bone[2]);   //15
				drawjoint(cur_frame, RA_bone[3]);   //16
		    glPopMatrix();

			glPushMatrix();
				drawjoint(cur_frame, H_bone[0]);  //17
				drawjoint(cur_frame, H_bone[1]);  //18
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
	if(myrank == 0) apply_send(Anim.cur_frm);
	MPI_Bcast(&M_DAT, 1, stMove_Data, 0, MPI_COMM_WORLD);
	if(myrank != 0) apply_receive();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    //gluLookAt(0.0, 2.0, Anim.distZ, 0.0, 1.0,0.0, 0.0,1.0, 0.0);
	static Affinef R180(Rad(180),'y');
	
	Affinef afProjector = afBody * R180 * R * R180;
	
	Affinef afCam = (afEye * afProjector).inv();
	glLoadMatrixf(afCam);

  glPushMatrix();
     glTranslatef(0, -nHeight, -30);
	 glRotatef(Anim.camera.x, 1.0f, 0.0f, 0.0f);
	 glRotatef(Anim.camera.y, 0.0f, 1.0f, 0.0f);
	 glRotatef(Anim.camera.z, 0.0f, 0.0f, 1.0f);
	 
    glEnable(GL_LIGHTING);
	 
  //hip -5 
      drawBody(Anim.cur_frm);

      glPushMatrix();
	      glBindTexture(GL_TEXTURE_2D, tex[1]);
		  glEnable(GL_TEXTURE_2D);
	      glScalef(100.0, 1.0, 100.0);
		  glTranslatef(-2, GnHeight, -0.5);
	      drawQuad(4, 1);
		  glDisable(GL_TEXTURE_2D);
	 
	  glPopMatrix();
   
	if (chk_end)
	 {
		// for end site tracking       
		int startFrame = (Anim.cur_frm < 50 ? 0: Anim.cur_frm - 50);
		for (int frame = startFrame; frame <= Anim.cur_frm; frame++)
		{
			glPushMatrix();	   
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
				glTranslatef(RA_bone[3].position_global[frame].x,RA_bone[3].position_global[frame].y,RA_bone[3].position_global[frame].z); 
				glutSolidCube(1.0);
			glPopMatrix();
		}

	 }
// ball display

		BBall.Draw();
		//shadow
		BBall.Shadow_ball(ShadowMat);

		glPushMatrix(); //left
	   	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(t_hand.grp_pos0.x,  t_hand.grp_pos0.y, t_hand.grp_pos0.z);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

       glPushMatrix(); //right
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(t_hand.grp_pos2.x,  t_hand.grp_pos2.y, t_hand.grp_pos2.z);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();
	
	   glPushMatrix();
	      glBindTexture(GL_TEXTURE_2D, tex[6]);
		  glEnable(GL_TEXTURE_2D);
		  glTranslatef(-paddle_v.x, paddle_v.y/2, paddle_v.z);
		  glRotatef(90, 1.0, 0.0, 0.0); 
	      drawQuad(20, 7); 
		  glDisable(GL_TEXTURE_2D);
	 
	   glPopMatrix();
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

  aspect = w/h;
 //gluPerspective(Anim.fov, aspect, 1, 1000);
  if (fov_v==0.0) fov_v =Anim.fov;
  gluPerspective(fov_v, aspect, 1.0, 1000);

  glMatrixMode(GL_MODELVIEW);

}
void TGLView::OnJoystick(unsigned int button, int x, int y, int z){
	std::cout << "B" << button << " x:" << x << " y:" << y << " z:" << z << std::endl;
}
void TGLView::OnSpecialKey(int key, int x, int y) 
{
  if (key == GLUT_KEY_LEFT)    { Anim.camera.y -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { Anim.camera.y += deltaRot; }
  if (key == GLUT_KEY_UP)      { Anim.camera.x -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { Anim.camera.x += deltaRot; }
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
	}
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

void TGLView::OnOpen(void){

	//SLCameraの初期化
 slcamera.InitSLC();
/*
 UINT ShowDataTimerId;
 timeBeginPeriod(1);
 ShowDataTimerId = timeSetEvent(500, 1, ShowDataTimerProc, (DWORD)&slcamera, TIME_PERIODIC);
*/


 //InitSLC(int* PreMainEyeAngle);

	//	GLの初期化
 GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
 tQuaternion nGround(0, GnHeight, 0, 0);
 MyVector vLight(-1, 50, 0); 
	
 glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
 glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
 glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
// glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

 glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHTING);
	
  //glClearColor(0.75, 0.75, 1.0, 0.0); sky color
  glClearColor(0.0, 0.0, 0.0, 0.0); //black
  glEnable(GL_DEPTH_TEST); //hidden surface removed
 
  glFrontFace(GL_CCW); //no counterclockwise
  glEnable(GL_CULL_FACE); 
  glCullFace(GL_BACK);
 
 //generate the shadow matrix
	CalcShadowMat(ShadowMat, nGround, vLight);
   // Init connection
   int ret = connection();
   if (ret == 1)
   {
	   init_paddle();
	   init_hand();
	   init_ball();	
    
	 // compute database search condition
	   Anim.Key_time = BBall.WhatTime(-1.5); //distance => frame
	   load_model(1);//1

	   Anim.Throw_num=random_throw();
	   conn_motion(0, Anim.Key_time);
   }

  // 	if (!bSender) glutFullScreen();
 	//MPI
   //debug ここでの同期は必要？
//	int i = 0;
//	MPI_Bcast(&i, 1, MPI_INT, 0, MPI_COMM_WORLD);
    Load_Texture();

	MPI_Barrier(MPI_COMM_WORLD);

//MPI
   if(myrank == 0){
		glutReshapeWindow(400,300);
   }else{
		glutFullScreen();
   }

//timer setting
	
	SetTimer(33, &TGLView::OnTimer); //Step()を呼び出す周期

	//SLCamera
	SetTimer(400, &TGLView::OnTimer2);
	//~SLCamera

	dwTime = GetTickCount();
}

void TGLView::Ball_Control(int cur_frm)
{
	
	if ( BBall.chk_shoot == REFLEX && BBall.chk_move != PAUSE )
	{
		BBall.p1=reflex_v;
	}
	else
	{
		if (chk_detect == V_COLLISION) {
		   BBall.p1.x= (LA_bone[3].position_global[cur_frm].x+RA_bone[3].position_global[cur_frm].x)/2; 
		   BBall.p1.y= (LA_bone[3].position_global[cur_frm].y+RA_bone[3].position_global[cur_frm].y)/2;
		   BBall.p1.z= (LA_bone[3].position_global[cur_frm].z+RA_bone[3].position_global[cur_frm].z)/2;
		}
		else if (chk_detect == H_COLLISION ) 
		{
			BBall.p1 =BBall.p0;
		}
	}

}
void TGLView::Step(){

	stepHz ++;
    //time distance 計測
	dwLastTime = dwTime;
	dwTime = GetTickCount();
	float pass = float(dwTime - dwLastTime) / 1000;

	//MPI
	if(myrank == 0)
	{
		 if (animating) 
		  {
			  Anim.cur_frm++;

			  if (Anim.Off_frm != 1 && Anim.cur_frm == Anim.Off_frm && BBall.chk_shoot != REFLEX && chk_detect == V_COLLISION)
			  { 
				  BBall.chk_shoot = REFLEX;
				  BBall.chk_move = MOVING; //start move ball
			  }
			 if (chk_detect == NOCOLLISION && BBall.chk_move == MOVING && BBall.chk_shoot != REFLEX) 
			  {
				  BBall.p1 = BBall.Parabola(BBall.angle, t);
				  if (t>=1 && BBall.minY <= GnHeight || BBall.p1.z > 50) 
					 init_ball();
				 t +=Anim.unit_Time;
			  }

			  if(( BBall.chk_shoot == REFLEX && Anim.cur_frm >= Anim.Off_frm ) || BBall.chk_move == CONTINUE)
			  {
				 reflex_v = BBall.Reflect(dt, off_v);
				 if (BBall.minZ < t_hand.grp_pos0.z) 
					 init_ball();
				 dt +=(Anim.unit_Time*2);
			  }
			
			 if (BBall.chk_shoot == SHOOTED ) 
			 {
				 conn_motion(1, Anim.Key_time); 
			     Play_wave("sound/ball_shoot.wav");
			 }
			 if (BBall.chk_shoot != SHOOTED && Anim.cur_frm >= Anim.Frame_num ) 
			 {
				 if ( BBall.minZ < t_hand.grp_pos0.z) 
				 {
					 init_ball();
				 }
				 else 
				 {
					 if(BBall.minZ != 0) BBall.chk_move = CONTINUE;
					 // throwing motion -> animation	      
				 }
				 //MPI communication 
                 //End_Interp(cur_frm-1); // 0~ frame_num-1
				 conn_motion(0, Anim.Key_time); 
				 Com_Interp(Anim.cur_frm);
			  }
		 
		 if (chk_detect != V_COLLISION && BBall.chk_move == MOVING && Anim.cur_frm >= Anim.Key_frm-5) 
			 ballColliding(Anim.cur_frm);
		 if (chk_detect == NOCOLLISION && BBall.chk_move != MOVING) handColliding();
		 Ball_Control(Anim.cur_frm);
	//SLCamera
		 slcamera.Step();
		 //slcamera.ShowData();
		 MainAngleSide = -slcamera.GetAngleSideData();
		 MainAngleVert = -slcamera.GetAngleVertData();
	//~SLCamera

		 } //animating
	}  //server only 実行

/*
	bool b = false;
	while(cmdSocket.Receive()) b = true;
	if (bSender && !b){
		CmdEye cmdEye(this);
		cmdEye.Send();
	}
*/
// all pc exit by old method 
//Stop folder 有無チェックのみ 00
	static int stopFileDetectCounter;
	stopFileDetectCounter ++;
	if (stopFileDetectCounter > 10)
	{
		if (_access("stop", 00) != -1)
		{
			Exit();
		}
		stopFileDetectCounter = 0;
	}

	if (bDraw) Invalidate();			//	描画要求
}

void TGLView::PrintHz(std::ostream& os) const{
	os << stepHz << "," << timeGetTime() << std::endl;
}

