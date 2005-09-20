// TUser.cpp: TUser クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "virtualhuman.h"
#include "TUser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace Spr;

inline bool IsValid(const Vec3f& v){
	return v.square() < Square(900);
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Spr::Vec3f TUserSpring::GetPos(){
	if (solid) return solid->GetFrame()->GetPosture() * pos;
	return Spr::Vec3f();
}
Spr::Vec3f TUserSpring::GetVel(){
	if (solid){
		Spr::Vec3f gPos = solid->GetOrientation() * pos;
		Spr::Vec3f vel = solid->GetVelocity() + (solid->GetAngularVelocity() ^ gPos);
		return vel;
	};
	return Spr::Vec3f();
}
Vec3f TUserSpring::GetForce(){
	return force;
}

TUser::TUser(){
	bTUser = true;
//	bTUser = false;
}

TUser::~TUser(){

}

void TUser::ConnectSLCamera(){
	if(slc.InitSLC()){
		bSLCamera = true;
		DSTR << "Connection SLCamera!" << std::endl;
	}
}

void TUser::DisconnectSLCamera(){
	slc.CloseSLCamera();
	bSLCamera = false;
	DSTR << "Disconnection SLCamera!" << std::endl;
}

void TUser::OnKeyDown(UINT nChar){
	if ( nChar == 'L' ){
		if(bTUser){
			if(bSLCamera) DisconnectSLCamera();	//	ラベリングカメラの切断
			else          ConnectSLCamera();	//	ラベリングカメラの再接続
		}
	}

}

extern Spr::GRMaterialData Mat(Spr::Vec4f c);

bool TUser::Draw(Spr::GRRender* render){
	if(!bTUser) return false;

	render->SetLineWidth(10);
	for(int i = 0; i < usprings.size(); i++){
		render->SetMaterial(Mat(Spr::Vec4f(0,1,1,1)));
		Spr::Vec3f pos = usprings[i].targetPos;
		render->DrawDirect(Spr::GRRender::POINTS, &pos, &pos+1);
	}
	return true;
}

void TUser::SetUserSprings(Spr::PHHuman& phHuman){
	usprings.clear();
	//	腰(基本立ち位置に固定)
	usprings.push_back(TUserSpring(phHuman.solids[0], Spr::Vec3f(phHuman.GetSolidInfo(0).scale.X(), 0, 0)));
	//	頭頂
	usprings.push_back(TUserSpring(phHuman.solids[3], Spr::Vec3f(0, phHuman.GetSolidInfo(3).scale.Y(), 0)));
	//	右手
	usprings.push_back(TUserSpring(phHuman.solids[6], Spr::Vec3f(0, 0, 0)));
	//	左手
	usprings.push_back(TUserSpring(phHuman.solids[9], Spr::Vec3f(0, 0, 0)));
	//	右肘
	usprings.push_back(TUserSpring(phHuman.solids[4], Spr::Vec3f(0, -phHuman.GetSolidInfo(4).scale.Y(), 0)));
	//	左肘
	usprings.push_back(TUserSpring(phHuman.solids[7], Spr::Vec3f(0, -phHuman.GetSolidInfo(7).scale.Y(), 0)));
	//	腰(回転防止のためもう１ヶ所)
	usprings.push_back(TUserSpring(phHuman.solids[0], Spr::Vec3f(-phHuman.GetSolidInfo(0).scale.X(), 0, 0)));

	for(int i = 0; i < usprings.size(); ++i) usprings[i].bTarget = false;
}

void TUser::SetUserSprings2(Spr::PHHuman& phUser, Spr::PHHuman& phHuman){
	usprings.clear();

	//	右手
	usprings.push_back(TUserSpring(phUser.solids[6], Spr::Vec3f(0, 0, 0)));
	//	左手
	usprings.push_back(TUserSpring(phUser.solids[9], Spr::Vec3f(0, 0, 0)));
	//	右肘
	usprings.push_back(TUserSpring(phUser.solids[4], Spr::Vec3f(0, -phUser.GetSolidInfo(4).scale.Y(), 0)));
	//	左肘
	usprings.push_back(TUserSpring(phUser.solids[7], Spr::Vec3f(0, -phUser.GetSolidInfo(7).scale.Y(), 0)));
	//	右足
	usprings.push_back(TUserSpring(phUser.solids[12], Spr::Vec3f(0, -phUser.GetSolidInfo(12).scale.Y(), 0)));
	//	左足
	usprings.push_back(TUserSpring(phUser.solids[16], Spr::Vec3f(0, -phUser.GetSolidInfo(16).scale.Y(), 0)));
	//	頭頂
	usprings.push_back(TUserSpring(phUser.solids[3], Spr::Vec3f(0, phUser.GetSolidInfo(3).scale.Y(), 0)));
	//	腰(基本立ち位置に固定)
	usprings.push_back(TUserSpring(phUser.solids[0], Spr::Vec3f( phUser.GetSolidInfo(0).scale.X(), 0, 0)));
	usprings.push_back(TUserSpring(phUser.solids[0], Spr::Vec3f(-phUser.GetSolidInfo(0).scale.X(), 0, 0)));

	//	VH を固定
	usprings.push_back(TUserSpring(phHuman.solids[12], Spr::Vec3f(0, -phHuman.GetSolidInfo(12).scale.Y(), 0)));
	usprings.push_back(TUserSpring(phHuman.solids[16], Spr::Vec3f(0, -phHuman.GetSolidInfo(16).scale.Y(), 0)));
	usprings.push_back(TUserSpring(phHuman.solids[0], Spr::Vec3f( phHuman.GetSolidInfo(0).scale.X(), 0, 0)));
	usprings.push_back(TUserSpring(phHuman.solids[0], Spr::Vec3f(-phHuman.GetSolidInfo(0).scale.X(), 0, 0)));

	for(int i = 0; i < usprings.size(); ++i) usprings[i].bTarget = false;
}

bool TUser::InputUserPosition(Spr::PHHuman& phHuman, Spr::HISpidar4* spidar, float dt, Spr::GRRender* render){
	if (!phHuman.IsLoaded()) return false;
	bool rv = true;
	const float SAFETYRATE = 0.10f;
	const float SPRING = 0.4f * SAFETYRATE;		//	バネ
	const float DAMPER = 0.9f * SAFETYRATE;		//	ダンパ

	/*if(bSLCamera){
		slc.Step(dt);
		slc.Draw(render);
	}*/
	SetTargetPosition(spidar);
	SetOneTargetPosition(&usprings[9],  Vec3f(-0.135, 0.10,-0.2), Vec3f(0,0,0));
	SetOneTargetPosition(&usprings[10], Vec3f( 0.135, 0.10,-0.2), Vec3f(0,0,0));
	SetOneTargetPosition(&usprings[11], Vec3f(-0.135, 0.95,-0.2), Vec3f(0,0,0));
	SetOneTargetPosition(&usprings[12], Vec3f( 0.135, 0.95,-0.2), Vec3f(0,0,0));
	Draw(render);

	for(int i = 0; i < usprings.size(); ++i){		
		float mass = usprings[i].solid->GetMass();
		float spring = SPRING * mass * 2 / (dt*dt);
		float damper = DAMPER * mass / (dt);

		Vec3f pos2 = usprings[i].GetPos(); // 現在のPos,Vel
		Vec3f vel2 = usprings[i].GetVel();

		/*if(bSLCamera && slc.markerNum > 0){
			if(i == 2){
				SetAttraction(&usprings[i]);
				if(usprings[i].attraction.norm() / mass > 1e3f) usprings[i].bTarget = true;
				else usprings[i].AddForce(usprings[i].attraction);
				//DSTR << usprings[i].attraction.norm() << std::endl;
			}
		}*/

		if(IsValid(usprings[i].targetPos) && usprings[i].bTarget){
			Vec3f dPos = usprings[i].targetPos - pos2;
			Vec3f dVel = usprings[i].targetVel - vel2;
			if (dVel.norm() > 1e5f) dVel = dVel.unit()*1e5f;
			if (dPos.norm() > 1e2f) dPos = dPos.unit()*1e2f;
			Vec3f force = spring*dPos + damper*dVel;
			if (force.norm() / mass > 1e6f) force = force.unit() * mass*1e6f;
			usprings[i].AddForce(force);			
		}
	}

	return rv;
}

void TUser::SetAttraction(TUserSpring* us){
	const float PCONS = 100;	// 比例定数
	Vec3f attraction = Vec3f(0,0,0);
	Vec3f dPos;
	float dis, disMin = 100;
	if(bSLCamera){
		for(int i = 0; i < slc.markerNum; ++i){
			dPos = slc.marker[i].pos - us->GetPos();
			dis = dPos.norm();
			attraction += dPos / (dis * dis);
			if(dis < disMin){
				disMin = dis;
				us->targetPos = slc.marker[i].pos;
				us->targetVel = Vec3f(0,0,0);
			}
		}
	}
	us->bTarget = false;
	us->attraction = attraction * PCONS;
}


void TUser::SetTargetPosition(Spr::HISpidar4* spidar){
	SetOneTargetPosition(&usprings[4], Vec3f( 0.135,0.1, 0.5), Vec3f(0,0,0));
	SetOneTargetPosition(&usprings[5], Vec3f(-0.135,0.1, 0.5), Vec3f(0,0,0));
	SetOneTargetPosition(&usprings[7], Vec3f( 0.135,0.95,0.5), Vec3f(0,0,0));
	SetOneTargetPosition(&usprings[8], Vec3f(-0.135,0.95,0.5), Vec3f(0,0,0));

/*	usprings[7].targetPos = Vec3f(0.0,0.92,0.5); // 腰[7]
	usprings[7].targetVel = Vec3f(0,0,0);
	usprings[7].bTarget = true;

	usprings[6].targetPos = Vec3f(0,1.72,0.5); // 頭頂[6]
	usprings[6].targetVel = Vec3f(0,0,0);
	usprings[6].bTarget = true;*/

	// Spidar のスケール調整込み 右手[0] 左手[1]
	for(int i = 0; i < 2; ++i){
		const float SCALE = 3.0f;
		Vec3f tPos, tVel, force;
		tPos = spidar[i].GetPos() * SCALE;
		tVel = spidar[i].GetVel() * SCALE;
		tPos.y += 1.4f;
		tPos.z += 0.2f;

		usprings[i].targetPos = tPos;
		usprings[i].targetVel = tVel;
		usprings[i].bTarget = true;
		//usprings[1].bTarget = false;
	}
}

void TUser::SetOneTargetPosition(TUserSpring* us, Spr::Vec3f pos, Spr::Vec3f vel){
	us->targetPos = pos;
	us->targetVel = vel;
	us->bTarget = true;
}
