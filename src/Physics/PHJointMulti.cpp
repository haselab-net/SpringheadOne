#include "Physics.h"
#pragma hdrstop
#include <iomanip>

namespace Spr{;
using namespace PTM;
template <class M>
PTM::TMatrixCol<DIMDEC(M::WIDTH), DIMDEC(M::HEIGHT), TYPENAME M::element_type> spMatTrans(const M& m){
	PTM::TMatrixCol<DIMDEC(M::WIDTH), DIMDEC(M::HEIGHT), TYPENAME M::element_type> rv;
	rv.col(0) = m.row(3);
	rv.col(1) = m.row(4);
	rv.col(2) = m.row(5);
	rv.col(3) = m.row(0);
	rv.col(4) = m.row(1);
	rv.col(5) = m.row(2);
	return rv;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
SGOBJECTIMP(PHJointBall, PHJointBase);
void PHJointBall::Integrate(double dt){
	//可動範囲制限が有効な場合
	if(minDot < 1){
		double K=.5;
		double B=.5;
		K*= solid->GetMass()/(dt*dt);
		B*= solid->GetMass()/dt;

		Vec3d dir = Vec3d(0,0,1);
		dir = position * dir;
		double d = center * dir;
		if (d < minDot){
			double theta = acos(d) - acos(minDot);
			Vec3d axis = (dir^center).unit();
			double vel = velocity * axis;
			if (vel < 0){				//	可動域から遠ざかっている場合
				AddTorque(K*theta*axis -B*axis*vel);
			}
		}
		Vec3d rot = position.rotation();
		double vz = velocity[2];
		bool bLimit = false;
		double prop;
		if (rot.Z() < minTwist && vz < 0){
			prop = minTwist - rot.Z();
			bLimit = true;
		}else if (rot.Z() > maxTwist && vz > 0){
			prop = maxTwist - rot.Z();
			bLimit = true;
		}
		if (bLimit){
			AddTorque(dir * (K*prop - B*vz)*0.2f);
		}
	}


	PreIntegrate(dt);
	//	delta_position から，関節の姿勢を計算．
	position = position * Quaterniond::Rot(delta_position);

	PropagateState();
	PHJointBase::Integrate(dt);	
/*	double v = velocity.norm();
	double limit = 40;
	if (v > limit) velocity = limit*velocity.unit();
*/
}

void PHJointBall::CompJointAxis(){
	for(int i=0; i<3; ++i){
		S.col(i).sub_vector(0, Vec3d()) = cRj.col(i);
		S.col(i).sub_vector(3, Vec3d()) = cross(cRj.col(i), -(crj - solid->GetCenter()));
	}
	S_tr = spMatTrans(S);
}

void PHJointBall::CompRelativePosition(){
	Matrix3d rot;
	position.to_matrix(rot);
	pRc = pRj * rot * cRj.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid) cp = GetParent()->solid->GetCenter();
	prc = (cRp * (prj - cp)) - (crj - solid->GetCenter());
}

void PHJointBall::CompRelativeVelocity()
{
	pvc = S.sub_matrix(3,0, PTM::TMatDim<3,3>()) * velocity;
	pwc = S.sub_matrix(0,0, PTM::TMatDim<3,3>()) * velocity;
}

void PHJointBall::CompCoriolisAccel()
{
	Vec3d ud = S.sub_matrix(0,0, PTM::TMatDim<3,3>()) * velocity;
	Vec3d wp = cRp * OfParent(&PHJointBall::w);
	Vec3d tmp = cross(ud, (crj - solid->GetCenter()));
	svitem(c, 0) = cross(wp, ud);
	svitem(c, 1) = cross(wp, cross(wp, prc)) - 2.0 * cross(wp, tmp) - cross(ud, tmp);
}

typedef Quaternionf Quaternion;
DEF_RECORD(XJointBall, {
	GUID Guid(){ return WBGuid("F8E58987-603F-458c-9F29-F90CE6E3B17C"); }
	XJointBase jointBase;
	Quaternion position;	//	関節角の初期値
	Vector velocity;		//	関節速度の初期値
});

DEF_RECORD(XConeLimit, {
	GUID Guid(){ return WBGuid("B912DED0-E4A1-454a-B75C-C7024DD21F3E"); }
	Vector center;			//	可動域の中心
	FLOAT minDot;			//	可動範囲
	FLOAT minTwist;			//	ひねりの可動範囲
	FLOAT maxTwist;			//	ひねりの可動範囲
});
class PHJointBallLoader : public FIObjectLoader<PHJointBall>
{
public:
	PHJointBallLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(Vector);
		db->REG_FIELD(Quaternion);
		db->REG_FIELD(Matrix3x3);
		db->REG_RECORD_PROTO(XJointBase);
		db->REG_RECORD_PROTO(XJointBall);
	}
	bool LoadData(FILoadScene* ctx, PHJointBall* j){
		ctx->objects.Push(j);
		XJointBall x;
		ctx->docs.Top()->GetWholeData(x);
		j->PHJointBase::LoadX(x.jointBase);
		j->position = x.position.unit();
		j->velocity = x.velocity;
		return true;
	}
	void Loaded(FILoadScene* ctx){
		ACAST(PHJointBall, ctx->objects.back());
		ctx->objects.Pop();
	}
};

