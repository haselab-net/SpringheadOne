#include "HIS/HISpidar4quad.h"
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <conio.h>
#include <iostream>
#include <Render/RDGlutView.h>
#include <Base/Debug.h>
#include <WinCmp/MultiMedia/CMMTimer.h>
#include <GL/glut.h>
#include <vector>

using namespace std;
using namespace VRRend;
using namespace WinCmp;
using namespace VRHaptic;
using namespace VRBase;

//#lib "glut32.lib"
Affinef afCam;
HISpidar4quad spidar;
float ratio = 0.2;					//安定性の比重を決める係数
int GraphicFlag = 0;
const Vec3f g(0,-9.8,0);
const float dt = 0.001;
const float k = 400;

GLfloat light0pos[] = {3.0, 3.0, 3.0, 1.0};		//光源位置設定
GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
GLfloat white[] = {1.0, 0.9, 0.9, 1.0};	

class ColObject
{
public:
	virtual void AddForce(Vec3f force,Vec3f pos){
	}
	virtual void Update(){
	}
	virtual void Draw(){
	}
};

class Collision
{
public:
	ColObject* obj1;
	ColObject* obj2;
	Vec3f colPos;
	Vec3f colDepth;
               
	void SetForce(){
		Vec3f force = k * colDepth;

		obj1->AddForce(force,colPos);            //WallはAddForceがないけどいいの？
		obj2->AddForce(-force,colPos);
	}
};

class Sphere 
{
public:
	Vec3f pos;
	float radius;
};

class Penguin : public ColObject
{
	friend class TestView;
	
	Affinef afObj;
	Vec3f mom;
	Vec3f anMom;
	Vec3f omega;
	Vec3f force, torque;
	Matrix3f inertia;
	float mass;
	Sphere head, body;
public:
	float leng,lengHead,lengBody; //後で消去
	Penguin(){
		mass = 0;
		leng = 0;
		lengHead = 0;
		lengBody = 0;
	}
	Penguin(Affinef inputAff){	
//	    float leng,lengHead,lengBody;
		float ipartsA,ipartsB,ixx,iyy,izz;
		Vec3f ex,ey,ez;

		afObj = inputAff;
	/********* sphere の初期値 **********/
	    head.radius = 0.03;							//球の半径
		body.radius = 0.05;
		float headMass = Square(head.radius);
		float bodyMass = Square(body.radius);
		mass = headMass + bodyMass;
		leng = head.radius + body.radius;
		lengHead = leng * bodyMass / mass;
		lengBody = leng * headMass / mass;
		head.pos = Vec3f(0,-lengHead,0);
		body.pos = Vec3f(0,lengBody,0);                

	/********** I の初期値 ************/
		ipartsA = headMass * Square(lengHead) + bodyMass * Square(lengBody);
		ipartsB = headMass * Square(head.radius) + bodyMass * Square(body.radius);
		ixx = ipartsA + 2 * ipartsB / 5;
		iyy = 2 * ipartsB / 5;
		izz = ipartsA + 2 * ipartsB / 5;
		ex = Vec3f(ixx,0,0);
		ey = Vec3f(0,iyy,0);
		ez = Vec3f(0,0,izz);
		inertia = Matrix3f(ex,ey,ez);

	/********** 運動量*角運動量の初期値 ***********/
		mom = Vec3f(0,0,0);
		omega = Vec3f(Rad(0.2),Rad(0),Rad(0.3));
		anMom = inertia * omega;
	}
	void AddForce(Vec3f inputForce, Vec3f pos){
		force += inputForce;      
		torque += ( (pos - afObj.Pos()) ^ inputForce);
	}
	void Update(){
		UpdateMom();
		UpdateAfObj();
	}
	void UpdateMom(){
	    mom += (force + mass * g) * dt;
     	anMom += torque * dt;
		mom *= 0.9999;
		anMom *= 0.9999;
		force = Vec3f(0,0,0);
		torque = Vec3f(0,0,0);
	}
	void UpdateAfObj(){
	    Matrix3f rot = afObj.Rot();
        Matrix3f gInertia = rot * inertia * rot.trans();
		Vec3f omega = gInertia.inv() * anMom;
        Matrix3f dangle(dt * omega.norm(), omega.unit());

	    afObj.Rot() = dangle * rot;
	    afObj.Pos() += mom / mass * dt;
	}
	Vec3f SetHeadPos(){
		return afObj * head.pos;
	}
	Vec3f SetBodyPos(){
		return afObj * body.pos;
	}
	float SetHeadRadius(){
		return head.radius;
	}
	float SetBodyRadius(){
		return body.radius;
	}
	void Draw(){
		glPushMatrix();
			glMultMatrixf(afObj);
			glPushMatrix();
				glMultMatrixf(Affinef(0,-lengHead,0));
				glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,red);
				glutSolidSphere(SetHeadRadius(), 32, 16);
			glPopMatrix();
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,white);
			glMultMatrixf(Affinef(0,lengBody,0));
			glutSolidSphere(SetBodyRadius(), 32, 16);
		glPopMatrix();
	}
};
#if 0
class Board : public ColObject
{
public:
    Vec3f normal;
	Vec3f pos;
};
#endif
class Wall : public ColObject
{
	Vec3f size;
	Affinef axis;
public:
	Wall(Affinef afObj,Vec3f s){
		size = s;
		axis = afObj;
	}
	Vec3f SetWallNormal(){
		return axis.Ez();
	}
	Vec3f SetWallPos(){
		return axis.Pos();
	}
	void AddForce(Vec3f inputForce, Vec3f pos){
	}
	void Update(){
	}
	void Draw(){
		Vec3f vertex1,vertex2,vertex3;
		vertex1 = axis.Pos() + size.X() * axis.Ex();
		vertex2 = vertex1 + size.Y() * axis.Ey();
		vertex3 = vertex2 + size.X() * -axis.Ex();
		glPushMatrix();
				glBegin(GL_POLYGON);
			  	glVertex3d(axis.PosX(),axis.PosY(),axis.PosZ());
				glVertex3d(vertex1.X(),vertex1.Y(),vertex1.Z());
			    glVertex3d(vertex2.X(),vertex2.Y(),vertex2.Z());
			    glVertex3d(vertex3.X(),vertex3.Y(),vertex3.Z());
			glEnd();
		glPopMatrix();	
	}
};

