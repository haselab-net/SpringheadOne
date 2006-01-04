#pragma once
#ifndef PHWATERCONTACT_H
#define PHWATERCONTACT_H

#include <Base/BaseUtility.h>
#include <SceneGraph/SGScene.h>
#include <Collision/CDMesh.h>
#include <vector>

//	FRM���g���ꍇ�́CUSE_FRM���`	FRM�̃\�[�X�͔���J�D
//	#define USE_FRM

#ifdef USE_FRM
#include "PHWaterResistanceMap.h"
#endif

namespace Spr{;

//���̕ӂ̋@�\�͏o�������Collision�ƃ}�[�W�������Ƃ���...
//PHW�T�t�B�b�N�X��PHWater�����N���X�̈�

class CDConvex;
class CDMesh;
//typedef std::vector<UTRef<CDConvex> > CDConveces;

//�W�I���g�������ʑ��ʑ̂����X�g�A�b�v��������
class PHWaterContactEngine;
class PHWGeometry : public UTRefCount{
public:
	PHWGeometry();
	UTRef<SGFrame>	frame;			//���̃W�I���g����������q�t���[��
	CDMesh*			mesh;
#ifdef USE_FRM
	PHWaterResistanceMap* frm;		///
#endif
	CDGeometries	conveces;		//���̃W�I���g�����\������ʑ��ʑ�
	void Set(SGFrame* f, CDMesh* g, PHWaterContactEngine* e);
};
typedef std::vector<UTRef<PHWGeometry> >	PHWGeometries;

class PHSolid;
class PHWSolid : public UTRefCount{
public:
	UTRef<PHSolid>		solid;		//����
	UTRef<SGFrame>		frame;		//���̃t���[��
	//UTRef<PHWResistanceMap> frm;
//	Affinef				posture;	//���̃t���[���̃��[���h�t���[���ɑ΂���posture
	PHWGeometries		geometries;	//���̂̃t���[���̌`��f�[�^
	void Init(PHWaterContactEngine* e);
	void EnumGeometries(SGFrame*, PHWaterContactEngine* e);
};
typedef std::vector<UTRef<PHWSolid> >		PHWSolids;

/**	���̂Ɨ��̂Ƃ̐ڐG�͌v�Z�G���W��.
	����ł�PHWater 1 �� PHSolid ��
	�������PHSolid���m�̐ڐG�͕ʓrPHContactEngine�ł���Ă��������D
 */
class PHWater;
class PHWaterContactEngine : public SGBehaviorEngine{
public:
	PHSolid* soPaddle;
	Vec3f paddleForceK;
	Vec3f paddleForceB;
	Vec3f paddleTorqueK;
	Vec3f paddleTorqueB;
public:
	SGOBJECTDEF(PHWaterContactEngine);
	bool bUseFrm;

	UTRef<PHWater>	water;			///��
	PHWSolids		solids;			///���ɐڐG���鍄��
#ifdef USE_FRM
	PHWaterResistanceMaps	frms;	///
#endif

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

