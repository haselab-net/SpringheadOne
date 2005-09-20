#ifndef PHYSICS_H
#define PHYSICS_H
#define xUSE_GRAPHICS

#define xUSE_DESKTOP		  //もしくはデスクトップ環境の場合
#define xUSE_USB20         //USB2.0環境を使用する場合

#include "HIS/HISpidar4quad.h"
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <conio.h>
#include <iostream>
#include <Render/RDGlutView.h>
#include <Base/Debug.h>
#include <Base/Utilities.h>
#include <WinBasis/WBMMTimer.h>
#include <vector>
#include <gl/glut.h>

using namespace LVRM;
using namespace std;

const Vec3f gravity(0,-9.8,0);
const float SPRING = 220; //yi original spring
#ifdef _DEBUG
const float dtHaptic = 0.04f;
#else
const float dtHaptic = 0.002f;
#endif

// 力覚を返すオブジェクト
// 衝突判定できる
class HaColObject{
protected:
	Affinef afObj;
	bool bCollisionable;
public:
	int objectId; // HaObjects::Push(HaColObject* o)において一意のidが入力される

	HaColObject(){
		objectId = 0;
		bCollisionable = true;
	}
	virtual void Init(){}
	virtual ~HaColObject(){}
	virtual void AddForce(Vec3f force,Vec3f pos){}
	virtual void SetMoment(Vec3f vel){}
	virtual void SetPosture(Affinef af){ afObj = af; }
	virtual Affinef GetPosture(){ return afObj; }
	virtual void Update(){}
	virtual void Draw(){}

	//debugYI
	// 衝突判定を行うか否かの設定
	void SetCollisionable(bool b){bCollisionable = b;}
	bool GetCollisionable(){return bCollisionable;}
};

class Collision
{
public:
	HaColObject* obj1;
	HaColObject* obj2;
	Vec3f colPos;
	Vec3f colDepth;
	Collision(){
		obj1 = obj2 = NULL;
	}
	void SetForce(){
		Vec3f force = SPRING * colDepth;

		obj1->AddForce(force,colPos);            //HaWallはAddForceがないけどいいの？
		obj2->AddForce(-force,colPos);
	}
};

class Sphere{
public:
	Sphere() { radius = 0; }
	Vec3f pos;
	float radius;
	void Draw();
};

class HaPenguin : public HaColObject{
	friend class TestView;
protected:
	Vec3f mom;
	Vec3f anMom;
	Vec3f omega;
	Vec3f force, torque;
	Matrix3f inertia;
	float mass;
	Sphere head, body;
public:
	bool checkUp;
	HaPenguin();
	HaPenguin(Affinef inputAff);
	void Init(){
		anMom = Vec3f(0,0,0);
		mom = Vec3f(0,0,0);
	}
	void AddForce(Vec3f inputForce, Vec3f pos);
	void SetMoment(Vec3f velocity);
	void Update();
	void UpdateMom();
	void UpdateAfObj();
	    
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
	void Draw();
};

class HaOppPenguin : public HaPenguin{
	friend class TestView;
public:
	HaOppPenguin();
	HaOppPenguin(Affinef inputAff);
	
};

class HaPuck : public HaPenguin{
	friend class TestView;
public:
	HaPuck();
	HaPuck(Affinef inputAff);
	void SetPosture(Affinef af){
		afObj = af;
		HaPenguin::checkUp = true;
	}

};

class HaWall : public HaColObject{
	Vec3f size;
public:
	HaWall(Affinef axis,Vec3f s){
		size = s;
		HaColObject::afObj = axis;
	}
	Vec3f SetWallNormal(){
		return HaColObject::afObj.Ez();
	}
	Vec3f SetWallPos(){
		return HaColObject::afObj.Pos();
	}
	void AddForce(Vec3f inputForce, Vec3f pos){}
	void Update(){}
//	void Draw();
};

class HaFloor : public HaWall{
	Vec3f goalpos1;
	Vec3f goalpos2;
	float goalrad;
public:
	HaFloor(Affinef axis, Vec3f s, Vec3f pos1,Vec3f pos2,float rad):HaWall(axis, s){
		goalpos1 = pos1;
		goalpos2 = pos2;
		goalrad = rad;
	}
	Vec3f SetGoalPos1(){
		return goalpos1;
	}
	Vec3f SetGoalPos2(){
		return goalpos2;
	}
	float SetGoalRad(){
		return goalrad;
	}
};

class HaCenterWall : public HaColObject{
	Vec3f size;
public:
	HaCenterWall(Affinef axis,Vec3f s){
		size = s;
		HaColObject::afObj = axis;
	}
	Vec3f SetWallNormal(){
		return HaColObject::afObj.Ez();
	}
	Vec3f SetWallPos(){
		return HaColObject::afObj.Pos();
	}
	void AddForce(Vec3f inputForce, Vec3f pos){}
	void Update(){}
};

