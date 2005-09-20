#include "environment.h"
#include "Physics.h"
#include "settings.h"
#include "HIS/HISpidar4quad.h"

//#ifndef USE_USB20
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"

//#else
 //USB2.0環境
#include <Device/DRUsb20Simple.h>
//#endif

#include <conio.h>
#include <iostream>
#include <Render/RDGlutView.h>
#include <Base/Debug.h>
#include <WinBasis/WBMMTimer.h>
#include <vector>

using namespace std;

//Vec3f fakeSpidarPos;

Affinef afCam;
DVDeviceManager devMan;
HISpidar4quad spidar;
HISpidar4quad spidar2; // SPIDAR-G用ダミー出力


float ratio = 0.2; // 安定性の比重を決める係数
int writeCycle=0;
int memoryNo=0;

int SndPuckPuck = 0;
int SndPenPuck = 0;
int SndPuckGoal = 0;
int SndRacPen = 0;
int SndRacOppPen1 = 0;
int SndRacOppPen2 = 0;
int SndRacPuck = 0;


//オブジェクト
HaObjects objects; // 力覚を返すオブジェクト郡
HaMemories memories;
HaCommands commands;


//グローバル座標系の単位ベクトル
Vec3f ex = Vec3f(1,0,0);
Vec3f ey = Vec3f(0,1,0);
Vec3f ez = Vec3f(0,0,1);


GLfloat light0pos[] = {3.0, 3.0, 3.0, 1.0};		//光源位置設定
GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
GLfloat white[] = {1.0, 0.9, 0.9, 1.0};	

void HaMemory::Write(HaObjects* objects){
	
//	hapticId = object->objectId;
	for (int i=0 ; i < objects->size() ; i++){
		HaColObject* object = (*objects)[i];
		if (typeid(*object) == typeid(HaPenguin)){
			pen[i-objects->penBase] = *(HaPenguin*)object;
		}
		else if (typeid(*object) == typeid(HaOppPenguin)){
			oppPen[i-objects->oppPenBase] = *(HaOppPenguin*)object;
		}
		else if (typeid(*object) == typeid(HaPuck)){
			puck[i-objects->puckBase] = *(HaPuck*)object;
		}
		else if (typeid(*object) == typeid(HaRacket)){
			racket = *(HaRacket*)object;
		}
	}
}	

/******************* Sphere ********************/

void Sphere::Draw(){
	glPushMatrix();
	glTranslatef(pos.X(), pos.Y(), pos.Z());
	glutSolidSphere(radius, 32, 16);
	glPopMatrix();
}

/******************* HaPenguin ********************/
HaPenguin::HaPenguin(){
	mass = 0;
}
HaPenguin::HaPenguin(Affinef inputAff){	
	checkUp = false;
	HaColObject::afObj = inputAff;
	/********* sphere の初期値 **********/
    head.radius = PenheadR;							//球の半径
	body.radius = PenbodyR;
	mass = Penmass;
	
	head.pos = PenheadPos;
	body.pos = PenbodyPos;      

	/********** I の初期値 ************/
	inertia.ExX() = 10*mass*body.radius*body.radius;
	inertia.EyY() = 10*mass*body.radius*body.radius;
	inertia.EzZ() = 10*mass*body.radius*body.radius/4;
	/********** 運動量*角運動量の初期値 ***********/
}

void HaPenguin::AddForce(Vec3f inputForce, Vec3f pos){
	force += inputForce;      
	torque += ( (pos - HaColObject::afObj.Pos()) ^ inputForce);
}

void HaPenguin::SetMoment(Vec3f velocity){
	mom = velocity * mass;
}

void HaPenguin::UpdateAfObj(){
	Matrix3f rot = HaColObject::afObj.Rot();
    Matrix3f gInertia = rot * inertia * rot.trans();
	Vec3f omega = gInertia.inv() * anMom;
    if (omega.norm() > 0 ){
		Matrix3f dangle(dtHaptic * omega.norm(), omega.unit());
		HaColObject::afObj.Rot() = dangle * rot;
	}
    HaColObject::afObj.Pos() += mom / mass * dtHaptic;
}

void HaPenguin::UpdateMom(){
    mom += (force + mass * gravity) * dtHaptic;
   	anMom += torque * dtHaptic;
	mom.X() *= 0.999;
	mom.Z() *= 0.999;
	mom.Y() *= 0.99;
	anMom *= 0.99;
	force = Vec3f(0,0,0);
	torque = Vec3f(0,0,0);
}

void HaPenguin::Update(){
	UpdateMom();
	UpdateAfObj();
}

void HaPenguin::Draw(){
	glPushMatrix();
		glMultMatrixf(HaColObject::afObj);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,red);
		head.Draw();
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,white);
		body.Draw();
	glPopMatrix();
}

/*************HaOppPenguin************/
HaOppPenguin::HaOppPenguin(){
}
HaOppPenguin::HaOppPenguin(Affinef inputAff){	

	HaPenguin::afObj = inputAff;
	/********* sphere の初期値 **********/
    head.radius = PenheadR;							//球の半径
	body.radius = PenbodyR;
	mass = Penmass;
	
	head.pos = PenheadPos;
	body.pos = PenbodyPos;                            

	/********** I の初期値 ************/
	inertia.ExX() = 10*mass*body.radius*body.radius;
	inertia.EyY() = 10*mass*body.radius*body.radius;
	inertia.EzZ() = 10*mass*body.radius*body.radius/4;

	/********** 運動量*角運動量の初期値 ***********/

}



