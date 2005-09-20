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
#include "tri.h"

using namespace std;


#define KEY_ESC -27
const float floorHeight = 0.0f;
const float dt = 1.0f / 30.0f;
const float gravity = 9.8f;
const float elasticity = 0.8f;
//const float elasticity = 0.0f;

class CmdEye:public Command{
public:
	TGLView* view;
	CmdEye(TGLView* v):view(v),Command(&(v->cmdSocket)){}
	int Id()const{ return 1; }
	int Receive(char* data){
		Affinef& af = *(Affinef*)data;
		view->afBody = af;
		//		cout << af << endl;
		view->Invalidate();
		return sizeof(af);
	}
	void Send(){
		SendId();
//		cout << view->afBody << endl;
		SendData(&view->afBody, sizeof(view->afBody));
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
    return true;
}
TGLView::TGLView(bool b):bSender(b){
	bDraw = true;
	bDrawCollision = true;
	bDrawNormal = true;
	fov_v = 30.0f;
	//	アバターの設定
	radius = 0.5f;
	afBody = Affinef(0, 0, 10);
	afBody.LookAtGL(Vec3f(0, 0, 0), Vec3f(0,1,0));

	//	プロジェクタ位置データのロード
	if (LoadSettings()) SetParam();

	//	通信の初期化
	cmdSocket.Register(new CmdEye(this));
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

	tableAngle = 0.0f;
	lastTableAngle = 0.0f;
	statusOfUser = 0.0f;
	lastStatusOfUser = 0.0f;
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
	
	Affinef afCam = (afProjector * afEye).inv();
	afCam.Rot() *= 10;
	glLoadMatrixf(afCam);
	glMultMatrixf(Affinef(Rad(-90), 'x'));
	WriteSphere();
//	glLoadMatrixf((afBody * afEye).inv());
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		(*it)->Draw();
//		(*it)->DrawGeometry();
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
	gluPerspective(fov_v, (float)(width)/(float)(height) , 0.1f , 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}
void TGLView::OnJoystick(unsigned int button, int x, int y, int z){
	std::cout << "B" << button << " x:" << x << " y:" << y << " z:" << z << std::endl;
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	switch(key){
	case '\x1b':
		Exit();
		break;
	case 'b':
		glutPostRedisplay();
		break;
	case 'c':
		bDrawCollision = !bDrawCollision;
		break;
	case 'C':
		hiStep.pSenCal = true; //キャリブレイション始め
		break;
	case 'd':
		bDraw = !bDraw;
		break;
	case 'E':
		hiStep.pSenCal = false; //キャリブレイション終了
		hiStep.PSenRange();
		hiStep.SetStatusOfUser(0);
		break;
	case 'f':
		glutFullScreen();
		break;
	case 'i':
		afBody = Affinef(0, 0, 10);
		afBody.LookAtGL(Vec3f(0, 0, 0), Vec3f(0,1,0));
		vel = Vec3f(0,0,0);
		dvel = Vec3f(0,0,0);
		break;
	case 'n':
		bDrawNormal = !bDrawNormal;
		break;
	case 's':
		bDraw = false;
		break;
	case 'w':
		glutReshapeWindow(400,300);
		break;
	default:
	break;
	}
}

void TGLView::OnMouse(Button button, bool on, int x, int y)
{
	mouseButton = button;
	mouseX = x;	mouseY = y;
	if(!on){
		mouseButton = NONE;
		if(!bSender) glutSetCursor(GLUT_CURSOR_NONE);
		else glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
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

	myinit();
	//	GLの初期化
	float	lmamb[] = {.7, .7, .7, 1},
			l0amb[] = {0, 0, 0, 1},
			l0dif[] = {1, 1, 1, 1},
			l0spc[] = {1, 1, 1, 1},
			l0pos[] = {0, 0, 10, 0};

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmamb);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
	if (!bSender) {
		glutFullScreen();
		glutSetCursor(GLUT_CURSOR_NONE);
	}
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
	if (specialKey[GLUT_KEY_LEFT]) afBody = afBody * Affinef(Rad(0.25), 'Y');
	if (specialKey[GLUT_KEY_RIGHT]) afBody = afBody * Affinef(Rad(-0.25), 'Y');
	if (specialKey[GLUT_KEY_PAGE_UP]) afBody = afBody * Affinef(Rad(0.25), 'X');
	if (specialKey[GLUT_KEY_PAGE_DOWN]) afBody = afBody * Affinef(Rad(-0.25), 'X');

/*
	if (bSender && bInitSuccess){
		//	ターンテーブル制御処理
	//	hiStep.Step();
	//	hiTurn.Step();
	//	hiStep.Print(std::cout);
		//	向きと速度の更新
		afBody.Rot() = Matrix3f(hiTurn.GetAngle(), 'y');
		Vec3f inputVel = -afBody.Ez() *  hiStep.GetVelocity();
		vel.X() = inputVel.X();
		vel.Z() = inputVel.Z();
	}
*/
	if (bSender && bInitSuccess){
		//	ターンテーブル制御処理
		float turnVel = hiTurn.GetTurnVel();
		hiStep.Step(turnVel);

		lastStatusOfUser = statusOfUser;
		statusOfUser = hiStep.GetStatusOfUser();

		std::cout << "Status of user = " << statusOfUser << std::endl;

		if(statusOfUser < 4){
			float inner = hiStep.GetInner();
			hiTurn.Step(inner);
		}else{
			hiTurn.Step(0);
		}			

		//	向きと速度の更新
		tableAngle = hiTurn.GetAngle();
		float dTableAngle = tableAngle - lastTableAngle;
		afBody.Rot() =  Matrix3f(dTableAngle, 'y') * afBody.Rot();
		lastTableAngle = tableAngle;

		if(statusOfUser < 5){
			Vec3f inputVel = -afBody.Ez() *  hiStep.GetVelocity() / 10;
			vel.X() = inputVel.X();
			vel.Z() = inputVel.Z();
		}else{
			float sideStepD = hiStep.GetSideStepDirection();
			Vec3f inputVelX = afBody.Ex() *  hiStep.GetVelocity() * cos(sideStepD);
			Vec3f inputVelZ = -afBody.Ez() *  hiStep.GetVelocity() * sin(sideStepD);

			vel.X() = inputVelX.X() + inputVelZ.X();

			if(inputVelZ.Z() + inputVelX.Z() < 0){
				vel.Z() = inputVelZ.Z() + inputVelX.Z();
			}
			else{
				vel.Z() = 0;
			}
		}
	}

/*  アクション放棄　2003.5.9 岩下
	if(statusOfUser != lastStatusOfUser && statusOfUser == 7){
		float jumpPower = 25.0f;//そのうちsumSensorの積分の関係式から出す。
		vel.Y() = jumpPower;
	}else if(statusOfUser != lastStatusOfUser && statusOfUser == 8){
		afBody.PosY() = afBody.PosY() - 5.0;
	}else if(statusOfUser != lastStatusOfUser && lastStatusOfUser == 8){
		afBody.PosY() = afBody.PosY() + 5.0;
	}
ここまで　アクション放棄 */

	//---------------------------------------------------------------
	//	速度の更新，重力もここで加える
	vel += dvel;
	dvel = Vec3f();
	vel.Y() -= gravity * dt;
	vel = 0.98f * vel;

	//---------------------------------------------------------------
	//	衝突判定を行い，反射の方向を求める．また，侵入をキャンセルする．
//	cpds.Detect((float*)afBody.Pos(), radius);		//	最近傍点を求める

	//	 衝突判定の結果を計算オブジェクトに通知
	const float epsilon = 1E-5;
	rnc.Clear();
	penCancel.Clear();
	//	衝突面の列挙
	if (floorHeight + radius - afBody.Pos().Y() > 0){
		rnc.Add(Vec3d(0,1,0));
		penCancel.Add(Vec3f(0, floorHeight + radius - afBody.Pos().Y(), 0), Vec3d(0,1,0));
	}
/*
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
*/
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
