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
using namespace std;

#define KEY_ESC -27
const float floorHeight = 0.0f;
//const float dt = 1.0f / 30.0f;
const float gravity = 9.8f;
const float elasticity = 0.8f;
//const float elasticity = 0.0f;
const float EYEOFFSET = 0.8f;

#ifdef _DEBUG
#endif //_DEBUG

// サウンドファイルの設定
#define SOUND_NUM_OF_FILES 4
char *sound_filename[] = { "Sound/Laser.WAV","Sound/Coin.WAV","Sound/tobiagaru.wav","Sound/water.wav" };

// プレー時間[秒]
#define TIMER_RESOLUTION 1000 //何msに一度タイムが減るか
#define PLAY_TIME 180 //ゲームオーバーまでのタイム

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

/*
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
*/
//TGLView::TGLView(bool b):bSender(b){
TGLView::TGLView(int n):nowMode(n){
	lastTableAngle = 0.0f;
	changedAngle = 0.0f;
	playerAngle = 0.0f;
	bDraw = true;
	bDrawCollision = true;
	bDrawNormal = true;
	birdsHight = 15;
	hitFlag = false;
	hitNode = NULL;
	bBox = false;
	showInnerData = false;

	switch (nowMode){
	case 0:
		bSender = false;
		birdsView = false;
		break;
	case 1:
		bSender = true;
		birdsView = false;
		break;
	case 2:
		bSender = false;
		birdsView = true;
		break;
	}

	fov_v = 40.0f; // 30.0f;
	screenNear = 0.1f;
	//screenFar = 1000.0f;
	//screenFar = 2000.0f;
	screenFar = 40000.0f;

	//	アバターの設定
	radius = 5.0f;
	//GLfloat height = -100;
	bodyHeight = 30;
	afBody = Affinef(0, bodyHeight, 10);
	//afBody.LookAtGL(Vec3f(0, bodyHeight, 0)/*視点の位置*/, Vec3f(0,-1,0)/*カメラの上方向*/);
	
	hand_radius = 1.0f;
	afLHand = Affinef(-5,0,-7); //追加：赤羽
	afRHand = Affinef(5,0,-7); //追加：赤羽

	//  自分の設定
//	VecMyself[0] = Vec4f();
//	VecMyself[1] = Vec4f(0.5, 0.0, 1.0);
//	VecMyself[2] = Vec4f(-0.5, 0.0, 1.0);
	VecMyself[0] = Vec3f(0.0, 0, -radius/2);
	VecMyself[1] = Vec3f( radius/2, 0, radius/2);
	VecMyself[2] = Vec3f(-radius/2, 0, radius/2);

	// test用の四角形
#ifdef _DEBUG
	VecTest[0] = Vec4f(0, 0, 0, 1.0);
	VecTest[1] = Vec4f(0, 0, 1, 1.0);
	VecTest[2] = Vec4f(1, 0, 1, 1.0);
	VecTest[3] = Vec4f(1, 0, 0, 1.0);
#endif //_DEBUG

	//	プロジェクタ位置データのロード
	//if (LoadSettings()) SetParam();

	//	通信の初期化
	//cmdSocket.Register(new CmdEye(this));
	//if (bSender) cmdSocket.Init(10100, 10000);
	//else cmdSocket.Init(10000, 10100);
	
	//	ターンテーブルの初期化
	bInitSuccess = false;
	if (bSender){
//		dev.RPool().Register(new DRTokin3D);
		dev.RPool().Register(new DRAdapio(10));
		dev.Init();
		std::cout << dev;
		std::cout << "Initialize Start!!!!!!" << std::endl;
		bInitSuccess = hiTurn.Init(dev);
		bInitSuccess = bInitSuccess && hiStep.Init(dev);
	}

	// cylinder -->>
	pio.Init(0x00,PORT0,PORT_Output);
	// <<-- cylinder

	// レーザーポインタ -->>
	udpRecv = new CUdpRecv(10000,200); // Port:10000 , 200msに一度受信
#ifdef USE_LASER_POINTER
	udpRecv->Start();
#endif
	laserX = 0;
	laserY = 0;
	pLaserNode = NULL;
	// <<-- レーザーポインタ

#ifdef _KATSU_TEST_APP
	// seesaw -->>

	for(int i=0;i<8;i++){
		aSeesaw[i].InitInstance(
			-1.05,0.5,1.2, //(x,y,z)
			2.0,0.05,4.0); //size
	}
	// <<--seesaw
#endif //_KATSU_TEST_APP

//#ifdef _OKADA_TEST_APP
	// MoveFloor -->>
	aMoveFloor.InitInstance(
		593, 43.1, -32.3, 			//(x,y,z)	  場所
		50, 5.0, 70,       //(w,h,d)	  大きさ(h>=0.2:すり抜けてしまうため）
		0.0f, 0.0f, -204, 200);    //(mx,my,mz,T)動く幅, 片道にかかる時間
	bMoveFloor.InitInstance(
		633, 43.1, -178, 			//(x,y,z)	  場所
		70, 5.0, 50,		    //(w,h,d)	  大きさ(h>=0.2:すり抜けてしまうため）
		165.0f, 0.0f, 0.0f, 300);    //(mx,my,mz,T)動く幅, 片道にかかる時間
	cMoveFloor.InitInstance(
		850, 43.1, -139,			//(x,y,z)	  場所
		71, 5.0, 65,		    //(w,h,d)	  大きさ
		0.0f, 34.5f, 0.0f, 300);    //(mx,my,mz,T)動く幅, 片道にかかる時間
	// <<--MoveFloor
//#endif //_OKADA_TEST_APP

//#ifdef _OKADA_TEST_APP
	// JumpZone -->>
	aJumpZone.InitInstance(
		280, 53, 102,				//(x,y,z)	  場所（始点）
		40, 0, 20,					//(w,h,d)	  大きさ(基本的に高さ h=0 )
		10);						//(jumpDist)  飛ぶ距離＆高さ
	bJumpZone.InitInstance(
		358, 67, 240,				//(x,y,z)	  場所（始点）
		30, 0, 20,					//(w,h,d)	  大きさ(基本的に高さ h=0 )
		10);						//(jumpDist)  飛ぶ距離＆高さ
	cJumpZone.InitInstance(
		224, 67, 193,				//(x,y,z)	  場所（始点）
		25, 0, 30,					//(w,h,d)	  大きさ(基本的に高さ h=0 )
		10);						//(jumpDist)  飛ぶ距離＆高さ
	dJumpZone.InitInstance(
		218, 67, 308,				//(x,y,z)	  場所（始点）
		25, 0, 30,					//(w,h,d)	  大きさ(基本的に高さ h=0 )
		10);						//(jumpDist)  飛ぶ距離＆高さ
	eJumpZone.InitInstance(
		320, 67, 317,				//(x,y,z)	  場所（始点）
		40, 0, 25,					//(w,h,d)	  大きさ(基本的に高さ h=0 )
		10);						//(jumpDist)  飛ぶ距離＆高さ
	fJumpZone.InitInstance(
		376, 86, 392,				//(x,y,z)	  場所（始点）
		40, 0, 25,					//(w,h,d)	  大きさ(基本的に高さ h=0 )
		10);						//(jumpDist)  飛ぶ距離＆高さ
	// <<-- JumpZone
//#endif //_OKADA_TEST_APP


	// Timerの初期化
	timer_val = 180; //180秒からスタート。

	// Vashの初期化
	vash_val = 100; //100vashからスタート

	strcpy(GameCommentStr,"Game Over!");
	this->isGameOver = false;

	GameTimerSize = 10; //ゲーム１ステップ間隔

	isSpotLight = false; // スポットライトをオフにする。
}

