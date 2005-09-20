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
static double ft=0.0;
//////////////////////////////////////////////////////
//SHADOW
static double ShadowMat[4][4];
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

//SLCamera
//////////////////////////////////////////
bool TGLView::OnTimer2(){
	//slcamera.ShowData();
	//JEONG 10/21
	/*std::cout << mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].x << " " << BBall.new_pos.x <<std::endl;
	std::cout << mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].y << " " << BBall.new_pos.y <<std::endl;
	std::cout << mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].z << " " << BBall.new_pos.z <<std::endl;
	std::cout << "BallAngleY=" << BallAngleSide << "��" << std::endl;
	std::cout << "BallAngleX=" << BallAngleVert << "��" << std::endl; */
	
	return true;
}

//10/21
void TGLView::BallAngle()
{
	BallDirection.X() = mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].x - BBall.new_pos.x;

	BallDirection.Y() = mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].y - BBall.new_pos.y;

	BallDirection.Z() = mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].z - BBall.new_pos.z;
//update jeong 12/12 y->z miss

	if(BallDirection.norm() != 0)    // ���K��
			BallDirection = BallDirection / BallDirection.norm();

	BallAngleSide = (int)((atan2(BallDirection.X(),BallDirection.Z())/PI1)*180);
	BallAngleVert = (int)((atan2(BallDirection.Y(),sqrt(BallDirection.X()*BallDirection.X()+
						BallDirection.Z()*BallDirection.Z()))/PI1)*180);

	//10/23
	if( BallAngleSide > 90)
		BallAngleSide = 90;
	if( BallAngleSide < -90)
		BallAngleSide = -90;
	if( BallAngleVert > 90)
		BallAngleVert = 90;
	if( BallAngleVert < -90)
		BallAngleVert = -90;
	//~10/23
	//10/24
     for(int j =0; j<2; j++){
		 for(int i = 1; i < AngleLogMax; i++){
			BallAngleLog[j][i] = BallAngleLog[j][i-1];
		 }
	 }
	 BallAngleLog[0][0] = BallAngleSide;
	 BallAngleLog[1][0] = BallAngleVert;
	 //~10/24
}

void TGLView::CheckEyeAngle(){
	EyeAngleSide = MainAngleSide;
	EyeAngleVert = MainAngleVert;
	if(EyeAngleVert > 0)
		EyeAngleVert = (float)EyeAngleVert-(float)abs(EyeAngleSide)*0.002;
	else
		EyeAngleVert = (float)EyeAngleVert+(float)abs(EyeAngleSide)*0.002;

	if(abs(PreMainAngleSide - MainAngleSide) < 3)
		EyeAngleSide = PreMainAngleSide;

	if(EyeAngleSide > 25)
	  EyeAngleSide = 25;
	if(EyeAngleSide < -25)
	  EyeAngleSide = -25;
	if(EyeAngleVert > 12)
	  //MainAngleVert = 13*abs(MainAngleSide)*0.001;
	  EyeAngleVert = 12;
	if(EyeAngleVert < -12)
	  //MainAngleVert = -13*abs(MainAngleSide)*0.001;
	  EyeAngleVert = -12;

}

void TGLView::SelectAngle(){

/*	for(int i = 1; i < BallLogMax; i++)
		BallPosLog2[i] = BallPosLog2[i-1];

	BallPosLog2[0].X() = BBall.new_pos.x;
	BallPosLog2[0].Y() = BBall.new_pos.y;
	BallPosLog2[0].Z() = BBall.new_pos.z;

	for(int i = 0; i < BallLogMax; i++){
		BallPosChange[i] = BallPosLog2[i] - BallPosLog2[i+1];
		BallPosChangeTotal += BallPosChange[i].norm();
	}
	BallPosChangeTotal = BallPosChangeTotal/BallLogMax;
*/
//	if( BallPosChangeTotal > 0.1){
	if( BBall.chk_state == HOLDING){
		MainAngleVert = BallAngleVert;
		MainAngleSide = BallAngleSide;
		//ConfirmAngle =true;
	}
	else{
		MainAngleVert = UserAngleVert;
		MainAngleSide = UserAngleSide;
		//ConfirmAngle = false;
	}

}