/******************* HaPuck ********************/
HaPuck::HaPuck(){
	
}
HaPuck::HaPuck(Affinef inputAff){	 
	float ipartsA,ipartsB,ixx,iyy,izz;
	Vec3f ex,ey,ez;

	HaPenguin::afObj = inputAff;
	/********* sphere の初期値 **********/
	HaPenguin::head.radius = PuckheadR;							//球の半径
	HaPenguin::body.radius = PuckbodyR;
	float headMass = 400 / 3 * HaPenguin::head.radius * Square(HaPenguin::head.radius) * 1.5;//パックの重さを若干増やした
	float bodyMass = 400 / 3 * HaPenguin::body.radius * Square(HaPenguin::body.radius) * 1.5;
	mass = headMass + bodyMass;
	float leng = HaPenguin::head.radius + HaPenguin::body.radius;
	float lengHead = leng * bodyMass / mass;
	float lengBody = leng * headMass / mass;
	HaPenguin::head.pos = Vec3f(0,lengHead,0);
	HaPenguin::body.pos = Vec3f(0,-lengBody,0);                

	/********** I の初期値 ************/
	ipartsA = headMass * Square(lengHead) + bodyMass * Square(lengBody);
	ipartsB = headMass * Square(HaPenguin::head.radius) + bodyMass * Square(HaPenguin::body.radius);
	ixx = ipartsA + 2 * ipartsB / 5;
	iyy = 2 * ipartsB / 5;
	izz = ipartsA + 2 * ipartsB / 5;
	ex = Vec3f(ixx,0,0);
	ey = Vec3f(0,iyy,0);
	ez = Vec3f(0,0,izz);
	inertia = Matrix3f(ex,ey,ez);
}

/************************* HaRacket ***********************/

void HaRacket::Update(){
	//SPIDAR位置取得
	if (env.iSpidar_type == Env_NO_SPIDAR) {
		int i;
		Vec3f temp;
		temp.X() = 0.0f;
		temp.Y() = 0.0f;
		temp.Z() = 0.0f;
		for (i=0; i<MOUSE_SAMPLE; i++) {
			temp.X() += (float)env.mousex[i];
			temp.Y() += (float)env.mousez[i];
			temp.Z() += (float)env.mousey[i];
		}
		//サンプル数で移動平均
		temp.X() = (float)temp.X() / (float)(MOUSE_SAMPLE);
		temp.Y() = (float)temp.Y() / (float)(MOUSE_SAMPLE);
		temp.Z() = (float)temp.Z() / (float)(MOUSE_SAMPLE);

		//正規化(800x600)
		temp.X() = (float)(temp.X() - 400.0f) / (float)400.0f;
		temp.Y() = (float)(temp.Y() -  10.0f) / (float)10.0f;
		temp.Z() = (float)(temp.Z() - 300.0f) / (float)300.0f;

		//SPIDAR座標化
		env.v3DMouse.X() = temp.Z() * (float)env.px *  2.0f;
		env.v3DMouse.Y() = temp.Y() * (float)env.py *  2.0f;
		env.v3DMouse.Z() = temp.X() * (float)env.pz * -2.0f;

		env.vSpidarLast = env.vSpidar;
		env.vSpidar = env.v3DMouse;
		env.vSpidarForceLast = env.vSpidarForce;
		env.vSpidarForce = Vec3f(0.0f,0.0f,0.0f);

		cout << "virtual SPIDAR pos = "<< env.vSpidar << " (X,Y) = " << env.mousex[0] << "," << env.mousey[0] <<endl;

	} else {
		spidar.Update();
		env.vSpidarLast = env.vSpidar;
		if (env.bRoll90) {
			Vec3f temp;
			temp = spidar.GetPos();
			env.vSpidar.X() = temp.Z();
			if (env.bTallNone) {
				env.vSpidar.Y() = TallDefault;//デフォルト
			} else {
				env.vSpidar.Y() = temp.Y();
			}
			env.vSpidar.Z() = -1.0f*temp.X();
		} else {
			env.vSpidar = spidar.GetPos();
			if (env.bTallNone) {
				env.vSpidar.Y() = TallDefault;//デフォルト
			}
		}
		env.vSpidarForceLast = env.vSpidarForce;
		env.vSpidarForce = spidar.GetForce();
	}

//	Safety();  //安全対策

	switch ( env.view->sceneManager.GetCurScene() ) {
		case 3: //あかいまる
		case 9: //映像用キャリブレーション画面
		if (env.iSpidar_type == Env_NO_SPIDAR) {
			env.view->game.user.X() = env.v3DMouse.X() * 1.0f;
			env.view->game.user.Y() = env.v3DMouse.Z() * 1.0f;
			env.view->game.user.Z() = env.v3DMouse.Y() * 1.0f;
		} else {
			env.view->game.user.X() = env.vSpidar.X() * env.viewscale.X();
			env.view->game.user.Y() = env.vSpidar.Y() * env.viewscale.Y();
			env.view->game.user.Z() = env.vSpidar.Z() * env.viewscale.Z();
		}

		default:
			if (env.iSpidar_type==Env_USB_SPIDAR_G) {
//					HaColObject::afObj.Pos() = spidar.GetPos()*1.25;
					HaColObject::afObj.Pos() = env.vSpidar;
					//本当はこのシフトは不要かもしれない
					/*HaColObject::afObj.PosZ() -= 0.6f;		
					HaColObject::afObj.PosY() -= 0.15f;
					*/
				} else {
					HaColObject::afObj.Pos() = env.vSpidar;
				}
			if (env.bHaptics) {
				env.dForce += force;
				env.dRatio += ratio;
				if (env.bRoll90) {
					Vec3f rollforce;
					rollforce.X() = -1.0f*force.Z();
					rollforce.Y() = force.Y();
					rollforce.Z() =  1.0f * force.X();
					spidar.SetForce(rollforce,ratio);			//力提示
				} else {
					spidar.SetForce(force,ratio);			//力提示
				}
			} else {
				env.dForce += force;
				env.dRatio += ratio;
				force.clear();
				spidar.SetForce(force,ratio);			//力提示はゼロ,になってないが
			}
		force = Vec3f(0,0,0);                  
		}
}

void AddSound(HaPenguin* penObj1,HaPenguin* penObj2){
	if (typeid(*penObj1) == typeid(HaPuck) && typeid(*penObj2) == typeid(HaPuck)){
		SndPuckPuck++;
	}
	if (typeid(*penObj1) == typeid(HaPuck) && typeid(*penObj2) == typeid(HaPenguin)){
		SndPenPuck++;
	}
	if (typeid(*penObj1) == typeid(HaPuck) && typeid(*penObj2) == typeid(HaOppPenguin)){
		SndPenPuck++;
	}
}

