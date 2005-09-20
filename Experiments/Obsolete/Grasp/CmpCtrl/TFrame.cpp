#include "TFrame.h"
#include "TShape.h"
#include <WinBasis/WBWin.h>
#include <gl/gl.h>

namespace LVRM{

TShape* TFrame::CalcProxyPos(Vec3f& newPrx, Vec3f& normal, const Vec3f& prx, const Vec3f& dev){
	Affinef inv = afParent.inv();
	Vec3f prxL = inv*prx;
	Vec3f devL = inv*dev;
	if (shape){
		TShape* rv = shape->CalcProxyPos(newPrx, normal, prxL, devL);
		if (rv){
			newPrx = afParent*newPrx;
			normal = afParent.Rot() * normal;
			return rv;
		}
	}
	for(TFrames::iterator it = children.begin(); it != children.end(); ++it){
		TShape* rv = (*it)->CalcProxyPos(newPrx, normal, prxL, devL);
		if (rv){
			newPrx = afParent * newPrx;
			normal = afParent.Rot() * normal;
			return rv;
		}
	}
	return NULL;
}
void TFrame::Draw(){
	glPushMatrix();
	glMultMatrixf(AfParent());
	if(shape) shape->Draw();
	for(TFrames::iterator it = children.begin(); it != children.end(); ++it){
		(*it)->Draw();
	}
	glPopMatrix();
}

}
