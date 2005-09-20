#include <Physics/PHJointMulti.h>
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
	PreIntegrate(dt);	
	
	//	delta_position Ç©ÇÁÅCä÷êﬂÇÃépê®ÇåvéZÅD
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
		S.col(i).sub_vector(0, Vec3d()) = m3fRotationChild.col(i);
		S.col(i).sub_vector(3, Vec3d()) = cross(m3fRotationChild.col(i), -(v3fPositionChild - solid->GetCenter()));
	}
	S_tr = spMatTrans(S);
}

void PHJointBall::CompRelativePosition(){
	Matrix3d rot;
	position.to_matrix(rot);
	pRc = m3fRotationParent * rot * m3fRotationChild.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid) cp = GetParent()->solid->GetCenter();
	prc = (cRp * (v3fPositionParent - cp)) - (v3fPositionChild - solid->GetCenter());
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
	Vec3d tmp = cross(ud, (v3fPositionChild - solid->GetCenter()));
	svitem(c, 0) = cross(wp, ud);
	svitem(c, 1) = cross(wp, cross(wp, prc)) - 2.0 * cross(wp, tmp) - cross(ud, tmp);
}

typedef Quaternionf Quaternion;
DEF_RECORD(XJointBall, {
	GUID Guid(){ return WBGuid("F8E58987-603F-458c-9F29-F90CE6E3B17C"); }
	XJointBase jointBase;
	Quaternion position;
	Vector velocity;
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
		XJointBall x;
		ctx->docs.Top()->GetWholeData(x);
		j->PHJointBase::LoadX(x.jointBase);
		j->position = x.position.unit();
		j->velocity = x.velocity;
		return true;
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
	}
};
DEF_REGISTER_BOTH(PHJointBall);





//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
SGOBJECTIMP(PHJointUniversal, PHJointBase);
void PHJointUniversal::Integrate(double dt){
	PreIntegrate(dt);
	
	//	delta_position Ç©ÇÁÅCä÷êﬂÇÃépê®ÇåvéZÅD
	position += delta_position;

	PropagateState();
	PHJointBase::Integrate(dt);	
}

void PHJointUniversal::CompJointAxis()
{
	Matrix3f mat = m3fRotationChild * Matrix3f::Rot((float)position[0], 'x');
	for(int i=0; i<2; ++i){
		S.col(i).sub_vector(0, Vec3d()) = mat.col(i);
		S.col(i).sub_vector(3, Vec3d()) = cross(mat.col(i), -(v3fPositionChild - solid->GetCenter()));
	}
	S_tr = spMatTrans(S);
}

void PHJointUniversal::CompRelativePosition(){
	rotX = Matrix3f::Rot((float)position.x, 'x');
	rotY = Matrix3f::Rot((float)position.y, 'y');
	Vec3f sy = rotX * m3fRotationChild.Ey();
	S.col(1).sub_vector(0, Vec3d()) = sy;
	S.col(1).sub_vector(3, Vec3d()) = cross(sy, -(v3fPositionChild - solid->GetCenter()));

	pRc = m3fRotationParent * rotY * rotX * m3fRotationChild.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid) cp = GetParent()->solid->GetCenter();
	prc = (cRp * (v3fPositionParent - cp)) - (v3fPositionChild - solid->GetCenter());
}

void PHJointUniversal::CompRelativeVelocity(){
	pvc = S.sub_matrix(3,0, TMatDim<3,2>()) * velocity;
	pwc = S.sub_matrix(0,0, TMatDim<3,2>()) * velocity;
}

void PHJointUniversal::CompCoriolisAccel(){
	Vec3d ud = S.sub_matrix(0,0, PTM::TMatDim<3,2>()) * velocity;
	Vec3d wp = cRp * OfParent(&PHJointUniversal::w);
	Vec3d tmp = cross(ud, (v3fPositionChild - solid->GetCenter()));
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
