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
	CDGeometries	conveces;		//���̃W�I���g�����\������ʑ��ʑ�
	void Set(SGFrame* f, CDMesh* g);
};
typedef std::vector<UTRef<PHWGeometry> >	PHWGeometries;

//���̂����`��f�[�^�����X�g�A�b�v��������
class PHSolid;
class PHWSolid : public UTRefCount{
public:
	UTRef<PHSolid>		solid;		//����
	UTRef<SGFrame>		frame;		//���̃t���[��
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
	///���t���[���̃��[���h�t���[���ɑ΂���posture
	Affinef			water_posture, water_posture_inv;
	PHWSolids		solids;			///���ɐڐG���鍄��

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
};


}
#endif

