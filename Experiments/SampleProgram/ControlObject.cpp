// ControlObject.cpp: ControlObject クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "ControlObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ControlObject::ControlObject()
{
	soObj1=NULL;
	soObj2=NULL;
	joObj1=NULL;
	joObj2=NULL;

	bObj = false;

	tete[0] = 0;
	tete[1] = 1;
	tete[2] = 2;
	//col = green;
	first = true;

	antei = false;
}

ControlObject::~ControlObject()
{

}

void ControlObject::ReLoad(){
	bObj = false;
	soObj1=NULL;
	soObj2=NULL;
	joObj1=NULL;
	joObj2=NULL;

	first = true;
	solids.clear();
	antei = false;
}

void ControlObject::FindObj(SGScene* scene){
	if(scene){
		scene->FindObject(soObj1, "soPole1");
		scene->FindObject(soObj2, "soPole2");
		
		if(soObj1 && soObj2){
			solids.push_back(soObj1);
			solids.push_back(soObj2);
			bObj = true;
		}
	}
}

void ControlObject::Draw(GRRender* render){
	render->SetMaterial(Mat(Spr::Vec4f(1,1,0,1)));
	render->SetLineWidth(10);
	Vec3f v = vh_zmp;
	render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
}

void ControlObject::OnKeyDown(UINT& nChar){
//0,1,2,8,9,K,T,H,O,D,F,G,Pは使用済み

	//　テスト
	if(nChar == 'E'){
		// 質量中心をセット
		//SetCenterMass();

		//オブジェクトのスケール変換
		//SetScale();
		//joObj1->SetAccel(1.0);
		//soObj1->AddForce(Vec3f(0.1,0.0,0.0));
		antei = true;

	}

	if(nChar == 'V'){
		soObj1->AddForce(Vec3f(0.1,0.0,0.0));
	}

///////////　力を加える　/////////////////

	//Y軸プラス
	if(nChar == '3'){
		Vec3d del;
		Vec3d pos;
		del = Vec3d(0.0,0.1,0.0);
		pos = soObj1->GetCenterPosition();
		soObj1->SetCenterPosition(pos+del);
		//soObj1->GetOrientation();
	}
	//Y軸マイナス
	if(nChar == '4'){
		Vec3d del;
		Vec3d pos;
		del = Vec3d(0.0,-0.1,0.0);
		pos = soObj1->GetCenterPosition();
		soObj1->SetCenterPosition(pos+del);
	}
	//Ｘ軸回転
	if(nChar == '5'){
		Matrix3d now;
		now = soObj1->GetRotation();
		now = Affinef::Rot(Radf(5),now.Ex()) * now;
		soObj1->SetRotation(now);
	}
	//Ｙ軸回転
	if(nChar == '6'){
		Matrix3d now;
		now = soObj1->GetRotation();
		now = Affinef::Rot(Radf(5),now.Ey()) * now;
		soObj1->SetRotation(now);
	}
	//Ｚ軸回転
	if(nChar == '7'){
		Matrix3d now;
		now = soObj1->GetRotation();
		now = Affinef::Rot(Radf(5),now.Ez()) * now;
		soObj1->SetRotation(now);
	}
}


void ControlObject::SetScale(){

	Spr::Affinef A;
	Spr::Affinef B;
	B.xx *= 2;
	B.yy *= 2;
	B.zz *= 2;

	SGFrame* transFrame = DCAST(SGFrame, soObj1->GetFrame()->ChildObject(0));
	A = transFrame->GetPosture();
	A = A*B;
	transFrame->SetPosture(A);
}

void ControlObject::SetCenterMass(){

	Vec3d del;
	del = Vec3d(0.2,0.2,0.2);
	soObj1->SetCenter(del);

}

void ControlObject::CalcZMP(SGScene* scene){

	GetAccel(scene->GetTimeStep());

	double x_zmp1 = 0;
	double x_zmp2 = 0;
	double x_zmp3 = 0;

	double z_zmp1 = 0;
	double z_zmp2 = 0;
	double z_zmp3 = 0;

	for(int i = 0; i < solids.size() ; ++i){
		double solidMass = solids[i]->GetMass();
		x_zmp1 += solidMass*(Accel[i].y - g_y) * solids[i]->GetCenterPosition().x;
		x_zmp2 += solidMass*(Accel[i].x - g_x) * solids[i]->GetCenterPosition().y;
		x_zmp3 += solidMass*(Accel[i].y - g_y);

		z_zmp1 += solidMass*(Accel[i].y - g_y) * solids[i]->GetCenterPosition().z;
		z_zmp2 += solidMass*(Accel[i].z - g_z) * solids[i]->GetCenterPosition().y;
		z_zmp3 += solidMass*(Accel[i].y - g_y);
	}
	vh_zmp.x = (x_zmp1 - x_zmp2)/x_zmp3;
	vh_zmp.z = (z_zmp1 - z_zmp2)/z_zmp3;

	Accel.clear();

}
void ControlObject::GetAccel(double dt){
	if(first){
		for(int i = 0; i < solids.size(); i++){
			preVelocity.push_back(Spr::Vec3d(0.0,0.0,0.0));
		}
		first = false;
	}

	for(int i = 0; i < solids.size(); i++){
		nowVelocity = solids[i]->GetVelocity();
		Accel.push_back((nowVelocity - preVelocity[i])/dt);
		preVelocity[i] = nowVelocity;
	}
}

void ControlObject::Step(SGScene* scene,GRRender* render){

	if(!bObj){
		FindObj(scene);
	}

	if(bObj){
		CalcZMP(scene);
		Draw(render);
		if(antei){
			if(abs(soObj1->GetCenterPosition().x)>1e-5)
			soObj1->AddForce(Vec3f(0.0,soObj1->GetMass()*9.8*abs(vh_zmp.x/soObj1->GetCenterPosition().x),0.0));
		}
		//joObj1->SetAccel(1.0);
		//soObj1->AddForce(Vec3d(1.1,0.0,10.0));
	}

}