/*
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
*/

bool TGLView::OnTimer(){
	/**/
/*
	// timer test -->>
	static count = 0;
	if(count == GameTimerSize){ //GameTimerSize = 10
		this->timer_val--;
		if(timer_val < 0){
			this->isGameOver = true; // game over
		}
		count=0;
	}
	else{
		count++;
	}
	// <<-- timer test
*/	
	Step();
	return true;
}
void TGLView::OnIdle(){
	glutPostRedisplay();
	Sleep(10);
}
//using namespace std;

void TGLView::OnDraw(){
	glClearColor(clearColor.X(), clearColor.Y(), clearColor.Z(), 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 全体の光源の設定 -->>
	//this->LightSetting(); // 光源の設定 : ここで実行すると、座標に固定した光源になる。
	// <<--全体の光源の設定

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	// アバターのスポットライトの on off -->>
	if(isSpotLight){
#ifdef _DEBUG
		std::cout << "Now Lighting...." << std::endl;
#endif //_DEBUG
		glEnable(GL_LIGHT5);
	}
	else{
		glDisable(GL_LIGHT5);
	}
	// <<-- アバターのスポットライトの on off

	glLoadIdentity();

	//static Affinef R180(Rad(180),'y'); // y軸周りに180度回転するアフィン行列

	float maxHeight = -999;
	/*
	Affinef afProjector = afBody;// * R180 * R * R180;
	
	Affinef afCam = afProjector;// * afEye; //afEye : 今の場合常に単位行列
	*/
	afCam = afBody;
	//cout <<"afBody:"<< afBody.Pos() << endl;
	Affinef afTmpB = afCam;

	//afCam =  Affinef(Rad(-30),'y')*afCam; 
	//afCam =  Affinef(0,0,0)*afCam;
	afCam =  afCam * Affinef(0,3,0); // 目線の高さ

	afTmpB.LookAtGL(afTmpB.Pos() + Vec3f(0,0,10));

	afBird = Affinef(0, birdsHight, 0) * afTmpB * Affinef(Rad(-90), 'x');	//  鳥瞰

	// 移動カメラの設定
	if (birdsView){
		glLoadMatrixf(afBird.inv());
	}else{
		glLoadMatrixf(afCam.inv());
	}
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		(*it)->Draw(); // wrlファイル内のオブジェクトの描画
//		(*it)->DrawGeometry();
	}


	//

	//

///////////////////////////////////////////////////////////////////////////////
// 追加のオブジェクトのエリアの開始
// この領域に、独自に動く物体などの描画関数を呼び出す。

#ifdef _KATSU_TEST_APP
	// Seesaw -->>
	glPushMatrix();
	aSeesaw.OnDraw(afBody.Pos().X(),afBody.Pos().Y(),afBody.Pos().Z());
	glPopMatrix();
	// <<-- Seesaw
#endif //_KATSU_TEST_APP

//#ifdef _OKADA_TEST_APP
	// MoveFloor -->>	
	bGravity = true;	
	glPushMatrix();
	aMoveFloor.OnDraw(&afBody.Pos().X(),&afBody.Pos().Y(),&afBody.Pos().Z(),&bGravity);	
	glPopMatrix();
	glPushMatrix();
	bMoveFloor.OnDraw(&afBody.Pos().X(),&afBody.Pos().Y(),&afBody.Pos().Z(),&bGravity);	
	glPopMatrix();
	glPushMatrix();
	cMoveFloor.OnDraw(&afBody.Pos().X(),&afBody.Pos().Y(),&afBody.Pos().Z(),&bGravity);	
	glPopMatrix();
	// <<-- MoveFloor
//#endif //_OKADA_TEST_APP

	// JumpZone -->>
	// 加速度の設定
	float dt = float(dwTime - dwLastTime) / 1000;
	glPushMatrix();
	aJumpZone.OnDraw(&afBody, &vel, dt);
	bJumpZone.OnDraw(&afBody, &vel, dt);
	cJumpZone.OnDraw(&afBody, &vel, dt);
	dJumpZone.OnDraw(&afBody, &vel, dt);
	eJumpZone.OnDraw(&afBody, &vel, dt);
	fJumpZone.OnDraw(&afBody, &vel, dt);
	gJumpZone.OnDraw(&afBody, &vel, dt);
	glPopMatrix();
/*
#ifdef _OKADA_TEST_APP
	std::cout << afBody.Pos() << std::endl;
#endif //_OKADA_TEST_APP
*/
	// <<-- JumpZone


//　追加のオブジェクトのエリアの終わり
///////////////////////////////////////////////////////////////////////////////


	// 各種情報の表示
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	// 池に落ちたときの処理
	if(afBody.Pos().Y() < 40 && !bFallingPond) {
		PlaySound( SOUND_WATER );
		bFallingPond = true;
	}
	if(afBody.Pos().Y() < 20) {
		FallPond();
	}

	// 文字列描画テスト -->>
	//glMultMatrixf(afBody*Affinef(Rad(90),'X'));
	if(isGameOver){
		this->GameOver();
	}
	else if(isGameStart){
		this->GameStart();
	}
	else{
		this->DrawVashVal();
	}
	// <<-- 文字列描画テスト
	//

	//
	// レーザーポインタ位置表示 -->>
	if( bDrawCollision ){
		glColor3f(0,1,1);
		Affinef afPoint;
		glBegin(GL_LINES);
			afPoint = afCam * afLaser * Affinef(0.002,0.002,0);
			glVertex3fv(afPoint.Pos());	
			afPoint = afCam * afLaser * Affinef(-0.002,-0.002,0);
			glVertex3fv(afPoint.Pos());	
			afPoint = afCam * afLaser * Affinef(0.002,-0.002,0);
			glVertex3fv(afPoint.Pos());	
			afPoint = afCam * afLaser * Affinef(-0.002,0.002,0);
			glVertex3fv(afPoint.Pos());	
		glEnd();
	}
	// <<-- レーザーポインタ位置表示
	//

	//
	/*
	glColor3f(1,0,0);
	glBegin(GL_LINES);
		glVertex3fv(afBody.Pos() + afLHand.Pos());	
		glVertex3fv(afBody.Pos() + afRHand.Pos());	
	glEnd();
	*/

#ifdef _SPIDAR_TEST
	Affinef afPoint;
	glBegin(GL_LINES);
	glColor3f(1,0,0);
		glVertex3fv(afBody.Pos());	
		afPoint = afBody * afLHand;
		glVertex3fv(afPoint.Pos());	

	glColor3f(0,0,1);
		glVertex3fv(afBody.Pos());	
		afPoint = afBody * afRHand;
		glVertex3fv(afPoint.Pos());	
	glEnd();
#endif //_SPIDAR_TEST
	//

	// 自分の位置表示
	glPushMatrix();
	glMultMatrixf(afBody);

	glColor3f(1,1,0);
	if(birdsView){
		glBegin(GL_LINE_LOOP);
			glVertex3fv(VecMyself[0]);
			glVertex3fv(VecMyself[1]);
			glVertex3fv(VecMyself[2]);
		glEnd();
	}

	//iwashita begin
	if(showInnerData){
		//glPushMatrix(); // 追加
		int nSensor;
		nSensor = hiStep.GetNSENSOR();
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
			for (int j = 0; j < nSensor; j++) {
				VecStartPosition[j] = Vec3f(0.0, 0.0, -1.6 + j * 0.2);
				VecSensor[j] = Vec3f(hiStep.GetSensorVoltage(j), 0.0, -1.6 + j * 0.2);
			}
/*
			VecStartPosition[0] = Vec3f(0.0, 0.0, -1.6);
			VecSensor[0] = Vec3f(hiStep.GetSensorVoltage(0), 0.0, -1.6);
			VecStartPosition[1] = Vec3f(0.0, 0.0, -1.4);
			VecSensor[1] = Vec3f(hiStep.GetSensorVoltage(1), 0.0, -1.4);
			VecStartPosition[2] = Vec3f(0.0, 0.0, -1.2);
			VecSensor[2] = Vec3f(hiStep.GetSensorVoltage(2), 0.0, -1.2);
			VecStartPosition[3] = Vec3f(0.0, 0.0, -1.0);
			VecSensor[3] = Vec3f(hiStep.GetSensorVoltage(3), 0.0, -1.0);
*/
			for (int j = 0; j < nSensor; j++){
				glVertex3fv(VecStartPosition[j]);
				glVertex3fv(VecSensor[j]);
			}
		glEnd();
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);

		//glPopMatrix();
//		glEnable(GL_DEPTH_TEST);
//		glEnable(GL_LIGHTING);
		//glPopMatrix();// 追加
	}		
	//iwashita end
	
