#ifndef PHPHYSICALMATERIAL_H
#define PHPHYSICALMATERIAL_H

#include <SceneGraph/SGObject.h>

namespace Spr {;

/// 物理マテリアルのデータ
class PHPhysicalMaterialData{
public:
	PHPhysicalMaterialData(){ InitData(); }
	///
	void InitData();
	///	抗力のばね係数
	float reflexSpring;
	///	抗力のダンパ係数
	float reflexDamper;
	///	摩擦力のばね係数
	float frictionSpring;
	///	摩擦力のダンパ係数
	float frictionDamper;
	///	最大静止摩擦係数
	float staticFriction;
	///	動摩擦係数
	float dynamicFriction;
};

/// 物理マテリアル
class PHPhysicalMaterial:public SGObject{
public:
	SGOBJECTDEF(PHPhysicalMaterial);
	/// 物理マテリアルのデータ
	PHPhysicalMaterialData pMatData;
	PHPhysicalMaterial(){
	};	
};

}
#endif