void AddHitSound(HaRacket* rackObj,HaPenguin* penObj){
	if (typeid(*penObj) == typeid(HaPenguin))  SndRacPen++;
	if (typeid(*penObj) == typeid(HaPuck))  SndRacPuck++;
	if (typeid(*penObj) == typeid(HaOppPenguin)){
		if (penObj->objectId = 13) SndRacOppPen1++;
		if (penObj->objectId = 14) SndRacOppPen2++;
	}
}

// =================================================================	
// HaColTest
// 衝突判定のためのクラス
// =================================================================	
void HaColTest::Check(vector<Collision> *colVector,HaPenguin *penObj1,HaPenguin *penObj2){
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
		AddSound(penObj1,penObj2);
	}
	if (fabs((headPos1 - bodyPos2).norm()) < headRadius1 + bodyRadius2){
		depth = (headRadius1 + bodyRadius2) - fabs((headPos1 - bodyPos2).norm());
		col.obj1 = penObj1;
		col.obj2 = penObj2;
		col.colDepth = depth * (headPos1 - bodyPos2).unit();
		col.colPos = bodyPos2 + (bodyRadius2 - depth / 2) * (headPos1 - bodyPos2).unit();
		colVector->push_back(col);
		AddSound(penObj1,penObj2);
	}
	if (fabs((bodyPos1 - headPos2).norm()) < bodyRadius1 + headRadius2){
		depth = (bodyRadius1 + headRadius2) - fabs((bodyPos1 - headPos2).norm());
		col.obj1 = penObj1;
		col.obj2 = penObj2;
		col.colDepth = depth * (bodyPos1 - headPos2).unit();
		col.colPos = headPos2 + (headRadius2 - depth / 2) * (bodyPos1 - headPos2).unit();
		colVector->push_back(col);
		AddSound(penObj1,penObj2);
	}
	if (fabs((bodyPos1 - bodyPos2).norm()) < bodyRadius1 + bodyRadius2){
		depth = (bodyRadius1 + bodyRadius2) - fabs((bodyPos1 - bodyPos2).norm());
		col.obj1 = penObj1;
		col.obj2 = penObj2;
		col.colDepth = depth * (bodyPos1 - bodyPos2).unit();
		col.colPos = bodyPos2 + (bodyRadius2 - depth / 2) * (bodyPos1 - bodyPos2).unit();
		colVector->push_back(col);
		AddSound(penObj1,penObj2);
	}
}

void HaColTest::Check(vector<Collision> *colVector,HaWall *walObj,HaPenguin *penObj){
	float disHead = (penObj->SetHeadPos() - walObj->SetWallPos()) * walObj->SetWallNormal();
	float disBody = (penObj->SetBodyPos() - walObj->SetWallPos()) * walObj->SetWallNormal();
	Collision col;
	if (penObj->checkUp == false){ 
		if (disHead < penObj->SetHeadRadius()){
			col.obj1 = walObj;
			col.obj2 = penObj;
			col.colDepth = (penObj->SetHeadRadius() - disHead) * (-walObj->SetWallNormal());
			col.colPos = penObj->SetHeadPos() + penObj->SetHeadRadius() * (-walObj->SetWallNormal());
			colVector->push_back(col);
		}
		if (disBody < penObj->SetBodyRadius()){
			col.obj1 = walObj;
			col.obj2 = penObj;
			col.colDepth = (penObj->SetBodyRadius() - disBody) * (-walObj->SetWallNormal());
			col.colPos = penObj->SetBodyPos() + penObj->SetBodyRadius() * (-walObj->SetWallNormal());
			colVector->push_back(col);
		}
	}
	else {
		if (penObj->SetBodyPos().Y() < lyResetb && penObj->SetHeadPos().Y() < lyReseth ){
				penObj->checkUp = false;
		}
	}
}

void HaColTest::Check(vector<Collision> *colVector,HaCenterWall* cWallObj,HaPenguin* penObj){
	float disHead = (penObj->SetHeadPos() - cWallObj->SetWallPos()) * cWallObj->SetWallNormal();
	float disBody = (penObj->SetBodyPos() - cWallObj->SetWallPos()) * cWallObj->SetWallNormal();
	Collision col;

	if (disHead < penObj->SetHeadRadius()){
		col.obj1 = cWallObj;
		col.obj2 = penObj;
		col.colDepth = (penObj->SetHeadRadius() - disHead) * (-cWallObj->SetWallNormal());
		col.colPos = penObj->SetHeadPos() + penObj->SetHeadRadius() * (-cWallObj->SetWallNormal());
		colVector->push_back(col);
	}
	if (disBody < penObj->SetBodyRadius()){
		col.obj1 = cWallObj;
		col.obj2 = penObj;
		col.colDepth = (penObj->SetBodyRadius() - disBody) * (-cWallObj->SetWallNormal());
		col.colPos = penObj->SetBodyPos() + penObj->SetBodyRadius() * (-cWallObj->SetWallNormal());
		colVector->push_back(col);
	}
}

void HaColTest::Check(vector<Collision> *colVector,HaCenterWall* cWallObj,HaOppPenguin* penObj){
	float disHead = (penObj->SetHeadPos() - cWallObj->SetWallPos()) * cWallObj->SetWallNormal();
	float disBody = (penObj->SetBodyPos() - cWallObj->SetWallPos()) * cWallObj->SetWallNormal();
	Collision col;

	if (-disHead < penObj->SetHeadRadius()){
		col.obj1 = cWallObj;
		col.obj2 = penObj;
		col.colDepth = (penObj->SetHeadRadius() + disHead) * cWallObj->SetWallNormal();
		col.colPos = penObj->SetHeadPos() + penObj->SetHeadRadius() * cWallObj->SetWallNormal();
		colVector->push_back(col);
	}
	if (-disBody < penObj->SetBodyRadius()){
		col.obj1 = cWallObj;
		col.obj2 = penObj;
		col.colDepth = (penObj->SetBodyRadius() + disBody) * cWallObj->SetWallNormal();
		col.colPos = penObj->SetBodyPos() + penObj->SetBodyRadius() * cWallObj->SetWallNormal();
		colVector->push_back(col);
	}
}

