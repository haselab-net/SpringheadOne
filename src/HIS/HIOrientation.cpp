#include "HIOrientation.h"


namespace Spr{;


bool HIOrientation::Init(DVDeviceManager& devMan){
	return devMan.VPool().Rent(dev);
}

void HIOrientation::Update(float dt){
	Matrix3f m;
	dev->GetMatrix(m);
	ori.from_matrix(m);
}



}