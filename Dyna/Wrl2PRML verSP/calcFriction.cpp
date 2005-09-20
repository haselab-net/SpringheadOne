#include <Base/Env.h>
#include <Solid/SOLID.h>
#include <Base/Affine.h>

#include "calcFriction.h"
#include "global.h"

using namespace std;
using namespace LVRM;

//#define PAST

Vec3d TCalcCollisionMotion::CalcApplicationPoint(Vec3d force, Vec3d torque, Vec3d normal, Vec3d interior){
	Matrix3d matrix;
	double nDotp;
	
	matrix.Ex() = Vec3d(		   0,  -force.Z(),   force.Y()	);
	matrix.Ey() = Vec3d(   force.Z(),	        0,  -force.X()	);
	matrix.Ez() = Vec3d(  -force.Y(),   force.X(),			 0	);
	
	nDotp = interior * normal;
	
	if (force.X() != 0 ) {
		matrix.ExX() += normal.X();
		matrix.EyX() += normal.Y();
		matrix.EzX() += normal.Z();
		torque.X() += nDotp;
	}else if (force.Y() != 0 ) {
		matrix.ExY() += normal.X();
		matrix.EyY() += normal.Y();
		matrix.EzY() += normal.Z();
		torque.Y() += nDotp;
	}else {
		matrix.ExZ() += normal.X();
		matrix.EyZ() += normal.Y();
		matrix.EzZ() += normal.Z();
		torque.Z() += nDotp;
	}
/*	cout << normal << endl;
	cout << "MATRIX" << endl;
	cout << matrix.ExX() << ", " << matrix.EyX() << ", " << matrix.EzX() << endl;
	cout << matrix.ExY() << ", " << matrix.EyY() << ", " << matrix.EzY() << endl;
	cout << matrix.ExZ() << ", " << matrix.EyZ() << ", " << matrix.EzZ() << endl;
	cout << "detaminant:" << matrix.det() << endl;*/
	assert(matrix.det());
	return matrix.inv() * torque;
};

void TCalcCollisionMotion::CalcCollisionMotion(Vec3d pen, Vec3d penMom, Vec3d vel, Vec3d velMom, 
									Vec3d normal, Vec3d interior, TPenaltyDetector& pc, TGeoNode* g1, TGeoNode* g2, double dt){
	Vec3d currSprPoint[2];
	Vec3d fric;
	Vec3d force;
	Affined dPos[2];
	
	// ƒoƒlŒW”‚ÌŒvŽZ
	double T = dt * TIM_MULT;
	if (g1->mass < g2->mass){
		k = g1->mass * (M_PI / T) * (M_PI / T);
	} else {
		k = g2->mass * (M_PI / T) * (M_PI / T);
	}

	// R—Í‚Æ•¨‘Ì‚Ì’†S‚ÉŠÖ‚·‚éR—Í‚Ìƒ‚[ƒƒ“ƒg‚ÌŒvŽZ
#ifdef PAST
	force = k * (pen + DAM_STF * (pen - pc.lastPenalty)/dt);
	Vec3d penMom1 = penMom - (g1->posture*g1->Center() ^ pen);
	Vec3d penMom2 = penMom - (g2->posture*g2->Center() ^ pen);
	Vec3d torque1 = k * (penMom1 + DAM_STF * (penMom1 - pc.lastPenaltyMoment[0])/dt);
	Vec3d torque2 = k * (penMom2 + DAM_STF * (penMom2 - pc.lastPenaltyMoment[1])/dt);
#endif
#ifndef PAST
	force = k * (pen + PEN_DAM_STF * vel);
	Vec3d penMom1 = penMom - (g1->posture*g1->Center() ^ pen);
	Vec3d penMom2 = penMom - (g2->posture*g2->Center() ^ pen);
	Vec3d velMom1 = velMom - (g1->posture*g1->Center() ^ vel);
	Vec3d velMom2 = velMom - (g2->posture*g2->Center() ^ vel);
	Vec3d torque1 = k * (penMom1 + PEN_DAM_STF * velMom1);
	Vec3d torque2 = k * (penMom2 + PEN_DAM_STF * velMom2);
	if (g1->polytopes.size() && _finite(g1->mass) && g1->mass != HUGE_VAL){
		g1->force -= force;
		g1->torque -= torque1;
	}
	if (g2->polytopes.size() && _finite(g2->mass) && g2->mass != HUGE_VAL){
		g2->force += force;
		g2->torque += torque2;
	}
#endif
#ifdef PAST
	if (g1->polytopes.size() && _finite(g1->mass) && g1->mass != HUGE_VAL){
		g1->momentum -= force*dt;
		g1->angularMomentum -= torque1 * dt;
	}
	if (g2->polytopes.size() && _finite(g2->mass) && g2->mass != HUGE_VAL){
		g2->momentum += force*dt;
		g2->angularMomentum += torque2 * dt;
	}
	pc.lastPenalty = pen;
	pc.lastPenaltyMoment[0] = penMom1;
	pc.lastPenaltyMoment[1] = penMom2;
#endif	
	//@–€ŽC—Í‚ÌŒvŽZ
	pc.applicationPoint = CalcApplicationPoint(pen, penMom, normal, interior);
	if (pc.state == TPenaltyDetector::CREATED){
		pc.sprPoint[0] = g1->posture.inv() * pc.applicationPoint;
		pc.sprPoint[1] = g2->posture.inv() * pc.applicationPoint;
		pc.lastFriction = Vec3d();
	}else if (pc.state == TPenaltyDetector::KEEP){
		Vec3d fricPen = g1->posture * pc.sprPoint[0] - g2->posture * pc.sprPoint[1];
		fricPen -= (fricPen * normal) * normal;
		double dynFricNorm = DYN_MYU * force.norm() / k;
		if (fricPen.norm() > dynFricNorm && fricPen != Vec3d()){
			fricPen = dynFricNorm * fricPen.unit();
		}else{
//			std::cout << "stop" << std::endl;
		}
		pc.sprPoint[0] = g1->posture.inv() * (pc.applicationPoint + fricPen / 2);
		pc.sprPoint[1] = g2->posture.inv() * (pc.applicationPoint - fricPen / 2);

		fric = k * fricPen;
		Vec3d lastFricPen = pc.lastFriction - (pc.lastFriction * normal) * normal;
		pc.lastFriction = fricPen;
		fric += k * FRI_DAM_STF * (fricPen - lastFricPen) / dt;
	}
#ifdef PAST
	if (g1->polytopes.size() && _finite(g1->mass) && g1->mass != HUGE_VAL){
		g1->momentum -= fric * dt;
		g1->angularMomentum -= (pc.applicationPoint - g1->posture * g1->Center()) ^ fric * dt;
	}
	if (g2->polytopes.size() && _finite(g2->mass) && g2->mass != HUGE_VAL){
		g2->momentum += fric * dt;
		g2->angularMomentum += (pc.applicationPoint - g2->posture * g2->Center()) ^ fric * dt;
	}
#endif
#ifndef PAST
	if (g1->polytopes.size() && _finite(g1->mass) && g1->mass != HUGE_VAL){
		g1->friction -= fric;
		g1->angularFriction -= (pc.applicationPoint - g1->posture * g1->Center()) ^ fric;
	}
	if (g2->polytopes.size() && _finite(g2->mass) && g2->mass != HUGE_VAL){
		g2->friction += fric;
		g2->angularFriction += (pc.applicationPoint - g2->posture * g2->Center()) ^ fric;
	}
#endif
}
		 