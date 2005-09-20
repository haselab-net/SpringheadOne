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
class CmdHit:public Command{
public:
	TGLView* view;
	CmdHit(TGLView* v):view(v),Command(&(v->cmdSocket)){}
	int Id()const{ return 2; }
	int Receive(char* data){
		int& hit = *(int*)data;
		view->hitID = hit;
		//		cout << af << endl;
		view->Invalidate();
		return sizeof(hit);
	}
	void Send(){
		SendId();
//		cout << view->afBody << endl;
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
//		cout << "Receive objectPosture = " << objectPosture;
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
//		cout << "Send objectPosture = " << objectPosture;
		SendData(&objectPosture, sizeof(objectPosture));
		cmdSocket->Flush();
	}
};
/*class CmdMoveObjectBox:public Command{
public:
	TGLView* view;
	CmdMoveObjectBox(TGLView* v):view(v),Command(&(v->cmdSocket)){}
	int Id()const{ return 4; }
	int Receive(char* data){
		vector<Vec4f>& boxPosture = *(vector<Vec4f>*)data;
		for(TGeoNodes::iterator it = view->allNodes.begin(); it != view->allNodes.end(); ++it){
			if(((*it)->objectID + 1) == view->hitID){
//				cout << "hitID = " << view->hitID << endl;
				for(int i = 0; i < 4; i++){
					(*it)->VecObjectBox[i] = boxPosture[i];
//					cout << "Receive VecObjectBox = " << (*it)->VecObjectBox[i] << endl;
				}
				for(int i = 0; i < 4; i++){
//					cout << "Receive boxPosture = " << boxPosture[i] << endl;
				}
			}
		}
		view->Invalidate();
		return sizeof(boxPosture);
	}
	void Send(){
		SendId();
		vector<Vec4f> boxPosture;
		for(TGeoNodes::iterator it = view->allNodes.begin(); it != view->allNodes.end(); ++it){
			if(((*it)->objectID + 1) == view->hitID){
//				cout << "hitID = " << view->hitID << endl;
				for(int i = 0; i < 4; i++){
					boxPosture.push_back((*it)->VecObjectBox[i]);
//					cout << "Send VecObjectBox = " << (*it)->VecObjectBox[i] << endl;
				}
				for(int i = 0; i < 4; i++){
//					cout << "Send boxPosture = " << boxPosture[i] << endl;
				}
			}
		}
		SendData(&boxPosture, sizeof(boxPosture));
		cmdSocket->Flush();
	}
};*/
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
    return true;
}
TGLView::TGLView(int n):nowMode(n){
	lastTableAngle = 0.0f;
	bDraw = true;
	bDrawCollision = true;
	bDrawNormal = true;
	birdsHight = 10;
	hitID = 0;
	hitNode = NULL;
	bBox = false;
	firstDraw = true;
	avatarHit = false;
	avatarDMove = false;
	objectHit = false;
	objectDMove = false;
	bHistorySave = false;			//  軌跡記録用フラグ
	bHistoryDraw = false;			//  軌跡表示フラグ
	bHistoryToFile = false;			//  軌跡ファイル書きだしフラグ

	switch (nowMode){
	case 0:
		bSender = false;
		birdsView = false;
		break;
	case 1:
		bSender = false;
		birdsView = false;
		break;
	case 2:
		bSender = true;
		birdsView = true;
		break;
	}

	fov_v = 30.0f;
	screenNear = 0.1f;
	screenFar = 10000.0f;
	//	アバターの設定
	radius = 1.0f;
	afBody = Affinef(0, 0, 10);
	afBody.LookAtGL(Vec3f(0, 0, 0), Vec3f(0, 1, 0));
	afBody = afBody * Affinef(Rad(180), 'y');

	//  自分の設定
	VecMyself[0] = Vec4f(0.0, 0.0, -radius, 1.0);
	VecMyself[1] = Vec4f( radius/2, 0.0, radius/2, 1.0);
	VecMyself[2] = Vec4f(-radius/2, 0.0, radius/2, 1.0);

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

	float maxHeight = -999;

	Affinef afProjector = afBody * R180 * R * R180;

	if(firstDraw){
		afCam = afProjector * afEye;
		firstDraw = false;
	}else if(!bSender){
		afCam = afProjector * afEye;
	}
	Affinef afTmpB = afCam;
	afTmpB.LookAtGL(afTmpB.Pos() + Vec3f(0,0,10));
	afBird = Affinef(0, birdsHight, 0) * afTmpB * Affinef(Rad(-90), 'x');	//  鳥瞰

	// 移動カメラの設定
	if (birdsView){
		glLoadMatrixf(afBird.inv());
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
	glMultMatrixf(afBody);

	glColor3f(1,1,0);
	glLineWidth(3.0);
	if(birdsView){
		glBegin(GL_LINE_LOOP);
			glVertex3fv(VecMyself[0]);
			glVertex3fv(VecMyself[1]);
			glVertex3fv(VecMyself[2]);
		glEnd();
	}
	glPopMatrix();

	//  軌跡記録
	if(bHistorySave){
		historyData.push_back(afBody.Pos());
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
/*	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex3f(worldNear.X() - 0.5, 0, worldNear.Z() + 0.5);
		glVertex3f(worldNear.X() + 0.5, 0, worldNear.Z() - 0.5);
		glVertex3f(worldNear.X() + 0.5, 0, worldNear.Z() + 0.5);
		glVertex3f(worldNear.X() - 0.5, 0, worldNear.Z() - 0.5);
	glEnd();
	glBegin(GL_POINTS);
		glVertex3fv(worldNear);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
		glVertex3f(worldFar.X() - 0.5, 0, worldFar.Z() + 0.5);
		glVertex3f(worldFar.X() + 0.5, 0, worldFar.Z() - 0.5);
		glVertex3f(worldFar.X() + 0.5, 0, worldFar.Z() + 0.5);
		glVertex3f(worldFar.X() - 0.5, 0, worldFar.Z() - 0.5);
	glEnd();
	glBegin(GL_POINTS);
		glVertex3fv(worldFar);
	glEnd();

	glBegin(GL_POINTS);
		glVertex3fv(collisionPoint);
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
		glVertex3f(collisionPoint.X() - 0.5, collisionPoint.Y(), collisionPoint.Z() + 0.5);
		glVertex3f(collisionPoint.X() + 0.5, collisionPoint.Y(), collisionPoint.Z() - 0.5);
		glVertex3f(collisionPoint.X() + 0.5, collisionPoint.Y(), collisionPoint.Z() + 0.5);
		glVertex3f(collisionPoint.X() - 0.5, collisionPoint.Y(), collisionPoint.Z() - 0.5);
	glEnd();
*/	
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
	if (hitID && bSender){
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3f(0.5, 0.5, 1.0);
		glLineWidth(10.0);
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			glBegin(GL_LINE_LOOP); //GL_QUADS);
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
	case 'a':
		cout << "Display afBody = " << afBody << endl;
		break;
	case 'b':
		birdsView = !birdsView;
		glutPostRedisplay();
		break;
	case 'c':
		hiTurn.Reset();
		break;
	case 'C':
		hiStep.pSenCal = true; //キャリブレイション始め
		break;
	case 'd':
		bDraw = !bDraw;
		break;
	case 'D':
		bDraw = false;
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
	case 'j':
		afBody.Pos().Y() += 10;
		break;
	case 'l':
		birdsHight -= 3;
		break;
	case 'm':
		bBox = !bBox;
		glutPostRedisplay();
		break;
	case 'n':
		bDrawNormal = !bDrawNormal;
		break;
	case 'r':
		if(radiusExtend){
			radiusExtend = 0.8 * radiusExtend;
		}else{
			radiusExtend = 0.8 * radius;
		}
		VecMyself[0] = Vec4f(0.0, 0.0, -radiusExtend, 1.0);
		VecMyself[1] = Vec4f( radiusExtend/2, 0.0, radiusExtend/2, 1.0);
		VecMyself[2] = Vec4f(-radiusExtend/2, 0.0, radiusExtend/2, 1.0);
		break;
	case 'R':
		if(radiusExtend){
			radiusExtend = 1.2 * radiusExtend;
		}else{
			radiusExtend = 1.2 * radius;
		}
		VecMyself[0] = Vec4f(0.0, 0.0, -radiusExtend, 1.0);
		VecMyself[1] = Vec4f( radiusExtend/2, 0.0, radiusExtend/2, 1.0);
		VecMyself[2] = Vec4f(-radiusExtend/2, 0.0, radiusExtend/2, 1.0);
		break;
	case 's':
		hiTurn.Stop();
		break;
	case 'S':
		hiTurn.Start();
		break;
	case 't':
	case 'u':
		birdsHight += 3;
		break;
	case 'w':
		glutReshapeWindow(400,300);
		break;
	case 'W':
		bHistorySave = !bHistorySave;
		break;
	case '2':
		afCam = Affinef(0, 0, -1) * afCam;
		break;
	case '4':
		afCam = Affinef(1, 0, 0) * afCam;
		break;
	case '6':
		afCam = Affinef(-1, 0, 0) * afCam;
		break;		
	case '8':
		afCam = Affinef(0, 0, 1) * afCam;
		break;
	case '\x1b':
		if(!historyData.empty() && !currentTime.empty())
			DataSave();
		Exit();
	default:
	break;
	}
}

void TGLView::OnMouse(Button button, bool on, int x, int y){
//	cout << "On Mouse 呼び出し中" << endl;
	mouseX = x, mouseY = y;
	mouseButton = button;
	static int nowHitID = 0;

	nMouseX = (2 * x / float(windowWidth) - 1);
	nMouseY = (-2 * y / float(windowHeight) + 1);
	if(!on){
		mouseButton = NONE;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case LEFT:
		if(on){
			if(!avatarHit && !objectHit){
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
		
				Collision();
//				cout << "CollisionAvatar = " << avatarHit << endl;
//				cout << "On Mouse object Hit, avatarHit, worldMouse, worldNear, on = " << objectHit << ", " << avatarHit << ", " << worldMouse << ", " << worldNear << ", " << on << endl;
				glutSetCursor(GLUT_CURSOR_CROSSHAIR);
				break;
			}else{
				avatarHit = false;
				objectHit = false;
				break;
			}
		}else{
//			avatarHit = false;
//			objectHit = false;
			hitNode = NULL;
			break;
		}
	case MIDDLE:
		if(on)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	case RIGHT:
		if(on){
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
			if(avatarDMove){
				Collision();
				afBody.Pos() = collisionPoint;
				avatarDMove = false;
				avatarHit = false;
//				cout << "On Mouse avatarDMove:: objectHit, avatarHit, worldMouse, worldNear, on = " << objectHit << ", " << avatarHit << ", " << worldMouse << ", " << worldNear << ", " << on << endl;
				break;
			}else{
				Collision();
				if(avatarHit){
					avatarDMove = true;
				}
			}
			if(objectDMove){
				Collision();
				cout << "after hitID = " << hitID << endl;
				hitID = nowHitID;
				for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
					if(((*it)->objectID + 1) == hitID){
						Affinef afGlobalPosture = (*it)->GetGlobalPosture();
						afGlobalPosture.Pos().X() = collisionPoint.X();
						afGlobalPosture.Pos().Z() = collisionPoint.Z();
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
				objectDMove = false;
				objectHit = false;
			}else{
				Collision();
				if(objectHit){
					objectDMove = true;
					nowHitID = hitID;
					cout << " before hitID = " << hitID << endl;
				}
			}
		}
		break;
	}
}
void TGLView::OnMouseMove(bool on, int x, int y){
//	cout << "On Mouse Move 呼び出し中" << endl;
	mouseX = x, mouseY = y;
	nMouseX = (2 * x / float(windowWidth) - 1);
	nMouseY = (-2 * y / float(windowHeight) + 1);
	GLdouble modelView[16], projection[16];
	static Vec3f moveVector;
	static float avatarAngle = 0, objectAngle = 0;
	static int ah = 0, oh = 0;
	float alpha = 0.95f;
	int moveFactor = 5;
		
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	Affined afModel((Affined&)modelView);
	Affined afProjection((Affined&)projection);

	worldNear = (afProjection * afBird.inv()).inv() * (Vec4f(nMouseX, nMouseY, -0.1, 1) * screenNear);
	worldNear = worldNear / worldNear.W();
//	cout << "x, y = " << x << ", " << y << endl;
//	cout << "On Mouse Move objectHit, avatarHit, worldMouse, worldNear, on = " << objectHit << ", " << avatarHit << ", " << worldMouse << ", " << worldNear << ", " << on << endl;

	if(avatarHit && (worldMouse != worldNear) && !on){
		moveVector = Vec3f(alpha * moveVector) + Vec3f((1 - alpha) * (worldNear - worldMouse));
//		cout << endl;
//		cout << "afBody.Rot() = " << afBody.Rot() << endl;
		if(moveVector.Z() != 0){
			avatarAngle = atan2(-moveVector.Z(), moveVector.X());
		}else{
			if(moveVector.X() <= 0){
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
		afBody.Pos() += moveFactor * birdsHight * moveVector;
		worldMouse = worldNear;
	}
	if(objectHit && (worldMouse != worldNear) && !on){
		moveVector = Vec3f(alpha * moveVector) + Vec3f((1 - alpha) * (worldNear - worldMouse));
		objectAngle = atan2(-moveVector.Z(), moveVector.X());
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			if(((*it)->objectID + 1) == hitID){
				Affinef afGlobalPosture = (*it)->GetGlobalPosture();
				if(moveVector.Z() != 0){
					objectAngle = atan2(-moveVector.Z(), moveVector.X());
				}else{
					if(moveVector.X() <= 0){
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
				afGlobalPosture.Pos() += moveFactor * birdsHight * moveVector;
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
				worldMouse = worldNear;
			}
		}
	}
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
//	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0dif);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spc);
//	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT1, GL_POSITION, l1pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
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
//			l1pos[] = {0, 5, 5, 1};

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
//	glLightfv(GL_LIGHT1, GL_AMBIENT, l0amb);
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, l0dif);
//	glLightfv(GL_LIGHT1, GL_SPECULAR, l0spc);
//	glLightfv(GL_LIGHT1, GL_POSITION, l1pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	
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
		hiTurn.Step(hiStep.inner);
		hiStep.Print(std::cout);
		//	向きと速度の更新
		float tableAngle = hiTurn.GetAngle();
		afBody.Rot() =  Matrix3f(tableAngle - lastTableAngle, 'y') * afBody.Rot();
		lastTableAngle = tableAngle;
		Vec3f inputVel = -afBody.Ez() *  hiStep.GetVelocity();
		vel.X() = inputVel.X();
		vel.Z() = inputVel.Z();
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
	afBody.Pos() = penCancel.Calculate(afBody.Pos(), normal);	//	侵入のキャンセル
	
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
	if (bDraw) Invalidate();			//	描画要求
}

void TGLView::Collision(){
//	cout << "Collision 呼び出し中" << endl;
	Vec4f VecTempA[3], VecCalcTemp[4], VecTempO[4];
	Vec4f c[3], n[3];
	float t, distance[2];
	bool firstComp = true;
	int countCheck = 0;
	objectHit = false;
	int judgeCount = 0;

	float innerScreenHeight = screenNear * tan( M_PI * ( fov_v / 2.0 ) / 180.0 ) * 2;
	float innerScreenWidth = windowWidth * innerScreenHeight / windowHeight;

	// アバタの姿勢計算
	for(int m = 0; m < 3; m++){
		VecTempA[m] = afBody * VecMyself[m];
	}
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
 	for(int i = 0; i < 3; i++){
		c[i] = VecTempA[i] - collisionPoint;
	}
	n[0] = Vec3f(c[0]) ^ Vec3f(c[1]);
	n[1] = Vec3f(c[1]) ^ Vec3f(c[2]);
	n[2] = Vec3f(c[2]) ^ Vec3f(c[0]);
	// アバタの中を選択しているかどうか
	for(int j = 0; j < 3; j++){
		if((planeNormal.unit() * n[j]) > 0.0){
			judgeCount--;
		}
	}
	if(!judgeCount){
		avatarHit = true;
		cout << "avatarHit!!" << endl;
	}else{
		avatarHit = false;
	}
	// アバタにヒットしなかったら、オブジェクトとの当たり判定を行う
	if(!avatarHit){
		hitNode = NULL;
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			for(int m = 0; m < 4; m++){
				VecTempO[m] = (*it)->VecObjectBox[m];
			}
			// バウンディングボックスの中に入っているかどうかの判定
			if((VecTempO[0].X() <= collisionPoint.X()) && (VecTempO[2].X() >= collisionPoint.X()) 
				&& (VecTempO[0].Z() >= collisionPoint.Z()) && (VecTempO[2].Z() <= collisionPoint.Z())){
				objectHit = true;
				// バウンディングボックスの中心と衝突点の距離
				distance[1] = abs(sqrt(pow((*it)->VecCenterOfBox.X() - collisionPoint.X(), 2) 
					+ pow((*it)->VecCenterOfBox.Z() - collisionPoint.Z(), 2)));
				if(!firstComp){
					if(distance[1] < distance[0]){
						hitNode = *it;
						distance[0] = distance[1];
						hitID = (*it)->objectID + 1;
						countCheck++;
					}
				}else{
					hitNode = *it;
					firstComp = false;
					distance[0] = distance[1];
					hitID = (*it)->objectID + 1;
					countCheck++;
				}
			}
		}
		if(!objectHit){
			return;
		}else{
			worldMouse = worldNear;
			return;
		}
	}else{
		avatarHit = true;
		worldMouse = worldNear;
		return;
	}
}
void TGLView::DataSave(){
	ofstream fout;		//  ファイル出力用オブジェクト
	fout.open("test.csv");

	if(!fout){
		cout << "Can't open output file." << endl;
		return;
	}
	fout << "Time[ms], x, z" << endl;
	Times::iterator itT = currentTime.begin();
	History::iterator itH = historyData.begin();
	for(; itT != currentTime.end(); ++itT, ++itH){
		fout << *itT << ", " << (*itH).X() << ", " << (*itH).Z() << endl;
	}
	fout.close();
	return;
}