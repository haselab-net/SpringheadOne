// Human.cpp: CHuman ƒNƒ‰ƒX‚ÌƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“
//
//////////////////////////////////////////////////////////////////////

#include <Physics/PHJoint.h>
#include "stdafx.h"
#include "Human.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace Spr;

bool CLeg::Connect(SGScene* s, const char* prefix){
	s->FindObject(joint[0], UTString("jo").append(prefix).append("X") );
	s->FindObject(joint[1], UTString("jo").append(prefix).append("Z") );
	s->FindObject(joint[2], UTString("jo").append(prefix).append("Y") );
	return joint[0] && joint[1] && joint[2];
}
Vec3f CLeg::GetPos(){
	Vec3f posW = joint[2]->solid->GetCenterPosition();
	Affinef afBody;

	if (joint[0]->GetParent()->solid) afBody = joint[0]->GetParent()->solid->GetFrame()->GetPosture();
	Vec3f posL = afBody.inv() * posW;
	return posL;
}
void CLeg::SetPos(Vec3f posL){
	joint[0]->SetPosition(posL.X());
	joint[1]->SetPosition(posL.Z());
	joint[2]->SetPosition(posL.Y());
}

bool CHuman::Connect(SGScene* s){
	s->FindObject(soBody, "soBody");
	bool rv = soBody!=NULL;
	rv &= leg[0].Connect(s, "L");
	rv &= leg[1].Connect(s, "R");
	if (rv) DSTR << "CHuman connection succeed." << std::endl;
	bGood = rv;
	return rv;
}
void CHuman::Step(){
	if (!bGood) return;
	//	—V‹r‚Æ—§‹r‚ÌˆÊ’u‚ğæ“¾
	Vec3f swPos = leg[swing].GetPos();	
	Vec3f stPos = leg[stance].GetPos();

	//	—V‹r‚Ì§Œä
	//	°‚©‚ç­‚µã‚É‹r‚ğã‚°‚é
	float swingToY = 0.4f + 0.05f;
	Vec3f hipPos = leg[swing].joint[1]->solid->GetCenterPosition();
	float down = hipPos.Y() - swingToY;
	Matrix3f bodyRot = soBody->GetRotation();
	down /= bodyRot.Ey().Y();
	if (!_finite(down)) down = 2.0f;
	float downMax = 2.0f + 0.5f;
	float downMin = 2.0f - 0.5f;
	if (down > downMax) down = downMax;
	if (down < downMin) down = downMin;
	leg[swing].joint[2]->SetPosition(down);

	//	—§‹r‚Ì§Œä
	if (bodyRot.Ey().Y() > 0.8){
		Vec3f pos = leg[stance].joint[2]->solid->GetCenterPosition();
		Vec3f tilt = bodyRot.Ey();
		Vec3f angVel = soBody->GetAngularVelocity();
		tilt.Y() = 0;
		pos -= tilt + angVel * 0.001;
		pos = bodyRot.inv() * pos;
		pos.Y() = 2.0f;
		leg[stance].SetPos(pos);
		//	—V‹r‚ğ”õ‚¦‚éD
		leg[swing].joint[0]->SetPosition(tilt.X() * 4);
		leg[swing].joint[1]->SetPosition(tilt.Z() * 4);
	}
	
	//	—V‹r‚ÌØ‚è‘Ö‚¦
	Vec3f pos = leg[stance].GetPos();
	pos.Y() = 0;
	if (pos.norm() > 0.2) std::swap(swing, stance);
}

CHuman::CHuman()
{
	bGood = false;
	swing = 1;
	stance = 0;
}

CHuman::~CHuman()
{

}
