#include "HRSolidProxy.h"

namespace LVRM{

HRSolidProxy::HRSolidProxy(){
	ClearSprings();
	kTrans = 1;
	bTrans = 1;
	kRotation = 1;
	bRotation = 1;
	trans.size(2);
	rotation.size(2);
}
void HRSolidProxy::SetSpringDamper(float kt, float bt, float kr, float br){
	kTrans = kt;
	bTrans = bt;
	kRotation = kr;
	bRotation = br;
}
void HRSolidProxy::ClearSprings(){
	sumKi = 0;
	sumKiRi = Vec3f();
	sumKiPi = Vec3f();
	sumKiPixRi = Vec3f();
	sumKiPixRix.clear();
}
void HRSolidProxy::MoveObject(Vec3f p, Vec3f a){
	Vec3f pLocal = objectPosture.Rot().inv() * p;

	trans[0] -= pLocal;

	objectPosture.Pos() += p;

//	ˆê’èˆÈã‚ÌŠO—Í‚ª‚©‚©‚ç‚È‚¢‚æ‚¤‚É‚·‚é(DrugMassê—p)
/*	
	Vec3f diff = proxyPosture.Pos() - objectPosture.Pos();
	float normMax = 0.002f;
	if (diff.norm() > normMax){
		Vec3f ndiff = diff.unit() * normMax;
		objectPosture.Pos() += diff - ndiff;
	}
*/	
	Vec3f aLocal = objectPosture.Rot().inv() * a;

	rotation[0] -= aLocal;

	float norm = a.norm();
	if (norm){
		objectPosture.Rot() = Matrix3f(norm, a/norm) * objectPosture.Rot();
	}
}
void HRSolidProxy::AddSpring(float k, const Vec3f& r, const Vec3f& p){
	Vec3f pLocal = proxyPosture.inv() * p;
	Vec3f rLocal = proxyPosture.inv() * r;
	sumKi += k;
	sumKiRi += k * rLocal;
	sumKiPi += k * pLocal;
	sumKiPixRi += k * pLocal % rLocal;
	sumKiPixRix += k*Matrix3f::Cross(pLocal)*Matrix3f::Cross(rLocal);
}
void HRSolidProxy::UpdateProxy(){
	using namespace PTM;
	TVector<6, float> x;
	x.clear();
	TMatrix<6,6,float> mat;
	TVector<6, float> b;
	mat.sub_matrix(0,0, TMatDim<3,3>()) = -(sumKi + kTrans) * Matrix3f();
	mat.sub_matrix(0,3, TMatDim<3,3>()) = Matrix3f::Cross(sumKiRi);
	b.sub_vector(0, TVecDim<3>()) = sumKiRi - sumKiPi + kTrans*trans[0];

	mat.sub_matrix(3,0, TMatDim<3,3>()) = Matrix3f::Cross(-sumKiRi);
	mat.sub_matrix(3,3, TMatDim<3,3>()) = sumKiPixRix - kRotation * Matrix3f();
	b.sub_vector(3, TVecDim<3>()) = sumKiPixRi + kRotation * rotation[0];
	mat.gauss(x, b);

	rotation.inc();
	trans.inc();
	
	Vec3f& deltaTrans = (Vec3f&)x.sub_vector(0, TVecDim<3>());
	Vec3f& deltaTheta = (Vec3f&)x.sub_vector(3, TVecDim<3>());
	rotation[0] = rotation[1] + deltaTheta;
	trans[0] = trans[1] + deltaTrans;

	UpdateProxyPosture();
}
void HRSolidProxy::UpdateProxyPosture(){
	float norm = rotation[0].norm();
	if (norm){
		proxyPosture = objectPosture * Affinef(norm , rotation[0]/norm, trans[0]);
	}else{
		proxyPosture = objectPosture;
		proxyPosture.Pos() += objectPosture.Rot() * trans[0];
	}
}

}	//	namespace VRHaptic