void HaColTest::Check(vector<Collision> *colVector,HaRacket *racObj,HaPenguin *penObj){
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
		AddHitSound(racObj,penObj);
	}
	if (fabs((rackPos - bodyPos).norm()) < rackRadius + bodyRadius){
		depth = (rackRadius + bodyRadius) - fabs((rackPos - bodyPos).norm());
		col.obj1 = racObj;
		col.obj2 = penObj;
		col.colDepth = depth * (rackPos - bodyPos).unit();
		col.colPos = bodyPos + (bodyRadius - depth / 2) * (rackPos - bodyPos).unit();
		colVector->push_back(col);
		AddHitSound(racObj,penObj);
	}
}

void HaColTest::Check(vector<Collision> *colVector,HaRacket *racObj,HaWall *walObj){
	float disRac = (racObj->SetRacketPos() - walObj->SetWallPos()) * walObj->SetWallNormal();
	Collision col;

	if (disRac < racObj->SetRacketRadius()){
		col.obj1 = racObj;
		col.obj2 = walObj;
		col.colDepth = (racObj->SetRacketRadius() - disRac) * walObj->SetWallNormal();
		col.colPos = racObj->SetRacketPos() + racObj->SetRacketRadius() * (-walObj->SetWallNormal());
		colVector->push_back(col);
	}
}

void HaColTest::Check(vector<Collision> *colVector,HaFloor *floorObj,HaPenguin *penObj){
	float disHead = (penObj->SetHeadPos() - floorObj->SetWallPos()) * floorObj->SetWallNormal();
	float disBody = (penObj->SetBodyPos() - floorObj->SetWallPos()) * floorObj->SetWallNormal();
	Collision col;

	if (disHead < penObj->SetHeadRadius()){
		col.obj1 = floorObj;
		col.obj2 = penObj;
		col.colDepth = (penObj->SetHeadRadius() - disHead) * (-floorObj->SetWallNormal());
		col.colPos = penObj->SetHeadPos() + penObj->SetHeadRadius() * (-floorObj->SetWallNormal());
		colVector->push_back(col);
	}
	if (disBody < penObj->SetBodyRadius()){
		col.obj1 = floorObj;
		col.obj2 = penObj;
		col.colDepth = (penObj->SetBodyRadius() - disBody) * (-floorObj->SetWallNormal());
		col.colPos = penObj->SetBodyPos() + penObj->SetBodyRadius() * (-floorObj->SetWallNormal());
		colVector->push_back(col);
	}
}

void HaColTest::Check(vector<Collision> *colVector,HaFloor *floorObj,HaOppPenguin *penObj){
	float disHead = (penObj->SetHeadPos() - floorObj->SetWallPos()) * floorObj->SetWallNormal();
	float disBody = (penObj->SetBodyPos() - floorObj->SetWallPos()) * floorObj->SetWallNormal();
	Collision col;

	if (disHead < penObj->SetHeadRadius()){
		col.obj1 = floorObj;
		col.obj2 = penObj;
		col.colDepth = (penObj->SetHeadRadius() - disHead) * (-floorObj->SetWallNormal());
		col.colPos = penObj->SetHeadPos() + penObj->SetHeadRadius() * (-floorObj->SetWallNormal());
		colVector->push_back(col);
	}
	if (disBody < penObj->SetBodyRadius()){
		col.obj1 = floorObj;
		col.obj2 = penObj;
		col.colDepth = (penObj->SetBodyRadius() - disBody) * (-floorObj->SetWallNormal());
		col.colPos = penObj->SetBodyPos() + penObj->SetBodyRadius() * (-floorObj->SetWallNormal());
		colVector->push_back(col);
	}
}

