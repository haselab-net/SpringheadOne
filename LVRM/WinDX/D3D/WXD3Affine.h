#ifndef LVRM_WXD3AFFINE_H
#define LVRM_WXD3AFFINE_H

#include <Base/Affine.h>
#include <d3drm.h>

namespace LVRM {

typedef Vec2f WXVec2;
/**	D3DVECTOR のクラス版	*/
class WXVec3:public Vec3f{
public:
	typedef Vec3f base_type;
	DEF_TVECTOR_BASIC_MEMBER(WXD3Vec3);
	WXD3Vec3(float xi = float(0), float yi = float(0), float zi = float(0)):Vec3f(xi, yi, zi){}
	WXD3Vec3(const PTM::TVectorBaseOp<DIMENC(3), PTM::TVectorIteratorPair<DIMENC(1), float>, PTM::TVectorBuffer<3, float> > &v):Vec3f(v){}
	WXD3Vec3(const D3DVECTOR& v):Vec3f(v.x, v.y, v.z){}
	operator D3DVECTOR&(){ return (D3DVECTOR&)*this; }
};
/**	D3DMATRIX のクラス版	*/
class WXAffine:public Affinef{
public:
	typedef WXD3Affine base_type;
	DEF_TMATRIX_BASIC_MEMBER(WXD3Affine);
	WXD3Affine(const PTM::TMatrixBaseOp<DIMENC(4), DIMENC(4), PTM::TVectorIteratorPair<DIMENC(1),float>,PTM::TVectorIteratorPair<DIMENC(4),float>, PTM::TVectorBuffer<16, float> >& a):Affinef(a){}
	WXD3Affine(const PTM::TMatrix<4, 4, float>& a){*this = a;}
	WXD3Affine(const Affinef& a):Affinef(a){}
	///	単位行列
	WXD3Affine(){}
	///	平行移動
	WXD3Affine(float px, float py, float pz):Affinef(px, py, pz){}
	///	基底ベクトルを直接代入
	WXD3Affine(WXD3Vec3 exi, WXD3Vec3 eyi, WXD3Vec3 posi=WXD3Vec3()):Affinef(exi, eyi, posi){}
	/**	基底ベクトルを直接代入.
		@param a	axis軸の基底ベクトル
		@param b	(axis+1)%3 軸の基底ベクトル
		@param axis	aの表す軸. 'x', 'y', 'z' のどれか
	*/
	WXD3Affine(WXD3Vec3 a, WXD3Vec3 b, char axis, WXD3Vec3 posi=WXD3Vec3()):Affinef(a, b, axis, posi) {}
	/**	軸周り回転.
		@param th	回転角(ラジアン)
		@param axis	回転軸. 'x', 'y', 'z' のどれか	*/
	WXD3Affine(float th, char axis , WXD3Vec3 posi=WXD3Vec3()):Affinef(th, axis, posi) {}
	/**	軸周り回転.
		@param th	回転角(ラジアン)
		@param axis	回転軸.	*/
	WXD3Affine(float th, WXD3Vec3 axis , WXD3Vec3 posi=WXD3Vec3()):Affinef(th, axis , posi){}

	/**	射影行列として初期化
		@param screen	カメラから見たビューポートの中心の位置
		@param size		ビューポートのサイズ
		@param front	手前のクリッピング平面とカメラの距離
		@param back		奥のクリッピング平面とカメラの距離	*/
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
