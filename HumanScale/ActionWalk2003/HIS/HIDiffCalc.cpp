#include "HIDiffCalc.h"
namespace LVRM {


void HDDiffCalc::Update(){
	posHist.inc();
	posHist[0] = fd3->GetPos();
	velHist.inc();
	velHist[0] = (posHist[posHist.size()-1] - posHist[0]) / (dt * posHist.size());
	accel = (velHist[velHist.size()-1] - velHist[0]) / (dt * velHist.size());
}


}	//	namespace LVRM
