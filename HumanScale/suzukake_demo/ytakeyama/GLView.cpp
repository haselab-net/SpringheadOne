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
	// VPC00〜VPC03はフレーム遅れなし
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
	GetPrivateProfileString("AVATOR","eyeheight","0",temp_str,100,ini_file); //アバタ−の中心から目までの高さ
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
	fov_v = 30.0f; //とりあえず視野角を初期化
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
	std::cout << "BallAngleY=" << BallAngleSide << "°" << std::endl;
	std::cout << "BallAngleX=" << BallAngleVert << "°" << std::endl; */
	
	return true;
}

//10/21
void TGLView::BallAngle()
{
	BallDirection.X() = mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].x - BBall.new_pos.x;

	BallDirection.Y() = mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].y - BBall.new_pos.y;

	BallDirection.Z() = mydb.vh.H_bone[1].position_global[mydb.vh.Anim.cur_frm].z - BBall.new_pos.z;
//update jeong 12/12 y->z miss

	if(BallDirection.norm() != 0)    // 正規化
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
	if(FlagFace2 == 0) //通常
		glBindTexture(GL_TEXTURE_2D, tex[0]);
	else if(FlagFace2 == 1) //キャッチ成功
		glBindTexture(GL_TEXTURE_2D, tex[3]);
	else if(FlagFace2 == 2) //キャッチ失敗
		glBindTexture(GL_TEXTURE_2D, tex[5]);
	else if(FlagFace2 == 3) //明後日の方向を見て投げられる
		glBindTexture(GL_TEXTURE_2D, tex[5]);
}