#ifdef _DEBUG
	// テスト用四角形の表示
	glColor3f(1, 1, 0);
	glBegin(GL_LINE_LOOP);
	for(int m = 0; m < 4; m++)
		glVertex3fv(VecTest[m]);
	glEnd();
#endif //_DEBUG
	glPopMatrix();
	//

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	///////////////////////////////////////////////////////////////////////////
	// 意味のよくわからない点をうっている？-->>
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
		glVertex3fv(worldNear);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_POINTS);
		glVertex3fv(worldFar);
	glEnd();

	glBegin(GL_LINES);
		glVertex3fv(worldNear);
		glVertex3fv(worldFar);
	glEnd();
	glColor3f(0, 1, 1);
	glBegin(GL_POINTS);
		glVertex3fv(collisionPoint);
	glEnd();
	// <<--意味のよくわからない点をうっている？
	///////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	if(bBox){
		//  すべてのバウンディングボックスの表示
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			if(maxHeight < (*it)->VecObjectBox[0].Y())
				maxHeight = (*it)->VecObjectBox[0].Y();
		}
		//  高さで色分け	
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			glColor3f(((*it)->VecObjectBox[0].Y()/maxHeight), 1, ((*it)->VecObjectBox[0].Y()/maxHeight));
			glBegin(GL_LINE_LOOP);
			for (int m = 0; m < 4; m++)
				glVertex3fv((*it)->VecObjectBox[m]);
			glEnd();
		}
		//  全て同じ色
