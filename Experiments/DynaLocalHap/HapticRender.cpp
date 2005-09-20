#include "HapticRender.h"
#ifdef _WIN32
#pragma hdrstop
#endif

HapticRender::HapticRender(){
	hapticDt = 0.001f;
	simulationDt = 0.01f;
	hapticTime = 0;
	forceScale = 1.0f; 
	posScale = 1.0f;
	
	proxy= Vec3f();

	newNormal = normal = Vec3f(0,1,0);
	planeState = IHPacketPlane::NONE;
}

HapticRender::~HapticRender(){}

void HapticRender::SetPlane(Vec3f p, Vec3f n, PlaneState ps, float pscale, float fs, Vec3f ofs){
	offset = ofs;
	posScale = pscale;
	forceScale = fs;
	newPos = p;
	newNormal = n;
	planeState = ps;
	if (planeState == IHPacketPlane::NEW){
		pos = p - 0.001f*n;
		normal = n;
	}
}
Vec3f HapticRender::GetForce(){
	return force;
}
void HapticRender::HapticStep(){
	if (planeState == IHPacketPlane::NONE){
		normal=Vec3f();
	}else{
		//	補間
		float alpha = 0.1f;
		pos += (newPos - pos) * alpha;
		normal += (newNormal-normal) * alpha;
		float ns = normal.norm();
		if (ns > 1e-10f) {
			ns = 1.0f / ns;
			normal *= ns;
		}else{
			normal = Vec3f(0,0,0);
		}
	}
//	DSTR << "I" << std::endl;
	//	プロキシを提示面上に移動
	proxy += normal * ((pos-proxy)*normal);
	//	プロキシを摩擦コーン内に移動
	const float dynaFric = 0.5f;
	pointer = device ? device->GetPos()*posScale : Vec3f();
	pointer += offset;
//	DSTR << "P" << std::endl;

	Vec3f spring = proxy - pointer;
	float depth = spring * normal;
	if (depth > 1e-10f){
		Vec3f fricSpring = spring - depth*normal;
		float fricLen = fricSpring.norm();
		if (fricLen > depth*dynaFric){
			fricLen = depth*dynaFric;
			fricSpring = fricSpring.unit()*fricLen;
			spring = normal*depth + fricSpring;
			proxy = spring + pointer;
		}
	}else{
		spring = Vec3f();
		proxy = pointer;
	}
#ifdef USE_IHC
	Vec3f dSpr = (spring - lastSpring) / 0.0001f;
#else
	Vec3f dSpr = (spring - lastSpring) / 0.001f;
#endif
	float sAlpha = 0.2f;
	dSpring = dSpr*sAlpha + dSpring*(1-sAlpha);
	lastSpring = spring;
//	DSTR << "X" << std::endl;
#ifdef USE_IHC
	const float K = 10000.0f;
	const float B = 2.0f;
#else
	const float K = 2000.0f;
	const float B = 2.0f;
#endif
	Vec3f curForce = spring * K + dSpring*B;
	force -= curForce * hapticDt;
//	DSTR << "F" << std::endl;
	if (device){
		device->SetForce(curForce/forceScale, Vec3f());
		//DSTR << "F:" << curForce << " P:" << pointer << std::endl;
//		DSTR << "r:" << proxy << "t:" << pointer << std::endl;
		device->Update(hapticDt);
	}
}
