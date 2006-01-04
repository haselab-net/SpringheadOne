//---------------------------------------------------------------------------
#ifndef SHSolidH
#define SHSolidH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Physics/PHSolid.h>
#include "SHScene.h"
#include "SHVector.h"
#include "SHMatrix.h"

//---------------------------------------------------------------------------
//コンポーネント化された剛体
class PACKAGE TSHSolid : public TSHComponent{
	DEFINE_OBJECT_IMP(Spr::PHSolid);

private:
	TSHMatrix3fDummy* FInertiaDummy;
	TSHVec3fDummy* FCenterDummy;
	TSHVec3fDummy* FVelocityDummy;
	TSHVec3fDummy* FAngularVelocityDummy;

	SHMatrix3f __fastcall GetInertia(){
		if (Imp) return (SHMatrix3f&)((Spr::Matrix3f)(Imp->GetInertia()));
		return SHMatrix3f();
	}
	void __fastcall SetInertia(SHMatrix3f i){
		if (Imp) Imp->SetInertia(i);
	}
	SHVec3f __fastcall GetCenter(){
		if (Imp) return (SHVec3f&)(Imp->GetCenter());
		return SHVec3f();
	}
	void __fastcall SetCenter(SHVec3f c){
		if (Imp) Imp->SetCenter(c);
	}
	SHVec3f __fastcall GetVelocity(){
		if (Imp) return (SHVec3f&)(Imp->GetVelocity());
		return SHVec3f();
	}
	void __fastcall SetVelocity(SHVec3f v){
		if (Imp) Imp->SetVelocity(v);
	}
	SHVec3f __fastcall GetAngularVelocity(){
		if (Imp) return (SHVec3f&)(Imp->GetAngularVelocity());
		return SHVec3f();
	}
	void __fastcall SetAngularVelocity(SHVec3f av){
		if (Imp) Imp->SetAngularVelocity(av);
	}

protected:
	friend class TSHSceneGraphDlg;

    void __fastcall SetMass(double m);
    double __fastcall GetMass();
    Spr::SGFrame* __fastcall GetFrame();
	void __fastcall SetFrame(Spr::SGFrame* f);
	AnsiString __fastcall GetFrameByName();
	void __fastcall SetFrameByName(AnsiString s);
	virtual void __fastcall Loaded();

public:
    // コンストラクタ
	__fastcall TSHSolid(TComponent* Owner);

	// 対象フレーム
	__property Spr::SGFrame* Frame={read=GetFrame, write=SetFrame};
    /// 慣性テンソルプロパティ
    TSHMatrix3f Inertia;
    /// 重心プロパティ
    TSHVec3f Center;
	///	速度プロパティ
	TSHVec3f Velocity;
	///	角速度プロパティ
	TSHVec3f AngularVelocity;

__published:
	// 質量
    __property double Mass  = { read=GetMass, write=SetMass, stored=false };
    // 慣性テンソル
    __property TSHMatrix3fDummy* InertiaDummy  = { read=FInertiaDummy, write=FInertiaDummy, stored=false };
	// 重心(ローカル座標系の原点からの相対座標)
    __property TSHVec3fDummy* CenterDummy  = { read=FCenterDummy, write=FCenterDummy, stored=false };
	// 速度(World座標系)
    __property TSHVec3fDummy* VelocityDummy = { read=FVelocityDummy, write=FVelocityDummy, stored=false };
	// 角速度(World座標系)
    __property TSHVec3fDummy* AngularVelocityDummy = { read=FAngularVelocityDummy, write=FAngularVelocityDummy, stored=false };

	// 対象フレーム
	__property AnsiString FrameDummy
		= {read=GetFrameByName, write=SetFrameByName, stored=false};


};
//---------------------------------------------------------------------------
#endif