class Racket : public ColObject
{
	Vec3f force;
	Sphere gat;   //微妙？
//	HISpidar4quad spidar;     
public:
	Racket(){
		gat.radius = 0.03;
	}
	void Update(){
		spidar.Update();
		gat.pos = spidar.GetPos();
		spidar.SetForce(force,ratio);			//力提示
		force = Vec3f(0,0,0);                  
	}
	float SetRacketRadius(){
		return gat.radius;
	}
	Vec3f SetRacketPos(){
		return gat.pos;
	}
	void AddForce(Vec3f inputForce,Vec3f pos){
		force += inputForce;
	}
	void Draw(){
		glPushMatrix();
			glTranslated(gat.pos.X(),gat.pos.Y(),gat.pos.Z());
			glutSolidSphere(gat.radius,32,16);
		glPopMatrix();
	}
};

class Memory{
	Penguin pen[3];
	Racket racket;
public:
	int HapticFlag, access;
	void Write(ColObject* object){
		int i = 0;

		if (typeid(*object) == typeid(Penguin)){
			pen[i] = *(Penguin*)object;
			i++;
		}
		else if (typeid(*object) == typeid(Racket)){
			racket = *(Racket*)object;
		}
	}
};

class ColTest{													//あたり判定
	static void Check(vector<Collision> *colVector,Penguin *penObj1,Penguin *penObj2){
		Vec3f headPos1 = penObj1->SetHeadPos();
		Vec3f bodyPos1 = penObj1->SetBodyPos();
		Vec3f headPos2 = penObj2->SetHeadPos();
		Vec3f bodyPos2 = penObj2->SetBodyPos();
		float headRadius1 = penObj1->SetHeadRadius();
		float bodyRadius1 = penObj1->SetBodyRadius();
		float headRadius2 = penObj2->SetHeadRadius();
		float bodyRadius2 = penObj2->SetBodyRadius();
		float depth;
		Collision col; 

		if (fabs((headPos1 - headPos2).norm()) < headRadius1 + headRadius2){
			depth = (headRadius1 + headRadius2) - fabs((headPos1 - headPos2).norm());
			col.obj1 = penObj1;
			col.obj2 = penObj2;
			col.colDepth = depth * (headPos1 - headPos2).unit();
			col.colPos = headPos2 + (headRadius2 - depth / 2) * (headPos1 - headPos2).unit();
			colVector->push_back(col);
		}
		if (fabs((headPos1 - bodyPos2).norm()) < headRadius1 + bodyRadius2){
			depth = (headRadius1 + bodyRadius2) - fabs((headPos1 - bodyPos2).norm());
			col.obj1 = penObj1;
			col.obj2 = penObj2;
			col.colDepth = depth * (headPos1 - bodyPos2).unit();
			col.colPos = bodyPos2 + (bodyRadius2 - depth / 2) * (headPos1 - bodyPos2).unit();
			colVector->push_back(col);
		}
		if (fabs((bodyPos1 - headPos2).norm()) < bodyRadius1 + headRadius2){
			depth = (bodyRadius1 + headRadius2) - fabs((bodyPos1 - headPos2).norm());
			col.obj1 = penObj1;
			col.obj2 = penObj2;
			col.colDepth = depth * (bodyPos1 - headPos2).unit();
			col.colPos = headPos2 + (headRadius2 - depth / 2) * (bodyPos1 - headPos2).unit();
			colVector->push_back(col);
		}
		if (fabs((bodyPos1 - bodyPos2).norm()) < bodyRadius1 + bodyRadius2){
			depth = (bodyRadius1 + bodyRadius2) - fabs((bodyPos1 - bodyPos2).norm());
			col.obj1 = penObj1;
			col.obj2 = penObj2;
			col.colDepth = depth * (bodyPos1 - bodyPos2).unit();
			col.colPos = bodyPos2 + (bodyRadius2 - depth / 2) * (bodyPos1 - bodyPos2).unit();
			colVector->push_back(col);
		}
	}
	static void Check(vector<Collision> *colVector,Wall *walObj,Penguin *penObj){
		float disHead = (penObj->SetHeadPos() - walObj->SetWallPos()) * walObj->SetWallNormal();
		float disBody = (penObj->SetBodyPos() - walObj->SetWallPos()) * walObj->SetWallNormal();
		Collision col;

		if (fabs(disHead) < penObj->SetHeadRadius()){
			col.obj1 = walObj;
			col.obj2 = penObj;
			col.colDepth = (penObj->SetHeadRadius() - fabs(disHead)) * (-walObj->SetWallNormal());
			col.colPos = penObj->SetHeadPos() + disHead * (-walObj->SetWallNormal());
			colVector->push_back(col);
		}
		if (fabs(disBody) < penObj->SetBodyRadius()){
			col.obj1 = walObj;
			col.obj2 = penObj;
			col.colDepth = (penObj->SetBodyRadius() - fabs(disBody)) * (-walObj->SetWallNormal());
			col.colPos = penObj->SetBodyPos() + disBody * (-walObj->SetWallNormal());
			colVector->push_back(col);
		}
	}
	static void Check(vector<Collision> *colVector,Racket *racObj,Penguin *penObj){
		Vec3f rackPos = racObj->SetRacketPos();
		Vec3f headPos = penObj->SetHeadPos();
		Vec3f bodyPos = penObj->SetBodyPos();
		float rackRadius = racObj->SetRacketRadius();
		float headRadius = penObj->SetHeadRadius();
		float bodyRadius = penObj->SetBodyRadius();
		float depth;
		Collision col;

		if (fabs((rackPos - headPos).norm()) < rackRadius + headRadius){
			depth = (rackRadius + headRadius) - fabs((rackPos - headPos).norm());
			col.obj1 = racObj;
			col.obj2 = penObj;
			col.colDepth = depth * (rackPos - headPos).unit();
			col.colPos = headPos + (headRadius - depth / 2) * (rackPos - headPos).unit();
			colVector->push_back(col);
		}
		if (fabs((rackPos - bodyPos).norm()) < rackRadius + bodyRadius){
			depth = (rackRadius + bodyRadius) - fabs((rackPos - bodyPos).norm());
			col.obj1 = racObj;
			col.obj2 = penObj;
			col.colDepth = depth * (rackPos - bodyPos).unit();
			col.colPos = bodyPos + (bodyRadius - depth / 2) * (rackPos - bodyPos).unit();
			colVector->push_back(col);
		}
	}
	static void Check(vector<Collision> *colVector,Racket *racObj,Wall *walObj){
		float disRac = (racObj->SetRacketPos() - walObj->SetWallPos()) * walObj->SetWallNormal();
		Collision col;

		if (fabs(disRac) < racObj->SetRacketRadius()){
			col.obj1 = racObj;
			col.obj2 = walObj;
			col.colDepth = (racObj->SetRacketRadius() - fabs(disRac)) * walObj->SetWallNormal();
			col.colPos = racObj->SetRacketPos() + (-walObj->SetWallNormal());
			colVector->push_back(col);
		}
	}
public:
	static void Check(vector<Collision> *colVector,ColObject *colObj1,ColObject *colObj2){
		Penguin *pen1,*pen2;
		Wall *wall;
		Racket *rack;

		if (typeid(*colObj1) == typeid(Penguin) && typeid(*colObj2) == typeid(Penguin)){
			pen1 = (Penguin *) colObj1;
			pen2 = (Penguin *) colObj2;
			Check(colVector,pen1,pen2);
		}
		else if (typeid(*colObj1) == typeid(Wall) && typeid(*colObj2) == typeid(Penguin)){
			wall = (Wall *) colObj1;
			pen1 = (Penguin *) colObj2;
			Check(colVector,wall,pen1);
		}
		else if (typeid(*colObj1) == typeid(Racket) && typeid(*colObj2) == typeid(Penguin)){
			rack = (Racket *) colObj1;
			pen1 = (Penguin *) colObj2;
			Check(colVector,rack,pen1);
		}
		else if (typeid(*colObj1) == typeid(Racket) && typeid(*colObj2) == typeid(Wall)){
			rack = (Racket *) colObj1;
			wall = (Wall *) colObj2;
			Check(colVector,rack,wall);
		}
		else {
			//break;　　　　　　　　　　　　　　//壁同士の場合　このままで大丈夫かな？
		}
	}
};

