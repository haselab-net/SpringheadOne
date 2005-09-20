#ifndef QUATERNION_H
#define QUATERNION_H

/**	@file Quaternion.h 4元数クラスの定義．	*/

#include "Affine.h"

namespace LVRM {

#define Matrix3 Matrix3f
#define SCALAR float

class Quaternion{
public:
	float value[4];

	Quaternion();
	Quaternion(const Matrix3& m);
	float& X(){ return value[0]; }
	float& Y(){ return value[1]; }
	float& Z(){ return value[2]; }
	float& W(){ return value[3]; }
	operator const float*() const{ return value; }
	operator float*(){ return value; }
	operator const Vec3f&() const { return *(Vec3f*)value; }
	operator Vec3f&() { return *(Vec3f*)value; }
	Vec3f Angle(){
		float theta = 2 * atan2(
                        ( (Vec3f*)this )->norm(), W()
                        );
		Vec3f a = *this;
		a = a.unit() * theta;
		return a;
	}
};


#undef Matrix3
#undef SCALAR

}	//	namespace LVRM

#endif