/*		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			glColor3f(0, 0, 1);
			glBegin(GL_LINE_LOOP);
			for (int m = 0; m < 4; m++)
				glVertex3fv((*it)->VecObjectBox[m]);
			glEnd();
		}*/
	}

	//  ヒットしたバウンディングボックスの表示
	if (hitNode != NULL){
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3f(0.5, 0.5, 1.0);
		glBegin(GL_LINE_LOOP); //GL_QUADS);
		Vec3f vtxs[] = {
			Vec3f(-1000, 0, -1000),	
			Vec3f( 1000, 0, -1000),	
			Vec3f( 1000, 0,  1000),	
			Vec3f(-1000, 0, 1000),	
		};
		for(int i = 0; i < 4; i++){
			glVertex3fv(hitNode->VecObjectBox[i]*0.3);
//			glVertex3fv(vtxs[i]*0.1);
		}
		glEnd();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
#endif //_DEBUG

	if (bDrawCollision){
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		cpds.Draw();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}

#ifdef _DEBUG
	if (bDrawNormal){
		//glEnd();
		glLoadIdentity();
		Vec3f pos(1,1,-8);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3f(1,0,0);
		
		for(int i=1; i<rnc.normals.size(); ++i){
			glBegin(GL_LINES);
			glVertex3fv(pos);
			glVertex3fv(pos + rnc.normals[i].unit());
			glEnd();
		}
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
#endif //_DEBUG

	Flip();
}

void TGLView::OnSize(int width, int height){
	glViewport(0, 0, width, height);	// window 全体を viewport にする
	glMatrixMode(GL_PROJECTION);	// 透視変換行列の設定
	glLoadIdentity();	// 変換行列の初期化
	
	// 遠近法のプロジェクション・マトリックスをセットアップします。
	gluPerspective(fov_v, (float)(width)/(float)(height) , screenNear , screenFar);
	
	glMatrixMode(GL_MODELVIEW);	//モデルビュー変換行列の設定
	windowWidth = width;
	windowHeight = height;
}

/*
void TGLView::OnJoystick(unsigned int button, int x, int y, int z){
	std::cout << "B" << button << " x:" << x << " y:" << y << " z:" << z << std::endl;
}
*/

void TGLView::OnKeyboard(int key, int x, int y){
	switch(key){
	case '\x1b':
		Exit();
		break;
	case '3':
		hiStep.SetThreshold( 0.1f );
		break;
	case '4':
		hiStep.SetThreshold( 0.3f );
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
	case 'a':
		isSpotLight = !isSpotLight;
		break;
	case 'b':
		birdsView = !birdsView;
		glutPostRedisplay();
		break;
	case 'c':
		hiTurn.Reset();
		break;
	case 'C':
		hiStep.beforeCalib();
		hiStep.pSenCal = true; //キャリブレイション始め
		break;
	case 'd':
		bDraw = !bDraw;
		break;
	case 'D':
		bDraw = false;
		break;
	case 'e':
		this->isGameOver = true;
		break;
	case 'E':
		hiStep.pSenCal = false; //キャリブレイション終了
		hiStep.PSenRange();
		hiStep.ValueOutput();
		break;
	case 'f':
		glutFullScreen();
		break;
	case 'g':
		this->isGameStart = true;
		this->isGameOver = false;
		playerAngle = 0.0f;
		break;
	case 'G':
		playerAngle += M_PI;
		if(playerAngle > M_PI) { playerAngle -= M_PI * 2; }
		if(playerAngle < -M_PI){ playerAngle += M_PI * 2; }
		break;
	case 'h':
		bDrawCollision = !bDrawCollision;
		break;
	case 'H':
		showInnerData = !showInnerData;
		break;
	case 'j':
		afBody.Pos().Y() += 10;
		break;
	case 'l':
		birdsHight -= 10;
		break;
	case 'm':
		bBox = !bBox;
		glutPostRedisplay();
		break;
	case 'n':
		bDrawNormal = !bDrawNormal;
		break;
	case 'o':
		if(hiStep.GetPitchParam() > 1){
			hiStep.SetPitchParam(hiStep.GetPitchParam() - 1);
		}
		break;
	case 'O':
		if(hiStep.GetPitchParam() < 15){
			hiStep.SetPitchParam(hiStep.GetPitchParam() + 1);
		}
		break;
	case 's':
		hiTurn.Stop();
		break;
	case 'S':
		hiTurn.Start();
		break;
	case 'u':
		birdsHight += 10;
		break;
	case 'w':
		glutReshapeWindow(400,300);
		break;
	case 'z':
		LaserAttack();
		break;
	default:
	break;
	}
}

void TGLView::OnMouse(Button button, bool state, int x, int y){
#ifdef _KATSU_TEST_APP
  switch (button) {
  //case GLUT_LEFT_BUTTON:
  case LEFT:
	  aSeesaw.r = 15.0;
	  Invalidate();			//	描画要求
    break;
  //case GLUT_MIDDLE_BUTTON:
  case MIDDLE:
	  aSeesaw.r = 0.0;
	  Invalidate();			//	描画要求
    break;
  //case GLUT_RIGHT_BUTTON:
  case RIGHT:
	  aSeesaw.r = -15.0;
	  Invalidate();			//	描画要求
	  //glutIdleFunc(EventHandlerIdle);
    break;
  default:
    break;
  }
#endif //_KATSU_TEST_APP

  // マウスでもレーザーポインタ座標を指定できるように
  laserX = x;
  laserY = y;
/*
	
	
	mouseButton = button;
	mouseX = (2 * x / float(windowWidth) - 1);
	mouseY = (-2 * y / float(windowHeight) + 1);
	if(!on){
		mouseButton = NONE;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case LEFT:
		if(on){
			GLdouble modelView[16], projection[16];
			GLint viewPort[4];
			
			glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
			glGetDoublev(GL_PROJECTION_MATRIX, projection);
			glGetIntegerv(GL_VIEWPORT, viewPort);

			Affined afModel((Affined&)modelView);
			Affined afProjection((Affined&)projection);

	//		cout << "(mouseX, mouseY, near) = (" << mouseX << ", " << mouseY << ", -0.1)" << endl;
	//		cout << "(mouseX, mouseY, far) = (" << mouseX << ", " << mouseY << ", -10.0)" << endl;
	//		Vec4f beforeNear(afProjection.inv() * Vec4f(0, 0, -0.1, screenNear));
	//		Vec4f beforeFar(afProjection.inv() * Vec4f(0, 0, -10.0, screenNear));
	//		cout << "afProjection.inv= " << afProjection.inv() << endl;
	//		cout << "beforeNear = " << beforeNear << endl;
	//		cout << "beforeFar = " << beforeFar << endl;

			worldNear = (afProjection * afBird.inv()).inv() * (Vec4f(mouseX, mouseY, -0.1, 1) * screenNear);
			worldFar = (afProjection * afBird.inv()).inv() * (Vec4f(mouseX, mouseY, -10.0, 1) * screenNear);
			worldNear = worldNear / worldNear.W();
			worldFar = worldFar / worldFar.W();

			cout << "worldNear = " << worldNear << endl;
			cout << "worldFar = " << worldFar << endl;
			cout << CollisionMouse() << endl;

			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			first[0] = x;
			first[1] = y;
			second[0] = x;
			second[1] = y;
		break;
		}
	case MIDDLE:
		if(on)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	}
	*/
}


void TGLView::OnMouseMove(bool on, int x, int y)
{
#ifndef USE_LASER_POINTER
	laserX = x;
	laserY = y;
	laserCaptured = on;
#endif // <<-- USE_LASER_POINTER
	/*
	switch(mouseButton){
	case LEFT:
		if ((second[0] != x) || (second[1] != y)){
			first[0] = second[0];
			first[1] = second[1];
			second[0] = x;
			second[1] = y;
			Vec3f mtmp(second[0] - first[0], 0, second[1] - first[1]);
			mouseVector = 0.9 * mouseVector + mtmp;
			Vec3f m(x/1024.0f - 0.5f, 0, y/768.0f - 0.5f);
			afEye.Pos() = m * 410 + Vec3f(0, afEye.Pos().Y(), 0);
			afEye.LookAtGL(afEye.Pos() + mouseVector);
		}
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
	*/
}

void TGLView::OnOpen(void){
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmamb);
	
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
	
	this->LightSetting(); // 光源の設定 : ここで実行すると、カメラとともに動く光源になってしまう。

	// アバターと一緒に動くスポットライトの設定
	this->SpotLightSetting();

	glEnable(GL_NORMALIZE);

	if (!bSender) glutFullScreen();

	SetTimer(33, &TGLView::OnTimer);
	bDrawNormal = false;

	//----- DirectSoundの初期化
	// 現在アクティブのWindowを得る。
	HWND hwnd = GetForegroundWindow(); 

	char windowTitle[30];
	GetWindowText( hwnd,windowTitle,30 ); // Windowタイトルを得る
	//if( !strcmp( windowTitle,"MultiWalk" ) ) hwnd = NULL; //このソフトかを確認

#ifdef USE_SOUND
	easyds = new CEasyDS( hwnd,SOUND_NUM_OF_FILES );
	// DirectSoundが使用可能な場合はWaveファイルを読み込む
	if( easyds->IsAvailable() ){
		std::cout << "DirectSound OK. WindowTitle:" << windowTitle << std::endl;
		for( int i=0; i<SOUND_NUM_OF_FILES; i++ ){
			if( easyds->Waves(i)->LoadFromFile( sound_filename[i] ) ){
				easyds->Waves(i)->SetSynchronous(3);
				std::cout << "Load Wave File:" << sound_filename[i] << std::endl;
			}else{
				std::cout << easyds->Waves(i)->ErrorMsg() << sound_filename[i] << std::endl;
			}
		}
	}
#endif

	dwTime = GetTickCount();
}

