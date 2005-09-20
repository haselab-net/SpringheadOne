#include <Base/Env.h>
#include <Device/DRAdapio.h>
#include <Device/DRTokin3D.h>

#include <Solid/SOLID.h>
#include <VertexBase.h>
#include <Polyhedron.h>
#include <Object.h>

#include "GeoNode.h"
#include "GLView.h"

#include <GL/glut.h>
#include <crtdbg.h>
#include <strstream>
#include <io.h>
#include <penalty.h>
#include "communication.h"
#include <fstream>
using namespace std;

#define KEY_ESC -27
const float floorHeight = -100000.0f;
//const float dt = 1.0f / 30.0f;
const float gravity = 9.8f;
const float elasticity = 0.8f;
//const float elasticity = 0.0f;
const float EYEOFFSET = 0.8f;
class CmdEye:public Command{
public:
	TGLView* view;
	CmdEye(TGLView* v):view(v),Command(&(v->cmdSocket)){}
	int Id()const{ return 1; }
	int Receive(char* data){
		Affinef& af = *(Affinef*)data;
		view->afBody = af;
		view->afBody.Pos().Y() -= EYEOFFSET;
		//		cout << af << endl;
		view->Invalidate();
		return sizeof(af);
	}
	void Send(){
		SendId();
//		cout << view->afBody << endl;
		Affinef af = view->afBody;
		af.Pos().Y() += EYEOFFSET;
		SendData(&af, sizeof(af));
		cmdSocket->Flush();
	}
};

