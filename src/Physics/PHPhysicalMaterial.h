#ifndef PHPHYSICALMATERIAL_H
#define PHPHYSICALMATERIAL_H

#include <SceneGraph/SGObject.h>

namespace Spr {;

/// �����}�e���A���̃f�[�^
class PHPhysicalMaterialData{
public:
	PHPhysicalMaterialData(){ InitData(); }
	///
	void InitData();
	///	�R�͂̂΂ˌW��
	float reflexSpring;
	///	�R�͂̃_���p�W��
	float reflexDamper;
	///	���C�͂̂΂ˌW��
	float frictionSpring;
	///	���C�͂̃_���p�W��
	float frictionDamper;
	///	�ő�Î~���C�W��
	float staticFriction;
	///	�����C�W��
	float dynamicFriction;
};

/// �����}�e���A��
class PHPhysicalMaterial:public SGObject{
public:
	SGOBJECTDEF(PHPhysicalMaterial);
	/// �����}�e���A���̃f�[�^
	PHPhysicalMaterialData pMatData;
	PHPhysicalMaterial(){
	};	
};

}
#endif