class PHJointBallSaver : public FIObjectSaver<PHJointBall>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHJointBall* j){
		XJointBall x;
		j->PHJointBase::SaveX(x.jointBase);
		x.position = j->position;
		x.velocity = j->velocity;
		doc->SetWholeData(x);
		if (j->minDot > -1 && j->center.norm() > 0){
			XConeLimit x;
			x.center = j->center;
			x.minDot = j->minDot;
			x.minTwist = j->minTwist;
			x.maxTwist = j->maxTwist;
			UTRef<FIDocNodeBase> docCone = ctx->CreateDocNode("ConeLimit");
			docCone->SetWholeData(x);
			doc->AddChild(docCone);
		}
	}
};
DEF_REGISTER_BOTH(PHJointBall);


class PHConeLimitLoader : public FIBaseLoader
{
public:
	virtual UTString GetNodeType() const{
		return "ConeLimit";
	}
	PHConeLimitLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(Vector);
		db->REG_RECORD_PROTO(XConeLimit);
	}
	virtual void Load(class FILoadScene* ctx){
		XConeLimit x;
		ctx->docs.Top()->GetWholeData(x);
		PHJointBall* ball = DCAST(PHJointBall, ctx->objects.Top());
		if (ball){
			ball->center = x.center;
			ball->minDot = x.minDot;
			ball->minTwist = x.minTwist;
			ball->maxTwist = x.maxTwist;
		}
		return;
	}
};
DEF_REGISTER_LOADER(PHConeLimit);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
SGOBJECTIMP(PHJointUniversal, PHJointBase);
void PHJointUniversal::Integrate(double dt){
	PreIntegrate(dt);
	
	//	delta_position から，関節の姿勢を計算．
	position += delta_position;

	PropagateState();
	PHJointBase::Integrate(dt);	
}

void PHJointUniversal::CompJointAxis()
{
	Matrix3f mat = cRj * Matrix3f::Rot((float)position[0], 'x');
	for(int i=0; i<2; ++i){
		S.col(i).sub_vector(0, Vec3d()) = mat.col(i);
		S.col(i).sub_vector(3, Vec3d()) = cross(mat.col(i), -(crj - solid->GetCenter()));
	}
	S_tr = spMatTrans(S);
}

void PHJointUniversal::CompRelativePosition(){
	rotX = Matrix3f::Rot((float)position.x, 'x');
	rotY = Matrix3f::Rot((float)position.y, 'y');
	Vec3f sy = rotX * cRj.Ey();
	S.col(1).sub_vector(0, Vec3d()) = sy;
	S.col(1).sub_vector(3, Vec3d()) = cross(sy, -(crj - solid->GetCenter()));

	pRc = pRj * rotY * rotX * cRj.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid) cp = GetParent()->solid->GetCenter();
	prc = (cRp * (prj - cp)) - (crj - solid->GetCenter());
}

void PHJointUniversal::CompRelativeVelocity(){
	pvc = S.sub_matrix(3,0, TMatDim<3,2>()) * velocity;
	pwc = S.sub_matrix(0,0, TMatDim<3,2>()) * velocity;
}

void PHJointUniversal::CompCoriolisAccel(){
	Vec3d ud = S.sub_matrix(0,0, PTM::TMatDim<3,2>()) * velocity;
	Vec3d wp = cRp * OfParent(&PHJointUniversal::w);
	Vec3d tmp = cross(ud, (crj - solid->GetCenter()));
	svitem(c, 0) = cross(wp, ud);
	svitem(c, 1) = cross(wp, cross(wp, prc)) - 2.0 * cross(wp, tmp) - cross(ud, tmp);
}

typedef Quaternionf Quaternion;
typedef Vec2f Coords2d;
DEF_RECORD(XJointUniversal, {
	GUID Guid(){ return WBGuid("4F9BB7AE-8525-4d3c-807D-95BC59B75EB2"); }
	XJointBase jointBase;
	Coords2d position;
	Coords2d velocity;
});
class PHJointUniversalLoader : public FIObjectLoader<PHJointUniversal>
{
public:
	PHJointUniversalLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(Vector);
		db->REG_FIELD(Quaternion);
		db->REG_FIELD(Matrix3x3);
		db->REG_FIELD(Coords2d);
		db->REG_RECORD_PROTO(XJointBase);
		db->REG_RECORD_PROTO(XJointUniversal);
	}
	bool LoadData(FILoadScene* ctx, PHJointUniversal* j){
		XJointUniversal x;
		ctx->docs.Top()->GetWholeData(x);
		j->PHJointBase::LoadX(x.jointBase);
		j->position = x.position;
		j->velocity = x.velocity;
		return true;
	}
};
class PHJointUniversalSaver : public FIObjectSaver<PHJointUniversal>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHJointUniversal* j){
		XJointUniversal x;
		j->PHJointBase::SaveX(x.jointBase);
		x.position = j->position;
		x.velocity = j->velocity;
		doc->SetWholeData(x);
	}
};
DEF_REGISTER_BOTH(PHJointUniversal);
}
