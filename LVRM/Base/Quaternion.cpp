#include "Quaternion.h"
#define Matrix3 Matrix3f
#define SCALAR float

namespace LVRM {

Quaternion::Quaternion(){
	X() = 0;
	Y() = 0;
	Z() = 0;
	W() = 0;
}
Quaternion::Quaternion(const Matrix3& m){ 
	SCALAR tr;
        SCALAR w;
        
	tr = m[0][0] + m[1][1] + m[2][2];
	if ( tr >= 0 ) {
		w = sqrt(tr+1); 
		W() = 0.5*w; 
		w = 0.5/w; 
		X() = (m.EyZ() - m.EzY()) * w; 
		Y() = (m.EzX() - m.ExZ()) * w; 
		Z() = (m.ExY() - m.EyX()) * w; 
	} else { 
		int i; 
		SCALAR max; 
		
		i=0; max=m.ExX(); 
		if ( m.EyY()>max )  { i=1; max=m.EyY(); } 
		if ( m.EzZ()>max )  { i=2; } 
		
		switch(i) { 
		case 0: 
			w = sqrt(m.ExX() - m.EyY() - m.EzZ() + 1); 
			X() = 0.5*w; 
			w = 0.5/w; 
			Y() = (m.EyX() + m.ExY()) * w; 
			Z() = (m.ExZ() + m.EzX()) * w; 
			W() = (m.EyZ() - m.EzY()) * w; 
			break; 
		case 1: 
			w = sqrt(m.EyY() - m.EzZ() - m.ExX() + 1); 
			Y() = 0.5*w; 
			w = 0.5/w; 
			Z() = (m.EzY() + m.EyZ()) * w; 
			X() = (m.EyX() + m.ExY()) * w; 
			W() = (m.EzX() - m.ExZ()) * w; 
			break; 
		case 2: 
			w = sqrt(m.EzZ() - m.ExX() - m.EyY() + 1); 
			Z() = 0.5*w; 
			w = 0.5/w; 
			X() = (m.ExZ() + m.EzX()) * w; 
			Y() = (m.EzY() + m.EyZ()) * w; 
			W() = (m.ExY() - m.EyX()) * w; 
			break; 
		} 
	} 
}

}	//	namespace LVRM

#undef Matrix3
#undef SCALAR
