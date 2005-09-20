//---------------------------------------------------------------------------
#ifndef SPH_CBMatrixH
#define SPH_CBMatrixH
//---------------------------------------------------------------------------
#include <Base/Affine.h>
#include "SHVector.h"
//---------------------------------------------------------------------------

///	3x3行列
class SHMatrix3f:public Spr::Matrix3f{
public:
	SHVec3f& __fastcall GetEx(){ return (SHVec3f&)Spr::Matrix3f::Ex(); }
	SHVec3f& __fastcall GetEy(){ return (SHVec3f&)Spr::Matrix3f::Ey(); }
	SHVec3f& __fastcall GetEz(){ return (SHVec3f&)Spr::Matrix3f::Ez(); }

	void __fastcall SetEx(SHVec3f v){ Spr::Matrix3f::Ex()=v; }
	void __fastcall SetEy(SHVec3f v){ Spr::Matrix3f::Ey()=v; }
	void __fastcall SetEz(SHVec3f v){ Spr::Matrix3f::Ez()=v; }

	DEF_TMATRIX_BASIC_MEMBER(SHMatrix3f);
	SHMatrix3f& operator = (const Spr::Matrix3f& m){
		(Spr::Matrix3f&)*this = m;
		return *this;
	}

	__property SHVec3f Ex={read=GetEx, write=SetEx};
	__property SHVec3f Ey={read=GetEy, write=SetEy};
	__property SHVec3f Ez={read=GetEz, write=SetEz};
};

//---------------------------------------------------------------------------
//	SHMatrix3f型のプロパティのソースコード向けインタフェース
class TSHMatrix3f
{
	//	SHMatrix3fの取得と設定
	typedef SHMatrix3f __fastcall (__closure *TGetter)();
	typedef void __fastcall (__closure *TSetter)(SHMatrix3f);
	TGetter getter;
	TSetter setter;

	SHVec3f __fastcall GetEx(){ return getter().Ex; }
	SHVec3f __fastcall GetEy(){ return getter().Ey; }
	SHVec3f __fastcall GetEz(){ return getter().Ez; }
	void __fastcall SetEx(SHVec3f v){ SHMatrix3f mat = getter(); mat.Ex = v; setter(mat); NotifyChange(); }
	void __fastcall SetEy(SHVec3f v){ SHMatrix3f mat = getter(); mat.Ey = v; setter(mat); NotifyChange(); }
	void __fastcall SetEz(SHVec3f v){ SHMatrix3f mat = getter(); mat.Ez = v; setter(mat); NotifyChange(); }

	//更新通知イベント
	Classes::TNotifyEvent FOnChange;
	//更新を通知
	void NotifyChange(TObject* o=NULL){if (FOnChange) FOnChange(o);}
public:

	//コンストラクタ
	TSHMatrix3f(TGetter g, TSetter s): FOnChange(NULL), getter(g), setter(s),
		Ex(&GetEx, &SetEx), Ey(&GetEy, &SetEy), Ez(&GetEz, &SetEz){}
	//代入
	TSHMatrix3f& operator=(const TSHMatrix3f& src){ return *this= SHMatrix3f(src); }
	TSHMatrix3f& operator=(const SHMatrix3f& src){ setter(src); NotifyChange(); return *this; }
	//SHMatrix3fに変換
	operator SHMatrix3f ()const{ return getter(); }
	TSHMatrix3f* operator ->(){ return this; }
	const TSHMatrix3f* operator ->() const { return this; }

	//更新通知イベント
	__property Classes::TNotifyEvent OnChange= {read=FOnChange, write=FOnChange};

	TSHMatrix3f* __fastcall Get(){return this;}
	void __fastcall Set(TSHMatrix3f* mat){ setter(*mat); }

	TSHVec3f Ex;
	TSHVec3f Ey;
	TSHVec3f Ez;
};

//---------------------------------------------------------------------------
//オブジェクトインスペクタ表示用のTSHMatrix3fの仮の姿
class TSHMatrix3fDummy: public TPersistent
{
	TSHMatrix3f& tmat;

	TSHVec3fDummy* FExDummy;
	TSHVec3fDummy* FEyDummy;
	TSHVec3fDummy* FEzDummy;
	TSHVec3f __fastcall GetEx(){ return tmat->Ex; }
	TSHVec3f __fastcall GetEy(){ return tmat->Ey; }
	TSHVec3f __fastcall GetEz(){ return tmat->Ez; }
	void __fastcall SetEx(TSHVec3f v){ tmat->Ex = v; }
	void __fastcall SetEy(TSHVec3f v){ tmat->Ey = v; }
	void __fastcall SetEz(TSHVec3f v){ tmat->Ez = v; }

public:
	__fastcall TSHMatrix3fDummy(TSHMatrix3f& mat):tmat(mat){
		FExDummy = new TSHVec3fDummy(tmat.Ex);
		FEyDummy = new TSHVec3fDummy(tmat.Ey);
		FEzDummy = new TSHVec3fDummy(tmat.Ez);
	}
	virtual __fastcall ~TSHMatrix3fDummy(){
		delete FExDummy; delete FEyDummy; delete FEzDummy;
	}
	virtual void __fastcall Assign(TPersistent* src);
__published:
	__property TSHVec3fDummy* ExDummy={read=FExDummy, write=FExDummy};
	__property TSHVec3fDummy* EyDummy={read=FEyDummy, write=FEyDummy};
	__property TSHVec3fDummy* EzDummy={read=FEzDummy, write=FEzDummy};
};

//---------------------------------------------------------------------------

#endif
