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
//�R���|�[�l���g�����ꂽ����
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
    // �R���X�g���N�^
	__fastcall TSHSolid(TComponent* Owner);

	// �Ώۃt���[��
	__property Spr::SGFrame* Frame={read=GetFrame, write=SetFrame};
    /// �����e���\���v���p�e�B
    TSHMatrix3f Inertia;
    /// �d�S�v���p�e�B
    TSHVec3f Center;
	///	���x�v���p�e�B
	TSHVec3f Velocity;
	///	�p���x�v���p�e�B
	TSHVec3f AngularVelocity;

__published:
	// ����
    __property double Mass  = { read=GetMass, write=SetMass, stored=false };
    // �����e���\��
    __property TSHMatrix3fDummy* InertiaDummy  = { read=FInertiaDummy, write=FInertiaDummy, stored=false };
	// �d�S(���[�J�����W�n�̌��_����̑��΍��W)
    __property TSHVec3fDummy* CenterDummy  = { read=FCenterDummy, write=FCenterDummy, stored=false };
	// ���x(World���W�n)
    __property TSHVec3fDummy* VelocityDummy = { read=FVelocityDummy, write=FVelocityDummy, stored=false };
	// �p���x(World���W�n)
    __property TSHVec3fDummy* AngularVelocityDummy = { read=FAngularVelocityDummy, write=FAngularVelocityDummy, stored=false };

	// �Ώۃt���[��
	__property AnsiString FrameDummy
		= {read=GetFrameByName, write=SetFrameByName, stored=false};


};
//---------------------------------------------------------------------------
#endif
