//---------------------------------------------------------------------------
#ifndef SPH_CBAffineH
#define SPH_CBAffineH
//---------------------------------------------------------------------------
#include <Base/Affine.h>
#include "SHVector.h"
//---------------------------------------------------------------------------
class SHAffinef:public Spr::Affinef{
public:
	SHVec3f& __fastcall GetEx(){ return (SHVec3f&)Spr::Affinef::Ex(); }
	SHVec3f& __fastcall GetEy(){ return (SHVec3f&)Spr::Affinef::Ey(); }
	SHVec3f& __fastcall GetEz(){ return (SHVec3f&)Spr::Affinef::Ez(); }
	SHVec3f& __fastcall GetPos(){ return (SHVec3f&)Spr::Affinef::Pos(); }
	void __fastcall SetEx(SHVec3f v) { Spr::Affinef::Ex()=v; }
	void __fastcall SetEy(SHVec3f v){ Spr::Affinef::Ey()=v; }
	void __fastcall SetEz(SHVec3f v){ Spr::Affinef::Ez()=v; }
	void __fastcall SetPos(SHVec3f v){ Spr::Affinef::Pos()=v; }

public:
	DEF_TMATRIX_BASIC_MEMBER(SHAffinef);
	DEF_TAFFINE_CONSTRUCTORS(SHAffinef);
	SHAffinef& operator = (const Spr::Affinef& v){
		(Spr::Affinef&)*this = v;
		return *this;
	}

	__property SHVec3f Ex={read=GetEx, write=SetEx};
	__property SHVec3f Ey={read=GetEy, write=SetEy};
	__property SHVec3f Ez={read=GetEz, write=SetEz};
	__property SHVec3f Pos={read=GetPos, write=SetPos};
};

//---------------------------------------------------------------------------
//	SHAffinef型のプロパティのソースコード向けインタフェース
class TSHAffinef
{
	//	SHAffinefの取得と設定
	typedef SHAffinef __fastcall (__closure *TGetter)();
	typedef void __fastcall (__closure *TSetter)(SHAffinef);
	TGetter getter;
	TSetter setter;

	SHVec3f __fastcall GetEx(){ return getter().Ex; }
	SHVec3f __fastcall GetEy(){ return getter().Ey; }
	SHVec3f __fastcall GetEz(){ return getter().Ez; }
	SHVec3f __fastcall GetPos(){ return getter().Pos; }
	void __fastcall SetEx(SHVec3f v){ SHAffinef af = getter(); af.Ex = v; setter(af); NotifyChange(); }
	void __fastcall SetEy(SHVec3f v){ SHAffinef af = getter(); af.Ey = v; setter(af); NotifyChange(); }
	void __fastcall SetEz(SHVec3f v){ SHAffinef af = getter(); af.Ez = v; setter(af); NotifyChange(); }
	void __fastcall SetPos(SHVec3f v){ SHAffinef af = getter(); af.Pos = v; setter(af); NotifyChange(); }

	//更新通知イベント
	Classes::TNotifyEvent FOnChange;
	//更新を通知
	void NotifyChange(TObject* o=NULL){if (FOnChange) FOnChange(o);}
public:
	TSHAffinef(TGetter g, TSetter s): FOnChange(NULL), getter(g), setter(s),
		Ex(&GetEx, &SetEx),
		Ey(&GetEy, &SetEy),
		Ez(&GetEz, &SetEz),
		Pos(&GetPos, &SetPos)
		 {}
	TSHAffinef& operator=(const TSHAffinef& src){ return *this= SHAffinef(src); }
	TSHAffinef& operator=(const SHAffinef& src){ setter(src); NotifyChange(); return *this; }
	//	SHAffinefに変換
	operator SHAffinef ()const{ return getter(); }
	TSHAffinef* operator ->(){ return this; }
	const TSHAffinef* operator ->() const { return this; }

	//更新通知イベント
	__property Classes::TNotifyEvent OnChange= {read=FOnChange, write=FOnChange};

	TSHAffinef* __fastcall Get(){return this;}
	void __fastcall Set(TSHAffinef* a){ setter(*a); }

	TSHVec3f Ex;
	TSHVec3f Ey;
	TSHVec3f Ez;
	TSHVec3f Pos;
};

//---------------------------------------------------------------------------
//オブジェクトインスペクタ表示用のTSHAffinefの仮の姿
class TSHAffinefDummy: public TPersistent
{
	TSHAffinef& taff;

	TSHVec3fDummy* FExDummy;
	TSHVec3fDummy* FEyDummy;
	TSHVec3fDummy* FEzDummy;
	TSHVec3fDummy* FPosDummy;
	TSHVec3f __fastcall GetEx(){ return taff->Ex; }
	TSHVec3f __fastcall GetEy(){ return taff->Ey; }
	TSHVec3f __fastcall GetEz(){ return taff->Ez; }
	TSHVec3f __fastcall GetPos(){ return taff->Pos; }
	void __fastcall SetEx(TSHVec3f v){ taff->Ex = v; }
	void __fastcall SetEy(TSHVec3f v){ taff->Ey = v; }
	void __fastcall SetEz(TSHVec3f v){ taff->Ez = v; }
	void __fastcall SetPos(TSHVec3f v){ taff->Pos = v; }
public:
	__fastcall TSHAffinefDummy(TSHAffinef& a):taff(a){
		FExDummy = new TSHVec3fDummy(taff.Ex);
		FEyDummy = new TSHVec3fDummy(taff.Ey);
		FEzDummy = new TSHVec3fDummy(taff.Ez);
		FPosDummy = new TSHVec3fDummy(taff.Pos);
	}
	virtual __fastcall ~TSHAffinefDummy(){
		delete FExDummy; delete FEyDummy;
		delete FEzDummy; delete FPosDummy;
	}
	virtual void __fastcall Assign(TPersistent* src);
__published:
	__property TSHVec3fDummy* ExDummy={read=FExDummy, write=FExDummy};
	__property TSHVec3fDummy* EyDummy={read=FEyDummy, write=FEyDummy};
	__property TSHVec3fDummy* EzDummy={read=FEzDummy, write=FEzDummy};
	__property TSHVec3fDummy* PosDummy={read=FPosDummy, write=FPosDummy};
};

//---------------------------------------------------------------------------

#endif
