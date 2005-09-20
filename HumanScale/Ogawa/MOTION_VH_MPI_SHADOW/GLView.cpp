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


/////////////////////////////////////////////////////////
#include "GLView.h"
#include "tex.h"      // primitive definition
#include "ball.h"

//MY ODBC
Cdirexec mydb;
CBall BBall;
SpidarInfo sp_info;
/////////////////////////////////////////////////////////
// check variables
static int full_mode =0;
static int chk_end =0;
static int chk_interp = 0;
static int animating = 0;
//////////////////////////////
//lighting & color definition
//static GLfloat light0pos[] = {0.0, 200.0, 200.0, 1.0};
static GLfloat light0pos[] = {80.0, 170.0, 200.0, 1.0};
static GLfloat light1pos[] = {200.0, 200.0, 0.0, 1.0};

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
	//shadow
	s_delta = 0;

	char* cname = getenv("COMPUTERNAME");
	char* p = cname;
	while(*p && !isdigit(*p)) p++;
	
	cpNum = -1;
		// �b��R�[�h
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
	// VPC00�`VPC03�̓t���[���x��Ȃ�
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

	// ini�t�@�C������f�[�^��ǂݍ���
	char temp_str[100];
	char ini_file[100];
	GetCurrentDirectory( 100,ini_file );
	strcat( ini_file,"\\setting.ini" );

	GetPrivateProfileString("AVATOR","radius","0.5",temp_str,100,ini_file); // �A�p�^�[�̔��a
	float ini_radius = atof( temp_str );
	GetPrivateProfileString("AVATOR","initialx","0",temp_str,100,ini_file); // x,y,z�����ʒu
	float ini_initialx = atof( temp_str );
	GetPrivateProfileString("AVATOR","initialy","0",temp_str,100,ini_file);
	float ini_initialy = atof( temp_str );
	GetPrivateProfileString("AVATOR","initialz","0",temp_str,100,ini_file);
	float ini_initialz = atof( temp_str );
	GetPrivateProfileString("AVATOR","eyeheight","0",temp_str,100,ini_file); //�A�o�^�|�̒��S����ڂ܂ł̍���
	float ini_eyeheight = atof( temp_str );
	afEye = Affinef(0,ini_eyeheight,0);
	GetPrivateProfileString("VIEWMODE","initialmode","0",temp_str,100,ini_file); // �����̎��_(0:�A�p�^ 1:�ォ�王�_ 2:���Վ��_)
	viewMode = atof( temp_str );
	GetPrivateProfileString("VIEWMODE","viewturn","0",temp_str,100,ini_file); // ���_�̉�]�p�x
	viewTurn = atof( temp_str );
	GetPrivateProfileString("TOPVIEW","topheight","6",temp_str,100,ini_file); // �����̏ォ�王�_�̍���
	topHeight = atof( temp_str );
	GetPrivateProfileString("TOPVIEW","topshift","0.2",temp_str,100,ini_file); // �ォ�王�_�̃A�p�^�[�̒��S����̂���
	topShift = atof( temp_str );
	GetPrivateProfileString("BIRDSVIEW","birdsheight","5",temp_str,100,ini_file); // �����̒��Վ��_�̍���
	birdsHeight = atof( temp_str );
	GetPrivateProfileString("BIRDSVIEW","birdsradius","30",temp_str,100,ini_file); // ���Վ��_�̊p�x
	birdsRadius = atof( temp_str );
	GetPrivateProfileString("BIRDSVIEW","birdsshift","1.1",temp_str,100,ini_file); // ���Վ��_�̃A�p�^�[�̒��S����̂���
	birdsShift = atof( temp_str );
	//	�A�o�^�[�̐ݒ�
	radius = ini_radius;//1.0f;
	// �A�o�^�[�̎�̐ݒ�
	hand_radius = ini_radius;//�ǉ��F�ԉH

	afInitialBody = Affinef(ini_initialx,ini_initialy,ini_initialz);
	afBody = afInitialBody;

	afBody.LookAtGL(Vec3f(ini_initialx,ini_initialy,ini_initialz+1),
		Vec3f(ini_initialx,ini_initialy+1, ini_initialz)); //posz(-EZ), posy(EY) ini_initialz
	//  �����̐ݒ�
	VecMyself[0] = Vec3f(        0, 0.0,-radius/2);
	VecMyself[1] = Vec3f( radius/2, 0.0, radius/2);
	VecMyself[2] = Vec3f(-radius/2, 0.0, radius/2);

	//  �Œ�J�����̐ݒ�F���݂͎g�p���Ă��Ȃ�
	bEye[0] = 0.0; bEye[1] = 10.0; bEye[2] = 0.0;
	bTarget[0] = 0.0; bTarget[1] = 0.0; bTarget[2] = 0.0;
	bUp[0] = 0.0; bUp[1] = 0.0; bUp[2] = -1.0;

	//	�v���W�F�N�^�ʒu�f�[�^�̃��[�h
	if (LoadSettings()) SetParam();

	/////////////////////////////////////////////////////////////////////////////////////
	//MPI
	int counts[9]={10, 60, 6, 3, 6, 9, 3, 1, 3};
	//�����łQ�ǉ�
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