typedef map<DtObjectRef, Object *> ObjectList;
extern ObjectList objectList;
void TGLView::InitCollisionDetector(){
	//	SOLIDの初期化
	int t_count=0;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		t_count++;
		TGeoNode::TVertices& vtxs = (*it)->vertices;
		//(*it)->p_afBody = &afBody; //TGeoNodes でafBodyを参照するために追加
		(*it)->p_TGLView = this; //TGeoNodes でTGLViewを参照するために追加
		if (!(*it)->faces.size()) continue;

		///////////////////////////////////////////////////////////////////////
		// 衝突判定を行わない物体を指定する。-->>
		// 物体名の先頭に "nc_" がついているものは、衝突判定から除外する。
		char str1,str2,str3;
		str1 = (*it)->Name().c_str()[0];
		str2 = (*it)->Name().c_str()[1];
		str3 = (*it)->Name().c_str()[2];
		if((str1 == 'n' || str1 == 'N') &&
			(str2 == 'c' || str2 == 'C') &&
			(str3 == '_')
			)
		{
#ifdef _DEBUG_KATSU
			printf("衝突判定をしない物体：name = %s faces.size()=%d  \n",(*it)->Name().c_str(),(*it)->faces.size());  //?? test
#endif //_DEBUG_KATSU

			// レーザーポインタオブジェクト抜き出し -->>
			if( (*it)->Name().c_str()[3] == 'p' && (*it)->Name().c_str()[4] == '_' ){
				pLaserNode = (TGeoNodePointer*)(TGeoNode*)(*it);
			}
			// <<-- レーザーポインタオブジェクト抜き出し

			continue;
		}
		//if(strcmp( "Box02",(*it)->Name().c_str())==0) continue;
		// <<-- 衝突判定を行わない物体を指定する。
		///////////////////////////////////////////////////////////////////////
		
		DtShapeRef shape = dtNewComplexShape();
		for(int i=0; i<(*it)->faces.size(); ++i){
			dtBegin(DT_POLYGON);
			TGeoNode::TFace& face = (*it)->faces[i];
			for(int v=0; v<face.size(); ++v){
				dtVertex(vtxs[face[v]].X(), vtxs[face[v]].Y(), vtxs[face[v]].Z());
			}
#ifdef _DEBUG_KATSU
			printf("%d:face.size()=%d  \n",i,face.size());  //?? test
#endif //_DEBUG_KATSU
			dtEnd();
		}

#ifdef _DEBUG_KATSU
		printf("name = %s faces.size()=%d  \n",(*it)->Name().c_str(),(*it)->faces.size());  //?? test
#endif //_DEBUG_KATSU
		dtEndComplexShape();
		dtCreateObject(*it, shape);
		(*it)->solid = objectList[*it]; // 物体を移動するために物体への参照をTGLNodeで保持するために追加。
		dtLoadMatrixf((*it)->GetPosture());
	}
#ifdef _DEBUG_KATSU
	printf("SOLIDの初期化:%d [TGLView::InitCollisionDetector()]\n",t_count);  //?? test
#endif //_DEBUG_KATSU

///////////////////////////////////////////////////////////////////////////////
//#define _DEBUG_KATSU
#ifdef _DEBUG_KATSU
	t_count = 0; //デバック用変数
	int add_count = 0; //デバック用変数
#endif //_DEBUG_KATSU

	//	チェックすべきオブジェクトの追加
	for (ObjectList::const_iterator i = objectList.begin(); i != objectList.end(); ++i) {

#ifdef _DEBUG_KATSU
		t_count++;
#endif //_DEBUG_KATSU

		if (i->first == &afBody) continue;
		TClosestPointDetector cpd(i->second);
		cpds.push_back(cpd);
#ifdef _DEBUG_KATSU
		add_count++;
#endif //_DEBUG_KATSU
	}
#ifdef _DEBUG_KATSU
	printf("チェックすべきオブジェクトの個数:%d(%d)(%d) [TGLView::InitCollisionDetector()]\n"
		,add_count,
		t_count,
		cpds.size());  //?? test
#endif //_DEBUG_KATSU
//#undef _DEBUG_KATSU
///////////////////////////////////////////////////////////////////////////////

}

void TGLView::Step(){
	//time distance 計測
	dwLastTime = dwTime;
	dwTime = GetTickCount();
	float dt = float(dwTime - dwLastTime) / 1000;

	//---------------------------------------------------------------
	// タイマーの更新
	timer_val = PLAY_TIME - ( dwTime - timer_start ) / TIMER_RESOLUTION;
	if(timer_val < 0){
		this->isGameOver = true; // game over
	}

	//---------------------------------------------------------------
	//	位置の更新．前回の更新で侵入する向きの速度はなくなっている．


//	cout << "afBody:" << afBody.Pos() << endl;
	
	//---------------------------------------------------------------
	//	キー入力
	float accel = gravity * dt;
	bool isControl = GetAsyncKeyState(VK_CONTROL) < 0; //Controlキーが押されているときは早くする
	bool isShift = GetAsyncKeyState(VK_SHIFT) < 0; //Shiftキーが押されているときは早くする
	if (keyboard[' ']) dvel.Y() += 2;
	if (specialKey[GLUT_KEY_UP]) dvel -= afBody.Ez() * accel * 5 * (isControl ? 4 : 1 ) * (isShift ? 3 : 1 ); // ちょっとテストのために早くしてみました
	if (specialKey[GLUT_KEY_DOWN]) dvel += afBody.Ez() * accel * 5 * (isControl ? 4 : 1 ) * (isShift ? 3 : 1 ); // ちょっとテストのために早くしてみました
	if (specialKey[GLUT_KEY_HOME]) vel = Vec3f();
	if (specialKey[GLUT_KEY_LEFT]){
		afBody = afBody * Affinef(Rad( (isControl ? 10 : 5) ), 'Y');
	}
	if (specialKey[GLUT_KEY_RIGHT]){
		afBody = afBody * Affinef(Rad(- (isControl ? 10 :5 ) ), 'Y');
	}
	if (bSender && bInitSuccess){
		//	ターンテーブル制御処理/
		float turnVel = hiTurn.GetTurnVel();
		hiStep.Step(turnVel);
		hiTurn.Step(hiStep.inner);
		//hiStep.Print(std::cout);
		//	向きと速度の更新
		float tableAngle = hiTurn.GetAngle();
		changedAngle = tableAngle - lastTableAngle;
		afBody.Rot() =  Matrix3f(changedAngle, 'y') * afBody.Rot();
		lastTableAngle = tableAngle;
		Vec3f inputVel = -afBody.Ez() *  hiStep.GetVelocity();
		vel.X() = inputVel.X();
		vel.Z() = inputVel.Z();
	}

	//	速度の更新，重力もここで加える
		vel += dvel;
		dvel = Vec3f();	

	//
	const float epsilon = 1E-5;
	Affinef afPoint;
	Vec3f lVecSum,rVecSum,bodyVecSum;

	// 右手
	afPoint = afBody * afRHand;
	cpds.Detect((float*)afPoint.Pos(), hand_radius);		//	右手の最近傍点を求める
	for(TClosestPointDetectors::iterator it = cpds.begin(); it != cpds.end(); ++it){
		for(TClosestPoints::iterator itp = it->points.begin(); itp != it->points.end(); ++itp){
			Vec3d cp = (Vec3d&)itp->point;
			Vec3d n = afBody * afRHand.Pos() - cp;
			//Vec3d n = afBody.Pos() - cp;
			if (n.square() > Square(epsilon)){
				//n = n.unit();
				rVecSum+=n;
			}
		}
	}
	cout << "rVecSum" << rVecSum << endl;
	// 左手
	afPoint = afBody * afLHand;
	cpds.Detect((float*)afPoint.Pos(), hand_radius);		//	左手の最近傍点を求める
	for(TClosestPointDetectors::iterator it = cpds.begin(); it != cpds.end(); ++it){
		for(TClosestPoints::iterator itp = it->points.begin(); itp != it->points.end(); ++itp){
			Vec3d cp = (Vec3d&)itp->point;
			Vec3d n = afBody * afLHand.Pos() - cp;
			//Vec3d n = afBody.Pos() - cp;
			if (n.square() > Square(epsilon)){
				//n = n.unit();
				lVecSum+=n;
			}
		}
	}
	cout << "lVecSum" << lVecSum << endl;
	//cpds.points.clear();
	//cpds.clear();

	//体
	cpds.Detect((float*)afBody.Pos(), radius);		//	最近傍点を求める
	for(TClosestPointDetectors::iterator it = cpds.begin(); it != cpds.end(); ++it){
		for(TClosestPoints::iterator itp = it->points.begin(); itp != it->points.end(); ++itp){
			Vec3d cp = (Vec3d&)itp->point;
			Vec3d n = afBody.Pos() - cp;
			if (n.square() > Square(epsilon)){
				//n = n.unit();
				bodyVecSum+=n;
			}
		}
	}
	cout << "bodyVecSum" << bodyVecSum << endl;

	bodyAccel = (50 * bodyVecSum  - vel*0.8)/40 + 1.2*gravity * Vec3f(0,-1,0);

	LHandAccel = (5 * lVecSum  - LHandVel*2);
	RHandAccel = (5 * rVecSum  - RHandVel*2);
	LHandVel *= 0.5;
	RHandVel *= 0.5;
	vel *= 0.5;


	afBody.Pos() += vel * dt;
	vel += bodyAccel * dt;

	afLHand.Pos() += LHandVel * dt;
	LHandVel += LHandAccel * dt;

	afRHand.Pos() += RHandVel * dt;
	RHandVel += RHandAccel * dt;
	if (bDraw) Invalidate(); //	描画要求
}