void TGLView::DrawPlayMode(int stat)
{
	switch(stat)
	{
	case NOEVENT:
		if (myrank == 0) SelectAngle();
		FlagFace2 = 0;

		//頭の回転
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

//VHがボールをキャッチ成功する場合の描画
void TGLView::DrawCatchSucc(){
	if (myrank == 0)
	{
		MainAngleVert = BallAngleVert;
		MainAngleSide = BallAngleSide;
	}
	FlagFace2 = 1;
	RotateHead();
}

//VHがボールをキャッチ失敗する場合の描画
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
		//目の描画
		glPushMatrix();
			//目の回転:JEONG 10/21 updated
			if( StateCamera == true || StateChange == true ){
				if (myrank == 0) CheckEyeAngle(); //目の回転の制限
				glRotatef(EyeAngleVert, 1.0f, 0.0f, 0.0f);
				glRotatef(EyeAngleSide, 0.0f, 1.0f, 0.0f);
			}

			glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			
			//右目
			glPushMatrix();
				//glTranslatef(-2.5, -1.8, -0.7);
				glTranslatef(-1.8, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();

			//左目
			glPushMatrix();
				//glTranslatef(-2.5, 1.0, -0.7);
				glTranslatef(1.0, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();
				 
		glPopMatrix(); ///目の描画終わり
	glPopMatrix(); //頭のの描画終わり
}

void TGLView::RotateHead(){

	//頭の回転
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
	  	
		//目の描画
		glPushMatrix();
		    glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			
			//右目
			glPushMatrix();
				//glTranslatef(-2.5, -1.8, -0.7);
				glTranslatef(-1.8, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();

			//左目
			glPushMatrix();
				//glTranslatef(-2.5, 1.0, -0.7);
				glTranslatef(1.0, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();
			  
		glPopMatrix(); ///目の描画終わり
	glPopMatrix(); //頭のの描画終わり
}

void TGLView::RotateEyeHeadDelay(){
	
	static int DelayTime = 0;
	if(DelayTime == 0){
		PreAngleSide = MainAngleSide;
		PreAngleVert = MainAngleVert;
	}

	//頭の回転
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
	  	
		//目の描画
		glPushMatrix();
			//目の回転:JEONG 10/21 updated
			if( StateCamera == true || StateChange == true || DelayFlag2 == false){
				if (myrank == 0) CheckEyeAngle(); //目の回転の制限
				glRotatef(EyeAngleVert, 1.0f, 0.0f, 0.0f);
				glRotatef(EyeAngleSide, 0.0f, 1.0f, 0.0f);
				
				if(DelayTime++ > 10){
					DelayFlag2 = true;
					DelayTime = 0;
				}
			}
		    glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			
			//右目
			glPushMatrix();
				//glTranslatef(-2.5, -1.8, -0.7);
				glTranslatef(-1.8, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();

			//左目
			glPushMatrix();
				//glTranslatef(-2.5, 1.0, -0.7);
				glTranslatef(1.0, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();
			  
		glPopMatrix(); ///目の描画終わり
	glPopMatrix(); //頭のの描画終わり

}

void TGLView::RotateEyeHead(){

	//頭の回転
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
		//目の描画
		glPushMatrix();
			//目の回転:JEONG 10/21 updated
			if( StateCamera == true || StateChange == true ){
				if (myrank == 0) CheckEyeAngle(); //目の回転の制限
				//glPushMatrix();
				//glRotatef((float)EyeAngleSide/(float)2, 0.0f, 1.0f, 0.0f);
				glRotatef((float)EyeAngleVert/(float)2, 1.0f, 0.0f, 0.0f);
				glRotatef((float)EyeAngleSide/(float)2, 0.0f, 1.0f, 0.0f);
				//glPopMatrix();
			}

			glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			
			//右目
			glPushMatrix();
				//glTranslatef(-2.5, -1.8, -0.7);
				glTranslatef(-1.8, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();

			//左目
			glPushMatrix();
				//glTranslatef(-2.5, 1.0, -0.7);
				glTranslatef(1.0, 0.7, 2.5);
				drawSphere(EYE_RAD);	
			glPopMatrix();
				 
		glPopMatrix(); ///目の描画終わり
	glPopMatrix(); //頭のの描画終わり

}

//~10/23

//////////////////////////////////////////

void TGLView::OnIdle(){
	//Sleep(5);
	// キーボードの処理はここで行う
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
	  //mydb.vh.Anim.distZ -= 5;
	  //cout << "Anim.distZ = " << mydb.vh.Anim.distZ << endl;
		afBody.Pos().Z() += 0.5;
	  break;
    case 'Z':
	  //mydb.vh.Anim.distZ += 5;
	  //cout << "Anim.distZ = " << mydb.vh.Anim.distZ << endl;
		afBody.Pos().Z() -= 0.5;
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
	//std::cout << devMan;
   
		//工作室のSPIDAR
	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)},
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};*/
	
	// VM室Big-SM_PIDAR用　モータの取り付け座標（ターンテーブルの中心から床上110cmが原点）	
	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.75f, 2.00f, 1.12f),	Vec3f( 1.65f, 2.17f,-1.34f), Vec3f(-1.65f,-0.9f,-1.44f), Vec3f( 1.65f,-0.99f, 0.50f)},
		{Vec3f( 1.58f, 2.00f, 1.12f),	Vec3f(-1.65f, 2.18f,-1.34f), Vec3f( 1.65f,-0.98f,-1.44f), Vec3f(-1.65f,-0.98f, 0.50f)}
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
//key_frame ; db内でKEY//////////////////////////////////////////////
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
      //Draw_Ellipse(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z, sk_bone.scale);
        if(bSeeTest){
			for(TGeoNodes::iterator it = allNodes[bSeeTest].begin(); it != allNodes[bSeeTest].end(); ++it){
				(*it)->Draw();
			}
		}
	  glTranslatef(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z);
    }

	glRotatef(sk_bone.rot[cur_frm].z, 0.0, 0.0, 1.0);  //z
	glRotatef(sk_bone.rot[cur_frm].x, 1.0, 0.0, 0.0);  //x
	glRotatef(sk_bone.rot[cur_frm].y, 0.0, 1.0, 0.0);  //y
	/*if (sk_bone.id == Hips){
		glRotatef(90.0, 0.0, 1.0, 0.0);  //y
	}*/
	
	 //glMaterialfv(GL_FRONT, GL_DIFFUSE, headcolor);//AMBIENT_AND
     //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
     //glMaterialf(GL_FRONT, GL_SHININESS, 10);

	 if(sk_bone.id == Hips){
		if(bSeeTest){
			for(TGeoNodes::iterator it = allNodes[10].begin(); it != allNodes[10].end(); ++it){
				(*it)->Draw();
			}
		}
	 }

	 if(sk_bone.id == Head)
	 {
		 //DrawPlayMode(mydb.vh.Anim.S_status);
		 if(bSeeTest){
			for(TGeoNodes::iterator it = allNodes[4].begin(); it != allNodes[4].end(); ++it){
				  (*it)->Draw();				
			}
		 }
	 }
	 else 
	 {
	     //glDisable(GL_TEXTURE_2D);
		 //glutSolidSphere(JOINT_RAD, 15, 12); // joint ball drawing
		
		if (sk_bone.id == RightAnkle || sk_bone.id == LeftAnkle ) // foot box drawing
		{
			   /*glPushMatrix();
				glTranslatef(0, 0, -1.5);
				glRotatef(-90, 0 ,1,0); 
				drawQuad(6.0, 1.0);
			   glPopMatrix();*/
				for(TGeoNodes::iterator it = allNodes[bSeeTest+1].begin(); it != allNodes[bSeeTest+1].end(); ++it){
					(*it)->Draw();
				}
		 }
		 else if (sk_bone.id== LeftWrist || sk_bone.id== RightWrist ) //hand end_site drawing
		 {
			 //MyVector m_size(0.75, 0.0, 0.75);
			  //glPushMatrix();
			  //	glScalef(1.0, 1.0, 2.0);
		      //	Draw_Ellipse(0, -3.14, 0, m_size);
			  // glPopMatrix();
			 	for(TGeoNodes::iterator it = allNodes[bSeeTest+1].begin(); it != allNodes[bSeeTest+1].end(); ++it){
					(*it)->Draw();
				}
		 }
	  }

}

void TGLView::Tshirts(){

	/*glColor3d(1.0, 0.0, 0.0);//以下 確認用に線で出力
	glBegin(GL_LINE_STRIP);
	for(int iii = 0; iii<8; iii++){
		glVertex3d(A0[iii][0], A0[iii][1], A0[iii][2]);
	}
	glColor3d(1.0, 0.0, 1.0);
	for(int iii = 0; iii<8; iii++){
		glVertex3d(A1[iii][0], A1[iii][1], A1[iii][2]);
	}
	glColor3d(0.0, 1.0, 0.0);
	for(int iii = 0; iii<8; iii++){
		glVertex3d(A2[iii][0], A2[iii][1], A2[iii][2]);
	}
	glColor3d(0.0, 0.0, 1.0);
	for(int iii = 0; iii<8; iii++){
		glVertex3d(A3[iii][0], A3[iii][1], A3[iii][2]);
	}
	//glColor3d(0.0, 1.0, 1.0);
	glVertex3d(A3[8][0], A3[8][1], A3[8][2]);
	glEnd();*/

	glDisable(GL_CULL_FACE);//以下 Tshirtsの描画
	glColor3d(0.0, 0.0, 0.2);
	glBegin(GL_TRIANGLE_STRIP);
		for(int iii = 0; iii<8; iii++){
			glVertex3d(A1[iii][0], A1[iii][1], A1[iii][2]);
			glVertex3d(A0[iii][0], A0[iii][1], A0[iii][2]);
		}
		glVertex3d(A1[0][0], A1[0][1], A1[0][2]);
		glVertex3d(A0[0][0], A0[0][1], A0[0][2]);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
		for(int iii = 0; iii<8; iii++){
			glVertex3d(A2[iii][0], A2[iii][1], A2[iii][2]);
			glVertex3d(A1[iii][0], A1[iii][1], A1[iii][2]);
		}
		glVertex3d(A2[0][0], A2[0][1], A2[0][2]);
		glVertex3d(A1[0][0], A1[0][1], A1[0][2]);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
		glVertex3d(A3[4][0], A3[4][1], A3[4][2]);
		glVertex3d(A2[0][0], A2[0][1], A2[0][2]);
		glVertex3d(A3[5][0], A3[5][1], A3[5][2]);
		glVertex3d(A2[1][0], A2[1][1], A2[1][2]);
		glVertex3d(A3[6][0], A3[6][1], A3[6][2]);
		glVertex3d(A2[2][0], A2[2][1], A2[2][2]);
	glEnd();

	glBegin(GL_TRIANGLES);
		glVertex3d(A3[6][0], A3[6][1], A3[6][2]);
		glVertex3d(A2[2][0], A2[2][1], A2[2][2]);
		glVertex3d(A3[7][0], A3[7][1], A3[7][2]);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
		glVertex3d(A3[7][0], A3[7][1], A3[7][2]);
		glVertex3d(A2[2][0], A2[2][1], A2[2][2]);
		glVertex3d(A3[8][0], A3[8][1], A3[8][2]);
		glVertex3d(A2[3][0], A2[3][1], A2[3][2]);
		glVertex3d(A3[0][0], A3[0][1], A3[0][2]);
		glVertex3d(A2[4][0], A2[4][1], A2[4][2]);
	glEnd();

	
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3d(A3[0][0], A3[0][1], A3[0][2]);
		glVertex3d(A2[4][0], A2[4][1], A2[4][2]);
		glVertex3d(A3[1][0], A3[1][1], A3[1][2]);
		glVertex3d(A2[5][0], A2[5][1], A2[5][2]);
		glVertex3d(A3[2][0], A3[2][1], A3[2][2]);
		glVertex3d(A2[6][0], A2[6][1], A2[6][2]);
		glVertex3d(A3[3][0], A3[3][1], A3[3][2]);
		glVertex3d(A2[7][0], A2[7][1], A2[7][2]);
		glVertex3d(A3[4][0], A3[4][1], A3[4][2]);
		glVertex3d(A2[0][0], A2[0][1], A2[0][2]);
	glEnd();
	
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3d(A2[4][0], A2[4][1], A2[4][2]);
		glVertex3d(A3[0][0], A3[0][1], A3[0][2]);
		glVertex3d(A2[5][0], A2[5][1], A2[5][2]);
		glVertex3d(A3[1][0], A3[1][1], A3[1][2]);
		glVertex3d(A2[6][0], A2[6][1], A2[6][2]);
		glVertex3d(A3[2][0], A3[2][1], A3[2][2]);
		glVertex3d(A2[7][0], A2[7][1], A2[7][2]);
		glVertex3d(A3[3][0], A3[3][1], A3[3][2]);
		glVertex3d(A2[0][0], A2[0][1], A2[0][2]);
		glVertex3d(A3[4][0], A3[4][1], A3[4][2]);
	glEnd();
	

	glBegin(GL_TRIANGLE_STRIP);
		glVertex3d(A3[3][0], A3[3][1], A3[3][2]);
		glVertex3d(A3[4][0], A3[4][1], A3[4][2]);
		glVertex3d(A3[2][0], A3[2][1], A3[2][2]);
		glVertex3d(A3[5][0], A3[5][1], A3[5][2]);
		glVertex3d(A3[6][0], A3[6][1], A3[6][2]);
	glEnd();

	glBegin(GL_TRIANGLES);
		glVertex3d(A3[2][0], A3[2][1], A3[2][2]);
		glVertex3d(A3[6][0], A3[6][1], A3[6][2]);
		glVertex3d(A3[7][0], A3[7][1], A3[7][2]);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
		glVertex3d(A3[2][0], A3[2][1], A3[2][2]);
		glVertex3d(A3[7][0], A3[7][1], A3[7][2]);
		glVertex3d(A3[1][0], A3[1][1], A3[1][2]);
		glVertex3d(A3[8][0], A3[8][1], A3[8][2]);
		glVertex3d(A3[0][0], A3[0][1], A3[0][2]);
	glEnd();

	glEnable(GL_CULL_FACE);
	

	glColor3d(1.0, 1.0, 1.0);

}

void TGLView::TshirtsVertexInit(){
	//頂点座標の初期化　
	//ポリゴン設計図がないと確実に意味不明
	//いろいろ変えたのでytakeyamaの乱雑なメモにしか設計図がない
	float adj,adjZ;
	adj = 5.0;
	adjZ = 0.7;
	AA0[0] = Vec3f(-1.0, 0.2, 0.0)*adj;
	AA0[1] = Vec3f(-0.7071, 0.2, 0.7071*adjZ)*adj;
	AA0[2] = Vec3f(0.0, 0.2, 1.0*adjZ)*adj;
	AA0[3] = Vec3f(0.7071, 0.2, 0.7071*adjZ)*adj;
	AA0[4] = Vec3f(1.0, 0.2, 0.0)*adj;
	AA0[5] = Vec3f(0.7071, 0.2, -0.7071*adjZ + 0.2)*adj;
	AA0[6] = Vec3f(0.0, 0.2, -1.0*adjZ + 0.2)*adj;
	AA0[7] = Vec3f(-0.7071, 0.2, -0.7071*adjZ + 0.2)*adj;

	adj = 5.5;
	AA1[0] = Vec3f(-1.0, 0.0, 0.0)*adj;
	AA1[1] = Vec3f(-0.7071, 0.0, 0.7071*adjZ)*adj;
	AA1[2] = Vec3f(0.0, 0.0, 1.0*adjZ)*adj;
	AA1[3] = Vec3f(0.7071, 0.0, 0.7071*adjZ)*adj;
	AA1[4] = Vec3f(1.0, 0.0, 0.0)*adj;
	AA1[5] = Vec3f(0.7071, 0.0, -0.7071*adjZ + 0.1)*adj;
	AA1[6] = Vec3f(0.0, 0.0, -1.0*adjZ + 0.1)*adj;
	AA1[7] = Vec3f(-0.7071, 0.0, -0.7071*adjZ + 0.1)*adj;

	adj = 7.0;
	AA2[0] = Vec3f(-1.0 + 0.1, 0.0, 0.0)*adj;
	AA2[1] = Vec3f(-0.7071 + 0.1, 0.0, 0.7071*adjZ + 0.2)*adj;
	AA2[2] = Vec3f(0.0, 0.0, 1.0*adjZ + 0.2)*adj;
	AA2[3] = Vec3f(0.7071 - 0.1, 0.0, 0.7071*adjZ + 0.2)*adj;
	AA2[4] = Vec3f(1.0 - 0.1, 0.0, 0.0)*adj;
	AA2[5] = Vec3f(0.7071, 0.0, -0.7071*adjZ + 0.2)*adj;
	AA2[6] = Vec3f(0.0, 0.0, -1.0*adjZ + 0.2)*adj;
	AA2[7] = Vec3f(-0.7071, 0.0, -0.7071*adjZ + 0.2)*adj;
	
	adj = 5.0;
	AA3[4] = Vec3f(-1.0, 0.0, 0.0)*adj;
	AA3[5] = Vec3f(-0.7071, 0.0, 0.7071*adjZ)*adj;
	AA3[6] = Vec3f(-0.1, 0.0, 0.9*adjZ - 0.1)*adj;
	AA3[7] = Vec3f(0.1, 0.0, 0.9*adjZ - 0.1)*adj;
	AA3[8] = Vec3f(0.7071, 0.0, 0.7071*adjZ)*adj;
	AA3[0] = Vec3f(1.0, 0.0, 0.0)*adj;
	AA3[1] = Vec3f( 0.7071 - 0.3, 0.0, -0.7071*adjZ - 0.4)*adj;
	AA3[2] = Vec3f(0.0, 0.0, -1.0*adjZ)*adj;//これは後でA3[1]とA3[3]の中点をとる
	AA3[3] = Vec3f(-0.7071 + 0.3, 0.0, -0.7071*adjZ - 0.4)*adj;

	/*for(int iii = 0; iii<9; iii++) AA3[iii] = Vec3f(0,0,0);
	AA3[4] = Vec3f(-10.0, 0.0, 0.0);
	AA3[5] = Vec3f(0.0, 10.0, 0.0);
	AA3[6] = Vec3f(0.0, 0.0, 10.0);
	AA3[7] = Vec3f(0.0, 0.0, 10.0);
	AA3[8] = Vec3f(0.0, 10.0, 0.0);
	AA3[0] = Vec3f(10.0, 0.0, 0.0);*/
	
	// VH 観賞用変数
	YTX = 0;
	YTY = 0;
}

static Affinef glRot2Af(double x, double y, double z, 
						Vec3f *AxisX, Vec3f *AxisY, Vec3f *AxisZ){
	//glRotateの角度を回転行列に変換する
	//glRotateでZXYの順に回すものとする
	//回っちゃった後の軸も引き継いでいく
	Vec3f AX, AY, AZ;
	Affinef AfTemp[3], result;

	AX = *AxisX;
	AY = *AxisY;
	AZ = *AxisZ;

	AfTemp[0] = Affinef(Rad(z), AZ);
	AX = AfTemp[0] * AX;
	AY = AfTemp[0] * AY;

	AfTemp[1] = Affinef(Rad(x), AX);
	AZ = AfTemp[1] * AZ;
	AY = AfTemp[1] * AY;

	AfTemp[2] = Affinef(Rad(y), AY);
	AZ = AfTemp[2] * AZ;
	AX = AfTemp[2] * AX;

	*AxisX = AX;
	*AxisY = AY;
	*AxisZ = AZ;

	result =  AfTemp[2] * AfTemp[1] * AfTemp[0];

	return result;
}

static Affinef AfRotBone(double x, double y, double z){
	//一つ先の骨がどの方向に向いているかを調べて、回転行列を作る
	Affinef result;
	double h = sqrt(x*x + z*z);
	
	if (h > 0){
		float phi, theta;
		phi = acos(x/h)*180.0/M_PI;
		theta = atan(h/y)*180.0/M_PI;
		if(z<0) phi = -1 * phi;
		result = Affinef(-1*theta, 'z') * Affinef(-1*phi, 'y');
	}else{
		if (y < 0){
			result = Affinef(Rad((double)180), 'z');
		}
	}

	return result;
}

void TGLView::TshirtsVertexSolver(int cur_frame){
	//ソルバとは名ばかりのゴリ押し手作業
	//この際、動けばそれでいい
	Vec3f AxisX, AxisY, AxisZ;//glRotateからAffinefへの変換で使う。回転軸。
	Vec3f hAxisX, hAxisY, hAxisZ;//PushMatrix用
	Vec3f tempV;//計算でちょっと使う
	Affinef boneRot[4], afTemp[2];//boneRotは一つ先の骨の角度を見るときに使う
	Affinef T, TT, TTT, TTTT;//global座標で見たときのbone.b_trans
	Affinef A0Rot, A1Rot, A2Rot, ArmRot;//glRotateをAffinefに変換したもの
	
	Affinef R180 = Affinef(Rad(180), 'y');
	double x,y,z,nx,ny,nz;
	double TenCmPerInch = 0.254;
	for(int iii = 0; iii<8; iii++){//初期値に戻す
		A0[iii] = AA0[iii];
		A1[iii] = AA1[iii];
		A2[iii] = AA2[iii];
		A3[iii] = AA3[iii];
	}
	A3[8] = AA3[8];
	AxisX = Vec3f(1.0 , 0.0 , 0.0);//擬似glRotate用の軸
	AxisY = Vec3f(0.0 , 1.0 , 0.0);
	AxisZ = Vec3f(0.0 , 0.0 , 1.0);

	/////////////////////////////////////////////////////    A0環
		A0Rot = glRot2Af(mydb.vh.A_bone[0].rot[cur_frame].x,
						  mydb.vh.A_bone[0].rot[cur_frame].y,
						  mydb.vh.A_bone[0].rot[cur_frame].z,
						  &AxisX, &AxisY, &AxisZ);
		
		T=Affinef(mydb.vh.A_bone[0].trans[cur_frame].x,
				  mydb.vh.A_bone[0].trans[cur_frame].y,
				  mydb.vh.A_bone[0].trans[cur_frame].z);
	
	afTemp[0] = T * A0Rot;
	for(int iii = 0; iii<8; iii++){
		A0[iii] = afTemp[0] * A0[iii];
	}

	////////////////////////////////////////////////////////// 次にA1環を生成
	x = mydb.vh.A_bone[1].b_trans.x;
	y = mydb.vh.A_bone[1].b_trans.y;
	z = mydb.vh.A_bone[1].b_trans.z;
	nx = mydb.vh.A_bone[2].b_trans.x;//次の骨の向いてる方向を見よう
	ny = mydb.vh.A_bone[2].b_trans.y;
	nz = mydb.vh.A_bone[2].b_trans.z;

	boneRot[0] = AfRotBone(x,y,z);
	boneRot[1] = AfRotBone(nx,ny,nz);
	A1Rot = glRot2Af( mydb.vh.A_bone[1].rot[cur_frame].x,
				      mydb.vh.A_bone[1].rot[cur_frame].y,
				      mydb.vh.A_bone[1].rot[cur_frame].z,
				      &AxisX, &AxisY, &AxisZ);

	tempV = A0Rot * Vec3f(x,y,z);
	TT=Affinef(tempV[0], tempV[1], tempV[2]);

	afTemp[0] = TT * T * boneRot[0] * A0Rot;
	afTemp[1] = TT * T * boneRot[1] * A1Rot *  A0Rot;
	for(int iii = 0; iii<8; iii++){
		A1[iii] = (afTemp[0] * A1[iii] + afTemp[1] * A1[iii])/2;
	}

	///////////////////////////////////////////////////// A2も同様に
	x = mydb.vh.A_bone[2].b_trans.x;
	y = mydb.vh.A_bone[2].b_trans.y;
	z = mydb.vh.A_bone[2].b_trans.z;
	nx = mydb.vh.H_bone[0].b_trans.x;//次の骨の向いてる方向を見よう
	ny = mydb.vh.H_bone[0].b_trans.y;
	nz = mydb.vh.H_bone[0].b_trans.z;

	boneRot[2] = AfRotBone(x,y,z);
	boneRot[3] = AfRotBone(nx,ny,nz);
	A2Rot = glRot2Af( mydb.vh.A_bone[2].rot[cur_frame].x,
		              mydb.vh.A_bone[2].rot[cur_frame].y,
					  mydb.vh.A_bone[2].rot[cur_frame].z,
					  &AxisX, &AxisY, &AxisZ);
	tempV = A1Rot * A0Rot * Vec3f(x,y,z);
	TTT = Affinef(tempV[0], tempV[1], tempV[2]);

	afTemp[0] = TTT * TT * T * boneRot[2] * A1Rot * A0Rot;
	afTemp[1] = TTT * TT * T * boneRot[3] * A2Rot * A1Rot * A0Rot;
	for(int iii = 0; iii<8; iii++){
		A2[iii] = (afTemp[0] * A2[iii] + afTemp[1] * A2[iii] )/2;
	}

	//////////////////////////////////  ２回使うのでまとめておく。
	Affinef totalRot = A2Rot * A1Rot * A0Rot;
	Affinef totalT = TTT * TT * T;
	///////////////////////////////////////////////  PushMatrix
	hAxisX = AxisX;
	hAxisY = AxisY;
	hAxisZ = AxisZ;
	///////////////////////////////////////     A3の右鎖骨
	x = mydb.vh.RA_bone[0].b_trans.x;
	y = mydb.vh.RA_bone[0].b_trans.y;
	z = mydb.vh.RA_bone[0].b_trans.z;
	tempV = totalRot * Vec3f(x,y,z);
	TTTT = Affinef(tempV[0], tempV[1], tempV[2]);//
	ArmRot = glRot2Af( mydb.vh.RA_bone[0].rot[cur_frame].x,
					    mydb.vh.RA_bone[0].rot[cur_frame].y,
					    mydb.vh.RA_bone[0].rot[cur_frame].z,
						&AxisX, &AxisY, &AxisZ);

	afTemp[0] = TTTT * totalT * ArmRot * totalRot;
	
	A3[3] = afTemp[0] * A3[3];
	A3[4] = afTemp[0] * A3[4];
	A3[5] = afTemp[0] * A3[5];
	A3[6] = afTemp[0] * A3[6];

	//////////////////////////////////////////   PopMatrix
	AxisX = hAxisX;
	AxisY = hAxisY;
	AxisZ = hAxisZ;
	///////////////////////////////////////    A3の左鎖骨
	x = mydb.vh.LA_bone[0].b_trans.x;
	y = mydb.vh.LA_bone[0].b_trans.y;
	z = mydb.vh.LA_bone[0].b_trans.z;
	tempV = totalRot * Vec3f(x,y,z);
	TTTT = Affinef(tempV[0], tempV[1], tempV[2]);
	ArmRot = glRot2Af( mydb.vh.LA_bone[0].rot[cur_frame].x,
					    mydb.vh.LA_bone[0].rot[cur_frame].y,
					    mydb.vh.LA_bone[0].rot[cur_frame].z,
						&AxisX, &AxisY, &AxisZ);
	afTemp[0] = TTTT * totalT * ArmRot * totalRot;

	A3[0] = afTemp[0] * A3[0];
	A3[1] = afTemp[0] * A3[1];
	A3[7] = afTemp[0] * A3[7];
	A3[8] = afTemp[0] * A3[8];
	
	A3[2] = (A3[3]+A3[1]) / 2;//中点を取って手を抜こう

	//cout << " ======= 変数出力 ======== " << endl;
	//cout << "A0rot % " << mydb.vh.A_bone[0].rot[cur_frame].x << " % " << mydb.vh.A_bone[0].rot[cur_frame].y << " % " << mydb.vh.A_bone[0].rot[cur_frame].z << endl;
	//cout << "A1rot % " << mydb.vh.A_bone[1].rot[cur_frame].x << " % " << mydb.vh.A_bone[1].rot[cur_frame].y << " % " << mydb.vh.A_bone[1].rot[cur_frame].z << endl;
	//cout << "A2rot % " << mydb.vh.A_bone[2].rot[cur_frame].x << " % " << mydb.vh.A_bone[2].rot[cur_frame].y << " % " << mydb.vh.A_bone[2].rot[cur_frame].z << endl;
	//cout << "LA0rot % " << mydb.vh.LA_bone[0].rot[cur_frame].x << " % " << mydb.vh.LA_bone[0].rot[cur_frame].y << " % " << mydb.vh.LA_bone[0].rot[cur_frame].z << endl;
	//cout << "RA0rot % " << mydb.vh.RA_bone[0].rot[cur_frame].x << " % " << mydb.vh.RA_bone[0].rot[cur_frame].y << " % " << mydb.vh.RA_bone[0].rot[cur_frame].z << endl;
	
	//cout << "A0trans" << mydb.vh.A_bone[0].b_trans.x << " # " << mydb.vh.A_bone[0].b_trans.y << " # " << mydb.vh.A_bone[0].b_trans.z << endl;
	//cout << "A1trans" << mydb.vh.A_bone[1].b_trans.x << " # " << mydb.vh.A_bone[1].b_trans.y << " # " << mydb.vh.A_bone[1].b_trans.z << endl;
	//cout << "A2trans" << mydb.vh.A_bone[2].b_trans.x << " # " << mydb.vh.A_bone[2].b_trans.y << " # " << mydb.vh.A_bone[2].b_trans.z << endl;
	//cout << "LA0trans" << mydb.vh.LA_bone[0].b_trans.x << " # " << mydb.vh.LA_bone[0].b_trans.y << " # " << mydb.vh.LA_bone[0].b_trans.z << endl;
	//cout << "RA0trans" << mydb.vh.RA_bone[0].b_trans.x << " # " << mydb.vh.RA_bone[0].b_trans.y << " # " << mydb.vh.RA_bone[0].b_trans.z << endl;

}

// all body drawing
void TGLView::drawBody(int cur_frame)
{
	glEnable(GL_LIGHTING);
glPushMatrix();

	bSeeTest = 10;
     drawjoint(cur_frame, mydb.vh.A_bone[0]); //1

		glPushMatrix();
			bSeeTest = 0;
			drawjoint(cur_frame, mydb.vh.LL_bone[0]); //2
			bSeeTest = 14;
			drawjoint(cur_frame, mydb.vh.LL_bone[1]); //3
			bSeeTest = 15;
			drawjoint(cur_frame, mydb.vh.LL_bone[2]); //4
		glPopMatrix();

		glPushMatrix();
			bSeeTest = 0;
			drawjoint(cur_frame, mydb.vh.RL_bone[0]); //5
			bSeeTest = 11;
			drawjoint(cur_frame, mydb.vh.RL_bone[1]); //6
			bSeeTest = 12;
			drawjoint(cur_frame, mydb.vh.RL_bone[2]); //7
		glPopMatrix();

	    glPushMatrix();
			bSeeTest = 0;
			drawjoint(cur_frame, mydb.vh.A_bone[1]); //8!
			drawjoint(cur_frame, mydb.vh.A_bone[2]); //9!

			glPushMatrix();
				drawjoint(cur_frame, mydb.vh.LA_bone[0]); //10
				drawjoint(cur_frame, mydb.vh.LA_bone[1]);//11!
				bSeeTest = 17;
				drawjoint(cur_frame, mydb.vh.LA_bone[2]);  //12
				bSeeTest = 18;
				drawjoint(cur_frame, mydb.vh.LA_bone[3]);  //13
			glPopMatrix();

			glPushMatrix();
				bSeeTest = 0;
				drawjoint(cur_frame, mydb.vh.RA_bone[0]);  //14
				drawjoint(cur_frame, mydb.vh.RA_bone[1]);  //15!
				bSeeTest = 7;
				drawjoint(cur_frame, mydb.vh.RA_bone[2]);   //16
				bSeeTest = 8;
				drawjoint(cur_frame, mydb.vh.RA_bone[3]);   //17
				
		    glPopMatrix();

			glPushMatrix();
				bSeeTest = 6;
				drawjoint(cur_frame, mydb.vh.H_bone[0]);  //18!
				bSeeTest = 5;
				drawjoint(cur_frame, mydb.vh.H_bone[1]);  //19
			glPopMatrix();

	glPopMatrix();

glPopMatrix();
//glEnable(GL_LIGHTING);

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
	 glRotatef(mydb.vh.Anim.camera.x + YTX, 1.0f, 0.0f, 0.0f);
	 glRotatef(mydb.vh.Anim.camera.y + YTY, 0.0f, 1.0f, 0.0f);
	 glRotatef(mydb.vh.Anim.camera.z, 0.0f, 0.0f, 1.0f);
	 
    //glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_NORMALIZE);
	
  //hip -5 
	 	//ytakeyama
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		TshirtsVertexSolver(mydb.vh.Anim.cur_frm);//ytakeyama

			/*glBegin(GL_LINE_STRIP);
				glColor3d(0.0, 0.0, 0.0);
				glVertex3d(0.0, 0.0, 0.0);			
				glVertex3d(5.0, 0.0, 0.0);
				glVertex3d(0.0, 5.0, 0.0);
				glVertex3d(0.0, 0.0, 5.0);
			glEnd();*/
		Tshirts();//ytakeyama 描画をする関数。drawTshirtsは使わない


      drawBody(mydb.vh.Anim.cur_frm);

	  //bounding box
	 
	 //shadow
	//glEnable(GL_STENCIL_TEST);							//shadow
	//glStencilFunc( GL_ALWAYS, 1, ~0);					//shadow
	//glStencilOp(GL_KEEP,GL_KEEP ,GL_REPLACE);			//shadow

	//床の描画　by武山さん
      /*glPushMatrix();
	      glBindTexture(GL_TEXTURE_2D, tex[1]);
		  glEnable(GL_TEXTURE_2D);
	      glScalef(100.0, 1.0, 100.0); //なして100倍とかしてるの？ by ytakeyama
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
		  glDisable(GL_TEXTURE_2D);//ytakeyama end 影が描けるか謎	
	  glPopMatrix();*/

	//BBall.Draw();//ytakeyama comment out
	//ytakeyama ボールと空を描きます
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
//	DrawShadow();//ytakeyama comment out
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
void TGLView::OnJoystick(unsigned int button, int x, int y, int z){
	std::cout << "B" << button << " x:" << x << " y:" << y << " z:" << z << std::endl;
}
void TGLView::OnSpecialKey(int key, int x, int y) 
{
  /*if (key == GLUT_KEY_LEFT)    { mydb.vh.Anim.camera.y -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { mydb.vh.Anim.camera.y += deltaRot; }
  if (key == GLUT_KEY_UP)      { mydb.vh.Anim.camera.x -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { mydb.vh.Anim.camera.x += deltaRot; }*/
  if (key == GLUT_KEY_LEFT)    { YTY -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { YTY += deltaRot; }
  if (key == GLUT_KEY_UP)      { YTX -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { YTX += deltaRot; }
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
//SLCameraの初期化
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

	//SLCamera
	//SetTimer(200, &TGLView::OnTimer2);
	//~SLCamera

	dwTime = timeGetTime(); //GetTickCount();
	TshirtsVertexInit();//ytakeyama

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
	//cout << pass << endl;

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
	//カラー・デプスバッファマスクをセットする
	//これで以下の内容のピクセルの色の値は、書き込まれない。
	glColorMask(0,0,0,0);								//shadow
	glDepthMask(0);										//shadow
	////影をつけたい領域にステンシル値２を付ける
	glEnable(GL_STENCIL_TEST);							//shadow
	glStencilFunc(GL_EQUAL, 1, ~0);					//shadow
	//ステンシル値が１のピクセルのステンシル値をインクリメント
	glStencilOp(GL_KEEP,GL_KEEP ,GL_INCR);				//shadow
	glDisable(GL_DEPTH_TEST);							//shadow

	//VHの影部分にステンシル値２を付ける
  	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
			//glMultMatrixf((GLfloat *)ShadowMat);		//shadow
			glMultMatrixd(ShadowMat[0]);			//shadow
		    drawBody(mydb.vh.Anim.cur_frm);				//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow

	//ボールの影部分にステンシル値２を付ける
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
	//ビットマスクを解除
	glColorMask(1,1,1,1);								//shadow
	glDepthMask(1);										//shadow

	//影をつける。ステンシル値が２の所だけが描画される
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
	  //箱の領域にステンシル値３を付ける
	glStencilFunc( GL_ALWAYS, 3, ~0);					//shadow
	glStencilOp(GL_KEEP,GL_KEEP ,GL_REPLACE);			//shadow
	//箱の描画
	glPushMatrix();										//shadow
	      glBindTexture(GL_TEXTURE_2D, tex[6]);			//shadow
		  glEnable(GL_TEXTURE_2D);						//shadow
		  glTranslatef(paddle_v.x, paddle_v.y/2, paddle_v.z);//shadow
		  glRotatef(90, 1.0, 0.0, 0.0);							//shadow
	      drawQuad(20, 7);								//shadow
		  glDisable(GL_TEXTURE_2D);						//shadow
	glPopMatrix();										//shadow

	glDisable(GL_STENCIL_TEST);							//shadow

  //同じく、影をつける領域に２をつける
	glStencilFunc( GL_EQUAL, 3, ~0);					//shadow
	//ステンシル値が３のピクセルのステンシル値をデクリメント
	glStencilOp(GL_KEEP,GL_KEEP ,GL_DECR);				//shadow

	//対ボックスの右側側面用
	glPushMatrix();										//shadow
		glDisable(GL_CULL_FACE);						//shadow
	        glMultMatrixd(ShadowMat3[0]);				//shadow
			BBall.Draw();								//shadow
		glEnable(GL_CULL_FACE);							//shadow
	glPopMatrix();										//shadow
	
	//ボールのステンシル値２付け(対ボックスのtop面用)
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
//床の描画
void TGLView::DrawFloor(void){								
	glDisable(GL_LIGHTING);									
	glBegin(GL_QUADS);										
		//面なので、左周りに定義しないと、表として扱われない。
		glVertex3f( 150, GnHeight+2, 150);					
		glVertex3f( 150, GnHeight+2,-150);					
		glVertex3f(-150, GnHeight+2,-150);					
		glVertex3f(-150, GnHeight+2, 150);					
	glEnd();												
	glEnable(GL_LIGHTING);									
}															
//////////	shadow	///////////箱に隠れる影？
void TGLView::DrawFloor2(void){								
	//ｙは箱のtop面の高さ
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