void TGLView::OnIdle(){
	
	//Sleep(5);
	// �L�[�{�[�h�̏����͂����ōs��
    //::OnKeyboard(press_key, 0, 0);
	int cur_frm =  mydb.vh.Anim.cur_frm;
	switch(press_key){
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

	case 'p':
		spidar_save();
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

void TGLView::shot_control()
{
     
	 BBall.chk_shoot = SHOOTED; 
	 BBall.chk_state = THROWING;
	 bHCollide = false;
	 BBall.shot_pos = BBall.new_pos;
	 t=0.0;
    
}
/////////////////////////////
void TGLView::init_spidar()
{
	
    devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();
	//std::cout << devMan;
   
		//�H�쎺��SPIDAR
	/*Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)},
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};*/

	// 2003.10 NEW SETTING
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
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

//////////////////// SPIDAR ����m�F ////////////////////
void TGLView::spidarCheck(){
		// SPIDAR �̍��W�\��
		for(int j = 0; j < 2; j++){
			printf("%d: (x,y,z) = (%f, %f, %f)\n", j, h_spidar[j].GetPos().X(), h_spidar[j].GetPos().Y(), h_spidar[j].GetPos().Z());
		}
		printf("\n");

		// ���[�^�[�̎��̒����\��
		/*for(int j = 0; j < 2; j++){
			for(int i = 0; i < 4; i++){
				std::cout << i + 4 * j << ": " << h_spidar[j].Motor()[i].Length()  << " ";
			}
			printf("\n");
		}
		printf("\n");*/
}

Vec3f TGLView::convertEuclidToPole(Vec3f euclid){
	Vec3f pole;	// X�F���a�AY�F�ƁAZ�F�� �Ƃ���B
	pole.X() = sqrt(pow(euclid.X(), 2) + pow(euclid.Y(), 2) + pow(euclid.Z(), 2));
	pole.Y() = atan2(euclid.Z(), euclid.X());
	pole.Z() = atan2(euclid.Y(), sqrt(pow(euclid.X(), 2) + pow(euclid.Z(), 2)));
	return pole;
}

Vec3f TGLView::convertPoleToEuclid(Vec3f pole){
	Vec3f euclid;	// X�F���a�AY�F�ƁAZ�F�� �Ƃ���B
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
		if(dis[i] < BBall.radius + sp_info.hand_rad/2){
			t_force[i]     = -K * ((BBall.radius + sp_info.hand_rad/2 - dis[i])) * (sp_info.grp_pos[i] - sp_info.ball_pos).unit();
			t_force[i].Y() = -t_force[i].Y();	// ���W�ϊ�
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
		sp_info.initBallPole.X() = BBall.radius + sp_info.hand_rad/2;
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

	if(dis < BBall.radius + sp_info.hand_rad/2){
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
		t_force[0].Y() = -t_force[0].Y();	// ���W�ϊ�
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
		sp_info.initBallPole.X() = BBall.radius + sp_info.hand_rad/2;
		sp_info.initBallPosition = convertPoleToEuclid(sp_info.initBallPole);
	}
}

/* If BallState is Fixed (Throwing or Falling) */
/*void TGLView::BallIsFixed(){
	Vec3f t_force[2];
	float dis[2];

	for(int i = 0; i < 2; i++){
		dis[i] = (sp_info.grp_pos[i] - sp_info.ball_pos).norm();
		if(dis[i] < BBall.radius + sp_info.hand_rad/2){
			t_force[i]     = -K * ((BBall.radius + sp_info.hand_rad/2 - dis[i])) * (sp_info.grp_pos[i] - sp_info.ball_pos).unit();
			t_force[i].Y() = -t_force[i].Y();	// ���W�ϊ�
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

if (myrank == 0 )
{
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
				sp_info.grp_pos[i].Z() = sp_info.user_hand[i].Z() - t_pos[i].Z(); //�@x,z sign , inverse
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
} //SERVER ONLY 

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
  float distance = JOINT_RAD+BBall.radius*10;
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
		  Play_wave("../vh_data/floor_bounce.wav"); //sound/
		  int off_frm = mydb.vh.Anim.Off_frm;
		  //reflect start point
		  dt = 0.0; //time seeting 10/24
	      off_v= mydb.vh.Ret_Global(off_frm, 100);
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
   //resetting
   m_StartTime = timeGetTime();
   mydb.vh.Anim.init();
   
   mydb.vh.Anim.Key_time = BBall.WhatTime(-1.5, -5.0, 5.0); //distance => frame
   temp_v= BBall.Parabola(BBall.angle, mydb.vh.Anim.Key_time);
   mydb.vh.Anim.Key_frm = mydb.vh.Anim.Key_time/mydb.vh.Anim.unit_Time;
//key_frame ; db����KEY//////////////////////////////////////////////
   mydb.param.pos[0]=temp_v.x;
   mydb.param.pos[1]=temp_v.y;
   mydb.param.pos[2]=temp_v.z;
   mydb.param.key_frm = mydb.vh.Anim.Key_frm;
   //mydb.param.key_type = init_type;
//////////////////////////////////////////////////////////////////////
   //varous pattern
	//strcpy(ssql, mydb.make_sql(1, mydb.vh.Anim.Key_frm,(double*)pos));
	//first search
	strcpy(ssql, mydb.make_sql(1));	
	maxrows =mydb.sqlexec(ssql, 1);
	if (maxrows > 0) mydb.vh.Anim.S_status = CATCHSUCCESS;
	else mydb.vh.Anim.S_status = CATCHFAIL;

	transition_motion(status_type);
	printf("catch type: %d, throw: %d \n", mydb.vh.Anim.catch_type, mydb.vh.Anim.throw_type);

	mydb.vh.setup_endeffector(SKULL, mydb.vh.Anim.Frame_num);
 //ELAPSED TIME   
   m_ElapsedTime = timeGetTime();
//   printf( "\t%u (ms)\n", m_ElapsedTime-m_StartTime );
// RESETTING //////////////////////////////
   BBall.chk_shoot = NOSHOOT;
   mydb.vh.Anim.cur_frm = 0;
/////////////////////////////////////////////

}

void TGLView::transition_motion(int s_type)
{
 char ssql[80];

   
	switch(s_type)
	{
	case READY_PHASE:
		mydb.vh.Anim.Off_frm =1;
		mydb.vh.Anim.throw_type = 0;

		mydb.vh.Anim.catch_type = FREE_MOVEMENT;
		mydb.vh.Anim.S_status = NOEVENT;
		//rotation 
	  	mydb.vh.Anim.Frame_num = mydb.vh.dfmotion.total_frm[0];
		mydb.vh.Anim.Pre_frm = mydb.vh.dfmotion.total_frm[0]; 
		mydb.vh.move_motion(0,mydb.vh.Anim.Frame_num);
		//strcpy(ssql, mydb.make_sql(0));
		//mydb.sqlexec(ssql);
		break;
	case CATCH_PHASE:
		//Data searched	
		  if (mydb.vh.Anim.S_status == CATCHSUCCESS)
		  {
				strcpy(ssql, mydb.make_sql(CATCH_PHASE));
				mydb.sqlexec(ssql);
				strcpy(ssql, mydb.make_sql(THROW_PHASE));
				mydb.sqlexec(ssql);
	
			  // first frame 
			  // mydb.vh.Com_Interp(tt, tt-1);
				mydb.vh.setup_endeffector(LEFT_ARM, mydb.vh.Anim.Frame_num);
				mydb.vh.setup_endeffector(RIGHT_ARM, mydb.vh.Anim.Frame_num);
		   }
		   else  // data no found 
		   {
			   mydb.vh.Anim.Off_frm =1;
			   mydb.vh.Anim.catch_type = FREE_MOVEMENT;
			   mydb.vh.Anim.throw_type = 0;
			   //first frame 
			   //mydb.vh.Com_Interp(0, frame_num);
			   //2004.01.15
			    mydb.vh.Anim.Frame_num = mydb.vh.dfmotion.total_frm[0];
				mydb.vh.Anim.Pre_frm = mydb.vh.dfmotion.total_frm[0]; 
				mydb.vh.move_motion(0, mydb.vh.Anim.Frame_num);
				//strcpy(ssql, mydb.make_sql(2));
				//mydb.sqlexec(ssql);
				//mydb.vh.Com_Interp(0, pre_maxfrm);  
		   }
		break;
	case MISS_PHASE:
		mydb.vh.Anim.Off_frm =1;
		mydb.vh.Anim.throw_type = 0;

		if (mydb.vh.Anim.S_status == CATCHMISSING)
		{
			if (BBall.Fall_pos.X() < -10 ) {
				mydb.vh.Anim.catch_type = CATCHMISS_LEFT; 
				//mydb.vh.MModel = mydb.vh.dfmotion.d_motion[1];
				mydb.vh.Anim.Frame_num = mydb.vh.dfmotion.total_frm[1];
				mydb.vh.Anim.Pre_frm = mydb.vh.dfmotion.total_frm[1]; 
				mydb.vh.move_motion(1, mydb.vh.Anim.Frame_num);
			}
			else 
			{
				mydb.vh.Anim.catch_type = CATCHMISS_RIGHT; 
				//mydb.vh.MModel = mydb.vh.dfmotion.d_motion[2];
				mydb.vh.Anim.Frame_num = mydb.vh.dfmotion.total_frm[2];
				mydb.vh.Anim.Pre_frm = mydb.vh.dfmotion.total_frm[2]; 
				mydb.vh.move_motion(2, mydb.vh.Anim.Frame_num);
			}
		}
		else 
		{
			mydb.vh.Anim.catch_type = FREE_MOVEMENT;
			//mydb.vh.MModel = mydb.vh.dfmotion.d_motion[0];
			mydb.vh.Anim.Frame_num = mydb.vh.dfmotion.total_frm[0];
			mydb.vh.Anim.Pre_frm = mydb.vh.dfmotion.total_frm[0]; 
			mydb.vh.move_motion(0, mydb.vh.Anim.Frame_num );
		}

		mydb.vh.Anim.S_status = NOEVENT;
		//2004.2. single motion
		 //strcpy(ssql, mydb.make_sql(0));
		 //mydb.sqlexec(ssql);
		//2004.4.15
		//mydb.vh.Com_Interp(0, pre_maxfrm);
	break;
	}

	
}

void TGLView::drawjoint(int cur_frm, MyBone sk_bone)
{

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
		 //DrawPlayMode(mydb.vh.Anim.S_status);
			glEnable(GL_LIGHTING);
		    glutSolidSphere(HEAD_RAD, 15, 12); 
			glPushMatrix();
		      mySolidCylinder(1.5, 1.5f, 3.0f, 0.5, 32);
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
				drawQuad(6.0, 1.0);
			   glPopMatrix();
		 }
		 else if (sk_bone.id== LeftWrist || sk_bone.id== RightWrist ) //hand end_site drawing
		 {
			 MyVector m_size(0.75, 0.0, 0.75);
			  glPushMatrix();
				glScalef(1.0, 1.0, 2.0);
				Draw_Ellipse(0, -3.14, 0, m_size);
			  glPopMatrix();
		 }
	  }

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

void TGLView::OnDraw()
{
     //MPI
	// �L�[�{�[�h���͂𑗐M
	MPI_Bcast(&press_key, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//���̃t���[���𑗐M
	if(myrank == 0) apply_send(mydb.vh.Anim.cur_frm);
	MPI_Bcast(&M_DAT, 1, stMove_Data, 0, MPI_COMM_WORLD);
	if(myrank != 0) apply_receive();

	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
	
	//gluLookAt(0.0, 2.0, mydb.vh.Anim.distZ, 0.0, 1.0,0.0, 0.0, 1.0, 0.0);
	//camera setting

	
	//�����炭������MODELVIEW�̐ݒ���s���Ă���B
	//afCam�Ƃ����s���P�ʍs��ɂ�����������΂悢
	//�Ȃ��ABall.h�̒���Affine.h���C���N���[�h���Ă���
	static Affinef R180(Rad(180),'y');
	Affinef afProjector = afBody * R180 * R * R180;
	Affinef afCam = (afEye * afProjector).inv();
	glLoadMatrixf(afCam);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);


	

	////////////////////////////shadow�̏���////////////////////////////////
	
	//��������V�[����`�悷��ׂ̐ݒ���s��
	Shadow->makeSceneWithShadow1();

		//��������V�[����`��
		renderScene();
	
	
	//�`�挋�ʂ��e�N�X�`���ɕۑ����Aviewport�����ɖ߂�
	Shadow->makeSceneWithShadow2();
	
		//�ˉe�s��A���f���r���[�s��Ȃǂ����ɖ߂�
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(eyeView[0], eyeView[1], eyeView[2], eyeView[3]);

	    glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
	
		//static Affinef R180(Rad(180),'y');
		//Affinef afProjector = afBody * R180 * R * R180;
		//Affinef afCam = (afEye * afProjector).inv();
		
		//afCam�͈ȑO��`���Ă���̂ŁALoad����������΂悢
		glLoadMatrixf(afCam);

		//glLightfv(GL_LIGHT0, GL_POSITION, light0pos);

	
	//�e�N�X�`���X�e�[�W���P�ɐݒ肵�A�����̊g�����߂Ȃǂ̐ݒ�����A
	//�e�N�X�`���s��𐶐����Ă���
	Shadow->makeSceneWithShadow3();
		//�ȉ��̂Q�̐������ƁA���炩�Ɍ��ʂ����������Ȃ�
		//glTranslatef(0, -nHeight, mydb.vh.Anim.camera.z);
		//glRotatef(mydb.vh.Anim.camera.y, 0.0f, 1.0f, 0.f);


		//MODULATE�͏�Z�ADECAL�̓e�N�X�`���̐F�����f����Ȃ��AREPLACE�̓e�N�X�`���F�݂̂��o�͂����B���Ǝv�����B 
		//glActiveTextureARB(GL_TEXTURE0_ARB);
		
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		//glMatrixMode(GL_MODELVIEW);
		
		//���̐ݒ�̂܂܁A�V�[����`��
		renderScene();

	//�㏈���B�e�N�X�`���s���Q�����e�N�X�`���̖�����������
	Shadow->makeSceneWithShadow4();
	

	/////////////////////////////////shadow�̏���/////////////////////////////////////
	

//MPI
	MPI_Barrier(MPI_COMM_WORLD);

glutSwapBuffers();		// Flip()���܂�

}

void TGLView::renderScene(void){

	

	//��ÂɋC�Â������A���̃v���O�����ł�y���̏㉺���t���ۂ��Ȃ�B
	//�ہItranslate�̂����̒l�𑝂₷�@=�@���_��y���̒l����f�炷�@���Ɉ�v���Ă���̂ŁB
	
	//glPushMatrix();
	//glTranslatef(0, -nHeight, mydb.vh.Anim.camera.z);
	glTranslatef(0, -nHeight + s_delta, mydb.vh.Anim.camera.z);
	glRotatef(mydb.vh.Anim.camera.y, 0.0f, 1.0f, 0.0f);
	 
	
    glEnable(GL_LIGHTING);

	/*
	//������\���i�ԐF
	glPushMatrix();
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
		glTranslatef(light0pos[0],light0pos[1],light0pos[2]);
		glutSolidSphere(10,10,10);
	glPopMatrix();
	*/
	
	/*
	glPushMatrix();
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
		glTranslatef(light0pos[0]/2.0, light0pos[1]/2.0, light0pos[2]/2.0);
		glutSolidSphere(10,10,10);
		//glutSolidCube(10.0);
	glPopMatrix();
	*/

	//���_��\���i�j
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	//glutSolidSphere(50,10,10);



 
    //hip -5 
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
        drawBody(mydb.vh.Anim.cur_frm);
	glPopMatrix();
	
	
	//���̕`��@by���R����
    glPushMatrix();
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	    //glBindTexture(GL_TEXTURE_2D, tex[0]); //tex[1]
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
	glPopMatrix();
	//ytakeyama end


	int tempflag = 0;
	
	//shadow
	//�u�g�̃e�N�X�`���̐ݒ�����邽�߂ɁA
	//�����Ńe�N�X�`���O��L���ɂ���K�v����
	//���Ȃ݂ɁA�u�����w�肵�Ă��Ȃ����́A�S�ăe�N�X�`���X�e�[�W�O�̐ݒ�Ƃ��Ĉ�����v
	//�݂����ȁA�֗��Ȏd�l�ɂ͂Ȃ��Ă��Ȃ����ۂ��B
	//glActiveTextureARB(GL_TEXTURE0_ARB);

	//�����͂����̐����ő��v���ۂ�
	glActiveTextureARB(0);
	

	//������VRML�̕`�敔�����ۂ�
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
		}//for end
	

	//shadow
	//�u�g�̃e�N�X�`���ݒ肪�I������̂ŁA�e�N�X�`���P�ɐݒ��߂��B
	//glActiveTextureARB(GL_TEXTURE1_ARB);

	Shadow->renderLight();


//glPopMatrix();

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
  //for VRML of Ytakeyama 10000�ȏ�
  //gluPerspective(fov_v, aspect, 1.0, 10000);

  //shadow�@�����䂪float�^�̂S�v�f�z��ŗ~�����̂ŁA�ύX���Ă��܂��Bogawa
  eyeView[0] = 80.0f;
  eyeView[1] = aspect;
  eyeView[2] = 1.0f;
  //eyeView[3] = 10000.0f;
  eyeView[3] = 1500.0f;

  ////////////////////////////////shadow//////////////////////////
  winWidth = w;
  winHeight = h;

  gluPerspective(eyeView[0], eyeView[1], eyeView[2], eyeView[3]);
  Shadow->setWindowSize(&winWidth, &winHeight);

  ///////////////////////////////shadow////////////////////////////////

  
  glMatrixMode(GL_MODELVIEW);

}

void TGLView::OnSpecialKey(int key, int x, int y) 
{
  const int mod = glutGetModifiers();

  if (key == GLUT_KEY_LEFT)    { mydb.vh.Anim.camera.y -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { mydb.vh.Anim.camera.y += deltaRot; }
  if (key == GLUT_KEY_UP)      { 
	  if(mod) s_delta = s_delta - 5.0f;
	  else mydb.vh.Anim.camera.z  -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { 
	  if(mod) s_delta = s_delta + 5.0f;
	  else mydb.vh.Anim.camera.z  += deltaRot; }

  glutPostRedisplay();
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	press_key = key;

	
}

void CALLBACK SpidarUpdateProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD)
{

	((TGLView*)dwUser)->update_spidar();

}

void TGLView::OnOpen(void)
{

  //InitCamera();	
	//	GL�̏�����
 GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
 
 
 glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
 glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
 glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
 glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
 glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


 ////////////////////////////  shadow ///////////////////////////////////////////

 //by ogawa
 //�O���[�o���ϐ��ł�light0pos�Ƃ����S�v�f�z�񂪂Ȃ����錾����Ă��܂����B
 //�ǂ��炩�P�����K�v�Ȃ��̂ŁA�������light0pos�������Ă����܂��B
 
 //GLfloat light0pos[] = {100.0, 1.0, 5.0, 1.0};	//shadow
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHTING);
 glLightfv(GL_LIGHT0, GL_POSITION, light0pos);		//shadow

	
  //glClearColor(0.75, 0.75, 1.0, 0.0); sky color
  glClearColor(0.0, 0.0, 0.0, 0.0); //black
  glEnable(GL_DEPTH_TEST); //hidden surface removed

  ///////////////////////////////���@shadow�֌W�̏������@��/////////////////////////////
  glClearDepth(1.0);
   
  //���OnSize()�ŁAeyeView�̒l�͍X�V�����
  eyeView[0] = 0;
  eyeView[1] = 0;
  eyeView[2] = 0;
  eyeView[3] = 0;
  

  //GL�̊g�����߂��g�����߂ɁA�ȉ����s���K�v����
  SetupGL1_4();
  SetupARBExtensions();
  SetupWGLExtensions();

  //Shadow�N���X�̃I�u�W�F�N�g�𐶐�
  //�����̈ʒu����`����Ă���A�S�v�f�̔z����R�s�[
  Shadow = new CMySoftShadowMap;
  Shadow->copyLightPos(light0pos);
  //Shadow->setWindowSize(&winWidth, &winHeight);
  
  Shadow->copyEyeView(eyeView);

  /////////////////////////////////���@�@�@shadow�@�@�@��///////////////////////////////////////


  glFrontFace(GL_CCW); //no counterclockwise
  glEnable(GL_CULL_FACE); 
  glCullFace(GL_BACK);
 

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
       //VH hand Zaxis position 2003 10 26
	   conn_motion(READY_PHASE);
   }
   
   Load_Texture();
//MPI
   //MPI031021
   //�Ȃ��������Ŏ~�܂��Ă��܂��̂ŁA�Ƃ肠�����A�R�����g�A�E�g
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
    sp_info.hand_rad = 4;
  //timer setting	
	timeBeginPeriod(1);
	SpidarUpdateTimer = timeSetEvent(SpidarUpdateRate, 1, SpidarUpdateProc, (DWORD)this, TIME_PERIODIC);
	SetTimer(33, &TGLView::OnTimer); //Step()���Ăяo������ 33
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
    //time distance �v��
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
				 Play_wave("../vh_data/ball_shoot.wav");//
			 	 
			 }
			 if (BBall.chk_shoot == NOSHOOT && mydb.vh.Anim.cur_frm >= mydb.vh.Anim.Frame_num ) 
			 {
                 //if any event isn't 
				 //init_ball();
				 
                 //End_Interp(cur_frm-1); // 0~ frame_num-1
				 //10/26
				 conn_motion(READY_PHASE); 
			  }
		 //virtual human hand and ball collision
		 if (!bVCollide && BBall.chk_state == THROWING && mydb.vh.Anim.cur_frm >= mydb.vh.Anim.Key_frm-5) 
		 {
			 ballColliding(mydb.vh.Anim.cur_frm);
			 if (mydb.vh.Anim.Off_frm > 1 && mydb.vh.Anim.cur_frm >= mydb.vh.Anim.Key_frm+5 && !bVCollide)
			 {
				 mydb.vh.Anim.S_status = CATCHMISSING;
				 conn_motion(MISS_PHASE); 
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
} //MPI rank 
	if (bDraw) Invalidate();			//	�`��v��
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
//update 2004/04/15
    M_DAT.vcamera[0] = mydb.vh.Anim.camera.x;
	M_DAT.vcamera[1] = mydb.vh.Anim.camera.y;
	M_DAT.vcamera[2] = mydb.vh.Anim.camera.z;
}
void TGLView::apply_receive()
{
   	//MPI031021
	//============ BEGIN ===============

	//����̂ݓK���ȃ��[�V�����f�[�^�Ŕz��𖄂߂Ă���
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

	 mydb.vh.Anim.camera.x = M_DAT.vcamera[0];
	 mydb.vh.Anim.camera.y = M_DAT.vcamera[1];
	 mydb.vh.Anim.camera.z = M_DAT.vcamera[2];

}

void TGLView::DrawShadow()
{
	//�J���[�E�f�v�X�o�b�t�@�}�X�N���Z�b�g����
	//����ňȉ��̓��e�̃s�N�Z���̐F�̒l�́A�������܂�Ȃ��B
	glColorMask(0,0,0,0);								
	glDepthMask(0);										
	////�e���������̈�ɃX�e���V���l�Q��t����
	glEnable(GL_STENCIL_TEST);							
	glStencilFunc(GL_EQUAL, 1, ~0);					
	//�X�e���V���l���P�̃s�N�Z���̃X�e���V���l���C���N�������g
	glStencilOp(GL_KEEP,GL_KEEP ,GL_INCR);				
	glDisable(GL_DEPTH_TEST);							

	//VH�̉e�����ɃX�e���V���l�Q��t����
  	glPushMatrix();										
		glDisable(GL_CULL_FACE);						
			//glMultMatrixf((GLfloat *)ShadowMat);		
			glMultMatrixd(ShadowMat[0]);			
		    drawBody(mydb.vh.Anim.cur_frm);				
		glEnable(GL_CULL_FACE);							
	glPopMatrix();										

	//�{�[���̉e�����ɃX�e���V���l�Q��t����
	glPushMatrix();										
		glDisable(GL_CULL_FACE);						
			glMultMatrixd(ShadowMat[0]);				
	        BBall.Draw();								
		glEnable(GL_CULL_FACE);							
	glPopMatrix();										

	glPushMatrix();										
		glDisable(GL_CULL_FACE);						
			glMultMatrixd(ShadowMat[0]);				
			glTranslatef(sp_info.grp_pos[0].X(),  sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z());
			glScalef(0.3, 1.2, 0.3);
	 		glutSolidSphere(sp_info.hand_rad, 15, 12);
		glEnable(GL_CULL_FACE);							
	glPopMatrix();										

	glPushMatrix();										
		glDisable(GL_CULL_FACE);						
			glMultMatrixd(ShadowMat[0]);				
			glTranslatef(sp_info.grp_pos[1].X(),  sp_info.grp_pos[1].Y(), sp_info.grp_pos[1].Z());
			glScalef(0.3, 1.2, 0.3);
	 		glutSolidSphere(sp_info.hand_rad, 15, 12);
		glEnable(GL_CULL_FACE);							
	glPopMatrix();										

	glEnable(GL_DEPTH_TEST);	
	//�r�b�g�}�X�N������
	glColorMask(1,1,1,1);								
	glDepthMask(1);										

	//�e������B�X�e���V���l���Q�̏��������`�悳���
	glStencilFunc( GL_EQUAL, 2, ~0 );							
	glEnable(GL_BLEND);											
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		
		glDisable(GL_DEPTH_TEST);								
			glColor4f(0.0f, 0.0f, 0.0f, 0.5f);					
			DrawFloor();										
		glEnable(GL_DEPTH_TEST);								
		
	glDisable(GL_BLEND);										
	glDisable(GL_STENCIL_TEST);	

//shadow
}

///////////////	shadow	/////////////////
//���̕`��
void TGLView::DrawFloor(void){								
	glDisable(GL_LIGHTING);									
	glBegin(GL_QUADS);										
		//�ʂȂ̂ŁA������ɒ�`���Ȃ��ƁA�\�Ƃ��Ĉ����Ȃ��B
		glVertex3f( 150, GnHeight+2, 150);					
		glVertex3f( 150, GnHeight+2,-150);					
		glVertex3f(-150, GnHeight+2,-150);					
		glVertex3f(-150, GnHeight+2, 150);					
	glEnd();												
	glEnable(GL_LIGHTING);									
}															
									