//10/23
void TGLView::ChangeFace(){
	if(FlagFace2 == 0) //�ʏ�
		glBindTexture(GL_TEXTURE_2D, tex[0]);
	else if(FlagFace2 == 1) //�L���b�`����
		glBindTexture(GL_TEXTURE_2D, tex[3]);
	else if(FlagFace2 == 2) //�L���b�`���s
		glBindTexture(GL_TEXTURE_2D, tex[5]);
	else if(FlagFace2 == 3) //������̕��������ē�������
		glBindTexture(GL_TEXTURE_2D, tex[5]);
}

void TGLView::DrawPlayMode(int stat)
{
	switch(stat)
	{
	case NOEVENT:
		if (myrank == 0) SelectAngle();
		FlagFace2 = 0;

		//���̉�]
	   if (abs(MainAngleSide) >= 40 || abs(MainAngleVert) >= 30 || DelayFlag == true)
		{
	//	   if(abs(MainAngleSide - PreMainAngleSide) > 10 || DelayFlag == true){
	//		   RotateEyeHeadDelay();
	//	   }
	//	   else{
			   RotateEyeHead();
	//	   }
		   //RotateHead();
		}
	   else{

		   RotateEye();
	   }
	   EndCatchMiss = false;
	   break;
	  case CATCHSUCCESS:
		 DrawCatchSucc();
		 break;
	  case CATCHFAIL:
	  case CATCHMISSING:
		DrawCatchMiss();
		break;
	}
	//10/23
//	 cout << PreMainAngleSide <<endl;
//	 cout << MainAngleSide <<endl;
	 PreMainAngleSide = MainAngleSide;
	 PreMainAngleVert = MainAngleVert;
//~10/23
}

//VH���{�[�����L���b�`��������ꍇ�̕`��
void TGLView::DrawCatchSucc(){
	if (myrank == 0)
	{
		MainAngleVert = BallAngleVert;
		MainAngleSide = BallAngleSide;
	}
	FlagFace2 = 1;
	RotateHead();
}

//VH���{�[�����L���b�`���s����ꍇ�̕`��
void TGLView::DrawCatchMiss(){
	if (myrank == 0)
	{
		MainAngleVert = BallAngleVert;
		MainAngleSide = BallAngleSide;
	}
	if(EndCatchMiss == true)
		FlagFace2 = 0;
	else
		FlagFace2 = 2;

	RotateHead();
}

