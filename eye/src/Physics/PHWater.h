#ifndef PHWATER_H
#define PHWATER_H

#include <Base/Affine.h>
#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Graphics/GRVisual.h>
#include <Physics/PHSolid.h>
#include <Collision/CDGeometry.h>

using namespace PTM;
namespace Spr{;

/*
	�E�݌v�ɂ���Ă�WaterContainer�Ƃ����������K������
	�EPHWaterClearForce�̑Ώۂ�Solid�Ɍ��炸�͂��������̑S�ʂȂ̂ŁA
		PHClearForce�Ƃ��ׂ�

	PHWater�̍��W�̎����F
	�EZ���͐Ð��ʂɒ������A������D
	�E�i�q��x�������Ɋi�q��mx + 1�C��mx * dh�Cy�������Ɋi�q��my + 1�C��my + dh�D
	�E���W���_�͊i�q�̒����D
	�E���̂Ƃ��됅�[depth�̓X�J���萔����height�Ɠ������i�q���ƂɈقȂ�l���Ƃ�悤�Ɋg��������
*/

class D3Mesh;
class GLMesh;
class GRMaterial;
class D3Material;
class D3Render;
class GLRender;

class PHWaterTrackTarget : public SGObject{
public:
	SGOBJECTDEF(PHWaterTrackTarget);

	bool		AddChildObject(SGObject* o, SGScene* s);
	size_t		NReferenceObjects(){return targets.size();}
	SGObject*	ReferenceObject(size_t i){return targets[i];}

	SGFrames targets;
};
#if 0
class PHWFRM : public UTRefCount{
public:
	class ForceSet{
	public:
		
	};
public:
	int id;
	float buo_scl, pres_scl, fric_scl, vel_scl, unit_mass, disp_scl;
    float wz, wa;
	float dlen;
	Affinef	posture;
	float v0;
	FIString	filename;
	Vec3f	d, s;

	float dthe, dphi;
	int nhsrc, ndata;
	float rate;
	int nthe, nphi, ntex, sym[3], n;
	vector<ForceSet> frc_set;

	bool Load(FIString);
};
#endif
class PHWater : public GRVisual{
public:
	SGOBJECTDEF(PHWater);
	bool		AddChildObject(SGObject* o, SGScene* s);///< ���[�h���Ɏg�p�D
	size_t		NReferenceObjects();					///< 
	SGObject*	ReferenceObject(size_t i);				///< �t���[����Ԃ��D
	void		Loaded(SGScene* scene);					///< ���[�h�I�����̏�����
	void		Step(SGScene* s);						///< ������i�߂�D
	Affinef		GetPosture(){ return frame->GetWorldPosture(); }
	float edgeWidth;									///< �`�掞�ɕt����ӂ��̕�

	///	�����_�����O
	virtual void Render(SGFrame* fr, GRRender* render);
	void CalcNormal();

	///�͂��N���A����
	void ClearForce(){}

	///�i�q���Ԃ̒l����`�⊮���ĕԂ�
	double LerpHeight(double x, double y);
/*
	double LerpVelocityU(double x, double y);
	double LerpVelocityV(double x, double y);
	Vec2f LerpVelocity(double x, double y){
		return Vec2f(LerpVelocityU(x,y), LerpVelocityV(x,y));
	}
*/
	//�p�����[�^
	UTRef<SGFrame> frame;	//	�e�t���[��
	int		my, mx;			//������
	TVec2<int> bound;		//���E ���̃Z���Ǝ��̃Z�������E�̃Z���ɂȂ�D
	TVec2<int> texOffset;	//bound �Ɠ��������C%mx ���Ȃ��D
	Vec2d	velocity;		//��헬���D���I�ɂ��̑��x�Ő��������D
	double	dh, dhinv;		//�i�q�̕�
	double	depth;			//���[
	double	gravity;		//�d��
	double	density;		//���x
	double	loss;			//�����W��

	PHWater();
	friend class PHWaterContactEngine;
	friend struct PHWConvexCalc;

//protected:
	typedef VMatrixCol<double> matrix_type;
	typedef VMatrixCol<Vec3d>	v3matrix_type;
	typedef VVector<double> vector_type;

	//�Ǐ]�Ώ�
	UTRef<PHWaterTrackTarget> targets;

	UTRef<GRMaterial> material;
	UTRef<D3Material> materialD3;

	int				mxy;	//mx * my
	double			rx;		//mx * dh / 2.0
	double			ry;		//my * dh / 2.0
	
	matrix_type		p;				//����
	matrix_type		u, utmp;		//x��������
	matrix_type		v, vtmp;		//y��������
	matrix_type		height, htmp;	//����
	v3matrix_type	normal;			//�@��
	v3matrix_type	tvec;			//
	Vec3d			vlight;			//���̌���

	///����������
	void Init(SGScene* scene);

	///���E������ݒ肷��
	void Bound();

	///�ϕ�����
	void Integrate(double dt);

	///���܃x�N�g���̌v�Z
	Vec3d calRefracVec(Vec3d n, Vec3d v, double ra);

	void RenderD3(SGFrame* fr, D3Render* render);
	void RenderGL(SGFrame* fr, GLRender* render);
	DWORD GetColor(float h);
};
class CDWater: public CDGeometry{
public:
	SGOBJECTDEF(CDWater);
	UTRef<PHWater> water;
	virtual void CalcBBox(Vec3f& bbMin, Vec3f& bbMax);
	virtual int GeometryID();
};

typedef UTRefArray<PHWater> PHWaters;

class PHWaterEngine : public /*SGBehaviorEngine*/PHSolverBase{
public:
	SGOBJECTDEF(PHWaterEngine);

	PHWaters waters;
	
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
	int GetPriority() const {return SGBP_DYNAMICALSYSTEM;}
	virtual void Step(SGScene* s);
	virtual void ClearForce();
	virtual void Loaded(SGScene* scene);
	virtual void Clear(SGScene* s);
	virtual size_t NChildObjects(){ return waters.size(); }
	virtual SGObject* ChildObject(size_t i){ return (SGObject*)&*(waters[i]); }

	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
};

}

#endif