/*
bool TGLView::CollisionMouse(){
	Vec4f VecTemp[4];
	Vec4f c[4], n[4];
	bool firstComp = true;
	hitFlag = true;				//  
	float t;
	int countCheck = 0;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		for(int m = 0; m < 4; m++){
			VecTemp[m] = (*it)->VecObjectBox[m];
			cout << "VecTemp[" << m << "] = " << VecTemp[m] << endl;
		}
		cout << "(*it)->objectID" << (*it)->objectID << endl;
		Vec4f objectNormal = Vec3f(VecTemp[1] - VecTemp[0]) ^ Vec3f(VecTemp[3] - VecTemp[0]);
		cout << "objectNormal = " << objectNormal << endl;
		cout << "objectNormal.unit() = " << objectNormal.unit() << endl;
		
		t = objectNormal.unit() * (VecTemp[0] - worldNear) / (objectNormal.unit() * (worldFar - worldNear));
		cout << "t = " << t << endl;
		
		collisionPoint = worldNear + (worldFar - worldNear) * t;
		cout << "collisionPoint = " << collisionPoint << endl;
		/*
		c[0] = VecTemp[0] - collisionPoint;
		c[1] = VecTemp[1] - collisionPoint;
	 	c[2] = VecTemp[2] - collisionPoint;
		c[3] = VecTemp[3] - collisionPoint;
		
		n[0] = Vec3f(c[0]) ^ Vec3f(c[1]);
		n[1] = Vec3f(c[1]) ^ Vec3f(c[2]);
		n[2] = Vec3f(c[2]) ^ Vec3f(c[3]);
		n[3] = Vec3f(c[3]) ^ Vec3f(c[0]);

//		if((n[0].norm() == 0.0) || (n[1].norm() == 0.0) || (n[2].norm() == 0.0) || (n[3].norm() == 0.0))
//			hitFlag = true;
		if((objectNormal.unit() * n[0]) < 0.0){
			cout << "n0" << endl;
			hitFlag = false;
		}
		if((objectNormal.unit() * n[1]) < 0.0){
			cout << "n1" << endl;
			hitFlag = false;
		}
		if((objectNormal.unit() * n[2]) < 0.0){
			cout << "n2" << endl;
			hitFlag = false;
		}
		if((objectNormal.unit() * n[3]) < 0.0){
			cout << "n3" << endl;
			hitFlag = false;
		}
		if((VecTemp[0].X() < collisionPoint.X()) && (VecTemp[2].X() > collisionPoint.X()) 
			&& (VecTemp[0].Z() > collisionPoint.Z()) && (VecTemp[2].Z < collisionPoint.Z()))
			hitFlag = true;
		if(hitFlag){
			if(!firstComp){
				if(hitNode->VecObjectBox[0].Y() < (*it)->VecObjectBox[0].Y()){
					hitNode = *it;
					hitFlag = true;
					countCheck++;
	//				return true;
				}
			}else{
				hitNode = *it;
				firstComp = false;
				hitFlag = true;
				countCheck++;
//				return true;
			}
		}
	}
	if(&hitNode == NULL)
		return false;
	cout << endl;
	cout << "Hit!!" << endl;
	cout << "worldNear = " << worldNear << endl;
	cout << "worldFar = " << worldFar << endl;
	for(int m = 0; m < 4; m++){
		cout << "hitNode[" << m << "] = " << hitNode->VecObjectBox[m] << endl;
	}
	cout << "countCheck = " << countCheck << endl;
	return true;
}
*/
/*bool TGLView::CollisionMouse(){
	Vec4f VecTestTemp[4];
	Vec4f collisionPoint, c[4], n[4];
	float t, d;
	for(int m = 0; m < 4; m++){
		VecTestTemp[m] = afBody * VecTest[m];
		cout << "VecTestTemp[" << m << "] = " << VecTestTemp[m] << endl;
	}
	avatarNormal = Vec3f(VecTestTemp[1] - VecTestTemp[0]) ^ Vec3f(VecTestTemp[3] - VecTestTemp[0]);
	cout << "avatarNormal = " << avatarNormal << endl;
	cout << "avatarNormal.unit() = " << avatarNormal.unit() << endl;
	t = avatarNormal.unit() * (VecTestTemp[0] - worldNear) / (avatarNormal.unit() * (worldFar - worldNear));
	cout << "t = " << t << endl;
	collisionPoint = worldNear + (worldFar - worldNear) * t;
	cout << "collisionPoint = " << collisionPoint << endl;
	// 平面と点の表裏判定
	d = avatarNormal.unit() * (collisionPoint - VecTestTemp[0]);
	// d ) 0 なら表
	// d = 0 なら平面上
	// d < 0 なら裏
	cout << "d = " << d << endl;
	if (d == 0){
		c[0] = VecTestTemp[0] - collisionPoint;
		c[1] = VecTestTemp[1] - collisionPoint;
	 	c[2] = VecTestTemp[2] - collisionPoint;
		c[3] = VecTestTemp[3] - collisionPoint;
		
		n[0] = Vec3f(c[0]) ^ Vec3f(c[1]);
		n[1] = Vec3f(c[1]) ^ Vec3f(c[2]);
		n[2] = Vec3f(c[2]) ^ Vec3f(c[3]);
		n[3] = Vec3f(c[3]) ^ Vec3f(c[0]);

		if((n[0].norm() == 0.0) || (n[1].norm() == 0.0) || (n[2].norm() == 0.0) || (n[3].norm() == 0.0))
			return true;
		if((avatarNormal.unit() * n[0]) < 0.0){
			cout << "n0" << endl;
			return false;
		}
		if((avatarNormal.unit() * n[1]) < 0.0){
			cout << "n1" << endl;
			return false;
		}
		if((avatarNormal.unit() * n[2]) < 0.0){
			cout << "n2" << endl;
			return false;
		}
		if((avatarNormal.unit() * n[3]) < 0.0){
			cout << "n3" << endl;
			return false;
		}
		return true;
	}
	return false;
}*/

