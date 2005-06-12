#ifndef PHWATER_H
#define PHWATER_H

#include <ImpD3D/D3Mesh.h>
#include <GraphicsGL/GLMesh.h>

namespace Spr{;

/*
	・設計によってはWaterContainerという命名が適当かも
	・PHWaterClearForceの対象はSolidに限らず力が加わるもの全般なので、
		PHClearForceとすべき
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

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;

};

class PHWater : public GRVisual{
public:
	//パラメータ
	struct PHWaterInfo{
		size_t	my, mx;		//分割数
		double	dh;			//格子の幅
		double	depth;		//水深
		double	gravity;	//重力
		double	hscale;		//高さのスケール
		//Vec4d	specular;	//描画特性
		//Vec4d	diffuse;
		//double	shininess;
		//Vec3d	vlight;		//平行光

		WaterInfo(){
			mx = my = 0;
			dh = 0.0;
			depth = 0.0;
			hscale = 1.0;
		}
	};

	SGOBJECTDEF(PHWater);

	bool		AddChildObject(SGObject* o, SGScene* s);///< ロード時に使用．
	size_t		NReferenceObjects();					///< 
	SGObject*	ReferenceObject(size_t i);				///< フレームを返す．
	void		Loaded(SGScene* scene);					///< ロード終了時の初期化
	void		Step(SGScene* s);						///< 時刻を進める．

	///	レンダリング
	virtual void Render(SGFrame* n, GRRender* render);

	/// パラメータの取得・設定
	const WaterInfo& GetInfo()const{return info;}
	void SetInfo(const WaterInfo& _info){info = _info;}

	/// パラメータを参照して初期化する
	void Init();

protected:
	typedef VMatrixCol<double> matrix_type;
	typedef VMatrixCol<Vec3d>	v3matrix_type;
	typedef VVector<double> vector_type;

	UTRef<SGFrame>	frame;
	UTRef<PHWater>	solid;
	UTRef<GRMaterial> material;

	UTRef<D3Mesh>	meshD3;	//D3用メッシュとマテリアル
	UTRef<D3Material> materialD3;
	UTRef<GLMesh>	meshGL;	//GL用メッシュ

	//パラメータ
	PHWaterInfo		info;
	int				mxy;	//mx * my
	double			dx;		//mx * dh / 2.0
	double			dy;		//my * dh / 2.0
	
	//自分を保持するSGFrameへの参照
	UTRef<SGFrame> frame;

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
	
	//境界条件を設定する
	void Bound();

};

}

#endif