#include "HISpidarMotor.h"
#include <algorithm>
#include <conio.h>
namespace LVRM {

HISpidarMotor::HISpidarMotor(){
	maxForce = 20;
	minForce = 1;
	voltPerNewton = 0.365296803653f;
	lengthPerPulse = 2.924062107079e-5f;
	da = NULL;
	counter = NULL;
}

}	//	namespace LVRM
