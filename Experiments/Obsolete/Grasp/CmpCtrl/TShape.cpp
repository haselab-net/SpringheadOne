#include "TShape.h"
#include <WinBasis/WBWin.h>
#include <gl/gl.h>

const float epsilon = 1.0e-4f;

namespace LVRM{

TShape* TConvexPolygon::CalcProxyPos(Vec3f& newPrx, Vec3f& normalRet, const Vec3f& prx, const Vec3f& dev){
	//	�v���L�V���ǂ̊O���Ńf�o�C�X���ǂ̓����ɂ��邩�H
	Vec3f& pos = vtxs.front().pos;
	float posDist = pos * normal;
	float prxDist = prx * normal;
	float devDist = dev * normal;
	if (!(prxDist >= posDist-epsilon && posDist >= devDist)) return NULL;
	
	//	�Փ˓_�̓|���S���̓������H
	Vec3f colPos = (posDist-devDist)/(prxDist-devDist)*prx + (prxDist-posDist)/(prxDist-devDist)*dev;
	for(TVtxs::iterator it = vtxs.begin(); it != vtxs.end(); ++it){
		if ((colPos - it->pos) * it->normal < 0) return NULL;
	}
		
	//	�v���L�V�̈ړ�������߂�
	newPrx = colPos;
	float devDepth = posDist-devDist;
	Vec3f noFrictionPrx = dev + devDepth * normal;
	Vec3f frictionForce = newPrx - noFrictionPrx;
	float ffn = frictionForce.norm();
	if (ffn > friction*devDepth){
		newPrx = noFrictionPrx + (friction*devDepth / ffn) * frictionForce;
	}
	normalRet = normal;
	return this;
}


TShape* TPlane::CalcProxyPos(Vec3f& newPrx, Vec3f& normal, const Vec3f& prx, const Vec3f& dev){
	if (prx.Z() >= -epsilon && dev.Z() <= 0){
		newPrx = prx;
		newPrx.Z() = 0;
		float frictionRadius = friction * (newPrx.Z() - dev.Z());
		Vec3f noFrictionPrx = dev;
		noFrictionPrx.Z() = 0;
		Vec3f frictionForce = newPrx - noFrictionPrx;
		float ffn = frictionForce.norm();
		if (ffn > frictionRadius){
			newPrx = noFrictionPrx + (frictionRadius/ffn)*frictionForce;
		}
		normal = Vec3f(0,0,1);
		return this;
	}else{
		return NULL;
	}
}

void TConvexPolygon::Draw(){
	glBegin(GL_POLYGON);
	for(TVtxs::iterator it = vtxs.begin(); it != vtxs.end(); ++it){
		glVertex3fv(it->pos);
	}
	glEnd();
}


}