//オブジェクト生成
vector<ColObject*> objects;             
Memory memory[2];


class TestView: public RDGlutView{
public:
	virtual void OnOpen(){
		glEnable(GL_LIGHT0);
		afCam.PosZ() = 2.0f;			//カメラ位置設定
		//afCam.PosY() = 100.0f;
		afCam.LookAtGL(Vec3f());
	}
	virtual void OnIdle(){
		Invalidate();
	}

	virtual void OnDraw(){
		glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
//		glColor3f(0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(afCam.inv());
		glLightfv(GL_LIGHT0,GL_POSITION,light0pos);		   //処理速度が遅い場合は消去
		for(int i=0; i<objects.size(); i++){                    
			objects[i]->Draw();                            //各物体の描画
		}
		Flip(); 
	}

	virtual void OnSize(int w, int h){
		printf("OnSize %d %d\n", w, h);
		glViewport(0, 0, w, h);

		glFlush();
		
#if 0
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.0, 3, 10, 100);
		Affinef afProjGL;
		glGetFloatv(GL_PROJECTION_MATRIX, afProjGL);
		std::cout << afProjGL << "\n";
#else
		glMatrixMode(GL_PROJECTION);
		Affinef afProj(Vec3f(0,0,500), Vec2f(w, h), 1, 10000);
		glLoadMatrixf(afProj);
		std::cout << afProj << "\n";
#endif
	}
	virtual void OnClose(){
		printf("OnClose\n");
	}
	virtual void OnMouse(Button b, bool s, int x, int y){
		printf("OnMouse %s %d %d\n", s ? "on" : "off", x, y);
	}
	virtual void OnKeyboard(int code, int x, int y){
		printf("OnKey %d\n", code);
		if (code == 0x1b){
			Exit();
		}
	}
};