void TGLView::LightSetting()
{
	static GLfloat lmamb[] = {0.1, 0.1, 0.1, 1};

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);
	
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmamb);
	
	// light0 に関する設定 -->>
	static GLfloat light0amb[] = {0.1, 0.1, 0.1, 1};	//光のあたらない部分の色
	static GLfloat light0spc[] = {0.5, 0.5, 0.5, 1};	//光の直接あたる部分の輝度
	static GLfloat light0dif[] = {1, 1, 1, 1};			//光源そのものの色
	static GLfloat light0pos[] = {1, 1, 1, 0};			//光源の位置
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0amb);	//光のあたらない部分の色
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0spc);	//光の直接あたる部分の輝度
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0dif);	//光源そのものの色
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);	//照明の位置
	// <<-- light0 に関する設定

	// light1 に関する設定 -->>
	static GLfloat light1amb[] = {0.1, 0.1, 0.1, 1};	//光のあたらない部分の色
	static GLfloat light1spc[] = {0.5, 0.5, 0.5, 1};	//光の直接あたる部分の輝度
	static GLfloat light1dif[] = {1, 1, 1, 1};			//光源そのものの色
	static GLfloat light1pos[] = {-1, 1, 1, 0};			//光源の位置
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1amb);	//光のあたらない部分の色
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1spc);	//光の直接あたる部分の輝度
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1dif);	//光源そのものの色
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);	//照明の位置
	// <<-- light1 に関する設定

	// light2 に関する設定 -->>
	static GLfloat light2amb[] = {0.1, 0.1, 0.1, 1};	//光のあたらない部分の色
	static GLfloat light2spc[] = {0.5, 0.5, 0.5, 1};	//光の直接あたる部分の輝度
	static GLfloat light2dif[] = {1, 1, 1, 1};			//光源そのものの色
	static GLfloat light2pos[] = {-1, 1, -1, 0};			//光源の位置
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2amb);	//光のあたらない部分の色
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2spc);	//光の直接あたる部分の輝度
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2dif);	//光源そのものの色
	glLightfv(GL_LIGHT2, GL_POSITION, light2pos);	//照明の位置
	// <<-- light2 に関する設定

	// light3 に関する設定 -->>
	static GLfloat light3amb[] = {0.1, 0.1, 0.1, 1};	//光のあたらない部分の色
	static GLfloat light3spc[] = {0.5, 0.5, 0.5, 1};	//光の直接あたる部分の輝度
	static GLfloat light3dif[] = {1, 1, 1, 1};			//光源そのものの色
	static GLfloat light3pos[] = {1, 1, -1, 0};			//光源の位置
	glLightfv(GL_LIGHT3, GL_AMBIENT, light3amb);	//光のあたらない部分の色
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3spc);	//光の直接あたる部分の輝度
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3dif);	//光源そのものの色
	glLightfv(GL_LIGHT3, GL_POSITION, light3pos);	//照明の位置
	// <<-- light3 に関する設定
/*
	// light4 に関する設定 -->>
	static GLfloat light4amb[] = {0.0, 0.0, 0.1, 1};	//光のあたらない部分の色
	static GLfloat light4spc[] = {0.5, 0.5, 0.5, 1};	//光の直接あたる部分の輝度
	static GLfloat light4dif[] = {0.0, 0.0, 1.0, 1};			//光源そのものの色
	static GLfloat light4pos[] = {0, -1, 0, 0};			//光源の位置
	glLightfv(GL_LIGHT4, GL_AMBIENT, light4amb);	//光のあたらない部分の色
	glLightfv(GL_LIGHT4, GL_SPECULAR, light4spc);	//光の直接あたる部分の輝度
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light4dif);	//光源そのものの色
	glLightfv(GL_LIGHT4, GL_POSITION, light4pos);	//照明の位置
	// <<-- light4 に関する設定
*/
/*
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 /** 0.2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, l0dif);
*/

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	//glEnable(GL_LIGHT4);
	
}

void TGLView::SpotLightSetting()
{
	// アバターと一緒に動く光源
	//	GLの初期化
	float	l0amb[] = {0.1, 0.1, 0.1, 1}, //光のあたらない部分の色
			l0dif[] = {1, 0.7843, 0.5, 1},//光源そのものの色
			l0spc[] = {0.8, 0.8, 0.8, 1};//光の直接あたる部分の輝度

	//Vec3f lightPos(0,1,1);
	GLfloat spotDirection[] = {0,-0.1,-1};// 方向 スポットライトテスト
	GLfloat spotCutoff[] = {10.0f};// スポットライトテスト

	glLightfv(GL_LIGHT5, GL_AMBIENT, l0amb);	//光のあたらない部分の色
	glLightfv(GL_LIGHT5, GL_DIFFUSE, l0dif);	//光源そのものの色
	glLightfv(GL_LIGHT5, GL_SPECULAR, l0spc);	//光の直接あたる部分の輝度
	glLightfv(GL_LIGHT5, GL_POSITION,  afBody.Pos());	//照明の位置

	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION,1);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION,1);
	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION,1);

	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION,spotDirection);
	glLightfv(GL_LIGHT5, GL_SPOT_CUTOFF,spotCutoff);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT,5);
		
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .25);
}

void TGLView::AddVashVal(int val)
{
	this->vash_val += val;
}

void TGLView::GetVashChar()
{
	//itoa(this->vash_val,this->vash_char,10);
#ifndef _USE_NUM3D
	sprintf(this->vash_char,"%3d",this->vash_val);
#endif // !_USE_NUM3D
#ifdef _USE_NUM3D
	sprintf(this->vash_char,"%4d",this->vash_val);
#endif // _USE_NUM3D
#ifdef _DEBUG_KATSU
	printf("%d:%s\n",this->vash_val,this->vash_char);
#endif //_DEBUG_KATSU
	//return this->vash_char;
}

void TGLView::AddTimerVal(int val)
{
	this->timer_val += val;
}

void TGLView::GetTimerChar()
{
	//itoa(this->timer_val,this->timer_char,10);
	sprintf(this->timer_char,"%3d",this->timer_val);
#ifdef _DEBUG_KATSU
	printf("%d:%s\n",this->timer_val,this->timer_char);
#endif //_DEBUG_KATSU
	//return this->timer_char;

}


