#ifndef CDPhysicalMaterial_H
#define CDPhysicalMaterial_H

#include <SceneGraph/SGObject.h>

namespace Spr {;

/// �����}�e���A���̃f�[�^
class CDPhysicalMaterialData{
public:
	CDPhysicalMaterialData(){ InitData(); }
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
class CDPhysicalMaterial:public SGObject{
public:
	SGOBJECTDEF(CDPhysicalMaterial);
	/// �����}�e���A���̃f�[�^
	CDPhysicalMaterialData pMatData;
	CDPhysicalMaterial(){
	};	
};

}
#endif