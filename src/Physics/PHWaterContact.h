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
class PHWaterRegistanceMap;
class PHWaterContactEngine;
class PHWGeometry : public UTRefCount{
public:
	PHWGeometry();
	UTRef<SGFrame>	frame;			//���̃W�I���g����������q�t���[��
	CDMesh*			mesh;
	PHWaterRegistanceMap* frm;		///
	CDGeometries	conveces;		//���̃W�I���g�����\������ʑ��ʑ�
	void Set(SGFrame* f, CDMesh* g, PHWaterContactEngine* e);
};
typedef std::vector<UTRef<PHWGeometry> >	PHWGeometries;

class PHSolid;
/** FRM�N���X�D

 */
class PHWForceTexture{		//ws��TfluidForceTex
public:
	int ndata;	//prs, fri�̃f�[�^��
	int rate;	//���̌v�����^�C���X�e�b�v�̋t��
	float v0;	//��{����
	std::vector<Vec3f>	prs, fri;	//valarray�̕����������ȁH
};
class PHWaterRegistanceMap;
class PHWHapticSource{		//ws��ThapticSource��TforceSet�i1��1�Ǝv����̂ŗZ��)
public:
	float dthe, dphi;
	int	  nthe, nphi;
	int   ntex;
	float v0;
	Vec3f pos, normal;
	std::vector<PHWForceTexture> ftex;
	float pressure;
	PHWaterRegistanceMap* frm;

	//	�����̐ݒ�	�����ŁCpressure���v�Z����D
	//	���ۂ́C�قځCprs �������l�������C prs * normal �����͕␳�l�Ȃ̂ŁC���ꂾ�����v�Z����D
	void SetVelocity(float theta, float phi, Vec3f v, float t);
	//	
	float GetPressure();
	//	Mesh���W�n�ł� Haptic Soruce �̈ʒu
	Vec3f GetPos();//{ return (pos * 25) + Vec3f(-0.5, -0.28, 0); }
};
class PHWaterRegistanceMap : public SGObject{	//ws��ThapticObj�݂����Ȃ���
public:
	SGOBJECTDEF(PHWaterRegistanceMap);
	PHWaterRegistanceMap();
	
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	virtual void Loaded(SGScene* scene);
	void InitMesh();
	void InitFrmMap();
	void SetVelocity(Vec3f vel, float t);
	
	UTRef<SGFrame>	frame;
	UTRef<CDMesh>	mesh;
	UTString		filename;
	float			pressureGain;	//haptic source���v�Z�������͂Ɋ|�����킹��萔�D
	Affinef			posture;		//mesh�ɑ΂��鑊�Εϊ��D
	TVec3<int>		sym;			//�V�����g���[�t���O
	std::vector<PHWHapticSource> hsrc;
	std::vector<PHWHapticSource*> vtxHsrcMap;
	std::vector<PHWHapticSource*> dirHsrcMap;

	//	������W�̍ŋߖT��hsrc�������邽�߂̃}�b�v
	enum{
		NTHETA=20,
		NPHI=40,
	} DirMapSize;
	float dTheta, dPhi;	//	�}�b�v�̊Ԋu
	//	�ŋߖ_��Hsrc��������
	PHWHapticSource* FindHsrc(Vec3f pos);
};
class  PHWaterRegistanceMaps: public std::vector<UTRef<PHWaterRegistanceMap> >{
};

class PHWSolid : public UTRefCount{
public:
	UTRef<PHSolid>		solid;		//����
	UTRef<SGFrame>		frame;		//���̃t���[��
	//UTRef<PHWRegistanceMap> frm;
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
	SGOBJECTDEF(PHWaterContactEngine);
	bool bUseFrm;

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