bool TGLView::LoadSettings(){
	char* cname = getenv("COMPUTERNAME");
	char* p = cname;
	while(*p && !isdigit(*p)) p++;
	int cpNum = -1;
	if (strlen(p)) cpNum = atoi(p);
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
//	cout << number << endl;
//        	for(int j=0;j<6;j++)
//	cout << setting[j] << endl;
    fclose(fp);
    return true;
}
//TGLView::TGLView(bool b):bSender(b){
TGLView::TGLView(int n):nowMode(n){
	cRadius = 0.1;
	cSides = 32;
	PI2 = 3.14159265 * 2;
	cStep = PI2 / cSides;
	lastTableAngle = 0.0f;
	bDraw = true;
	bDrawCollision = false;
	bDrawNormal = true;
	showInnerData = false;
	getSIGGRAPHData = false;
//	file("abc.txt");
	centerGFile = NULL;
	turnDataFile = NULL;
//	dataFileSIGGRAPH = NULL;
	switch (nowMode){
	case 0:
		bSender = false;
//		birdsView = false;
		break;
	case 1:
		bSender = true;
//		birdsView = false;
		break;
	case 2:
		bSender = false;
//		birdsView = true;
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
	GetPrivateProfileString("STEPWALK","pitchparam","0.4",temp_str,100,ini_file); // �ړ����x���ߗp�ϐ�
	hiStep.SetPitchParam( atof( temp_str ) );

/*
	float ini_radius = 0;
	if( GetPrivateProfileString("AVATOR","radius","0.5",temp_str,100,ini_file) )
		ini_radius = atof( temp_str );
	float ini_initialx = 0;
	if( GetPrivateProfileString("AVATOR","initialx","0",temp_str,100,ini_file) )
		ini_initialx = atof( temp_str );
	float ini_initialy = 0;
	if( GetPrivateProfileString("AVATOR","initialy","0",temp_str,100,ini_file) )
		ini_initialy = atof( temp_str );
	float ini_initialz = 0;
	if( GetPrivateProfileString("AVATOR","initialz","0",temp_str,100,ini_file) )
		ini_initialz = atof( temp_str );
	birdsHeight = 0;
	if( GetPrivateProfileString("VIEWMODE","initialmode","0",temp_str,100,ini_file) )
		viewMode = atof( temp_str );
	if( GetPrivateProfileString("VIEWMODE","viewheight","5",temp_str,100,ini_file) )
		birdsHeight = atof( temp_str );
*/

	fov_v = 30.0f;
	//	�A�o�^�[�̐ݒ�
	radius = ini_radius;//1.0f;
	afInitialBody = Affinef(ini_initialx,ini_initialy,ini_initialz);
	afBody = afInitialBody;
	vel = Vec3f(0,0,0);
	dvel = Vec3f(0,0,0);
	//dwTime = GetTickCount(); //OnOpen()�ő������悤�ɂ���
	bFalling = false;
	fallingTime = 0;

	//afBody.LookAtGL(Vec3f(0, 110, 0), Vec3f(0,1,0));
	//afBody.LookAtGL(Vec3f(0, 0, 30), Vec3f(0,100,0));
	afBody.LookAtGL(Vec3f(ini_initialx,ini_initialy,ini_initialz+1),
		Vec3f(ini_initialx,ini_initialy+1,ini_initialz));

	//  �����̐ݒ�
	/*
	VecMyself[0] = Vec3f();
	VecMyself[1] = Vec3f(0.5, 0.0, 1.0);
	VecMyself[2] = Vec3f(-0.5, 0.0, 1.0);
	*/
	VecMyself[0] = Vec3f(        0, 0.0,-radius/2);
	VecMyself[1] = Vec3f( radius/2, 0.0, radius/2);
	VecMyself[2] = Vec3f(-radius/2, 0.0, radius/2);

	VecSquare[0] = Vec3f(-radius/2 + 1.0, 0.0, -radius/2 + 1.0);
	VecSquare[1] = Vec3f(-radius/2 + 1.0, 0.0,  radius/2 + 1.0);
	VecSquare[2] = Vec3f( radius/2 + 1.0, 0.0,  radius/2 + 1.0);
	VecSquare[3] = Vec3f( radius/2 + 1.0, 0.0, -radius/2 + 1.0);

	double cStep = PI2 / (double)cSides;

	//  �Œ�J�����̐ݒ�F���݂͎g�p���Ă��Ȃ�
	bEye[0] = 0.0; bEye[1] = 10.0; bEye[2] = 0.0;
	bTarget[0] = 0.0; bTarget[1] = 0.0; bTarget[2] = 0.0;
	bUp[0] = 0.0; bUp[1] = 0.0; bUp[2] = -1.0;

	//	�v���W�F�N�^�ʒu�f�[�^�̃��[�h
	if (LoadSettings()) SetParam();

	//	�ʐM�̏�����
	cmdSocket.Register(new CmdEye(this));
	if (bSender) cmdSocket.Init(10100, 10000);
	else cmdSocket.Init(10000, 10100);
	
	//	�^�[���e�[�u���̏�����
	bInitSuccess = false;
	if (bSender){
		dev.RPool().Register(new DRTokin3D);
		dev.RPool().Register(new DRAdapio(10));
		dev.Init();
		std::cout << dev;
		bInitSuccess = hiTurn.Init(dev);
		bInitSuccess = bInitSuccess && hiStep.Init(dev);
	}
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
	Sleep(10);
}
void TGLView::OnDraw(){
	glClearColor(clearColor.X(), clearColor.Y(), clearColor.Z(), 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	static Affinef R180(Rad(180),'y');
	
	Affinef afProjector = afBody * R180 * R * R180;

	Affinef afCam;
	Affinef afBird;
	// =====�e�X�g�R�[�h by T.Kobayashi
	//afBird = afBody; //���_�̓A�p�^�Ɠ���
	//afBird = afBody * Affinef( Rad(-180),'y' );  //���_��Y���𒆐S��180����]
	//afBird = afBody * Affinef( 0,0.5f,0 ); //���_��Y��������0.5�ړ������ʒu��
	//afBird = afBody * Affinef( 0,birdsHeight,0 ) * Affinef( Rad(-90),'x' );
	//afBird = afBody * Affinef( 0,birdsHeight,birdsHeight*1.1 ) * Affinef( Rad(-30),'x' ) * Affinef( Rad(-90),'z' );

	// �ړ��J�����̐ݒ�
	switch(viewMode){
	case 0: // �A�o�^�[�̎��_
		afCam = afProjector * Affinef( Rad(-viewTurn),'z' );
		break;

	case 1: // �^�ォ��̎��_
		afCam = afBody * Affinef(0,topHeight,0) * Affinef(Rad(-90), 'x') * Affinef( Rad(-viewTurn),'z' );
		break;

	case 2: // �΂߂���̎��_
		//birdsShift = birds
		//afCam = afBody * Affinef(0,topHeight,birdsHeight*dvel.Z()*0.5) * Affinef(Rad(-90), 'x') * Affinef( Rad(-viewTurn),'z' );
		afCam = afBody * Affinef(0,birdsHeight,birdsHeight*birdsShift) * Affinef( Rad(-birdsRadius),'x' ) * Affinef( Rad(-viewTurn),'z' );
		break;
	}
	glLoadMatrixf(afCam.inv());

/* �L�[�Ŏ��_�؂�ւ��������R�����g��
	if (birdsView){
		//Affinef afBird = Affinef(0,80,0) * afCam * Affinef(Rad(-90), 'x'); //��Ɉړ����ꂽ?
		glLoadMatrixf(afBird.inv());
	}else{
		glLoadMatrixf(afCam.inv());
	}
*/

	// �Œ�J�����̐ݒ�F�ʏ�͎g�p���Ȃ�
/*	if (birdsView){
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(bEye[0], bEye[1], bEye[2],
			bTarget[0], bTarget[1], bTarget[2],
			bUp[0], bUp[1], bUp[2]);
	}else{
		glLoadMatrixf(afCam);
	}*/

	glCullFace(GL_FRONT);

	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		(*it)->Draw();
//		(*it)->DrawGeometry();
	}

	//  �����̈ʒu�\��
	if( showInnerData ){
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glMultMatrixf(afBody);

		glColor3f(1,1,1);
		glBegin(GL_LINE_LOOP);
			glVertex3fv(VecMyself[0]);
			glVertex3fv(VecMyself[1]);
			glVertex3fv(VecMyself[2]);
		glEnd();
/*
	Vec2f ccenter = hiStep.GetCenterOfGravity();

	glBegin(GL_POLYGON);
		for(int i = 0; i < cSides; i++){
			double t = cStep * (double)i;
			VecCircle[i] = Vec3f(cRadius * sin(t) + 1.0 + ccenter.X() * 0.2, 0.0, 
			cRadius * cos(t) + 1.0 - ccenter.Y() * 0.2);
			glVertex3fv(VecCircle[i]);
		}
//		std::cout << hiStep.GetCenterOfGravity().X() * 10 << std::endl;
	glEnd();
*/
		Vec2f lend = hiStep.GetDirection() * 4;
		glBegin(GL_LINES);
			glVertex3f(1.0, 0.0, 1.0);
			glVertex3f(1.0 + lend.X(), 0.0, 1.0 - lend.Y());
		glEnd();
		Vec2f ccenter = hiStep.cog;
		glColor3f(1,0,0);
		glBegin(GL_POLYGON);
			for(int i = 0; i < cSides; i++){
				double t = cStep * (double)i;
				VecCircle[i] = Vec3f(cRadius * sin(t) + 1.0 + ccenter.X(), 0.0, 
				cRadius * cos(t) + 1.0 - ccenter.Y());
				glVertex3fv(VecCircle[i]);
			}
		glEnd();
		glColor3f(0,1,1);
		ccenter = hiStep.cogAve;
		glBegin(GL_POLYGON);
			for(int i = 0; i < cSides; i++){
				double t = cStep * (double)i;
				VecCircle[i] = Vec3f(cRadius * sin(t) + 1.0 + ccenter.X(), 0.0, 
				cRadius * cos(t) + 1.0 - ccenter.Y());
				glVertex3fv(VecCircle[i]);
			}
		glEnd();

		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		glBegin(GL_POLYGON);
			for(int j = 0; j < 4; j++){
				glVertex3fv(VecSquare[j]);
			}
		glEnd();
		glBegin(GL_LINES);
			VecStartPosition[0] = Vec3f(0.0, 0.0, -1.6);
			VecSensor[0] = Vec3f(hiStep.GetSensorVoltage(0), 0.0, -1.6);
			VecStartPosition[1] = Vec3f(0.0, 0.0, -1.4);
			VecSensor[1] = Vec3f(hiStep.GetSensorVoltage(1), 0.0, -1.4);
			VecStartPosition[2] = Vec3f(0.0, 0.0, -1.2);
			VecSensor[2] = Vec3f(hiStep.GetSensorVoltage(2), 0.0, -1.2);
			VecStartPosition[3] = Vec3f(0.0, 0.0, -1.0);
			VecSensor[3] = Vec3f(hiStep.GetSensorVoltage(3), 0.0, -1.0);
			for (int j = 0; j < 4; j++){
				glVertex3fv(VecStartPosition[j]);
				glVertex3fv(VecSensor[j]);
			}
		glEnd();
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	if (1){
#if 0
		glPushMatrix();
		char picthChar[11] = "picthParam";
		char topHeightChar[10] = "topHeight";
		//glRasterPos2f(afCam.PosX(), afCam.PosZ());
		//glRasterPos3f(0.0f, 2,0);
		//glRasterPos3f(VecMyself[0].X(),VecMyself[0].Y()+1,VecMyself[0].Z());
		glRasterPos3f(afCam.PosX(),afCam.PosY()-5,afCam.PosZ());
		for (int i = 0; i < 10; i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, picthChar[i]);
		}
		//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '');
		Affinef afText = Affinef( 1,0,0) * afCam;
		//glRasterPos3f(afCam.PosX()+1,afCam.PosY()-5,afCam.PosZ());
		glRasterPos3f(afText.PosX()+1,afText.PosY()-5,afText.PosZ());
		for (int i = 0; i < 9; i++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, topHeightChar[i]);
		}
#endif
	}

	if (bDrawCollision){
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		cpds.Draw();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	if (bDrawNormal){
		glEnd();
		glLoadIdentity();
		Vec3f pos(1,1,-8);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3f(1,0,0);
		for(int i=0; i<rnc.normals.size(); ++i){
			glBegin(GL_LINES);
			glVertex3fv(pos);
			glVertex3fv(pos + rnc.normals[i].unit());
			glEnd();
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	glutPostRedisplay();
	Flip();
}

void TGLView::OnSize(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(fov_v, (float)(width)/(float)(height) , 0.1f , 1000.0f);
	Affinef af;
	af.InitProjectionGL(Vec3f(-10.5f, 0, 1.5f), Vec2f(2.4f, 2.4f* (float)(height)/(float)(width)), 0.1f, 1000.0f);
	Affinef mirror;
	mirror.Ey() *= -1;
	af = af * mirror;
	glLoadMatrixf(af);
	glMatrixMode(GL_MODELVIEW);
}
void TGLView::OnJoystick(unsigned int button, int x, int y, int z){
	std::cout << "B" << button << " x:" << x << " y:" << y << " z:" << z << std::endl;
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	switch(key){
	case '0':
		viewMode = 0; //���_(�A�p�^�[)
		break;
	case '1':
		viewMode = 1; //���_(�ォ��)
		break;
	case '2': 
		viewMode = 2; //���_(�΂߂���)
		break;
	case '3':
		hiStep.SetThreshold( 0.1f );
		break;
	case '4':
		hiStep.SetThreshold( 0.2f );
		break;
	case '5':
		hiStep.SetThreshold( 0.3f );
		break;
	case '6':
		hiStep.SetThreshold( 0.4f );
		break;
	case '7':
		hiStep.SetThreshold( 0.5f );
		break;
	case '8':
		hiStep.SetThreshold( 0.7f );
		break;
	case '9':
		hiStep.SetThreshold( 0.8f );
		break;
	case 'a':
		hiStep.rotateSwitch = !hiStep.rotateSwitch;
		break;
	case 'A':
		hiStep.rotateSwitch = true;
		break;
	case 'b':
		glutPostRedisplay();
		break;
	case 'c':
		hiTurn.Reset();
		break;
	case 'C':
		hiStep.pSenCal = true; //�L�����u���C�V�����n��
		break;
	case 'd':
		bDraw = !bDraw;
		break;
	case 'D':
		bDraw = false;
		break;
	case 'E':
		hiStep.pSenCal = false; //�L�����u���C�V�����I��
		hiStep.PSenRange();
		break;
	case 'f':
		glutFullScreen();
		break;
	case 'g':      // direction �𔽓]
		hiStep.SetDirection( Matrix2f(Rad(180)) * hiStep.GetDirection() );
		break;
	case 'h':
		bDrawCollision = !bDrawCollision;
		break;
	case 'H':
		showInnerData = !showInnerData;
		break;
	case 'i':
		afBody = afInitialBody;
		afBody.LookAtGL(Vec3f(afInitialBody.PosX(),afInitialBody.PosY(),afInitialBody.PosZ()+1),
			Vec3f(afInitialBody.PosX(),afInitialBody.PosY()+1,afInitialBody.PosZ()));
		vel = Vec3f(0,0,0);
		dvel = Vec3f(0,0,0);
		hiStep.CloseSIGGRAPHFile();
		break;
	case 'j':
		afBody.Pos().Y() += 10;
		break;
	case 'l':
		if(viewMode==1)	topHeight -= 1; //�ォ�王�_�̍������~
		else if(viewMode==2) birdsHeight -= 1; //���Վ��_�̍������~
		break;
	case 'm':
		hiStep.PSenMin(); //min�i����������Ȃ��Ƃ��j�̂Ƃ��̈��̓Z���T�l���Z�b�g����
		break;
	case 'M':
		hiStep.PSenMax(); //max(��ԏd�����̂�������Ă���)�̂Ƃ��̈��̓Z���T�l���Z�b�g����
		break;
	case 'n':
		bDrawNormal = !bDrawNormal;
		break;
	case 'p':
		hiStep.printForCenterG =! hiStep.printForCenterG;
		if(hiStep.printForCenterG){
			printFileNameTime = timeGetTime();			
			centerGFile = new std::ofstream(_itoa(printFileNameTime,printFileName,10));
			std::cout << "Now Getting CenterG Data..." << std::endl;
		}else{
			if( centerGFile )
				delete centerGFile;
			std::cout << "Stop Getting CenterG Data..." << std::endl;
		}
		break;
	case 'P':
		hiTurn.printTurnData =! hiTurn.printTurnData;
		if(hiTurn.printTurnData){
			folderName = "turnData/";
			strcpy( folderNameBack,folderName );
//			strcat( fileName,_itoa(printTurnDataTime,printTurnData,10) );
			printTurnDataTime = timeGetTime();			
//			std::cout << "time is " << timeGetTime() << std::endl;
//			_itoa(printTurnDataTime,printTurnData,10);
			turnDataFile = new std::ofstream(strcat(folderNameBack,_itoa(printTurnDataTime,printTurnData,10)));
//			turnDataFile = new std::ofstream(_itoa(printTurnDataTime,printTurnData,10));
			std::cout << "Now Getting Turn Data..." << std::endl;
		}else{
			if( turnDataFile )
				delete turnDataFile;
			std::cout << "Stop Getting Turn Data..." << std::endl;
		}
		break;
	case 'r':
		if (hiStep.GetPitchParam() < 1.0f)
			hiStep.SetPitchParam(hiStep.GetPitchParam() + 0.1f);
		break;
	case 'R':
		if (hiStep.GetPitchParam() > 0.1f)
			hiStep.SetPitchParam(hiStep.GetPitchParam() - 0.1f);
		break;
	case 's':
		hiTurn.Stop();
		break;
	case 'S':
		hiTurn.Start();
		break;
	case 't':
		getSIGGRAPHData = !getSIGGRAPHData;
		break;
	case 'T':
		getSIGGRAPHData = false;
		break;
	case 'u':
		if(viewMode==1)	topHeight += 1; //�ォ�王�_�̍����㏸
		else if(viewMode==2) birdsHeight += 1; //���Վ��_�̍����㏸
		break;
	case 'w':
		glutReshapeWindow(400,300);
		break;
	case '\x1b': //Esc
		Exit();
		break;
	default:
	break;
	}
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

	//	GL�̏�����
	float	lmamb[] = {.7, .7, .7, 1},
			l0amb[] = {0, 0, 0, 1},
			l0dif[] = {1, 1, 1, 1},
			l0spc[] = {1, 1, 1, 1},
			l0pos[] = {0, 0, 10, 0},
			l1pos[] = {0, 5, 5, 1};

	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmamb);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT1, GL_POSITION, l1pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	if (!bSender) glutFullScreen();

	SetTimer(33, &TGLView::OnTimer);
	bDrawNormal = false;

	//----- DirectSound�̏�����
	// ���݃A�N�e�B�u��Window�𓾂�B
	HWND hwnd = GetForegroundWindow(); 
	// Window�^�C�g���ł��̃\�t�g�����m�F�B
	char windowTitle[30];
	GetWindowText( hwnd,windowTitle,30 );
	//if( !strcmp( windowTitle,"MultiWalk" ) ) hwnd = NULL;
#ifdef USE_SOUND
	easyds = new CEasyDS( hwnd,2 );
	// DirectSound���g�p�\�ȏꍇ��Wave�t�@�C����ǂݍ���
	if( easyds->IsAvailable() ){
		easyds->Waves(0)->LoadFromFile( "collision.wav" );
		easyds->Waves(0)->SetSynchronous(3);
		easyds->Waves(1)->LoadFromFile( "fall.wav" );
		easyds->Waves(1)->SetSynchronous(3);
		std::cout << "DirectSound OK. WindowTitle:" << windowTitle << std::endl;
	}
#endif

	dwTime = GetTickCount();
}

typedef std::map<DtObjectRef, Object *> ObjectList;
extern ObjectList objectList;
void TGLView::InitCollisionDetector(){
	//	SOLID�̏�����
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		TGeoNode::TVertices& vtxs = (*it)->vertices;
		if (!(*it)->faces.size()) continue;
		
		DtShapeRef shape = dtNewComplexShape();
		for(int i=0; i<(*it)->faces.size(); ++i){
			TGeoNode::TFace& face = (*it)->faces[i];
			dtBegin(DT_POLYGON);
			for(int v=0; v<face.size(); ++v){
				dtVertex(vtxs[face[v]].X(), vtxs[face[v]].Y(), vtxs[face[v]].Z());
			}
			dtEnd();
		}
		dtEndComplexShape();
		dtCreateObject(*it, shape); 
		(*it)->solid = objectList[*it];
		dtLoadMatrixf((*it)->GetPosture());
	}

	//	�`�F�b�N���ׂ��I�u�W�F�N�g�̒ǉ�
	for (ObjectList::const_iterator i = objectList.begin(); i != objectList.end(); ++i) {
		if (i->first == &afBody) continue;
		TClosestPointDetector cpd(i->second);
		cpds.push_back(cpd);
	}
}

void TGLView::Step(){
	//time distance �v��

	dwLastTime = dwTime;
	dwTime = GetTickCount();
	float dt = float(dwTime - dwLastTime) / 1000;

	//---------------------------------------------------------------
	//	�ʒu�̍X�V�D�O��̍X�V�ŐN����������̑��x�͂Ȃ��Ȃ��Ă���D
	afBody.Pos() += vel * dt;
	if (afBody.Pos().X() > 15) { afBody.Pos().X() -= 30; }
	if (afBody.Pos().X() < -15) { afBody.Pos().X() += 30; }
	if (afBody.Pos().Z() > 15) { afBody.Pos().Z() -= 30; }
	if (afBody.Pos().Z() < -15) { afBody.Pos().Z() += 30; }
	
	std::cout << "X = " << afBody.Pos().X() << std::endl;
	std::cout << "Z = " << afBody.Pos().Z() << std::endl;
	std::cout << "Y = " << afBody.Pos().Y() << std::endl;
	//---------------------------------------------------------------
	//	�L�[����
	float accel = gravity * dt;
	if (keyboard[' ']) dvel.Y() += 4;
	if (specialKey[GLUT_KEY_UP]) dvel -= afBody.Ez() * accel;
	if (specialKey[GLUT_KEY_DOWN]) dvel += afBody.Ez() * accel;
	if (specialKey[GLUT_KEY_HOME]) vel = Vec3f();
	if (specialKey[GLUT_KEY_LEFT]){
		afBody = afBody * Affinef(Rad(5), 'Y');
	}
	if (specialKey[GLUT_KEY_RIGHT]){
		afBody = afBody * Affinef(Rad(-5), 'Y');
	}

	/*
	// �}�E�X����
	static POINT p = { 320,240 };
	GetCursorPos( &p );
	afBody = afBody * Affinef( Rad(moveRadius*(320-p.x)),'Y' );
	SetCursorPos( 320,240 );

	// ����ɐi��
	dvel -= afBody.Ez() * autoMoveZ;
	*/

	if(hiStep.printForCenterG)
		hiStep.Print(*centerGFile);

	if(hiTurn.printTurnData)
		hiTurn.PrintData(*turnDataFile);

	if (bSender && bInitSuccess){
		//	�^�[���e�[�u�����䏈��
		float turnVel = hiTurn.GetTurnVel();
		hiStep.Step(turnVel);
		hiTurn.Step(hiStep.inner);
//		OutPutSIGGRAPHData();
		if (getSIGGRAPHData)
			hiStep.DataOutput(afBody.Pos().X(),afBody.Pos().Y(),afBody.Pos().Z());
		//	�����Ƒ��x�̍X�V
		float tableAngle = hiTurn.GetAngle();
		float dTableAngle = tableAngle - lastTableAngle;
		afBody.Rot() =  Matrix3f(dTableAngle, 'y') * afBody.Rot();
		lastTableAngle = tableAngle;
		//if( fabs(dTableAngle) < Rad(0.5) ){
			Vec3f inputVel = -afBody.Ez() *  hiStep.GetVelocity();
			vel.X() = inputVel.X();
			vel.Z() = inputVel.Z();
		//}
	}

	//---------------------------------------------------------------
	//	���x�̍X�V�C�d�͂������ŉ�����
	vel += dvel;
	dvel = Vec3f();
	vel.Y() -= gravity * dt;
	vel = 0.98f * vel;
	if( vel.Y() < -4 ){
		vel.Y() = -4;
	}

	//---------------------------------------------------------------
	//	�Փ˔�����s���C���˂̕��������߂�D�܂��C�N�����L�����Z������D
	cpds.Detect((float*)afBody.Pos(), radius);		//	�ŋߖT�_�����߂�

	//	 �Փ˔���̌��ʂ��v�Z�I�u�W�F�N�g�ɒʒm
	const float epsilon = 1E-5;
	rnc.Clear();
	penCancel.Clear();
	//	�Փ˖ʂ̗�
	if (floorHeight + radius - afBody.Pos().Y() > 0){
		rnc.Add(Vec3d(0,1,0));
		penCancel.Add(Vec3f(0, floorHeight + radius - afBody.Pos().Y(), 0), Vec3d(0,1,0));
	}
	for(TClosestPointDetectors::iterator it = cpds.begin(); it != cpds.end(); ++it){
		for(TClosestPoints::iterator itp = it->points.begin(); itp != it->points.end(); ++itp){
			Vec3d cp = (Vec3d&)itp->point;
			Vec3d n = afBody.Pos() - cp;
			if (n.square() > Square(epsilon)){
				n = n.unit();
				rnc.Add(n);
				penCancel.Add(cp, n);
			}
		}
	}
	Vec3f normal = rnc.Calculate(vel);					//	���˖ʂ̖@��
	afBody.Pos() = penCancel.Calculate(afBody.Pos(), normal);	//	�N���̃L�����Z��
	
	// �i�����x�������Ƃ��͉����Ȃ炷
	if (-vel*normal > 6*gravity*dt){	//	�N�����x���傫���Ƃ�
		vel -= (vel * normal) * normal; //���˕Ԃ��Ȃ��Ƃ��̎�
		//vel -= (1+elasticity) * (vel * normal) * normal; // ���˕Ԃ��Ƃ��̎�
#ifdef USE_SOUND
		//easyds->Waves(0)->Play();
#endif
	}else if (vel*normal < 0){			//	�N�����x���������Ƃ��͒�~
		vel -= (vel * normal) * normal;
	}

	if( bFalling ){
		if( vel.Y() > -3 ){
			// �����I������Ƃ��ɉ����Ȃ炷
#ifdef USE_SOUND
			easyds->Waves(0)->Play();
#endif
			bFalling = false;
		}
	}else if( vel.Y() < -3 ){
		// ��莞�Ԉȏ�AY�����ւ̑��x��-3�ȉ��������Ƃ��͗������Ă���Ɣ��f�B
		if( fallingTime > 0.7f ){
			// �������͂��߂��Ƃ��ɉ����Ȃ炷
#ifdef USE_SOUND
			easyds->Waves(1)->Play();
#endif
			fallingTime = 0;
			bFalling = true;
		}else{
			fallingTime += dt;
		}
	}

	// �������Ⴍ�Ȃ����珉���ʒu�ֈړ�
	if( afBody.PosY() < -30 ){
		afBody = afInitialBody;
		afBody.LookAtGL(Vec3f(afInitialBody.PosX(),afInitialBody.PosY(),afInitialBody.PosZ()+1),
			Vec3f(afInitialBody.PosX(),afInitialBody.PosY()+1,afInitialBody.PosZ()));
		vel = Vec3f(0,-10,0);
		dvel = Vec3f(0,0,0);
	}

	bool b = false;
	while(cmdSocket.Receive()) b = true;
	if (bSender && !b){
		CmdEye cmdEye(this);
		cmdEye.Send();
	}
	static int stopFileDetectCounter;
	stopFileDetectCounter ++;
	if (stopFileDetectCounter > 10){
		if (_access("stop", 00) != -1){
			Exit();
		}
		stopFileDetectCounter = 0;
	}
	if (bDraw) Invalidate();			//	�`��v��
}
/*
void TGLView::OutPutSIGGRAPHData(){
	if(dataFileSIGGRAPH == NULL && afBody.Pos().Y() > 30){
		if(3.6 < afBody.Pos().X() && afBody.Pos().X() < 4.3 && 0.3 < afBody.Pos().Z() && afBody.Pos().Z() < 1.0){
			folderName = "pointA/";
			strcpy( folderNameBack,folderName );
			fileNameSIGGRAPHTime = timeGetTime();			
			dataFileSIGGRAPH = new std::ofstream(strcat(folderNameBack,_itoa(fileNameSIGGRAPHTime,fileSIGGRAPHTime,10)));
			std::cout << "Now Getting Turn Data..." << std::endl;
		}else if(-1.7 < afBody.Pos().X() && afBody.Pos().X() < -2.4 && 3.6 < afBody.Pos().Z() && afBody.Pos().Z() < 4.4) {
			folderName = "pointB/";
			strcpy( folderNameBack,folderName );
			fileNameSIGGRAPHTime = timeGetTime();			
			dataFileSIGGRAPH = new std::ofstream(strcat(folderNameBack,_itoa(fileNameSIGGRAPHTime,fileSIGGRAPHTime,10)));
			std::cout << "Now Getting Turn Data..." << std::endl;
		}
	}else if(afBody.Pos().Y() > 30){
		hiStep.Print(*dataFileSIGGRAPH);
//		float inner2 = atan2(walkingDirection.Y(), walkingDirection.X());
//		*dataFileSIGGRAPH << afBody.Pos().X() << "," << afBody.Pos().Z() << "," << cog.X() << "," << cog.Y() << "," << -inner << "," << inner2 << "," << step << "," << dwTime << std::endl;
	}
	if(afBody.Pos().Y() < 30){
		if (dataFileSIGGRAPH){
			delete dataFileSIGGRAPH;
			std::cout << "Stop Getting Turn Data..." << std::endl;
			dataFileSIGGRAPH = NULL;
		}
	}
}
*/