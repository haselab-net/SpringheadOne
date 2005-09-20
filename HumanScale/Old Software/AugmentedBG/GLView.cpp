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
const float floorHeight = 10.0f;
const float dt = 1.0f / 30.0f;
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
		view->Invalidate();
		return sizeof(af);
	}
	void Send(){
		SendId();
		Affinef af = view->afBody;
		af.Pos().Y() += EYEOFFSET;
		SendData(&af, sizeof(af));
		cmdSocket->Flush();
	}
};
class CmdHit:public Command{
public:
	TGLView* view;
	CmdHit(TGLView* v):view(v),Command(&(v->cmdSocket)){}
	int Id()const{ return 2; }
	int Receive(char* data){
		int& hit = *(int*)data;
		view->hitID = hit;
		view->Invalidate();
		return sizeof(hit);
	}
	void Send(){
		SendId();
		int hit = view->hitID;
		SendData(&hit, sizeof(hit));
		cmdSocket->Flush();
	}
};
class CmdMoveObject:public Command{
public:
	TGLView* view;
	CmdMoveObject(TGLView* v):view(v),Command(&(v->cmdSocket)){}
	int Id()const{ return 3; }
	int Receive(char* data){
		Affinef& objectPosture = *(Affinef*)data;
		for(TGeoNodes::iterator it = view->allNodes.begin(); it != view->allNodes.end(); ++it){
			if(((*it)->objectID + 1) == view->hitID){
				(*it)->SetPosture(objectPosture);
			}
		}
		view->Invalidate();
		return sizeof(objectPosture);
	}
	void Send(){
		SendId();
		Affinef objectPosture;
		for(TGeoNodes::iterator it = view->allNodes.begin(); it != view->allNodes.end(); ++it){
			if(((*it)->objectID + 1) == view->hitID){
				objectPosture = (*it)->GetPosture();
			}
		}
		SendData(&objectPosture, sizeof(objectPosture));
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
	ostrstream ostrfn(filename, sizeof(filename));
	ostrfn << "sf" << projNum << '\0';

	FILE *fp;
	int number;
	int set = 0;
	char ch[128];
    
    if((fp = fopen(filename, "r")) == NULL){
        cout << "Can not read or find file sf1" << endl;
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
	stVision = StereoV(cpNum);
    return true;
}
TGLView::TGLView(int n):nowMode(n){
	bZoom = false;
	nowRotateBirdCam = false;
	birdCamRotateAngle = 0;
	tmpElevation[0] = 0;
	tmpElevation[1] = 1;
	birdsHeight = 5100;
	elevationHeight = birdsHeight;
	elevationDistance = 0;
	elevationSensor = NULL;
	nowElevation = false;
	viewElevation = Rad(-90);
	cRadius = 0.1;
	cSides = 32;
	PI2 = 3.14159265 * 2;
	cStep = PI2 / cSides;
	lastTableAngle = 0.0f;
	bTest = false;
	bDraw = true;
	bDrawCollision = true;
	bDrawNormal = true;
	showInnerData = false;
	getSIGGRAPHData = false;
	quarterView = false;
	quarterViewM = false;
	//	jumpPAve = 1.0f;
	jumpRate = 0.1f;  //ジャンプの移動平均割合
	hitID = 0;
	bBox = false;
	firstDraw = true;
	avatarHit = false;
	objectHit = false;
	bHistorySave = false;
	bHistoryDraw = false;
	bHistoryToFile = false;
	nowLeftBottunOn = false;
	nowRightBottunOn = false;
	numSensor = hiStep.GetNumSensor();

	slcamera.InitSLC();

	switch (nowMode){
	case 0:
		bSender = false;
		birdsView = false;
		break;
	case 1:
		bSender = false;
		birdsView = true;
		break;
	case 2:
		bSender = true;
		birdsView = false;
		break;
	case 3:
		bSender = true;
		birdsView = true;
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
	GetPrivateProfileString("VIEWMODE","initialmode","0",temp_str,100,ini_file); // 初期の視点(0:アパタ 1:上から視点 2:鳥瞰視点)
	viewMode = atof( temp_str );
	GetPrivateProfileString("VIEWMODE","viewturn","0",temp_str,100,ini_file); // 視点の回転角度
	viewTurn = atof( temp_str );
	GetPrivateProfileString("TOPVIEW","topheight","6",temp_str,100,ini_file); // 初期の上から視点の高さ
	topHeight = atof( temp_str );
	GetPrivateProfileString("TOPVIEW","topshift","0.2",temp_str,100,ini_file); // 上から視点のアパターの中心からのずれ
	topShift = atof( temp_str );
	GetPrivateProfileString("BIRDSVIEW","birdsradius","30",temp_str,100,ini_file); // 鳥瞰視点の角度
	birdsRadius = atof( temp_str );
	GetPrivateProfileString("BIRDSVIEW","birdsshift","1.1",temp_str,100,ini_file); // 鳥瞰視点のアパターの中心からのずれ
	birdsShift = atof( temp_str );
	GetPrivateProfileString("STEPWALK","pitchparam","0.4",temp_str,100,ini_file); // 移動速度調節用変数
	hiStep.SetPitchParam( atof( temp_str ) );

	fov_v = 30.0f;
	screenNear = 0.1f;
	screenFar = 10000.0f;
	//	アバターの設定
	radius = 1.0f;
//	afBody = Affinef(350, 0, -230);
	afBody = Affinef(0, 0, 0);
	afBody.LookAtGL(Vec3f(0, 0, 0), Vec3f(0, 1, 0));
	afBody = afBody * Affinef(Rad(180), 'y');
	quarter = Affinef(Rad(-90), 'x');
	quarter.Pos() = Vec3f(0, birdsHeight, 0); 

// アバターの手の設定
hand_radius = ini_radius;//追加：赤羽
afLHand = Affinef(-5,0,-5); //追加：赤羽
afRHand = Affinef(5,0,-5); //追加：赤羽


	afInitialBody = Affinef(ini_initialx,ini_initialy,ini_initialz);
	afBody = afInitialBody;
	vel = Vec3f(0,0,0);
	dvel = Vec3f(0,0,0);
	//dwTime = GetTickCount(); //OnOpen()で代入するようにした
	bFalling = false;
	fallingTime = 0;

	//afBody.LookAtGL(Vec3f(0, 110, 0), Vec3f(0,1,0));
	//afBody.LookAtGL(Vec3f(0, 0, 30), Vec3f(0,100,0));
	afBody.LookAtGL(Vec3f(ini_initialx,ini_initialy,ini_initialz+1),
		Vec3f(ini_initialx,ini_initialy+1,ini_initialz));

//  自分の設定
//	VecMyself[0] = Vec4f();
//	VecMyself[1] = Vec4f(0.5, 0.0, 1.0);
//	VecMyself[2] = Vec4f(-0.5, 0.0, 1.0);
	VecMyself[0] = Vec4f(0.0, 0.0, -radius, 1.0);
	VecMyself[1] = Vec4f( radius/2, 0.0, radius/2, 1.0);
	VecMyself[2] = Vec4f(-radius/2, 0.0, radius/2, 1.0);

	VecSquare[0] = Vec3f(-radius/2 + 1.0, 0.0, -radius/2 + 1.0);
	VecSquare[1] = Vec3f(-radius/2 + 1.0, 0.0,  radius/2 + 1.0);
	VecSquare[2] = Vec3f( radius/2 + 1.0, 0.0,  radius/2 + 1.0);
	VecSquare[3] = Vec3f( radius/2 + 1.0, 0.0, -radius/2 + 1.0);

	double cStep = PI2 / (double)cSides;

	// test用の四角形
	VecTest[0] = Vec4f(0, 0, 0, 1.0);
	VecTest[1] = Vec4f(0, 0, 1, 1.0);
	VecTest[2] = Vec4f(1, 0, 1, 1.0);
	VecTest[3] = Vec4f(1, 0, 0, 1.0);

	//	プロジェクタ位置データのロード
	if (LoadSettings()) SetParam();

	//	通信の初期化
	cmdSocket.Register(new CmdEye(this));
	cmdSocket.Register(new CmdHit(this));
	cmdSocket.Register(new CmdMoveObject(this));
//	cmdSocket.Register(new CmdMoveObjectBox(this));
	if (bSender) cmdSocket.Init(10100, 10000);
	else cmdSocket.Init(10000, 10100);
	
	//	ターンテーブルの初期化
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
	afC = stVision.ShiftCamera(afBody);
	Affinef afProjector = afC * R180 * R * R180;
//	Affinef afProjector = afBody * R180 * R * R180;

	float maxHeight = -999;

//	Affinef afProjector = afBody * R180 * R * R180;

	if(firstDraw){
		afCam = afProjector * afEye;
//		firstDraw = false;
	}else if(!birdsView){
		afCam = afProjector * afEye;
	}
//	Affinef afTmpB = afCam;
//	afTmpB.LookAtGL(afTmpB.Pos() + Vec3f(0,0,10));
	if(firstDraw){
		afBird = Affinef(0, birdsHeight, 0) * afCam * Affinef(Rad(-90), 'x');	//  鳥瞰
		firstDraw = false;
	}
	Affinef afTmpBody = afBody * Affinef(Rad(-90), 'x');

	if (!nowLeftBottunOn && !nowRotateBirdCam){
		afBird.Rot() = Matrix3f(afTmpBody.Rot());			//  キーボード・ターンテーブルによる回転
		afBird.Pos().X() = afBody.Pos().X();
		afBird.Pos().Z() = afBody.Pos().Z();
	}

	afQuarter = afBody * quarter;
	// 移動カメラの設定
	if (birdsView){
		glLoadMatrixf(afBird.inv());
	}else if(quarterView || quarterViewM){
		glLoadMatrixf(afQuarter.inv());
	}else{
		glLoadMatrixf(afCam.inv());
	}
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		(*it)->Draw();
	}

	//  自分の位置表示
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	//  暗幕
	if(bTest){
		glColor3f(0,0,0);
		glBegin(GL_POLYGON);
			glVertex3f(10000, 0, 10000);
			glVertex3f(10000, 0, -10000);
			glVertex3f(-10000, 0, -10000);
			glVertex3f(-10000, 0, 10000);
		glEnd();
	}
	glMultMatrixf(afBody);

	//  軌跡記録
	if(bHistorySave){
		historyData.push_back(afBird.Pos());
		currentTime.push_back(timeGetTime());
	}
	//  軌跡表示
	if(bHistorySave && bHistoryDraw){
		glBegin(GL_LINE_STRIP);
		for(History::iterator it = historyData.begin(); it != historyData.end(); ++it){
			glVertex3fv((*it));
		}
		glEnd();
	}
	
	glColor3f(1,1,0);
	glLineWidth(3.0);
	if(birdsView || quarterView){
		glBegin(GL_LINE_LOOP);
			glVertex3fv(VecMyself[0]);
			glVertex3fv(VecMyself[1]);
			glVertex3fv(VecMyself[2]);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(afEye.Pos().X() - 60, 0, afEye.Pos().Z() - 20);
			glVertex3f(afEye.Pos().X(), 0, afEye.Pos().Z());
			glVertex3f(afEye.Pos().X(), 0, afEye.Pos().Z());
			glVertex3f(afEye.Pos().X() + 60, 0, afEye.Pos().Z() - 20);
		glEnd();
	}

	glPopMatrix();
	
	if(bBox){
		//  すべてのバウンディングボックスの表示
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			if(maxHeight < (*it)->VecObjectBox[0].Y())
				maxHeight = (*it)->VecObjectBox[0].Y();
		}
		//  高さで色分け	
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			glColor3f(1, 0, 0);
			glBegin(GL_LINES);
				glVertex3f((*it)->VecCenterOfBox.X() - 5, 0, (*it)->VecCenterOfBox.Z() + 5);
				glVertex3f((*it)->VecCenterOfBox.X() + 5, 0, (*it)->VecCenterOfBox.Z() - 5);
				glVertex3f((*it)->VecCenterOfBox.X() + 5, 0, (*it)->VecCenterOfBox.Z() + 5);
				glVertex3f((*it)->VecCenterOfBox.X() - 5, 0, (*it)->VecCenterOfBox.Z() - 5);
			glEnd();
		}
		//  全て同じ色
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			glColor3f(0, 1, 0);
			glBegin(GL_LINE_LOOP);
			for (int m = 0; m < 4; m++)
				glVertex3fv((*it)->VecObjectBox[m]);
			glEnd();
		}
	}

	//  ヒットしたバウンディングボックスの表示
	if (hitID && bSender && objectHit){
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3f(0.5, 0.5, 1.0);
		glLineWidth(10.0);
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			glBegin(GL_LINE_LOOP); //GL_QUADS);
			Vec3f vtxs[] = {
			Vec3f(-1000, 0, -1000),	
			Vec3f( 1000, 0, -1000),	
			Vec3f( 1000, 0,  1000),	
			Vec3f(-1000, 0, 1000), 
			};	
			if(((*it)->objectID + 1) == hitID){
				for(int i = 0; i < 4; i++){
					glVertex3fv((*it)->VecObjectBox[i]);

				}
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			if(((*it)->objectID + 1) == hitID){
				for(int i = 4; i < 8; i++){
					glVertex3fv((*it)->VecObjectBox[i]);
				}
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			if(((*it)->objectID + 1) == hitID){
				glVertex3fv((*it)->VecObjectBox[4]);
				glVertex3fv((*it)->VecObjectBox[0]);
				glVertex3fv((*it)->VecObjectBox[1]);
				glVertex3fv((*it)->VecObjectBox[5]);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			if(((*it)->objectID + 1) == hitID){
				glVertex3fv((*it)->VecObjectBox[5]);
				glVertex3fv((*it)->VecObjectBox[1]);
				glVertex3fv((*it)->VecObjectBox[2]);
				glVertex3fv((*it)->VecObjectBox[6]);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			if(((*it)->objectID + 1) == hitID){
				glVertex3fv((*it)->VecObjectBox[6]);
				glVertex3fv((*it)->VecObjectBox[2]);
				glVertex3fv((*it)->VecObjectBox[3]);
				glVertex3fv((*it)->VecObjectBox[7]);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			if(((*it)->objectID + 1) == hitID){
				glVertex3fv((*it)->VecObjectBox[7]);
				glVertex3fv((*it)->VecObjectBox[3]);
				glVertex3fv((*it)->VecObjectBox[0]);
				glVertex3fv((*it)->VecObjectBox[4]);
			}
			glEnd();
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}

	if( showInnerData ){
		Vec2f ccenter = hiStep.GetCenterOfGravity();
		glBegin(GL_POLYGON);
		for(int i = 0; i < cSides; i++){
			double t = cStep * (double)i;
			VecCircle[i] = Vec3f(cRadius * sin(t) + 1.0 + ccenter.X() * 0.2, 0.0, 
			cRadius * cos(t) + 1.0 - ccenter.Y() * 0.2);
			glVertex3fv(VecCircle[i]);
		}
		Vec2f lend = hiStep.GetDirection() * 20;
		glBegin(GL_LINES);
			glVertex3f(1.0, 0.0, 1.0);
			glVertex3f(1.0 + lend.X(), 0.0, 1.0 - lend.Y());
		glEnd();
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
			for (int j = 0; j < numSensor; j++){
				VecStartPosition[j] = Vec3f(0.0, 0.0, -1.6 + 0.2 * j);
				VecSensor[j] = Vec3f(hiStep.GetSensorVoltage(j), 0.0, -1.6 + 0.2 * j);
			}
			for (int j = 0; j < numSensor; j++){
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
	Flip();
}

void TGLView::OnSize(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov_v, (float)(width)/(float)(height) , screenNear , screenFar);
	glMatrixMode(GL_MODELVIEW);
	windowWidth = width;
	windowHeight = height;
}

void TGLView::OnJoystick(unsigned int button, int x, int y, int z){
	std::cout << "B" << button << " x:" << x << " y:" << y << " z:" << z << std::endl;
}

void TGLView::OnKeyboard(int key, int x, int y){
	static float radiusExtend = 0;
	switch(key){
	case '0':
		//視点変更(アパターから　→　上から　→　斜めから　→　アバター・・・)
		viewMode = viewMode + 1;
		viewMode = viewMode % 3;
		break;
	// 実験用にコメントアウト
	case '1':
		hiStep.SetThreshold( 0.1f );
		break;
	case '2': 
		hiStep.SetThreshold( 0.2f );
		break;
	case '3':
		hiStep.SetThreshold( 0.3f );
		break;
	case '4':
		hiStep.SetThreshold( 0.4f );
		break;
	case '5':
		hiStep.SetThreshold( 0.5f );
		break;
	case '6':
		hiStep.SetThreshold( 0.6f );
		break;
	case '7':
		hiStep.SetThreshold( 0.7f );
		break;
	case '8':
		hiStep.SetThreshold( 0.8f );
		break;
	case '9':
		hiStep.SetThreshold( 0.9f );
		break;
	case '/':
		afCam = Affinef(0, 0, -1) * afCam;
		break;
	case ';':
		afCam = Affinef(1, 0, 0) * afCam;
		break;
	case ':':
		afCam = Affinef(-1, 0, 0) * afCam;
		break;		
	case '@':
		afCam = Affinef(0, 0, 1) * afCam;
		break;
	case 'a':
		hiStep.rotateSwitch = !hiStep.rotateSwitch;
		break;
	case 'A':
		hiStep.rotateSwitch = true;
		break;
	case 'b':
		quarterView = false;
		viewElevation = -M_PI/2;
		birdsView = !birdsView;
		glutPostRedisplay();
		break;
	case 'B':
		bBox = !bBox;
		break;
	case 'C':
		hiStep.pSenCal = true; //キャリブレイション始め
		break;
	case 'c':
		hiTurn.Reset();
		break;
	case 'D':
		bDraw = false;
		break;
	case 'd':
		bDraw = !bDraw;
		break;
	case 'E':
		hiStep.pSenCal = false; //キャリブレイション終了
		hiStep.PSenRange();
		break;
	case 'f':
		glutFullScreen();
		break;
	case 'g':      // direction を反転
		hiStep.SetDirection( Matrix2f(Rad(180)) * hiStep.GetDirection() );
		break;
	case 'h':
		bDrawCollision = !bDrawCollision;
		break;
	case 'H':
		bHistoryDraw = !bHistoryDraw;
		break;
	case 'i':
		afBody = afInitialBody;
		afBody.LookAtGL(Vec3f(afInitialBody.PosX(),afInitialBody.PosY(),afInitialBody.PosZ()+1),
			Vec3f(afInitialBody.PosX(),afInitialBody.PosY()+1,afInitialBody.PosZ()));
		vel = Vec3f(0,0,0);
		dvel = Vec3f(0,0,0);
		hiStep.CloseSIGGRAPHFile();
		break;
	case 'I':
		showInnerData = !showInnerData;
		break;
	case 'j':
		afBody.Pos().Y() += 10;
		break;
	case 'l':
		birdsHeight -= 3;
		afBird = Affinef(0, -3, 0) * afBird;
		break;
	case 'm':
		hiStep.PSenSetMin(); //min（何も乗っけないとき）のときの圧力センサ値をセットする
		break;
	case 'M':
		hiStep.PSenSetMax(); //max(一番重いものを乗っけている)のときの圧力センサ値をセットする
		break;
	case 'n':
		bDrawNormal = !bDrawNormal;
		break;
	case 'O':
		if(radiusExtend){
			radiusExtend = 1.2 * radiusExtend;
		}else{
			radiusExtend = 1.2 * radius;
		}
		VecMyself[0] = Vec4f(0.0, 0.0, -radiusExtend, 1.0);
		VecMyself[1] = Vec4f( radiusExtend/2, 0.0, radiusExtend/2, 1.0);
		VecMyself[2] = Vec4f(-radiusExtend/2, 0.0, radiusExtend/2, 1.0);
		break;
	case 'o':
		if(radiusExtend){
			radiusExtend = 0.8 * radiusExtend;
		}else{
			radiusExtend = 0.8 * radius;
		}
		VecMyself[0] = Vec4f(0.0, 0.0, -radiusExtend, 1.0);
		VecMyself[1] = Vec4f( radiusExtend/2, 0.0, radiusExtend/2, 1.0);
		VecMyself[2] = Vec4f(-radiusExtend/2, 0.0, radiusExtend/2, 1.0);
		break;
	case 'q':
		birdsView = false;
		quarterView = !quarterView;
		viewElevation = Rad(-90);
		break;
	case 'r':
		if (hiStep.GetPitchParam() < 1.0f)
			hiStep.SetPitchParam(hiStep.GetPitchParam() + 0.1f);
		break;
	case 'R':
		if (hiStep.GetPitchParam() > 0.1f)
			hiStep.SetPitchParam(hiStep.GetPitchParam() - 0.1f);
		break;
	case 'S':
		hiTurn.Start();
		break;
	case 's':
		hiTurn.Stop();
		break;
	case 't':
		bTest = !bTest;
		break;
	case 'u':
		birdsHeight += 3;
		afBird = Affinef(0, 3, 0) * afBird;
		break;
	case 'W':
		bHistorySave = !bHistorySave;
		break;
	case 'w':
		glutReshapeWindow(512, 512);
		break;
	case '\x1b': //Esc
		slcamera.CloseSLCamera();
		if(!historyData.empty() && !currentTime.empty())
			DataSave();
		Exit();
		break;
	default:
	break;
	}
}

void TGLView::OnMouse(Button button, bool on, int x, int y){
	mouseX = x;
	mouseY = y;
	mouseButton = button;
	static int nowHitID = 0;

	if(!on){
		mouseButton = NONE;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case LEFT:
		if(on){
			if(quarterView && nowElevation && !bZoom){						//  ズーム有効
				bZoom = true;
				return;
			}
			if(bZoom){												//  ズーム無効
				bZoom = false;
				return;
			}
			if(nowRotateBirdCam) return;			//  鳥瞰カメラ回転モードの時マウス入力禁止
			if(avatarHit){
				nowLeftBottunOn = false;
				nowRotateBirdCam = true;
				avatarHit = false;
				return;
			}
			if(objectHit){
				objectHit = false;
				return;
			}
			InverseTransform();
			Collision();
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			beforeRotateAfBody = afBody;
			break;
		}
	case MIDDLE:
		if(on)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	case RIGHT:
		if(on){
			if(avatarHit){
				nowLeftBottunOn = false;
				nowRotateBirdCam = true;
				avatarHit = false;
				return;
			}
			if(objectHit){
				objectHit = false;
				return;
			}
			if(!quarterView){
				viewElevation = Rad(-90);
				quarterView = true;
				birdsView = false;
				if(nowElevation){
					tmpElevation[0] = 0;
					tmpElevation[1] = 1;
					startElevationAngle = elevationSensor->GetEuler().X();
				}
			}else{
				quarterView = false;
				bZoom = false;
				birdsView = true;
			}
		}
		break;
	}
}
void TGLView::OnMouseMove(bool on, int x, int y){
	mouseX = x;
	mouseY = y;
	InverseTransform();
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
}
void TGLView::InverseTransform(){
	Vec4f VecCalcTemp[4];
	float t;

	nMouseX = (2 * mouseX / float(windowWidth) - 1);
	nMouseY = (-2 * mouseY / float(windowHeight) + 1);
	
	GLdouble modelView[16], projection[16];
	GLint viewPort[4];
			
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewPort);

	Affined afModel((Affined&)modelView);
	Affined afProjection((Affined&)projection);

	worldNear = (afProjection * afBird.inv()).inv() * (Vec4f(nMouseX, nMouseY, -0.1, 1) * screenNear);
	worldNear = worldNear / worldNear.W();
	worldFar = (afProjection * afBird.inv()).inv() * (Vec4f(nMouseX, nMouseY, -10.0, 1) * screenNear);
	worldFar = worldFar / worldFar.W();
	// 平面の法線を計算するための四角形ポリゴンの姿勢計算
	for(int n = 0; n < 4; n++){
		VecCalcTemp[n] = afBody * VecTest[n];
	}
	// 平面の法線計算
	planeNormal = Vec3f(VecCalcTemp[1] - VecCalcTemp[0]) ^ Vec3f(VecCalcTemp[3] - VecCalcTemp[0]);
	// 平面との衝突点の計算

	t = (Vec3f(planeNormal.unit()) * Vec3f(VecCalcTemp[0] - worldNear)) / 
		( Vec3f(planeNormal.unit()) * Vec3f(worldFar - worldNear));
	collisionPoint = worldNear + (worldFar - worldNear) * t;
}
void TGLView::LightSettings(void){
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
	glLightfv(GL_LIGHT1, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT1, GL_POSITION, l1pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
}
void TGLView::OnOpen(void){
	//	GLの初期化
	float	lmamb[] = {.7, .7, .7, 1},
			l0amb[] = {0, 0, 0, 1},
			l0dif[] = {1, 1, 1, 1},
			l0spc[] = {1, 1, 1, 1},
			l0pos[] = {0, 0, 10, 0};

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

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_NORMALIZE);
	if (!bSender) glutFullScreen();

	SetTimer(33, &TGLView::OnTimer);
	bDrawNormal = false;
}

typedef map<DtObjectRef, Object *> ObjectList;
extern ObjectList objectList;
void TGLView::InitCollisionDetector(){
	//	SOLIDの初期化
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

	//	チェックすべきオブジェクトの追加
	for (ObjectList::const_iterator i = objectList.begin(); i != objectList.end(); ++i) {
		if (i->first == &afBody) continue;
		TClosestPointDetector cpd(i->second);
		cpds.push_back(cpd);
	}
}

void TGLView::Step(){
//	cout << "afBody.Pos() = " << afBody.Pos() << endl;
//	afBird.Pos().X() += 0.1;
//	cout << "afBird.Pos().X() = " << afBird.Pos().X() << endl;
	slcamera.Step();  //bool変数が返ってくる？？
	float direction = slcamera.GetEyeAngle();
	bool isWalking;
	isWalking = hiStep.GetIsWalking();
//	if ( vel.norm() <= 0.1){
//	if (!isWalking){ 
	if(hiStep.GetVelocity() < hiTurn.GetStopSpeed()){
		//止まってる時（カメラで覗き込み）
		slcamera.GetLabelPos(labelP);
		afBody = stVision.StereoLC(labelP,afBody);
	}
	else {
		stVision.InitParam();
	}

	//time distance 計測

	dwLastTime = dwTime;
	dwTime = GetTickCount();
	float dt = float(dwTime - dwLastTime) / 1000;

	//---------------------------------------------------------------
	//	位置の更新．前回の更新で侵入する向きの速度はなくなっている．
	afBody.Pos() += vel * dt;
	
	//---------------------------------------------------------------
	//	キー入力
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
	if (bSender && bInitSuccess){
		//	ターンテーブル制御処理/
		float turnVel = hiTurn.GetTurnVel();
		hiStep.Step(turnVel);
/*      */

#ifdef SLC_USE		
//		hiTurn.SetVel(hiStep.GetVelocity());
		hiTurn.SetIsWalking(isWalking);
		hiTurn.Step(slcamera.GetEyeAngle());
#else
		hiTurn.Step(hiStep.inner);
#endif
		//		OutPutSIGGRAPHData();
		if (getSIGGRAPHData)
			hiStep.DataOutput(afBody.Pos().X(),afBody.Pos().Y(),afBody.Pos().Z());
		//	向きと速度の更新
		float tableAngle = hiTurn.GetAngle();
		afBody.Rot() =  Matrix3f(tableAngle - lastTableAngle, 'y') * afBody.Rot();
		lastTableAngle = tableAngle;
#ifndef SLC_USE			// 直進のみの場合
			Vec3f inputVel = -afBody.Ez() *  hiStep.GetVelocity();
			vel.X() = inputVel.X();
			vel.Z() = inputVel.Z();
			            //ここまで 
#else
		    // 目線方向でターンテーブルを回す場合
			float hiStepInner = hiStep.GetInner();
			Vec3f inputVelX = afBody.Ex() *  hiStep.GetVelocity() * sin(hiStepInner);
			Vec3f inputVelZ = -afBody.Ez() *  hiStep.GetVelocity() * cos(hiStepInner);
			vel.X() = inputVelX.X() + inputVelZ.X();
			vel.Z() = inputVelZ.Z() + inputVelX.Z();
#endif
	}

	//---------------------------------------------------------------
	//	速度の更新，重力もここで加える
	vel += dvel;
	dvel = Vec3f();
	vel.Y() -= gravity * dt;
	vel = 0.98f * vel;

	//---------------------------------------------------------------
	//	衝突判定を行い，反射の方向を求める．また，侵入をキャンセルする．
	cpds.Detect((float*)afBody.Pos(), radius);		//	最近傍点を求める

	//	 衝突判定の結果を計算オブジェクトに通知
	const float epsilon = 1E-5;
	rnc.Clear();
	penCancel.Clear();
	//	衝突面の列挙
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
	Vec3f normal = rnc.Calculate(vel);					//	反射面の法線
//	afBody.Pos() = penCancel.Calculate(afBody.Pos(), normal);	//	侵入のキャンセル
	
	//	速度を反射させる．
	if (-vel*normal > 6*gravity*dt){	//	侵入速度が大きいときは跳ね返る
		vel -= (1+elasticity) * (vel * normal) * normal;
	}else if (vel*normal < 0){			//	侵入速度が小さいときは停止
		vel -= (vel * normal) * normal;
	}
	bool b = false;
	while(cmdSocket.Receive()) b = true;
	if (bSender && !b){
		CmdEye cmdEye(this);
		cmdEye.Send();
		CmdHit cmdHit(this);
		cmdHit.Send();
		CmdMoveObject cmdMoveObject(this);
		cmdMoveObject.Send();
	}
	static int stopFileDetectCounter;
	stopFileDetectCounter ++;
	if (stopFileDetectCounter > 10){
		if (_access("stop", 00) != -1){
			Exit();
		}
		stopFileDetectCounter = 0;
	}
	if ( avatarHit || objectHit ){
		Move();
	}
	if (nowRotateBirdCam)
		CamRotate();
	if (bDraw) Invalidate();			//	描画要求
	if (quarterView && !(nowElevation)){
		InitElevation();
	}
	if (quarterView && nowElevation){
		NowElevation();
	}
	if (quarterViewM && !(nowElevation)){
		InitElevationM();
	}
	if (quarterViewM && nowElevation){
		NowElevationM();
	}
}

void TGLView::Collision(){
	Vec4f VecTempA[3], VecCalcTemp[4], VecTempO[4];
	Vec4f c[3], n[3];
	float distance[2];
	bool firstComp = true;
	int countCheck = 0;
	avatarHit = true;
	objectHit = false;

	// アバタの姿勢計算
	for(int m = 0; m < 3; m++){
		VecTempA[m] = afBody * VecMyself[m];
	}
//	cout << "collisionPoint = " << collisionPoint << endl;
 	for(int i = 0; i < 3; i++){
		c[i] = VecTempA[i] - collisionPoint;
	}
	n[0] = Vec3f(c[0]) ^ Vec3f(c[1]);
	n[1] = Vec3f(c[1]) ^ Vec3f(c[2]);
	n[2] = Vec3f(c[2]) ^ Vec3f(c[0]);
	// アバタの中を選択しているかどうか
	for(int j = 0; j < 3; j++){
		if((planeNormal.unit() * n[j]) > 0.0){
			avatarHit = false;
		}
	}
	if(avatarHit){
		avatarMoveVector = collisionPoint;
		return;
	}
	// アバタにヒットしなかったら、オブジェクトとの当たり判定を行う
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		for(int m = 0; m < 4; m++){
			VecTempO[m] = (*it)->VecObjectBox[m];
		}
		// バウンディングボックスの中に入っているかどうかの判定
		if((VecTempO[0].X() < collisionPoint.X()) && (VecTempO[2].X() > collisionPoint.X()) 
			&& (VecTempO[0].Z() > collisionPoint.Z()) && (VecTempO[2].Z() < collisionPoint.Z())){
			objectHit = true;
			cout << (*it)->Name() << endl;
			// バウンディングボックスの中心と衝突点の距離
			distance[1] = abs(sqrt(pow((*it)->VecCenterOfBox.X() - collisionPoint.X(), 2) 
				+ pow((*it)->VecCenterOfBox.Z() - collisionPoint.Z(), 2)));
			if(!firstComp){
				if(distance[1] < distance[0]){
					distance[0] = distance[1];
					hitID = (*it)->objectID + 1;
					countCheck++;
				}
			}else{
				firstComp = false;
				distance[0] = distance[1];
				hitID = (*it)->objectID + 1;
				countCheck++;
			}
		}
	}
	if(objectHit){
		objectMoveVector = collisionPoint;
		return;
	}
}
void TGLView::DataSave(){
	ofstream fOut;		//  ファイル出力用オブジェクト
	char fName[32];
	char fExtension[] = ".csv";
	_itoa(int(timeGetTime()), fName, 10);
	strcat(fName, fExtension);
	cout << fName << endl;

	fOut.open(fName);

	if(!fOut.is_open()){
		cout << "Can't open output file." << endl;
		return;
	}
	fOut << "Time[ms], x, z" << endl;
	Times::iterator itT = currentTime.begin();
	History::iterator itH = historyData.begin();
	for(; itT != currentTime.end(); ++itT, ++itH){
		fOut << *itT << ", " << (*itH).X() << ", " << (*itH).Z() << endl;
	}
	fOut.close();
	return;
}
void TGLView::Move(){
	Vec3f tmpAvatarMoveVector;
	static Vec3f avatarRotVector;
	Vec3f tmpObjectMoveVector;
	static Vec3f objectRotVector;
	float avatarAngle = 0;
	float objectAngle = 0;
	float alpha = 0.9f, beta = 0.98f;
	int moveFactor = 5;
	float clickError = 0.05;		//  クリック時に生じるずれ

	if(avatarHit){
		nowLeftBottunOn = true;
		tmpAvatarMoveVector = avatarMoveVector;
		avatarMoveVector = alpha * avatarMoveVector + (1 - alpha) * Vec3f(collisionPoint);			//  アバターの位置計算
		//  クリック時に生じるずれを抑える
		if((abs(tmpAvatarMoveVector.X() - avatarMoveVector.X()) < clickError) 
			&& (abs(tmpAvatarMoveVector.Z() - avatarMoveVector.Z()) < clickError)){
			avatarMoveVector = tmpAvatarMoveVector;
			return;
		}
		avatarRotVector = avatarMoveVector - tmpAvatarMoveVector;									//  アバターの移動量（現在の位置-過去の位置）
		if(avatarRotVector.Z() != 0){
			avatarAngle = atan2(-avatarRotVector.Z(), avatarRotVector.X());
		}else{
			if(avatarRotVector.X() <= 0){
				avatarAngle = -M_PI;
			}else{
				avatarAngle = 0;
			}
		}
		Affinef afTempA = Affinef((avatarAngle - M_PI/2), 'Y');
		// スケールを反映させる
		if((afBody.Ex().norm() != 1) 
			|| (afBody.Ex().norm() != 1) 
			|| (afBody.Ex().norm() != 1)){
			afTempA.Ex() *= afBody.Ex().norm();
			afTempA.Ey() *= afBody.Ey().norm();
			afTempA.Ez() *= afBody.Ez().norm();
		}
		afBody.Rot() = Matrix3f(afTempA.Rot());
		afBody.Pos() = avatarMoveVector;
		afBird.Pos().X() = avatarMoveVector.X();
		afBird.Pos().Z() = avatarMoveVector.Z();
	}
	if(objectHit){
		tmpObjectMoveVector = objectMoveVector;
		objectMoveVector = beta * objectMoveVector + (1 - beta) * Vec3f(collisionPoint);
		//  クリック時に生じるずれを抑える
		if((abs(tmpObjectMoveVector.X() - objectMoveVector.X()) < clickError) 
			&& (abs(tmpObjectMoveVector.Z() - objectMoveVector.Z()) < clickError)){
			objectMoveVector = tmpObjectMoveVector;
			return;
		}
		objectRotVector = objectMoveVector - tmpObjectMoveVector;										//  オブジェクトの移動量計算
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			if(((*it)->objectID + 1) == hitID){
				Affinef afGlobalPosture = (*it)->GetGlobalPosture();
				afGlobalPosture.Pos() = objectMoveVector;
				if(objectRotVector.Z() != 0){
					objectAngle = atan2(-objectRotVector.Z()/2, objectRotVector.X()/2);
				}else{
					if(objectRotVector.X() <= 0){
						objectAngle = -M_PI;
					}else{
						objectAngle = 0;
					}
				}
				Affinef afTempO = Affinef((objectAngle - M_PI/2), 'Y');
				// スケールを反映させる
				if((afGlobalPosture.Ex().norm() != 1) 
					|| (afGlobalPosture.Ex().norm() != 1) 
					|| (afGlobalPosture.Ex().norm() != 1)){
					afTempO.Ex() *= afGlobalPosture.Ex().norm();
					afTempO.Ey() *= afGlobalPosture.Ey().norm();
					afTempO.Ez() *= afGlobalPosture.Ez().norm();
				}
				afGlobalPosture.Rot() = Matrix3f(afTempO.Rot());
				(*it)->SetGlobalPosture(afGlobalPosture);
				(*it)->solid->prev = (*it)->solid->curr;
				(*it)->solid->curr = Transform(afGlobalPosture);
				// Xの最大値、最小値
				float maxX, minX;
				// Zの最大値、最小値
				float maxZ, minZ;
				// Yの最大値
				float maxY, minY;
				
				for(int i = 0; i < (*it)->vertices.size(); i++){
					Vec3f VecTempV = afGlobalPosture * (*it)->vertices[i];
					if(i){
						if(maxX < VecTempV.X())
							maxX = VecTempV.X();
						if(minX > VecTempV.X())
							minX = VecTempV.X();
						if(maxZ < VecTempV.Z())
							maxZ = VecTempV.Z();
						if(minZ > VecTempV.Z())
							minZ = VecTempV.Z();
						if(maxY < VecTempV.Y())
							maxY = VecTempV.Y();
						if(minY > VecTempV.Y())
							minY = VecTempV.Y();
					}else{
						maxX = VecTempV.X();
						minX = VecTempV.X();
						maxZ = VecTempV.Z();
						minZ = VecTempV.Z();
						maxY = VecTempV.Y();
						minY = VecTempV.Y();
					}
				}
				(*it)->VecObjectBox[0] = Vec4f(minX, maxY, maxZ, 1);
				(*it)->VecObjectBox[1] = Vec4f(minX, maxY, minZ, 1);
				(*it)->VecObjectBox[2] = Vec4f(maxX, maxY, minZ, 1);
				(*it)->VecObjectBox[3] = Vec4f(maxX, maxY, maxZ, 1);
				(*it)->VecObjectBox[4] = Vec4f(minX, minY, maxZ, 1);
				(*it)->VecObjectBox[5] = Vec4f(minX, minY, minZ, 1);
				(*it)->VecObjectBox[6] = Vec4f(maxX, minY, minZ, 1);
				(*it)->VecObjectBox[7] = Vec4f(maxX, minY, maxZ, 1);
				(*it)->VecCenterOfBox.X() = ((*it)->VecObjectBox[0].X() + (*it)->VecObjectBox[2].X()) / 2;
				(*it)->VecCenterOfBox.Z() = ((*it)->VecObjectBox[0].Z() + (*it)->VecObjectBox[2].Z()) / 2;
			}
		}
	}
}

void TGLView::CamRotate(){
	static float nowTheta = 0;
	Affinef afCamRotate;
	static bool rotationInit = false;

	if(!rotationInit){
		afCamRotate = afBody * beforeRotateAfBody.inv();
		birdCamRotateAngle = acos(afCamRotate.ExX());
		if ( asin(afCamRotate.ExZ()) < 0 ){
			birdCamRotateAngle = -birdCamRotateAngle;
		}
		rotationInit = true;
	}
	if ( abs(Deg(nowTheta - birdCamRotateAngle)) <= 5 ){
		nowRotateBirdCam = false;
		birdCamRotateAngle = 0;
		nowTheta = 0;
		rotationInit = false;
		return;
	}
	if ( birdCamRotateAngle > 0 ){
		nowTheta += Rad(5);
		afBird = afBird * Affinef(Rad(-5), 'z');
	}else{
		nowTheta -= Rad(5);
		afBird = afBird * Affinef(Rad(+5), 'z');
	}
}
void TGLView::InitElevation(){
	tmpElevation[0] = 0;
	tmpElevation[1] = 1;
	dev.VPool().Rent(elevationSensor);
	if (!elevationSensor){
		nowElevation = false;
		return;
	}
	startElevationAngle = elevationSensor->GetEuler().X();
	nowElevation = true;
}
void TGLView::NowElevation(){
	tmpElevation[1] = elevationSensor->GetEuler().X();
	if(tmpElevation[0] != tmpElevation[1]){
		if(!bZoom){
			viewElevation = viewElevation - (tmpElevation[1] - startElevationAngle);
		}else{
			float tmpElevationDiff = Deg(abs(tmpElevation[0] - tmpElevation[1])) * 7;		//  ズーム用
			if(tmpElevation[0] > tmpElevation[1]){
				birdsHeight += tmpElevationDiff;
				afBird = Affinef(0, +tmpElevationDiff, 0) * afBird;
			}else{
				birdsHeight -= tmpElevationDiff;
				afBird = Affinef(0, -tmpElevationDiff, 0) * afBird;
			}
		}

		elevationHeight = birdsHeight * cos(viewElevation + M_PI/2);
		elevationDistance = birdsHeight * sin(viewElevation + M_PI/2);

		quarter.Pos() = Vec3f();
		quarter = Affinef(viewElevation, 'x');
		quarter.Pos() = Vec3f(0, elevationHeight, elevationDistance);
		tmpElevation[0] = tmpElevation[1];
		startElevationAngle = tmpElevation[1];
	}
}
void TGLView::InitElevationM(){
	tmpMouseY[0] = 0;
	tmpMouseY[1] = 1;
	viewElevation = Rad(-90);
	startMouseY = mouseY;
	nowElevation = true;
}
void TGLView::NowElevationM(){
	tmpMouseY[1] = mouseY;
	if(tmpMouseY[0] != tmpMouseY[1]){
		viewElevation = viewElevation + Rad(tmpMouseY[1] - startMouseY);
		elevationHeight = birdsHeight * cos(viewElevation + M_PI/2);
		elevationDistance = birdsHeight * sin(viewElevation + M_PI/2);
		cout << "tmp-start = " << tmpMouseY[1] - startMouseY << endl;
		cout << "tmp = " << tmpMouseY[0] << ", " << tmpMouseY[1] << endl;

		quarter.Pos() = Vec3f();
		quarter = Affinef(viewElevation, 'x');
		quarter.Pos() = Vec3f(0, elevationHeight, elevationDistance);
		tmpMouseY[0] = tmpMouseY[1];
		startMouseY = tmpMouseY[1];
		cout << "Elevation = " << Deg(viewElevation) << endl;
		cout << "height, distance = " << elevationHeight << ", " << elevationDistance << endl;
	}
}