class HaRacket : public HaColObject{
	Vec3f force;
	Sphere gat;
public:
	HaRacket(){
//		gat.radius = 0.05; //ラケットの半径(racket.wrl)
		gat.radius = 0.049; //ラケットの半径(racket03.WRL)
	}
	void Update();
	float SetRacketRadius(){
		return gat.radius;
	}
	Vec3f SetRacketPos(){
		return HaColObject::afObj.Pos();
	}
	void AddForce(Vec3f inputForce,Vec3f pos){
		force += inputForce;
	}
	void Draw(){
		glPushMatrix();
			glMultMatrixf(afObj);
			glutSolidSphere(gat.radius,32,16);
		glPopMatrix();
	}
};

class HaMemory{
public:
	HaPenguin pen[1];
	HaOppPenguin oppPen[2];
	HaRacket racket;
	HaPuck puck[4];
	HaMemory(){}
	void Write(class HaObjects* object);
};
class HaMemories{
public:
	HaMemory data[2];
	HaMemory& operator [](int id){ return data[id]; }
	volatile int graphicRead;		//	Graphicが読んでいるmemoryの番号
	volatile int graphicShouldRead;	//	Graphicが読むべきmemoryの番号
	HaMemories(){
		graphicRead = 0;
		graphicShouldRead = 0;
	}
};

class HaColTest{													//あたり判定
	static void Check(vector<Collision> *colVector,HaPenguin* penObj1,HaPenguin* penObj2);

	static void Check(vector<Collision> *colVector,HaWall* walObj,HaPenguin* penObj);

	static void Check(vector<Collision> *colVector,HaRacket* racObj,HaPenguin* penObj);

	static void Check(vector<Collision> *colVector,HaRacket* racObj,HaWall* walObj);

	static void Check(vector<Collision> *colVector,HaFloor *floorObj,HaOppPenguin *penObj);

	static void Check(vector<Collision> *colVector,HaCenterWall* cWallObj,HaOppPenguin* penObj);

	static void Check(vector<Collision> *colVector,HaCenterWall* cWallObj,HaPenguin* penObj);

	static void Check(vector<Collision> *colVector,HaFloor *floorObj,HaPenguin *penObj);

	static void Check(vector<Collision> *colVector,HaFloor *floorObj1,HaPuck *puckObj);

public:
	static void Check(vector<Collision> *colVector,HaColObject *colObj1,HaColObject *colObj2);
};

void CalibSpidar();
void InitPhysics(bool bDisplay=false);
void CleanupPhysics();

void InitHaptics();

void Safety(); // 安全対策


//オブジェクト
extern HaMemories memories;

class HaObjects: public vector<HaColObject*>{
public:
	int penBase;
	int oppPenBase;
	int puckBase;
	void Push(HaColObject* o){
		o->objectId = size();
		push_back(o);
	}
	~HaObjects(){
		for(iterator it = begin(); it != end(); ++it){
			delete *it;
		}
		clear();
	}
};
extern HaObjects objects;

class HaCommand:public UTRefCount{
public:
	int objectId;

	HaCommand(int i):objectId(i){}
	virtual ~HaCommand(){}
	virtual void Run(){}
};
class HaCommands{
public:
	typedef vector< UTRef<HaCommand> > HaCmdArray;
	HaCmdArray data[3];
	HaCmdArray& operator [] (int id) { return data[id]; }
	volatile bool hapticRead;			//	Hapticがcommandを読み終わるとtrueにする。
										//	Graphicsは配列に値をセットしたら false にする。
	volatile int hapticShouldRead;		//	Hapticが読むべきcommandの番号
	HaCommands(){
		hapticRead = true;
		hapticShouldRead = 0;
	}
};
extern HaCommands commands;

class InputForce : public HaCommand{
	Vec3f force;
	Vec3f pos;						//
public:
	InputForce(int id, Vec3f f, Vec3f p):HaCommand(id), force(f), pos(p){}
	void Run(){
		objects[HaCommand::objectId]->AddForce(force,pos);
	}
};

class SetVelocity : public HaCommand{
	Vec3f velocity;
public:
	SetVelocity(int id, Vec3f vel):HaCommand(id),velocity(vel){}
	void Run(){
			objects[HaCommand::objectId]->SetMoment(velocity);
	}
};

class MoveObject : public HaCommand{
public:
	Affinef afMove;

	MoveObject(int id, const Affinef& af):HaCommand(id),afMove(af){}
	void Run(){
		objects[HaCommand::objectId]->SetPosture(afMove);
		objects[HaCommand::objectId]->Init();
	}
};

#endif