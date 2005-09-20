#include "HIOrientation.h"


namespace Spr{;

HIOBJECTIMPABST(HIOrientation, HIForceDevice6D);

bool HIOrientation::Init(DVDeviceManager& devMan){
	return devMan.VPool().Rent(dev);
}

void HIOrientation::Update(float dt){
	Matrix3f m;
	dev->GetMatrix(m);
	ori.from_matrix(m);
}



}