void HaColTest::Check(vector<Collision> *colVector,HaFloor *floorObj,HaPuck *puckObj){
	float disHead = (puckObj->SetHeadPos() - floorObj->SetWallPos()) * floorObj->SetWallNormal();
	float disBody = (puckObj->SetBodyPos() - floorObj->SetWallPos()) * floorObj->SetWallNormal();
	Vec3f headPosDash1 = Vec3f(puckObj->SetHeadPos().X(), floorObj->GetPosture().PosY(), puckObj->SetHeadPos().Z()) - floorObj->SetGoalPos1();
	Vec3f headPosDash2 = Vec3f(puckObj->SetHeadPos().X(), floorObj->GetPosture().PosY(), puckObj->SetHeadPos().Z()) - floorObj->SetGoalPos2();
	Vec3f bodyPosDash1 = Vec3f(puckObj->SetBodyPos().X(), floorObj->GetPosture().PosY(), puckObj->SetBodyPos().Z()) - floorObj->SetGoalPos1();
	Vec3f bodyPosDash2 = Vec3f(puckObj->SetBodyPos().X(), floorObj->GetPosture().PosY(), puckObj->SetBodyPos().Z()) - floorObj->SetGoalPos2();
     
	Collision col;

	if (puckObj->SetBodyPos().Z() > -lzback){					//fieldの前と後ろでゴール判定を分けている

		if (headPosDash1.norm() > floorObj->SetGoalRad()){         //goal1とpuckのhead
			if (disHead < puckObj->SetHeadRadius() ){
				col.obj1 = floorObj;
				col.obj2 = puckObj;
				col.colDepth = (puckObj->SetHeadRadius() - disHead) * (-floorObj->SetWallNormal());
				col.colPos = puckObj->SetHeadPos() + puckObj->SetHeadRadius() * (-floorObj->SetWallNormal());
				colVector->push_back(col);
			}
		}
		else {
			if (puckObj->SetHeadPos().Y() < floorObj->SetGoalPos1().Y()){
				float headDepth1 = headPosDash1.norm() - (floorObj->SetGoalRad() - puckObj->SetHeadRadius());
				if (headDepth1 > 0){
					col.obj1 = floorObj;
					col.obj2 = puckObj;
					col.colDepth = headDepth1 * headPosDash1.unit();
					col.colPos = puckObj->SetHeadPos() + puckObj->SetHeadRadius() * headPosDash1.unit();
					colVector->push_back(col);
				}
			}
/*			else{
				Vec3f headEdgePos1 = floorObj->SetGoalPos1() + floorObj->SetGoalRad() * headPosDash1.unit();
				Vec3f vecHtoE1 = headEdgePos1 - puckObj->SetHeadPos();
				Vec3f headColPos1 = puckObj->SetHeadPos() + puckObj->SetHeadRadius() * vecHtoE1.unit();
				if (vecHtoE1.norm() < puckObj->SetHeadRadius()){
					col.obj1 = floorObj;
					col.obj2 = puckObj;
					col.colDepth =0.001* headColPos1 - vecHtoE1;
					col.colPos = headColPos1;
					colVector->push_back(col);
					}
				}
*/		}

		if (bodyPosDash1.norm() > floorObj->SetGoalRad()){         //goal1とpuckのbody
			if (disBody < puckObj->SetBodyRadius() ){
				col.obj1 = floorObj;
				col.obj2 = puckObj;
				col.colDepth = (puckObj->SetBodyRadius() - disBody) * (-floorObj->SetWallNormal());
				col.colPos = puckObj->SetBodyPos() + puckObj->SetBodyRadius() * (-floorObj->SetWallNormal());
				colVector->push_back(col);
			}
		}
		else {
			if (puckObj->SetBodyPos().Y() < floorObj->SetGoalPos1().Y()){
				float bodyDepth1 = bodyPosDash1.norm() - (floorObj->SetGoalRad() - puckObj->SetBodyRadius());
				if (bodyDepth1 > 0){
					col.obj1 = floorObj;
					col.obj2 = puckObj;
					col.colDepth = bodyDepth1 * bodyPosDash1.unit();
					col.colPos = puckObj->SetBodyPos() + puckObj->SetBodyRadius() * bodyPosDash1.unit();
					colVector->push_back(col);
				}
			}
/*			else{
				Vec3f bodyEdgePos1 = floorObj->SetGoalPos1() + floorObj->SetGoalRad() * bodyPosDash1.unit();
				Vec3f vecBtoE1 = bodyEdgePos1 - puckObj->SetBodyPos();
				Vec3f bodyColPos1 = puckObj->SetBodyPos() + puckObj->SetBodyRadius() * vecBtoE1.unit();
				if (vecBtoE1.norm() < puckObj->SetBodyRadius()){
					col.obj1 = floorObj;
					col.obj2 = puckObj;
					col.colDepth = 0.001 * bodyColPos1 - vecBtoE1;
					col.colPos = bodyColPos1;
					colVector->push_back(col);
				}
			}
*/		}
	}

	else{
	
	if (headPosDash2.norm() > floorObj->SetGoalRad()){				//goal2とpuckのhead
		if (disHead < puckObj->SetHeadRadius() ){
			col.obj1 = floorObj;
			col.obj2 = puckObj;
			col.colDepth = (puckObj->SetHeadRadius() - disHead) * (-floorObj->SetWallNormal());
			col.colPos = puckObj->SetHeadPos() + puckObj->SetHeadRadius() * (-floorObj->SetWallNormal());
			colVector->push_back(col);
		}
	}
	else {
		if (puckObj->SetHeadPos().Y() < floorObj->SetGoalPos2().Y()){
			float headDepth2 = headPosDash2.norm() - (floorObj->SetGoalRad() - puckObj->SetHeadRadius());
			if (headDepth2 > 0){
				col.obj1 = floorObj;
				col.obj2 = puckObj;
				col.colDepth = headDepth2 * headPosDash2.unit();
				col.colPos = puckObj->SetHeadPos() + puckObj->SetHeadRadius() * headPosDash2.unit();
				colVector->push_back(col);
			}
		}
/*		else{
			Vec3f headEdgePos2 = floorObj->SetGoalPos2() + floorObj->SetGoalRad() * headPosDash2.unit();
			Vec3f vecHtoE2 = headEdgePos2 - puckObj->SetHeadPos();
			Vec3f headColPos2 = puckObj->SetHeadPos() + puckObj->SetHeadRadius() * vecHtoE2.unit();
			if (vecHtoE2.norm() < puckObj->SetHeadRadius()){
				col.obj1 = floorObj;
				col.obj2 = puckObj;
				col.colDepth = 0.001 * headColPos2 - vecHtoE2;
				col.colPos = headColPos2;
				colVector->push_back(col);
			}
		}
*/	}
	
	if (bodyPosDash2.norm() > floorObj->SetGoalRad()){         //goal2とpuckのbody
		if (disBody < puckObj->SetBodyRadius() ){
			col.obj1 = floorObj;
			col.obj2 = puckObj;
			col.colDepth = (puckObj->SetBodyRadius() - disBody) * (-floorObj->SetWallNormal());
			col.colPos = puckObj->SetBodyPos() + puckObj->SetBodyRadius() * (-floorObj->SetWallNormal());
			colVector->push_back(col);
		}
	}
	else {
		if (puckObj->SetBodyPos().Y() < floorObj->SetGoalPos2().Y()){
			float bodyDepth2 = bodyPosDash2.norm() - (floorObj->SetGoalRad() - puckObj->SetBodyRadius());
			if (bodyDepth2 > 0){
				col.obj1 = floorObj;
				col.obj2 = puckObj;
				col.colDepth = bodyDepth2 * bodyPosDash2.unit();
				col.colPos = puckObj->SetBodyPos() + puckObj->SetBodyRadius() * bodyPosDash2.unit();
				colVector->push_back(col);
			}
		}
/*		else{
			Vec3f bodyEdgePos2 = floorObj->SetGoalPos2() + floorObj->SetGoalRad() * bodyPosDash2.unit();
			Vec3f vecBtoE2 = bodyEdgePos2 - puckObj->SetBodyPos();
			Vec3f bodyColPos2 = puckObj->SetBodyPos() + puckObj->SetBodyRadius() * vecBtoE2.unit();
			if (vecBtoE2.norm() < puckObj->SetBodyRadius()){
				col.obj1 = floorObj;
				col.obj2 = puckObj;
				col.colDepth = 0.001 * bodyColPos2 - vecBtoE2;
				col.colPos = bodyColPos2;
				colVector->push_back(col);
			}
		}
*/	}
}
}

