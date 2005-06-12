#ifndef PHWATER_H
#define PHWATER_H

#include <ImpD3D/D3Mesh.h>
#include <GraphicsGL/GLMesh.h>

namespace Spr{;

/*
	�E�݌v�ɂ���Ă�WaterContainer�Ƃ����������K������
	�EPHWaterClearForce�̑Ώۂ�Solid�Ɍ��炸�͂��������̑S�ʂȂ̂ŁA
		PHClearForce�Ƃ��ׂ�
*/

class PHWaterEngine : public /*SGBehaviorEngine*/PHSolverBase{
public:
	SGOBJECTDEF(PHWaterEngine);
	
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
	int GetPriority() const {return SGBP_WATERENGINE;}
	virtual void Step(SGScene* s);
	virtual void ClearForce();
	virtual void Loaded(SGScene* scene);
	virtual void Clear(SGScene* s){ waters.clear(); }
	virtual size_t NChildObjects(){ return waters.size(); }
	virtual SGObject* ChildObject(size_t i){ return waters[i]; }

	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;

};

class PHWater : public GRVisual{
public:
	//�p�����[�^
	struct PHWaterInfo{
		size_t	my, mx;		//������
		double	dh;			//�i�q�̕�
		double	depth;		//���[
		double	gravity;	//�d��
		double	hscale;		//�����̃X�P�[��
		//Vec4d	specular;	//�`�����
		//Vec4d	diffuse;
		//double	shininess;
		//Vec3d	vlight;		//���s��

		WaterInfo(){
			mx = my = 0;
			dh = 0.0;
			depth = 0.0;
			hscale = 1.0;
		}
	};

	SGOBJECTDEF(PHWater);

	bool		AddChildObject(SGObject* o, SGScene* s);///< ���[�h���Ɏg�p�D
	size_t		NReferenceObjects();					///< 
	SGObject*	ReferenceObject(size_t i);				///< �t���[����Ԃ��D
	void		Loaded(SGScene* scene);					///< ���[�h�I�����̏�����
	void		Step(SGScene* s);						///< ������i�߂�D

	///	�����_�����O
	virtual void Render(SGFrame* n, GRRender* render);

	/// �p�����[�^�̎擾�E�ݒ�
	const WaterInfo& GetInfo()const{return info;}
	void SetInfo(const WaterInfo& _info){info = _info;}

	/// �p�����[�^���Q�Ƃ��ď���������
	void Init();

protected:
	typedef VMatrixCol<double> matrix_type;
	typedef VMatrixCol<Vec3d>	v3matrix_type;
	typedef VVector<double> vector_type;

	UTRef<SGFrame>	frame;
	UTRef<PHWater>	solid;
	UTRef<GRMaterial> material;

	UTRef<D3Mesh>	meshD3;	//D3�p���b�V���ƃ}�e���A��
	UTRef<D3Material> materialD3;
	UTRef<GLMesh>	meshGL;	//GL�p���b�V��

	//�p�����[�^
	PHWaterInfo		info;
	int				mxy;	//mx * my
	double			dx;		//mx * dh / 2.0
	double			dy;		//my * dh / 2.0
	
	//������ێ�����SGFrame�ւ̎Q��
	UTRef<SGFrame> frame;

	matrix_type		p;				//����
	matrix_type		u, utmp;		//x��������
	matrix_type		v, vtmp;		//y��������
	matrix_type		height, htmp;	//����
	v3matrix_type	normal;			//�@��
	v3matrix_type	tvec;			//
	Vec3d			vlight;			//���̌���

	double
		*p_array,
		*u_array, *utmp_array,
		*v_array, *vtmp_array,
		*height_array, *htmp_array,
		*normal_array,
		*tvec_array;
	
	//���E������ݒ肷��
	void Bound();

};

}

#endif