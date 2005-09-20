#ifndef LVRM_WXD3AFFINE_H
#define LVRM_WXD3AFFINE_H

#include <Base/Affine.h>
#include <d3drm.h>

namespace LVRM {

typedef Vec2f WXVec2;
/**	D3DVECTOR �̃N���X��	*/
class WXVec3:public Vec3f{
public:
	typedef Vec3f base_type;
	DEF_TVECTOR_BASIC_MEMBER(WXD3Vec3);
	WXD3Vec3(float xi = float(0), float yi = float(0), float zi = float(0)):Vec3f(xi, yi, zi){}
	WXD3Vec3(const PTM::TVectorBaseOp<DIMENC(3), PTM::TVectorIteratorPair<DIMENC(1), float>, PTM::TVectorBuffer<3, float> > &v):Vec3f(v){}
	WXD3Vec3(const D3DVECTOR& v):Vec3f(v.x, v.y, v.z){}
	operator D3DVECTOR&(){ return (D3DVECTOR&)*this; }
};
/**	D3DMATRIX �̃N���X��	*/
class WXAffine:public Affinef{
public:
	typedef WXD3Affine base_type;
	DEF_TMATRIX_BASIC_MEMBER(WXD3Affine);
	WXD3Affine(const PTM::TMatrixBaseOp<DIMENC(4), DIMENC(4), PTM::TVectorIteratorPair<DIMENC(1),float>,PTM::TVectorIteratorPair<DIMENC(4),float>, PTM::TVectorBuffer<16, float> >& a):Affinef(a){}
	WXD3Affine(const PTM::TMatrix<4, 4, float>& a){*this = a;}
	WXD3Affine(const Affinef& a):Affinef(a){}
	///	�P�ʍs��
	WXD3Affine(){}
	///	���s�ړ�
	WXD3Affine(float px, float py, float pz):Affinef(px, py, pz){}
	///	���x�N�g���𒼐ڑ��
	WXD3Affine(WXD3Vec3 exi, WXD3Vec3 eyi, WXD3Vec3 posi=WXD3Vec3()):Affinef(exi, eyi, posi){}
	/**	���x�N�g���𒼐ڑ��.
		@param a	axis���̊��x�N�g��
		@param b	(axis+1)%3 ���̊��x�N�g��
		@param axis	a�̕\����. 'x', 'y', 'z' �̂ǂꂩ
	*/
	WXD3Affine(WXD3Vec3 a, WXD3Vec3 b, char axis, WXD3Vec3 posi=WXD3Vec3()):Affinef(a, b, axis, posi) {}
	/**	�������].
		@param th	��]�p(���W�A��)
		@param axis	��]��. 'x', 'y', 'z' �̂ǂꂩ	*/
	WXD3Affine(float th, char axis , WXD3Vec3 posi=WXD3Vec3()):Affinef(th, axis, posi) {}
	/**	�������].
		@param th	��]�p(���W�A��)
		@param axis	��]��.	*/
	WXD3Affine(float th, WXD3Vec3 axis , WXD3Vec3 posi=WXD3Vec3()):Affinef(th, axis , posi){}

	/**	�ˉe�s��Ƃ��ď�����
		@param screen	�J�������猩���r���[�|�[�g�̒��S�̈ʒu
		@param size		�r���[�|�[�g�̃T�C�Y
		@param front	��O�̃N���b�s���O���ʂƃJ�����̋���
		@param back		���̃N���b�s���O���ʂƃJ�����̋���	*/
	WXD3Affine(WXD3Vec3 screen, Vec2f size, float front=1.0f, float back=10000.0f){
		Vec3f sc = screen;
		Vec2f sz = size;
		InitProjectionD3D(sc, sz, front, back);
	}
	operator D3DMATRIX*(){ return (D3DMATRIX*) this; }

#ifdef __BORLANDC__
	typedef float FLOAT4[4];
    operator FLOAT4*() { return (FLOAT4*)this; }
#endif
};

}	//	namespace LVRM

#endif