void HaColTest::Check(vector<Collision> *colVector,HaColObject *colObj1,HaColObject *colObj2){
	HaPenguin *pen1,*pen2;
	HaOppPenguin *oppPen;
	HaPuck *puck;
	HaWall *wall;
	HaFloor *floor;
	HaCenterWall *cWall;
	HaRacket *rack;
	if (dynamic_cast<HaPenguin*>(colObj1) != NULL && dynamic_cast<HaPenguin*>(colObj2) != NULL){
		pen1 = (HaPenguin *) colObj1;
		pen2 = (HaPenguin *) colObj2;
		Check(colVector,pen1,pen2);
	}
	else if (typeid(*colObj1) == typeid(HaWall) && dynamic_cast<HaPenguin*>(colObj2) != NULL){
		wall = (HaWall *) colObj1;
		pen1 = (HaPenguin *) colObj2;
		Check(colVector,wall,pen1);
	}
	else if (typeid(*colObj1) == typeid(HaFloor) && typeid(*colObj2) == typeid(HaPenguin)){
		floor = (HaFloor *) colObj1;
		pen1 = (HaPenguin *) colObj2;
		Check(colVector,floor,pen1);
	}
	else if (typeid(*colObj1) == typeid(HaFloor) && typeid(*colObj2) == typeid(HaOppPenguin)){
		floor = (HaFloor *) colObj1;
		pen1 = (HaOppPenguin *) colObj2;
		Check(colVector,floor,pen1);
	}
	else if (typeid(*colObj1) == typeid(HaFloor) && typeid(*colObj2) == typeid(HaPuck)){
		floor = (HaFloor *) colObj1;
		puck = (HaPuck *) colObj2;
		Check(colVector,floor,puck);
	}
	else if (typeid(*colObj1) == typeid(HaCenterWall) && typeid(*colObj2) == typeid(HaPenguin)){
		cWall = (HaCenterWall *) colObj1;
		pen1 = (HaPenguin *) colObj2;
		Check(colVector,cWall,pen1);
	}
	else if (typeid(*colObj1) == typeid(HaCenterWall) && typeid(*colObj2) == typeid(HaOppPenguin)){
		cWall = (HaCenterWall *) colObj1;
		oppPen = (HaOppPenguin *) colObj2;
		Check(colVector,cWall,oppPen);
	}
	else if (dynamic_cast<HaRacket*>(colObj1) != NULL && dynamic_cast<HaPenguin*>(colObj2) != NULL){
		rack = (HaRacket *) colObj1;
		pen1 = (HaPenguin *) colObj2;
		Check(colVector,rack,pen1);
	}
	else if (dynamic_cast<HaRacket*>(colObj1) != NULL && dynamic_cast<HaWall*>(colObj2) != NULL){
		rack = (HaRacket *) colObj1;
		wall = (HaWall *) colObj2;
		Check(colVector,rack,wall);
	}
}

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
		glLightfv(GL_LIGHT0,GL_POSITION,light0pos);			//処理速度が遅い場合は消去

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
			CleanupPhysics();
			Exit();
		}
	}
};

// 力覚を計算するためのグローバル関数
// timerFunc()から呼ばれる
void Step(){
	static vector<Collision> collisions;
	collisions.clear();

	for(int i=0; i<objects.size(); i++){
		if(objects[i]->GetCollisionable()){
			for(int j=i+1; j<objects.size(); j++){
				if(objects[j]->GetCollisionable())
					HaColTest::Check(&collisions, objects[i], objects[j]);
			}
		}
	}
	for(int i=0; i<collisions.size(); i++){
		collisions[i].SetForce();
	}
	for(int i=0; i<objects.size(); i++){
		objects[i]->Update();						
	}
}

void WriteMemory(){
	if (writeCycle >= (1.0/60)/dtHaptic){										//20回に一回書き込む
		if (memories.graphicRead == memories.graphicShouldRead){
			if (memories.graphicRead == 1) memoryNo = 0;                  //書き込み可能なメモリーを選択
			else memoryNo = 1;
			
			memories[memoryNo].Write(&objects);
			
			memories.graphicShouldRead = memoryNo;						  //グラフィック側が読むメモリ
			writeCycle = 0;
		}
	}else{
		writeCycle++;
	}
}

void ReadCommand(){
	if (commands.hapticRead) return;
	for (int j = 0; j < commands[commands.hapticShouldRead].size() ; j++ ){
		commands[commands.hapticShouldRead][j]->Run();
	}
	commands[commands.hapticShouldRead].clear();
	commands.hapticRead = true;						//	読み込み完了
}

WBMMTimer timer;
void timerFunc(void* arg){
//	cout << "timer:" << timeGetTime() << " ";
//	cout <<  memories.data->racket.GetPosture() <<endl;

	Step();
//#ifdef USE_GRAPHICS
	if (env.bUseGraphics) {
		WriteMemory();
		ReadCommand();
	}
//#endif
};

//タイマーの初期化
void InitTimer()	{
	timer.Set(timerFunc, NULL);
#ifdef _DEBUG
	timer.Resolution(50);
	timer.Interval(50);
#else
	timer.Resolution(4);
	timer.Interval(4);
#endif

#ifdef _DEBUG
	timer.Thread();
#else
	timer.Create();
#endif
}


void CalibSpidar(){
	spidar.Calib();
}


void Register_Mouse() {
	cout << "マウスをSPIDARとして使用します" <<endl;
	int i;
	for (i=0;i<MOUSE_SAMPLE;i++) {
		env.mousez[i] = 0;
	}
}

//デバイスを登録
void InitDevice() {
	switch (env.iSpidar_type) {
		case Env_NO_SPIDAR:
			Register_Mouse();
			break;
		case Env_ISA_BIG_SPIDAR:
			//ISA環境の登録
			devMan.RPool().Register(new DRContecIsaDa(0x300));
			devMan.RPool().Register(new DRContecIsaCounter(0x200));
			devMan.RPool().Register(new DRContecIsaCounter(0x210));
			break;
		case Env_USB_SPIDAR_G:
		  	//USB2.0環境の登録
			devMan.RPool().Register(new DRUsb20Simple(0));
			break;
		default:
			Register_Mouse();
			break;
	}
	devMan.Init();
	std::cout << devMan;
}

