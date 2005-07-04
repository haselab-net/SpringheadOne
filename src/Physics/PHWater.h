#ifndef PHWATER_H
#define PHWATER_H

#include <Base/Affine.h>
#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Graphics/GRVisual.h>
#include <Physics/PHSolid.h>

using namespace PTM;
namespace Spr{;

/*
	・設計によってはWaterContainerという命名が適当かも
	・PHWaterClearForceの対象はSolidに限らず力が加わるもの全般なので、
		PHClearForceとすべき

	PHWaterの座標の取り方：
	・Z軸は静水面に直交し、上向き．
	・格子はx軸方向に格子数mx + 1，幅mx * dh，y軸方向に格子数my + 1，幅my + dh．
	・座標原点は格子の中央．
	・今のところ水深depthはスカラ定数だがheightと同じく格子ごとに異なる値をとるように拡張したい
*/

class D3Mesh;
class GLMesh;
class GRMaterial;
class D3Material;
class D3Render;
class GLRender;

class PHWater : public GRVisual{
public:
	SGOBJECTDEF(PHWater);

	bool		AddChildObject(SGObject* o, SGScene* s);///< ロード時に使用．
	size_t		NReferenceObjects();					///< 
	SGObject*	ReferenceObject(size_t i);				///< フレームを返す．
	void		Loaded(SGScene* scene);					///< ロード終了時の初期化
	void		Step(SGScene* s);						///< 時刻を進める．

	///	レンダリング
	virtual void Render(SGFrame* n, GRRender* render);

	///力をクリアする
	void ClearForce(){}

	///フレームを取得
	SGFrame* GetFrame(){return frame;}
	PHSolid* GetSolid(){return solid;}

	///格子中間の値を線形補完して返す
	double LerpHeight(double x, double y);

	//パラメータ
	int		my, mx;		//分割数
	double	dh;			//格子の幅
	double	depth;		//水深
	double	gravity;	//重力
	double	hscale;		//高さのスケール
	double	density;	//密度
	double	loss;		//損失係数
	//Vec4d	specular;	//描画特性
	//Vec4d	diffuse;
	//double	shininess;
	//Vec3d	vlight;		//平行光
		/// パラメータの取得・設定
	//const WaterInfo& GetInfo()const{return info;}
	//void SetInfo(const WaterInfo& _info){info = _info;}

	PHWater();

protected:
	typedef VMatrixCol<double> matrix_type;
	typedef VMatrixCol<Vec3d>	v3matrix_type;
	typedef VVector<double> vector_type;

	UTRef<SGFrame>	frame;
	UTRef<PHSolid>	solid;
	UTRef<GRMaterial> material;

	UTRef<D3Mesh>	meshD3;	//D3用メッシュとマテリアル
	UTRef<D3Material> materialD3;
	UTRef<GLMesh>	meshGL;	//GL用メッシュ

	int				mxy;	//mx * my
	double			dx;		//mx * dh / 2.0
	double			dy;		//my * dh / 2.0
	
	matrix_type		p;				//圧力
	matrix_type		u, utmp;		//x方向流速
	matrix_type		v, vtmp;		//y方向流速
	matrix_type		height, htmp;	//高さ
	v3matrix_type	normal;			//法線
	v3matrix_type	tvec;			//
	Vec3d			vlight;			//光の向き

	double
		*p_array,
		*u_array, *utmp_array,
		*v_array, *vtmp_array,
		*height_array, *htmp_array,
		*normal_array,
		*tvec_array;

	///初期化処理
	void Init(SGScene* scene);

	///
	void Shift();

	///境界条件を設定する
	void Bound();

	///積分する
	void Integrate(double dt);

	///屈折ベクトルの計算
	Vec3d calRefracVec(Vec3d n, Vec3d v, double ra);

	void RenderD3(SGFrame* n, D3Render* render);
	void RenderGL(SGFrame* n, GLRender* render);
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

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
};

}

#endif