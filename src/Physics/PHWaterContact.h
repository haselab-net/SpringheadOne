#pragma once
#ifndef PHWATERCONTACT_H
#define PHWATERCONTACT_H

#include <Base/BaseUtility.h>
#include <SceneGraph/SGScene.h>
#include <vector>

namespace Spr{;

//���̕ӂ̋@�\�͏o�������Collision�ƃ}�[�W�������Ƃ���...
//PHW�T�t�B�b�N�X��PHWater�����N���X�̈�

class CDConvex;
class CDMesh;
//typedef std::vector<UTRef<CDConvex> > CDConveces;

//�W�I���g�������ʑ��ʑ̂����X�g�A�b�v��������
class PHWGeometry : public UTRefCount{
public:
	UTRef<SGFrame>	frame;			//���̃W�I���g����������q�t���[��
	Vec3f			bbmin, bbmax;	//���̃W�I���g����BBOX
	CDMesh*			mesh;
	CDGeometries	conveces;		//���̃W�I���g�����\������ʑ��ʑ�
	void Set(SGFrame* f, CDMesh* g);
};
typedef std::vector<UTRef<PHWGeometry> >	PHWGeometries;

class PHSolid;
/** FRM�N���X�D

 */
class PHWForceTexture{		//ws��TfluidForceTex

};
class PHWHapticSource{		//ws��ThapticSource��TforceSet�i1��1�Ǝv����̂ŗZ��)

};
class PHWaterRegistanceMap : public SGObject{	//ws��ThapticObj�݂����Ȃ���
public:
	SGOBJECTDEF(PHWaterRegistanceMap);

	virtual bool AddChildObject(SGObject* o, SGScene* s);
	virtual void Loaded(SGScene* scene);
	
	UTRef<PHSolid>	solid;
	UTString		filename;
};
typedef std::vector<UTRef<PHWaterRegistanceMap> > PHWaterRegistanceMaps;

class PHWSolid : public UTRefCount{
public:
	UTRef<PHSolid>		solid;		//����
	UTRef<SGFrame>		frame;		//���̃t���[��
	//UTRef<PHWRegistanceMap> frm;
	Affinef				posture;	//���̃t���[���̃��[���h�t���[���ɑ΂���posture
	PHWGeometries		geometries;	//���̂̃t���[���̌`��f�[�^
	void Init();
	void EnumGeometries(SGFrame*);
};
typedef std::vector<UTRef<PHWSolid> >		PHWSolids;

/**	���̂Ɨ��̂Ƃ̐ڐG�͌v�Z�G���W��.
	����ł�PHWater 1 �� PHSolid ��
	�������PHSolid���m�̐ڐG�͕ʓrPHContactEngine�ł���Ă��������D
 */
class PHWater;
class PHWaterContactEngine : public SGBehaviorEngine{
public:
	SGOBJECTDEF(PHWaterContactEngine);

	UTRef<PHWater>	water;			///��
	PHWSolids		solids;			///���ɐڐG���鍄��
	PHWaterRegistanceMaps	frms;	///

	std::vector<Vec3f> tris;		///<	�f�o�b�O�p:���ʉ���3�p�`
	std::vector<Vec3f> points;		///<	�f�o�b�O�p:���̂Əd�Ȃ��Ă��鐅

	///
	PHWaterContactEngine();

	///	�q�I�u�W�F�N�g�̒ǉ�
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	���[�h�������̏���
	virtual void Loaded(SGScene* scene);
	///	�����������ALoaded���Ă�
	virtual void Init(SGScene* scene);
	///
	int GetPriority() const { return SGBP_WATERCONTACTENGINE; }
	///	���Ԃ� dt �i�߂�D
	virtual void Step(SGScene* s);
	///	
	virtual void Clear(SGScene* s);
	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
	///
	virtual void Render(GRRender* r, SGScene* s);
};


}
#endif