//SPIDARの初期化
void InitSpidar() {

	float PX,PY,PZ;	//廃止予定
	PX = env.px;
	PY = env.py;
	PZ = env.pz;

	//case文に対応するためのとりあえずの初期化
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(てきとう)
		{Vec3f(-PX,-PY, PZ),	Vec3f( PX,-PY,-PZ), Vec3f( PX, PY, PZ), Vec3f(-PX, PY,-PZ)},
		{Vec3f(-PX,-PY, PZ),	Vec3f( PX,-PY,-PZ), Vec3f( PX, PY, PZ), Vec3f(-PX, PY,-PZ)}
	};

	switch (env.iSpidar_type) {
		case Env_NO_SPIDAR:
			env.v3DMouse = Vec3f(0.0f,0.0f,0.0f);
			env.vSpidar = env.v3DMouse;
			env.vSpidarLast = env.vSpidar;
			env.vSpidarForce = Vec3f(0.0f,0.0f,0.0f);
			env.vSpidarForceLast = env.vSpidarForce;
			break;
		case Env_ISA_BIG_SPIDAR:
			cout<<"等身大環境\n";
			/* size memo
			PX = 1.20f;	//	x方向の辺の長さ/2
			PY = 1.12f;	//	y方向の辺の長さ/2
			PZ = 0.99f;	//	z方向の辺の長さ/2
			*/
			//ISA環境
			cout<<"ISA(Contex) SPIDAR\n";
			//8F環境のSPIDAR配置2003/02/21調整
			//SubScreenを正面にして、前左下、前右上、後左上、後右下の配置
			motorPos[0][0] = Vec3f( PX, PY, PZ);
			motorPos[0][1] = Vec3f(-PX, PY,-PZ);
			motorPos[0][2] = Vec3f(-PX,-PY, PZ);
			motorPos[0][3] = Vec3f( PX,-PY,-PZ);


			//ダミー用SPIDAR初期位置（てきとう）
			motorPos[1][0] = Vec3f( PX, PY,-PZ);
			motorPos[1][1] = Vec3f(-PX, PY, PZ);
			motorPos[1][2] = Vec3f(-PX,-PY,-PZ);
			motorPos[1][3] = Vec3f( PX,-PY, PZ);

//			bSpidarExist = spidar.Init(devMan, motorPos[0], 0.3653f, 2.924062107079e-5f, 0.5f, 20.0f);
			env.bSpidarExist = spidar.Init(devMan, motorPos[0], 0.3653f, 2.924062107079e-5f, 0.5f, 20.0f);
			//SPIDAR-Gであまった系のためのダミー出力
			spidar2.Init(devMan, motorPos[4], 0.3653f, 2.924062107079e-5f, 0.5f, 20.0f);
			std::cout<<"DeviceName = " << spidar.Motor()[0].da->RealDevice()->Name()<< std::endl;

			break;

		case Env_USB_SPIDAR_G:
			//デスクトップ環境
			cout<<"デスクトップ環境\n";
/*
			PX = 0.265f;	//	x方向の辺の長さ/2
			PY = 0.265f;	//	y方向の辺の長さ/2
			PZ = 0.265f;	//	z方向の辺の長さ/2
*/
			cout<<"USB2.0 SPIDAR-G\n";
			//USB2.0 SPIDAR-G 環境 2003/02/11
/*			Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
				{Vec3f(-PX,-PY, PZ),	Vec3f( PX,-PY,-PZ), Vec3f( PX, PY, PZ), Vec3f(-PX, PY,-PZ)}
			};*/
/*			motorPos[0][0] = Vec3f(-PX,-PY, PZ);
			motorPos[0][1] = Vec3f( PX,-PY,-PZ);
			motorPos[0][2] = Vec3f( PX, PY, PZ);
			motorPos[0][3] = Vec3f(-PX, PY,-PZ);
*/
			motorPos[0][0] = Vec3f(-PX, PY, PZ);
			motorPos[0][1] = Vec3f(PX, PY,-PZ);
			motorPos[0][2] = Vec3f(-PX,-PY, -PZ);
			motorPos[0][3] = Vec3f( PX,-PY,PZ);

			//メイン出力の0-3番
			env.bSpidarExist = spidar.Init(devMan, motorPos[0], 0.3653f, -2.924062107079e-5f, 0.5f, 20.0f);
			//SPIDAR-Gであまった系のためのダミー出力
			spidar2.Init(devMan, motorPos[4], 0.3653f, -2.924062107079e-5f, 0.5f, 20.0f);
			std::cout<<"DeviceName = " << spidar.Motor()[0].da->RealDevice()->Name()<< std::endl;
			break;

		default:
			break;
	}

/* sample
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
	bSpidarExist = spidar.Init(devMan, motorPos[0], 0.3653f, 2.924062107079e-5f, 0.5f, 20.0f);
*/

	CalibSpidar(); //最初のキャリブレーション
}

//壁を配置
void InitWalls() {

	Affinef afLeft,afRight,afUp,afDown,afBack,afCenter,afFront;
	Vec3f sizeDown,sizeUp,sizeLeft,sizeRight,sizeBack,sizeCenter,sizeFront;

	//壁の初期設定
	afDown.Pos() = Vec3f(-lx,-lydown,-lzback);
	afUp.Pos() = Vec3f(lx,ly,-lzback);
	afLeft.Pos() = Vec3f(-lx,ly,-lzback);
	afRight.Pos() = Vec3f(lx,-lydown,-lzback);
	afBack.Pos() = Vec3f(lx,ly,-(lzfront+2*lzback));
	afCenter.Pos() = Vec3f(lx,ly,-lzback);
	afFront.Pos() = Vec3f(-lx,-lydown,lzfront);

	//法線は中心（原点）に向かう方向とする
	afDown.Rot() = Matrix3f(ez,ex,ey);
	afUp.Rot() = Matrix3f(ez,-ex,-ey);
	afLeft.Rot() = Matrix3f(ez,-ey,ex);
	afRight.Rot() = Matrix3f(ez,ey,-ex);
	afBack.Rot() = Matrix3f(-ex,-ey,ez);  
	afCenter.Rot() = Matrix3f(-ex,-ey,ez);
	afFront.Rot() = Matrix3f(ex,ey,-ez);	

	sizeDown = Vec3f(lzback+lzfront,2*lx,0);
	sizeUp = Vec3f(lzback+lzfront,2*lx,0);
	sizeLeft = Vec3f(lzback+lzfront,2*ly,0);
	sizeRight = Vec3f(lzback+lzfront,2*ly,0);
	sizeBack = Vec3f(2*lx,2*lydown,0);
	sizeCenter = Vec3f(2*lx,2*lydown,0);
	sizeFront = Vec3f(2*lx,2*lydown,0);

	Vec3f goalfront = Vec3f(0,-lydown,lzfront);     //ゴールの位置・大きさ
	Vec3f goalback = Vec3f(0,-lydown,-(lzfront+2*lzback));
//	float goalr = goal;


	objects.Push(new HaRacket); //これを先に設定しないとあたり判定がなくなる
								//debugYI ↑なぜだろう？

	objects.Push(new HaFloor(afDown,sizeDown,goalfront,goalback,goalr));
//	objects.Push(new HaWall(afDown,sizeDown));
	objects.Push(new HaWall(afUp,sizeUp));
	objects.Push(new HaWall(afLeft,sizeLeft));
	objects.Push(new HaWall(afRight,sizeRight));
	objects.Push(new HaWall(afBack,sizeBack));
	objects.Push(new HaWall(afFront,sizeFront));
	objects.Push(new HaCenterWall(afCenter,sizeCenter));

}