void Step(){
	vector<Collision> collisions;
	for(int i=0; i<objects.size(); i++){
		for(int j=i+1; j<objects.size(); j++){
			ColTest::Check(&collisions, objects[i], objects[j]);
		}
	}
	for(int i=0; i<collisions.size(); i++){
		collisions[i].SetForce();
	}
	for(int i=0; i<objects.size(); i++){
		objects[i]->Update();						//WallはUpdateがないけどいいの？
	}
}

CMMTimer timer;
void timerFunc(void* arg){
	Step();
	int i=0,k;
	if (i == 20){
		if (GraphicFlag == 0) k = 0;                  //どちらが読み終わったか
		else k = 1;
		memory[k].HapticFlag = 0;                     //書き込み中
		for (int j = 0; j < objects.size() ; j++ ){
			memory[k].Write(objects[j]);
	}
	memory[k].HapticFlag = 1;                         //書き込み終了
	memory[k].access = k;							  //グラフィック側が読むメモリ
	i=-1;
	}
	i++;
};

void main(){
	DVDeviceManager devMan;
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	devMan.Init();
	
	std::cout << devMan;
	
    //スパイダー初期化
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	spidar.Init(devMan, motorPos[0], 0.5776f, 2.924062107079e-5f, 0.5f, 20.0f);

	spidar.Calib();
	Vec3f ex = Vec3f(1,0,0);			//グローバル座標系の単位ベクトル
	Vec3f ey = Vec3f(0,1,0);
	Vec3f ez = Vec3f(0,0,1);	
	float lx = 0.7;						//各枠の長さ
	float ly = 0.7;				
	float lz = 0.7;
	Affinef afLeft,afRight,afUp,afDown,afFlont,afPen1,afPen2,afPen3;
	Vec3f sizeDown,sizeUp,sizeLeft,sizeRight,sizeFlont;

	afDown.Pos() = Vec3f(-lx/2,-ly/2,-lz/2);		//壁の初期設定
	afUp.Pos() = Vec3f(lx/2,ly/2,-lz/2);
	afLeft.Pos() = Vec3f(-lx/2,ly/2,-lz/2);
	afRight.Pos() = Vec3f(lx/2,-ly/2,-lz/2);
	afFlont.Pos() = Vec3f(lx/2,ly/2,-lz/2);
	afDown.Rot() = Matrix3f(ez,ex,ey);
	afUp.Rot() = Matrix3f(ez,-ex,-ey);
	afLeft.Rot() = Matrix3f(ez,-ey,ex);
	afRight.Rot() = Matrix3f(ez,ey,-ex);
	afFlont.Rot() = Matrix3f(-ex,-ey,ez);  
	sizeDown = Vec3f(lz,lx,0);
	sizeUp = Vec3f(lz,lx,0);
	sizeLeft = Vec3f(lz,ly,0);
	sizeRight = Vec3f(lz,ly,0);
	sizeFlont = Vec3f(lx,ly,0);

	afPen1.Pos() = Vec3f(lx/4,ly/4,lz/4);
	afPen2.Pos() = Vec3f(-lx/4,-ly/4,-lz/4);
	afPen3.Pos() = Vec3f(-lx/4,ly/4,-lz/4);
	afPen1.Rot() = Matrix3f(ex,ey,ez);
	afPen2.Rot() = Matrix3f(ex,ey,ez);
	afPen3.Rot() = Matrix3f(ex,ey,ez);

	Wall wallDown(afDown,sizeDown),wallUp(afUp,sizeUp),wallLeft(afLeft,sizeLeft),wallRight(afRight,sizeRight),wallFlont(afFlont,sizeFlont);
	Racket racket;
	Penguin penguin1(afPen1),penguin2(afPen2),penguin3(afPen3);	
				

	objects.push_back(&racket);						//objectsに追加
	objects.push_back(&wallDown);
	objects.push_back(&wallUp);
	objects.push_back(&wallLeft);
	objects.push_back(&wallRight);
	objects.push_back(&wallFlont);
	objects.push_back(&penguin1);
	objects.push_back(&penguin2);
	objects.push_back(&penguin3);

	timer.Set(timerFunc, NULL);
	timer.Resolution(1);
	timer.Interval(1);
//	timer.Create();
	timer.Thread();
	TestView view;
	view.WindowPosition(10,10, 300, 200);
	view.Init("OpenGL Window");
	view.Run();
}

///////////////////////////////////////////////////////////////////////////




