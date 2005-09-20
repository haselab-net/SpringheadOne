#include "IHCSpidar4quad.hpp"
#include "IHCDeviceManager.hpp"
#include "spidar.h"
#include "iodefine.h"
#include <stdio.h>

extern IHCDeviceManager* pdevMan;
extern IHCSpidar4quad* pSpidar[SPIDAR_4_SIZE];

void SpidarThread()
{
	PORT.PDTRA.WORD &= ~0x8000; //Port8 Up

	int i;
	int faceType = 0;

	for(i=0;i<SPIDAR_4_SIZE;i++){
		const float dumperBmin = 50.0f; //1.3[N/ms-1] lamda = 0.5
		//const float dumperBmin = 0.8f; //[N/ms-1]　意外に大きい値だけど。。。 lamda = 0.1
		pSpidar[i]->Update(pdevMan->ShFreqHz);
#if 1
		float getForceSquare = pSpidar[i]->getForce.square();
		if(getForceSquare > 1e-10f){
			Vec3f dPos = pSpidar[i]->localPos - pSpidar[i]->sendPos;
			Vec3f p,df,pv;
			switch(faceType){
			case 0:// alg 0 (face)
				p = ((dPos*pSpidar[i]->getForce)/getForceSquare) * pSpidar[i]->getForce;
				pv = ((pSpidar[i]->velEnc * pSpidar[i]->getForce)/getForceSquare) * pSpidar[i]->getForce;
				df = - p * pSpidar[i]->stiffnessK - pv * pSpidar[i]->dumperB;
				break;
			case 1:// alg 1 (convex)
				df =   dPos * pSpidar[i]->stiffnessK - pSpidar[i]->velEnc * pSpidar[i]->dumperB;
				break;
			case 2:// alg 2 (concave)
				df = - dPos * pSpidar[i]->stiffnessK - pSpidar[i]->velEnc * pSpidar[i]->dumperB;
				break;
			}
			pSpidar[i]->localForce = pSpidar[i]->getForce + df;
			pSpidar[i]->SetForce(pSpidar[i]->localForce);
		}
		else{ //力覚を提示していないときのメカ自体のバンパー成分の除去
			pSpidar[i]->localForce = pSpidar[i]->velEnc * dumperBmin;
			pSpidar[i]->SetForce(pSpidar[i]->localForce);
			//pSpidar[i]->localForce.Print();
			//pSpidar[i]->velEnc.Print();
		}
#else
		pSpidar[i]->localForce = pSpidar[i]->getForce;
		pSpidar[i]->SetForce(pSpidar[i]->localForce);
#endif
	}
	pdevMan->Update();

	PORT.PDTRA.WORD |= 0x8000;  //Port8 Down
}