void ChrSetPenguin(int num) {
	Affinef afPenguin;
	afPenguin.Pos() = InitPenguinPos[num];
	afPenguin.Rot() = Matrix3f(ex,ey,ez);
	objects.Push(new HaPenguin(afPenguin));
}

void InitChrs() {
	Affinef afPen1,afPen2,afPen3,afPuck1,afPuck2,afPuck3,afPuck4;

	afPen1.Pos() = startPen1;
	afPen2.Pos() = startPen2;
	afPen3.Pos() = startPen3;
	afPen1.Rot() = Matrix3f(ex,ey,ez);
	afPen2.Rot() = Matrix3f(ex,ey,ez);
	afPen3.Rot() = Matrix3f(ex,ey,ez);

	afPuck1.Pos() = startPu1;
	afPuck2.Pos() = startPu2;
	afPuck3.Pos() = startPu3;
	afPuck4.Pos() = startPu4;
	afPuck1.Rot() = Matrix3f(ex,ey,ez);
	afPuck2.Rot() = Matrix3f(ex,ey,ez);
	afPuck3.Rot() = Matrix3f(ex,ey,ez);
	afPuck4.Rot() = Matrix3f(ex,ey,ez);

	objects.puckBase=objects.size();

	HaPuck *hp1 = new HaPuck(afPuck1);
	//hp1->SetColEnabled(false);
	HaPuck *hp2 = new HaPuck(afPuck2);
	//hp2->SetColEnabled(false);
	HaPuck *hp3 = new HaPuck(afPuck3);
	//hp3->SetColEnabled(false);
	HaPuck *hp4 = new HaPuck(afPuck4);
	//hp4->SetColEnabled(false);
	
	objects.Push(hp1);
	objects.Push(hp2);
	objects.Push(hp3);
	objects.Push(hp4);

/*
	objects.Push(new HaPuck(afPuck1));
	objects.Push(new HaPuck(afPuck2));
	objects.Push(new HaPuck(afPuck3));
	objects.Push(new HaPuck(afPuck4));
*/

	objects.penBase=objects.size();
//	ChrSetPenguin(0);
	objects.Push(new HaPenguin(afPen1));

	objects.oppPenBase=objects.size();
//	ChrSetPenguin(1);
//	ChrSetPenguin(2);
	objects.Push(new HaOppPenguin(afPen2));
	objects.Push(new HaOppPenguin(afPen3));
}

/*
void InitPhysics(bool bDisplay){

	InitDevice();	//デバイスを登録
	InitSpidar();	//SPIDARの初期化
	InitWalls();		//壁・ゴールを配置
	InitChrs();		//キャラクタを配置
	InitTimer();	//タイマーの初期化

	if (bDisplay){
		TestView view;
		view.WindowPosition(10,10, 300, 200);
		view.Init("OpenGL Window");
		view.Run();
	}
}
*/

//安全に終了
void CleanupPhysics(){
	Vec3f force;

	env.bHaptics = false;
	force.clear();
	spidar.SetForce(force,ratio);
	spidar.SetMinForce();
	CalibSpidar(); //安全のために、値は狂うがキャリブレーション

	timer.Release();
}

///////////////////////////////////////////////////////////////////////////
void InitHaptics(){

	InitDevice();	//デバイスを登録
	InitSpidar();	//SPIDARの初期化
	InitWalls();		//壁・ゴールを配置
	InitChrs();		//キャラクタを配置
	InitTimer();	//タイマーの初期化
}
///////////////////////////////////////////////////////////////////////////

//安全対策
void Safety() {
	Vec3f	diff,fdiff;
	diff = env.vSpidarLast - env.vSpidar;
	fdiff = env.vSpidarForceLast - env.vSpidarForce;

	//微小な動きの検出
	if (env.iSpidar_type == Env_NO_SPIDAR) {
		//MOUSEモードのときは常に握っていることにする
			env.iStaticFrame = 0;
			env.iGraspFrame ++;
	} else {
		if (diff.norm()<=0.00001) {
			env.iStaticFrame ++;
			env.iGraspFrame  = 0;
		} else {
			env.iStaticFrame = 0;
			env.iGraspFrame ++;
		}
	}

	//	cout << diff.norm() << " sf=" <<env.iStaticFrame << endl;
	if (env.iStaticFrame>=10000) {
		cout << "しばらく入力がないので停止します" << endl;
		env.bEmergency = true;
	}

//	cout << env.vSpidarForce.norm() << endl; //max30
	if (env.vSpidarForce.norm()>=30.0f) {
		cout << "強い力が出たので停止します" << endl;
		cout << "SPIDARへの指令値=" <<env.vSpidarForce.norm() << endl;
		env.bEmergency = true;
	}


	if (fdiff.norm()>=2.0f) {
		cout << "暴走を検出したので停止します" << endl;
		cout << "前フレームとの指令値の差.norm()=" <<fdiff.norm() << endl;
		env.bEmergency = true;
	}

	if (env.bEmergency) {
		CleanupPhysics();
		exit(0);
	}
}