#include <Base/Env.h>
#include <Solid/SOLID.h>
#include <Base/Affine.h>

#include "calcFriction.h"

using namespace std;
using namespace LVRM;

Vec3f TCalcFriction::SprPoint(Vec3f& force, Vec3f& torque){
	Matrix3f invForce;
	
	invForce.Ex() = Vec3f(0, force.Z(), -force.Y());
	invForce.Ey() = Vec3f(-force.Z(), 0, force.X());
	invForce.Ez() = Vec3f(force.Y(), -force.X(), 0);
	
	return invForce.trans() * torque;
};

void TCalcFriction::CalcFriction(Vec3f force, Vec3f torque, Vec3f normal, TPenaltyDetector& pc, 
						   		 TGeoNode* g1, TGeoNode* g2, float dt){
	Vec3f gSpPoint[2], currSpP;
	Vec3f fric, dynFric, damper, angFric;
	Affinef dPos[2];
	float d = 0.01;
	
	///< ƒoƒlŒW”‚ÌŒvŽZ
	if (g1->mass < g2->mass){
		k = g1->mass * (3 / dt) * (3 / dt) / 2;
	} else {
		k = g2->mass * (3 / dt) * (3 / dt) / 2;
	}
	cout << k << endl;
	currSpP = SprPoint(force, torque);		/// Õ“Ë‚Ì’†S“_‚ÌŒvŽZ
	if (pc.state == TPenaltyDetector::NOCHECK){
		fric = Vec3f();
		angFric = Vec3f();
		damper = Vec3f();
		gSpPoint[0] = Vec3f();
		gSpPoint[1] = Vec3f();
	}
	if (pc.state == TPenaltyDetector::CREATED){
		pc.sprPoint[0] = currSpP; //SprPoint(force, torque);
		pc.sprPoint[1] = currSpP; //SprPoint(force, torque);
		pc.lastPosture[0] = g1->posture;
		pc.lastPosture[1] = g2->posture;
		fric = Vec3f();
		angFric = Vec3f();
		damper = Vec3f();
	}
	if (pc.state == TPenaltyDetector::KEEP){
		float fricSize;
		Vec3f fricDire;
		dPos[0] = g1->posture * pc.lastPosture[0].inv();
		dPos[1] = g2->posture * pc.lastPosture[1].inv();
		fricDire = (dPos[1].Pos() - dPos[0].Pos());
		gSpPoint[0] = pc.sprPoint[0] + dPos[0].Pos();
		gSpPoint[1] = pc.sprPoint[1] + dPos[1].Pos();
		fricSize = (gSpPoint[0] - gSpPoint[1]).norm();
		if (fricDire.norm() > 0) {
			fricDire = fricDire.unit();
			fric = k * fricSize * fricDire;
		}else{
			fric = k * (gSpPoint[1] - gSpPoint[0]);
		}
//		fric = k * (gSpP[1] - gSpP[0]);		/// g1‚É‚©‚©‚é–€ŽC—Í
//		assert(fric * force - fric.norm() * force.norm());
		if (force.norm() > 0 && fric.norm() > 0 && fric * force != fric.norm() * force.norm()){ 
			fric = force.unit() ^ fric ^ force.unit();
		}else{
			fric = Vec3f();
			assert(0);
		}
		gSpPoint[1] = gSpPoint[0] + fric / k;
//		pc.sprPoint[1] = g2->posture.inv() * gSpP[1];
		if (fric.norm() > 0) {
			dynFric = (myu * force.norm()) * fric.unit();
		} else {
			dynFric = Vec3f();
		}
		if (dynFric.norm() < fric.norm() && 0 < fric.norm()){
//			gSpP[1] = gSpP[0] + dynFric.norm() * fric.unit();
//			pc.sprPoint[1] = g2->posture.inv() * gSpP[1];
//			assert(-100 < fric.norm() && fric.norm() < 100);
			fric = dynFric.norm() * fric.unit();
//			gSpPoint[1] = gSpPoint[0] + fric / k;
			gSpPoint[0] = currSpP - (fric / k) / 2;
			gSpPoint[1] = currSpP + (fric / k) / 2;
		}else{
			std::cout << "stop" << std::endl;
		}
		if (fric.norm() > 0) {
			damper = d * (fric.norm() - pc.lastFric.norm()) / dt * fric.unit();
//			if (0.1f < fric.size() - pc.lastFric.size()) damper = d * (fric - pc.lastFric) / dt;
//			if (0 < fric.size() - pc.lastFric.size() && fric.size() - pc.lastFric.size() <= 0.1) damper = -fric + d * (fric - pc.lastFric) / dt;
//			if (fric.size() - pc.lastFric.size() <= 0) damper = -fric;
		}else{
			damper = Vec3f();
		}
		angFric = 0.01f * currSpP ^ (fric + damper);
		pc.sprPoint[0] = gSpPoint[0];
		pc.sprPoint[1] = gSpPoint[1];
	}

	if (g1->polytopes.size() && _finite(g1->mass) && g1->mass != HUGE_VAL){
		g1->momentum += (fric + damper) * dt;
		if (g1->angularMomentum.norm() > 0.1) g1->angularMomentum += angFric;
	}
	if (g2->polytopes.size() && _finite(g2->mass) && g2->mass != HUGE_VAL){
		g2->momentum -= (fric + damper) * dt;
		if (g2->angularMomentum.norm() > 0.1) g2->angularMomentum -= angFric;
	}
	cout << fric.norm() << " , " << damper.norm() << "   " << g2->Vel().norm() << "   " << force.norm() << endl;
	pc.lastPosture[0] = g1->posture;
	pc.lastPosture[1] = g2->posture;	
	pc.lastFric = fric;
//	assert(-50 < g1->momentum.norm() && g1->momentum.norm() < 50);
//	assert(-50 < g2->momentum.norm() && g2->momentum.norm() < 50);
//	return fric;
//	g1->Add(g2, -fric);
//	g2->Add(g1, fric);

/*		dynFric = ((g1->momentum - g2->momentum) / dt * fric.unit()) * fric.unit();
		if(dynFric.size() < fric.size()){
			gSpP[1] = gSpP[0] - dynFric.size() * fric.unit();
			pc.sprPoint[1] = (Point)(g2->posture.inv() * gSpP[1]);
			fric = k * (gSpP[0] - gSpP[1]);
		}
//		if ((g1->Vel() - g2->Vel()).norm() < 0.3) fric = Vec3f(0.0, 0.0, 0.0);
		return fric;
	}
	return Vec3f(0.0, 0.0, 0.0);*/
};
		 