void TGLView::GameOver()
{
	static int count = 0;
	if(count >= 300){
		this->isGameOver = false;
		//this->isGameStart = true;
		count = 0;
		return;
	}
	count++;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	int x_disp,y_disp;
	if(GameCommentStr[0]=='Y'){
		x_disp = -700;
		y_disp = 100;
	}
	else{
		x_disp = -370;
		y_disp = 100;
	}
	const int str_size = 64;
	char str[str_size];
	
	glColor3f(0.5,0.0,0.0); //文字の色
	int j;
	for(j=0;j<5;j++){
		glPushMatrix();
		glMultMatrixf(afBody*Affinef(x_disp-j,y_disp+j, -1500));
		for(int i=0;i<strlen(GameCommentStr);i++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN,GameCommentStr[i]);
		}
		glPopMatrix();
	}
	glColor3f(1.0,0.0,0.0); //文字の色
	for(;j<7;j++){
		glPushMatrix();
		glMultMatrixf(afBody*Affinef(x_disp-j,y_disp+j, -1500));
		for(int i=0;i<strlen(GameCommentStr);i++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN,GameCommentStr[i]);
		}
		glPopMatrix();
	}

	x_disp = -370;
	y_disp = 100;
	glColor3f(0.0,0.0,0.5); //文字の色
	sprintf(str,"%3d vash",this->vash_val);
	for(j=0;j<7;j++){
		glPushMatrix();
		glMultMatrixf(afBody*Affinef(x_disp+100-j,y_disp-200+j, -1500));
		for(int i=0;i<strlen(str);i++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
		}
		glPopMatrix();
	}

	glColor3f(0.0,0.0,1.0); //文字の色
	for(;j<10;j++){
		glPushMatrix();
		glMultMatrixf(afBody*Affinef(x_disp+100-j,y_disp-200+j, -1500));
		for(int i=0;i<strlen(str);i++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
		}
		glPopMatrix();
	}


	glPushMatrix();
	glMultMatrixf(afBody*Affinef(x_disp-400,y_disp-500, -2000));

	//this->vash_val = 800;
	this->GetVashChar();
	if(this->vash_val >= 950){
		strcpy(str,"You are crazy!");
	}
	else if(this->vash_val >= 900){
		strcpy(str,"You are perfect!");
	}
	else{
		strcpy(str,"You need more training!");
	}
	
	glColor3f(1.0,1.0,0.0); //文字の色
	for(int i=0;i<strlen(str);i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	}
	glPopMatrix();
	glPushMatrix();
	glMultMatrixf(afBody*Affinef(x_disp-100,y_disp-650, -2000));

	this->GetVashChar();
	if(this->vash_val >= 950){
		strcpy(str,"Don't try again!");
	}
	else if(this->vash_val >= 900){
		strcpy(str,"please try again!");
	}
	else{
		strcpy(str,"try again!");
	}
	
	glColor3f(1.0,1.0,0.0); //文字の色
	for(int i=0;i<strlen(str);i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	}
	glPopMatrix();
}

void TGLView::GameStart()
{
	this->isGameOver = false;
	static int i = 0;
	if(i >= 100){
		i=0;
		this->isGameStart = false;
		return;
	}
	const int str_size = 64;
	char str[str_size];
	if(i==0){
		afBody = Affinef(0, bodyHeight, 10);
		//afBody.LookAtGL(Vec3f(0, bodyHeight, 0)/*視点の位置*/, Vec3f(0,-1,0)/*カメラの上方向*/);

		// ゲームアイテムの初期化
		for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
			(*it)->InitInstance(); //役割としては、ゲームが新しくなるので、コインをまた描画するようにする。など、
		}

	}
	i++;
		strcpy(GameCommentStr,"Game Over!");
		strcpy(str,"Let's get Vash!");

	this->timer_val = PLAY_TIME;
	this->timer_start = dwTime;
	this->vash_val = 100;
	sprintf(timer_char,"%3d",this->timer_val);
	sprintf(vash_char,"%4d",this->vash_val);
	bLastIsJump = false;
	bFallingPond = false;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	int x_disp,y_disp;
	x_disp = -450;
	y_disp = 100;
	
	glColor3f(0.5,0.5,0.0); //文字の色
	int j;
	for(j=0;j<5;j++){
		glPushMatrix();
		glMultMatrixf(afBody*Affinef(x_disp-j,y_disp+j, -1500));
		for(int i=0;i<strlen(str);i++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
		}
		glPopMatrix();
	}
	glColor3f(1.0,1.0,0.0); //文字の色
	glPushMatrix();
	glMultMatrixf(afBody*Affinef(x_disp-j,y_disp+j, -1500));
	for(int i=0;i<strlen(str);i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	}
	glPopMatrix();

}

TGLView::~TGLView()
{
	// レーザーポインタ -->>
	udpRecv->Stop();
	delete udpRecv;
	// <<-- レーザーポインタ

#ifdef USE_SOUND
	if( easyds ){
		delete easyds;
		easyds = NULL;
	}
#endif
}

void TGLView::DrawVashVal()
{
	GLint x_disp,y_disp;
	x_disp = 110;
	y_disp = 600;
	this->GetVashChar();
	this->GetTimerChar();
	int j;
	for(j=0;j<6;j++){
		glPushMatrix();
		//glMultMatrixf(afBody*Affinef(Rad(90),'X'));
		glMultMatrixf(afBody*Affinef(x_disp-j,y_disp-j,-2000));

		if(this->timer_val < 60){
			glColor3f(0.5,0,0);
		}
		else{
			glColor3f(0,0,0.5); //タイマーの色
		}
		for(int i=0;i<strlen(timer_char);i++){
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,timer_char[i]);
		}
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,' ');

		glColor3f(0.5,0.5,0); //バッシュの色
		for(int i=0;i<strlen(vash_char);i++){
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, vash_char[i]);
		}
		//glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'v');

		glPopMatrix();
	}
	
	glPushMatrix();
	//glMultMatrixf(afBody*Affinef(Rad(90),'X'));
	glMultMatrixf(afBody*Affinef(x_disp-j,y_disp-j,-2000));

	if(this->timer_val < 60){
		glColor3f(1,0,0);
	}
	else{
		glColor3f(0,0,1); //タイマーの色
	}
	for(int i=0;i<strlen(timer_char);i++){
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,timer_char[i]);
	}
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,' ');

	glColor3f(1,1,0); //バッシュの色
	for(int i=0;i<strlen(vash_char);i++){
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, vash_char[i]);
	}
	//glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,'v');

	glPopMatrix();

}

bool TGLView::LaserAttack()
{
	if( ! pLaserNode ) return false;
/*
	Affinef afLookAt = afCam;
	Affinef afPoint = afCam * afLaser;
	afLookAt.LookAtGL(afPoint.Pos());
	return pLaserNode->Attack(afLookAt);
*/
	if( ! pLaserNode->Attack(afCam,afLaser) ) return false;

	//弾を打ったときにVashを減らす
	AddVashVal( -2 );
	PlaySound( SOUND_ATTACK );

	return true;
}

void TGLView::FallPond()
{
/*
	Affinef afR = Affinef(Rad(-90), 'y');
	afBody.Rot() = afR.Rot();
	afBody.Pos() = Vec3f(62, 80, -273) ;
*/
/*
	afBody = Affinef(0, bodyHeight, 10);
	afBody.LookAtGL(Vec3f(0, bodyHeight, 0) //視点の位置
	, Vec3f(0,1,0) //カメラの上方向
	);
	bFallingPond = false;
	playerAngle = 0.0f;
*/
}

void TGLView::PlaySound(int SoundNum)
{
#ifdef USE_SOUND
	if( easyds->IsAvailable() ){
		cout << "AttackSound" << endl;
		easyds->Waves(SoundNum)->Play();
	}
#endif
}