void TGLView::RotateEye(){
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		ChangeFace();
		//glBindTexture(GL_TEXTURE_2D, tex[0]);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
			//JEONG
			glRotatef(90, 0.0f, 1.0f, 0.0f);
			glRotatef(85, 1.0f, 0.0f, 0.0f);
			drawSphere(HEAD_RAD);
		 glPopMatrix(); 
		//SLCamera 10/21
		//�ڂ̕`��
		glPushMatrix();
			//�ڂ̉�]:JEONG 10/21 updated
			if( StateCamera == true || StateChange == true ){
				if (myrank == 0) CheckEyeAngle(); //�ڂ̉�]�̐���
				glRotatef(EyeAngleVert, 1.0f, 0.0f, 0.0f);
				glRotatef(EyeAngleSide, 0.0f, 1.0f, 0.0f);
			}

			glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			
			//�E��
			glPushMatrix();
				//glTranslatef(-2.5, -1.8, -0.7);
				glTranslatef(-1.8, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();

			//����
			glPushMatrix();
				//glTranslatef(-2.5, 1.0, -0.7);
				glTranslatef(1.0, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();
				 
		glPopMatrix(); ///�ڂ̕`��I���
	glPopMatrix(); //���̂̕`��I���
}

void TGLView::RotateHead(){

	//���̉�]
	if( StateCamera == true || StateChange == true ){
	  	glRotatef(MainAngleVert, 1.0, 0.0, 0.0);  //x
		glRotatef(MainAngleSide, 0.0, 1.0, 0.0);  //y
	}
	

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		ChangeFace();
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
			glRotatef(90, 0.0f, 1.0f, 0.0f);
			glRotatef(85, 1.0f, 0.0f, 0.0f);
			drawSphere(HEAD_RAD);
		 glPopMatrix(); 
	  	
		//�ڂ̕`��
		glPushMatrix();
		    glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			
			//�E��
			glPushMatrix();
				//glTranslatef(-2.5, -1.8, -0.7);
				glTranslatef(-1.8, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();

			//����
			glPushMatrix();
				//glTranslatef(-2.5, 1.0, -0.7);
				glTranslatef(1.0, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();
			  
		glPopMatrix(); ///�ڂ̕`��I���
	glPopMatrix(); //���̂̕`��I���
}

void TGLView::RotateEyeHeadDelay(){
	
	static int DelayTime = 0;
	if(DelayTime == 0){
		PreAngleSide = MainAngleSide;
		PreAngleVert = MainAngleVert;
	}

	//���̉�]
	if( DelayFlag2 == true && (StateCamera == true || StateChange == true )){
	  	glRotatef(PreAngleVert, 1.0, 0.0, 0.0);  //x
		glRotatef(PreAngleSide, 0.0, 1.0, 0.0);  //y
		DelayFlag = false;
	}

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		ChangeFace();
		//glBindTexture(GL_TEXTURE_2D, tex[4]);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
			//glRotatef(40, 0.0f, 0.0f, 1.0f);
			glRotatef(90, 0.0f, 1.0f, 0.0f);
			glRotatef(85, 1.0f, 0.0f, 0.0f);
			drawSphere(HEAD_RAD);
		 glPopMatrix(); 
	  	
		//�ڂ̕`��
		glPushMatrix();
			//�ڂ̉�]:JEONG 10/21 updated
			if( StateCamera == true || StateChange == true || DelayFlag2 == false){
				if (myrank == 0) CheckEyeAngle(); //�ڂ̉�]�̐���
				glRotatef(EyeAngleVert, 1.0f, 0.0f, 0.0f);
				glRotatef(EyeAngleSide, 0.0f, 1.0f, 0.0f);
				
				if(DelayTime++ > 10){
					DelayFlag2 = true;
					DelayTime = 0;
				}
			}
		    glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			
			//�E��
			glPushMatrix();
				//glTranslatef(-2.5, -1.8, -0.7);
				glTranslatef(-1.8, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();

			//����
			glPushMatrix();
				//glTranslatef(-2.5, 1.0, -0.7);
				glTranslatef(1.0, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();
			  
		glPopMatrix(); ///�ڂ̕`��I���
	glPopMatrix(); //���̂̕`��I���

}

void TGLView::RotateEyeHead(){

	//���̉�]
	if( StateCamera == true || StateChange == true ){
	  	glRotatef((float)MainAngleVert/(float)2, 1.0, 0.0, 0.0);  //x
		glRotatef((float)MainAngleSide/(float)2, 0.0, 1.0, 0.0);  //y
	}

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		ChangeFace();
		glEnable(GL_TEXTURE_2D);

		glPushMatrix();
			//glRotatef(40, 0.0f, 0.0f, 1.0f);
			glRotatef(90, 0.0f, 1.0f, 0.0f);
			glRotatef(85, 1.0f, 0.0f, 0.0f);
			drawSphere(HEAD_RAD);
		glPopMatrix(); 
		  
		//SLCamera 10/21
		//�ڂ̕`��
		glPushMatrix();
			//�ڂ̉�]:JEONG 10/21 updated
			if( StateCamera == true || StateChange == true ){
				if (myrank == 0) CheckEyeAngle(); //�ڂ̉�]�̐���
				//glPushMatrix();
				//glRotatef((float)EyeAngleSide/(float)2, 0.0f, 1.0f, 0.0f);
				glRotatef((float)EyeAngleVert/(float)2, 1.0f, 0.0f, 0.0f);
				glRotatef((float)EyeAngleSide/(float)2, 0.0f, 1.0f, 0.0f);
				//glPopMatrix();
			}

			glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			
			//�E��
			glPushMatrix();
				//glTranslatef(-2.5, -1.8, -0.7);
				glTranslatef(-1.8, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();

			//����
			glPushMatrix();
				//glTranslatef(-2.5, 1.0, -0.7);
				glTranslatef(1.0, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();
				 
		glPopMatrix(); ///�ڂ̕`��I���
	glPopMatrix(); //���̂̕`��I���

}

//~10/23

//////////////////////////////////////////

void TGLView::OnIdle(){
	//Sleep(5);
	// �L�[�{�[�h�̏����͂����ōs��
	//::OnKeyboard(press_key, 0, 0);
	int cur_frm =  mydb.vh.Anim.cur_frm;
	switch(press_key){
	 case 'w': //y
		  sp_info.ball_pos.Y() += delta_Pad;
		  printf( "hand y pos: %f ball y: %f \n", mydb.vh.LA_bone[3].position_global[cur_frm].y, BBall.new_pos.y);
		  break;
	 case 's': 
		  //BBall.new_pos.y -= delta_Pad;
		  sp_info.ball_pos.Y() -= delta_Pad;
		  printf( "hand y pos: %f ball y: %f \n", mydb.vh.LA_bone[3].position_global[cur_frm].y, BBall.new_pos.y);
		  break;
	 case 'a': //x
		  BBall.new_pos.x += delta_Pad;
		  printf( "hand x pos: %f ball x: %f \n", mydb.vh.LA_bone[3].position_global[cur_frm].x, BBall.new_pos.x);
		  break;
	 case 'd': 
		  BBall.new_pos.x -= delta_Pad;
		  printf( "hand x pos: %f ball x: %f \n", mydb.vh.LA_bone[3].position_global[cur_frm].x, BBall.new_pos.x);
		  break;
	 case 'A': //z
		  BBall.new_pos.z += delta_Pad;
		  printf( "hand z pos: %f ball z: %f \n", mydb.vh.LA_bone[3].position_global[cur_frm].z, BBall.new_pos.z);
		  break;
	 case 'D': 
		  BBall.new_pos.z -= delta_Pad;
		  printf( "hand z pos: %f ball z: %f \n", mydb.vh.LA_bone[3].position_global[cur_frm].z, BBall.new_pos.z);
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
	case ' ': 
		animating = !animating; //space
		printf( "cur frame: %d  hand y pos: %f ball x: %f ball y: %f ball z: %f \n", cur_frm, mydb.vh.LA_bone[3].position_global[cur_frm].y, BBall.new_pos.x, BBall.new_pos.y, BBall.new_pos.z);
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
		bMinForce = true;

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

	case 'O': //SLCamera�̃L�����u���[�V����
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
	//std::cout << devMan;
   
		//�H�쎺��SPIDAR
	/*Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)},
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};*/
	
	// VM��Big-SM_PIDAR�p�@���[�^�̎��t�����W�i�^�[���e�[�u���̒��S���珰��110cm�����_�j	
	/*Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-1.75f, 2.00f, 1.12f),	Vec3f( 1.65f, 2.17f,-1.34f), Vec3f(-1.65f,-0.9f,-1.44f), Vec3f( 1.65f,-0.99f, 0.50f)},
		{Vec3f( 1.58f, 2.00f, 1.12f),	Vec3f(-1.65f, 2.18f,-1.34f), Vec3f( 1.65f,-0.98f,-1.44f), Vec3f(-1.65f,-0.98f, 0.50f)}
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
		if(dis[i] < BBall.radius + s_rad){
			t_force[i]     = -K * ((BBall.radius + s_rad - dis[i])) * (sp_info.grp_pos[i] - sp_info.ball_pos).unit();
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
		  dt = 0.0; //time seeting 10/24
	      off_v= mydb.vh.Ret_Global(off_frm, 100);
	  }
  }  

}

bool TGLView::bboxColliding()
{

	double dis=0.0;

	if (BBall.new_pos.x < mydb.vh.A_bone[0].bbox[0])
	    //dis = (BBall.new_pos.x - A_bone[0].center.x)*(BBall.new_pos.x - A_bone[0].center.x);
		dis = (BBall.new_pos.x - mydb.vh.A_bone[0].bbox[0])*(BBall.new_pos.x - mydb.vh.A_bone[0].bbox[0]);
    else if (BBall.new_pos.x > mydb.vh.A_bone[0].bbox[3]) 
		dis = (BBall.new_pos.x - mydb.vh.A_bone[0].bbox[3])*(BBall.new_pos.x - mydb.vh.A_bone[0].bbox[3]);
	
	if (BBall.new_pos.y < mydb.vh.A_bone[0].bbox[1])
		dis += (BBall.new_pos.y - mydb.vh.A_bone[0].bbox[1])*(BBall.new_pos.y - mydb.vh.A_bone[0].bbox[1]);
    else if (BBall.new_pos.y > mydb.vh.A_bone[0].bbox[4]) 
		dis += (BBall.new_pos.y - mydb.vh.A_bone[0].bbox[4])*(BBall.new_pos.y - mydb.vh.A_bone[0].bbox[4]);

	if (BBall.new_pos.z < mydb.vh.A_bone[0].bbox[2])
		dis += (BBall.new_pos.z - mydb.vh.A_bone[0].center.z)*(BBall.new_pos.z - mydb.vh.A_bone[0].center.z);
		//dis += (BBall.new_pos.z - A_bone[0].bbox[2])*(BBall.new_pos.z - A_bone[0].bbox[2]);
    else if (BBall.new_pos.z > mydb.vh.A_bone[0].bbox[5]) 
		//dis += (BBall.new_pos.z - A_bone[0].bbox[5])*(BBall.new_pos.z - A_bone[0].bbox[5]);
		dis += (BBall.new_pos.z - mydb.vh.A_bone[0].center.z)*(BBall.new_pos.z - mydb.vh.A_bone[0].center.z);

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
		strcpy(ssql, mydb.make_sql(0));
		mydb.sqlexec(ssql);
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
			   mydb.vh.Anim.catch_type = FREE_MOVEMENT;//STANCE_POSE; 
			   mydb.vh.Anim.throw_type = 0;
			   //first frame 
			   //mydb.vh.Com_Interp(0, frame_num);
			   //2004.01.15
				strcpy(ssql, mydb.make_sql(2));
				mydb.sqlexec(ssql);
				//mydb.vh.Com_Interp(0, pre_maxfrm);  
		   }
		break;
	case MISS_PHASE:
		mydb.vh.Anim.Off_frm =1;
		mydb.vh.Anim.throw_type = 0;

		if (mydb.vh.Anim.S_status == CATCHMISSING)
		{
			if (BBall.Fall_pos.X() < -10 ) mydb.vh.Anim.catch_type = CATCHMISS_LEFT; 
			else mydb.vh.Anim.catch_type = CATCHMISS_RIGHT; 
		}
		else mydb.vh.Anim.catch_type = FREE_MOVEMENT;

		mydb.vh.Anim.S_status = NOEVENT;
		//2004.2. single motion
		 strcpy(ssql, mydb.make_sql(0));
		 mydb.sqlexec(ssql);
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
	
	 glMaterialfv(GL_FRONT, GL_DIFFUSE, headcolor);//AMBIENT_AND
     //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
     //glMaterialf(GL_FRONT, GL_SHININESS, 10);

	 if(sk_bone.id == Head)
	 {
		 DrawPlayMode(mydb.vh.Anim.S_status);
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
	
     drawjoint(cur_frame, mydb.vh.A_bone[0]); //1
    
		glPushMatrix();
			drawjoint(cur_frame, mydb.vh.LL_bone[0]); //2
			drawjoint(cur_frame, mydb.vh.LL_bone[1]); //3
			drawjoint(cur_frame, mydb.vh.LL_bone[2]); //4
		glPopMatrix();

		glPushMatrix();
			drawjoint(cur_frame, mydb.vh.RL_bone[0]); //5
			drawjoint(cur_frame, mydb.vh.RL_bone[1]); //6
			drawjoint(cur_frame, mydb.vh.RL_bone[2]); //7
		glPopMatrix();

	    glPushMatrix();
			drawjoint(cur_frame, mydb.vh.A_bone[1]); //8
			drawjoint(cur_frame, mydb.vh.A_bone[2]); //9
	
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.LA_bone[0]); //10
				drawjoint(cur_frame, mydb.vh.LA_bone[1]);//11
				drawjoint(cur_frame, mydb.vh.LA_bone[2]);  //12
				drawjoint(cur_frame, mydb.vh.LA_bone[3]);  //13
			glPopMatrix();
		
			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.RA_bone[0]);  //14
				drawjoint(cur_frame, mydb.vh.RA_bone[1]);  //15
				drawjoint(cur_frame, mydb.vh.RA_bone[2]);   //16
				drawjoint(cur_frame, mydb.vh.RA_bone[3]);   //17
		    glPopMatrix();

			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.H_bone[0]);  //18
				drawjoint(cur_frame, mydb.vh.H_bone[1]);  //19
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

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//shadow
	///////////////////////////////////////////////////////////////////////
	//�X�e���V���o�b�t�@���N���A���Ȃ���
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    //gluLookAt(0.0, 2.0, mydb.vh.Anim.distZ, 0.0, 1.0,0.0, 0.0, 1.0, 0.0);

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
	  //bounding box
	 
	 //shadow
	glEnable(GL_STENCIL_TEST);							//shadow
	glStencilFunc( GL_ALWAYS, 1, ~0);					//shadow
	glStencilOp(GL_KEEP,GL_KEEP ,GL_REPLACE);			//shadow

	//���̕`��@by���R����
      glPushMatrix();
	      glBindTexture(GL_TEXTURE_2D, tex[1]);
		  glEnable(GL_TEXTURE_2D);
	      glScalef(100.0, 1.0, 100.0); //�Ȃ���100�{�Ƃ����Ă�́H by ytakeyama
		  glTranslatef(-2, GnHeight, -0.5);
	      //drawQuad(4, 1);//ytakeyama draw gnd
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
		  glDisable(GL_TEXTURE_2D);//ytakeyama end �e���`���邩��	
	  glPopMatrix();

	//BBall.Draw();//ytakeyama comment out
	//ytakeyama �{�[���Ƌ��`���܂�
//SPIDAR-H HAND Position JEONG
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
//JEONG 2004/03	
	
//shadow 
	DrawShadow();
//MPI
	MPI_Barrier(MPI_COMM_WORLD);

glutSwapBuffers();		// Flip()���܂�

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
  gluPerspective(fov_v, aspect, 1.0, 10000);

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

void TGLView::InitCamera()
{
//SLCamera�̏�����
/////////////////////////////////////////
 if (myrank == 0) StateCamera = slcamera.InitSLC();
 FlagFace = false;
 FlagFace2 = 0;
 DelayFlag = false;
 DelayFlag2 = false;
 PreAngleSide = 0;
 PreAngleVert = 0;
 EyeAngleSide = 0;
 EyeAngleVert = 0;
 BallAngleSide = 0;
 BallAngleVert = 0;
 UserAngleSide = 0;
 UserAngleVert = 0;
 MainAngleSide = 0;
 MainAngleVert = 0;
 PreMainAngleSide = 0;
 PreMainAngleVert = 0;
 //10/25
 EndCatchMiss = false;

 for(int j =0; j<2; j++){
	for(int i = 0; i < AngleLogMax; i++){
			 AngleLog[j][i] = 0;
	}
 }
 for(int j =0; j<2; j++){
	for(int i = 0; i < AngleLogMax; i++){
			 UserAngleLog[j][i] = 0;
			 BallAngleLog[j][i] = 0;
	}
 }
 
 for(int i=0; i <= BallLogMax; i++){
	 BallPosChange[BallLogMax] = Vec3f(0.0, 0.0, 0.0);
	 BallPosLog2[BallLogMax] = Vec3f(0.0, 0.0, 0.0);
 }
 BallPosChangeTotal = 0.0;
/////////////////////////////////////////
//~SLCamera

}
void TGLView::OnOpen(void)
{

  //InitCamera();	
	//	GL�̏�����
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
  //shadow�֌W�̏�����
  glDepthFunc(GL_LEQUAL);								//shadow
  //�[�x�o�b�t�@�̃N���A�l�̐ݒ�
  glClearDepth(1.0);								//shadow
  //�X�e���V���o�b�t�@�N���A�l�̐ݒ�
  glClearStencil(0);								//shadow
  ////////////////////////////////////////////////////////////////////////////
 
  glFrontFace(GL_CCW); //no counterclockwise
  glEnable(GL_CULL_FACE); 
  glCullFace(GL_BACK);
 
//generate the shadow matrix
	CalcShadowMat(ShadowMat, nGround1, vLight);

	// MYDB Init connection
   int ret = mydb.sqlconn();
   if (ret == 1)
   {
	   init_paddle();
	   init_hand();
	   init_ball();	
    
	 // compute database search condition
	   mydb.load_model(1);//1
       //VH hand Zaxis position 2003 10 26
	   //mydb.vh.Anim.Throw_num=mydb.random_throw();
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
  //timer setting	
	timeBeginPeriod(1);
	SpidarUpdateTimer = timeSetEvent(SpidarUpdateRate, 1, SpidarUpdateProc, (DWORD)this, TIME_PERIODIC);

	SetTimer(33, &TGLView::OnTimer); //Step()���Ăяo������ 33

	//SLCamera
	//SetTimer(200, &TGLView::OnTimer2);
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
					 EndCatchMiss = true;
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
				 if (BBall.minZ < -300) //BBall.init_pos.z
					 init_ball();
				 dt +=(mydb.vh.Anim.unit_Time*2);
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
				 //10/26
				 //mydb.vh.Anim.Key_time = BBall.WhatTime(-1.5); //distance => frame
				 conn_motion(CATCH_PHASE); 
				 // new
				 BBall.chk_shoot = NOSHOOT;
				 bHCollide = false;
				 //bVCollide = false;
				 t=0.0; //resetting time
				 //Play_wave("sound/ball_shoot.wav");
			 	 FlagFace = true;
				 //~SLCamera
			 }
			 if (BBall.chk_shoot == NOSHOOT && mydb.vh.Anim.cur_frm >= mydb.vh.Anim.Frame_num ) 
			 {
                 //if any event isn't 
				 //init_ball();
				 //~SLCamera
				 FlagFace = false;
				 //MPI communication 
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
/*
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
		 }*/
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
			 //new10/24
			 for(int j =0; j<2; j++){
				 for(int i = 1; i < AngleLogMax; i++){
					 UserAngleLog[j][i] = UserAngleLog[j][i-1];
				 }
			 }
			 AngleLog[0][0] = MainAngleSide;
			 AngleLog[1][0] = MainAngleVert;
		 }
		 //~new10/24
		//~SLCamera
		
	 } //animating
	}  //server only ���s
 
	if (bDraw) Invalidate();			//	�`��v��
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
	
	//SPIDAR-H JEONG 10/23
	
    M_DAT.Hand_Pos[0] = sp_info.grp_pos[0].X();
	M_DAT.Hand_Pos[1] = sp_info.grp_pos[0].Y();
	M_DAT.Hand_Pos[2] = sp_info.grp_pos[0].Z();

	M_DAT.Hand_Pos[3] = sp_info.grp_pos[1].X();
	M_DAT.Hand_Pos[4] = sp_info.grp_pos[1].Y();
	M_DAT.Hand_Pos[5] = sp_info.grp_pos[1].Z();

	//SLCamera
	M_DAT.Temp_Rot[0] = MainAngleSide;
	M_DAT.Temp_Rot[1] = MainAngleVert;
	M_DAT.Temp_Rot[2] = EyeAngleSide;
	M_DAT.Temp_Rot[3] = EyeAngleVert;
	M_DAT.Temp_Rot[4] = BallAngleSide;
	M_DAT.Temp_Rot[5] = BallAngleVert;
	M_DAT.Temp_Rot[6] = UserAngleSide;
	M_DAT.Temp_Rot[7] = UserAngleVert;
	if (StateCamera)
		M_DAT.Temp_Rot[8] = 1;
	else
		M_DAT.Temp_Rot[8] = 0;

	M_DAT.RotBall[0] = nowBallPole.X();
	M_DAT.RotBall[1] = nowBallPole.Y();
	M_DAT.RotBall[2] = nowBallPole.Z();

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
	
	//SPIDAR-H JEONG 10/23
	sp_info.grp_pos[0].X() = M_DAT.Hand_Pos[0];
	sp_info.grp_pos[0].Y() = M_DAT.Hand_Pos[1];
	sp_info.grp_pos[0].Z() = M_DAT.Hand_Pos[2];

	sp_info.grp_pos[1].X() = M_DAT.Hand_Pos[3];
	sp_info.grp_pos[1].Y() = M_DAT.Hand_Pos[4];
	sp_info.grp_pos[1].Z() = M_DAT.Hand_Pos[5];

	//fprintf(stderr, "spidar pos:%f, %f, %f \n",sp_info.grp_pos[0].X(), sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z()); 
		//SLCamera
	MainAngleSide = M_DAT.Temp_Rot[0];
	MainAngleVert = M_DAT.Temp_Rot[1];
	EyeAngleSide = M_DAT.Temp_Rot[2];
	EyeAngleVert = M_DAT.Temp_Rot[3];
	BallAngleSide =M_DAT.Temp_Rot[4];
	BallAngleVert = M_DAT.Temp_Rot[5];
	UserAngleSide = M_DAT.Temp_Rot[6];
	UserAngleVert =M_DAT.Temp_Rot[7];
	if (M_DAT.Temp_Rot[8] == 1)
		StateCamera = true;
	else
		StateCamera = false;
    //fprintf(stderr, "eye pos:%d, %d \n",MainAngleSide, MainAngleVert); 
	 nowBallPole.X() = M_DAT.RotBall[0];
     nowBallPole.Y() = M_DAT.RotBall[1];
	 nowBallPole.Z() = M_DAT.RotBall[2];
}

void TGLView::DrawShadow()
{
	//�J���[�E�f�v�X�o�b�t�@�}�X�N���Z�b�g����
	//����ňȉ��̓��e�̃s�N�Z���̐F�̒l�́A�������܂�Ȃ��B
	glColorMask(0,0,0,0);								//shadow
	glDepthMask(0);										//shadow
	////�e���������̈�ɃX�e���V���l�Q��t����
	glEnable(GL_STENCIL_TEST);							//shadow
	glStencilFunc(GL_EQUAL, 1, ~0);					//shadow
	//�X�e���V���l���P�̃s�N�Z���̃X�e���V���l���C���N�������g
	glStencilOp(GL_KEEP,GL_KEEP ,GL_INCR);				//shadow
	glDisable(GL_DEPTH_TEST);							//shadow

	//VH�̉e�����ɃX�e���V���l�Q��t����
  	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
			//glMultMatrixf((GLfloat *)ShadowMat);		//shadow
			glMultMatrixd(ShadowMat[0]);			//shadow
		    drawBody(mydb.vh.Anim.cur_frm);				//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow

	//�{�[���̉e�����ɃX�e���V���l�Q��t����
	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
			glMultMatrixd(ShadowMat[0]);				//shadow
	        BBall.Draw();								//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow

	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
			glMultMatrixd(ShadowMat[0]);				//shadow
	        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(sp_info.grp_pos[0].X(),  sp_info.grp_pos[0].Y(), sp_info.grp_pos[0].Z());
	 		glutSolidSphere(s_rad, 15, 12);
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow

	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
			glMultMatrixd(ShadowMat[0]);				//shadow
	        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(sp_info.grp_pos[1].X(),  sp_info.grp_pos[1].Y(), sp_info.grp_pos[1].Z());
	 		glutSolidSphere(s_rad, 15, 12);
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow

	glEnable(GL_DEPTH_TEST);	
	//�r�b�g�}�X�N������
	glColorMask(1,1,1,1);								//shadow
	glDepthMask(1);										//shadow

	//�e������B�X�e���V���l���Q�̏��������`�悳���
	glStencilFunc( GL_EQUAL, 2, ~0 );							//shadow
	glEnable(GL_BLEND);											//shadow
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		//shadow	
		glDisable(GL_DEPTH_TEST);								//shadow
			glColor4f(0.0f, 0.0f, 0.0f, 0.5f);					//shadow
			DrawFloor();										//shadow
		glEnable(GL_DEPTH_TEST);								//shadow
		
	glDisable(GL_BLEND);										//shadow
	glDisable(GL_STENCIL_TEST);	
	
/*
	  //���̗̈�ɃX�e���V���l�R��t����
	glStencilFunc( GL_ALWAYS, 3, ~0);					//shadow
	glStencilOp(GL_KEEP,GL_KEEP ,GL_REPLACE);			//shadow
	//���̕`��
	glPushMatrix();										//shadow
	      glBindTexture(GL_TEXTURE_2D, tex[6]);			//shadow
		  glEnable(GL_TEXTURE_2D);						//shadow
		  glTranslatef(paddle_v.x, paddle_v.y/2, paddle_v.z);//shadow
		  glRotatef(90, 1.0, 0.0, 0.0);							//shadow
	      drawQuad(20, 7);								//shadow
		  glDisable(GL_TEXTURE_2D);						//shadow
	glPopMatrix();										//shadow

	glDisable(GL_STENCIL_TEST);							//shadow

  //�������A�e������̈�ɂQ������
	glStencilFunc( GL_EQUAL, 3, ~0);					//shadow
	//�X�e���V���l���R�̃s�N�Z���̃X�e���V���l���f�N�������g
	glStencilOp(GL_KEEP,GL_KEEP ,GL_DECR);				//shadow

	//�΃{�b�N�X�̉E�����ʗp
	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
	        glMultMatrixd(ShadowMat3[0]);				//shadow
			BBall.Draw();								//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow
	
	//�{�[���̃X�e���V���l�Q�t��(�΃{�b�N�X��top�ʗp)
	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
	        glMultMatrixd(ShadowMat2[0]);				//shadow
			BBall.Draw();								//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow
*/
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
//////////	shadow	///////////���ɉB���e�H
void TGLView::DrawFloor2(void){								
	//���͔���top�ʂ̍���
	float y=0;												
	y = 16.5 + paddle_v.y/2;								
	glDisable(GL_LIGHTING);									
	glBegin(GL_QUADS);										
		
		glVertex3f( 100, y, 100);							
		glVertex3f( 100, y,-100);							
		glVertex3f(-100, y,-100);							
		glVertex3f(-100, y, 100);							
	glEnd();												
	glEnable(GL_LIGHTING);									
}															
