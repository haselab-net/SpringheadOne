//---------------------------------------------------------------------------
#ifndef TSHFrameH
#define TSHFrameH
//---------------------------------------------------------------------------
#include "SHComponent.h"
#include "SHAffine.h"
#include <SceneGraph/SGFrame.h>
//---------------------------------------------------------------------------


class PACKAGE TSHFrame : public TSHComponent{
	DEFINE_OBJECT_IMP(Spr::SGFrame);
private:
	TSHAffinefDummy* FPostureDummy;
	SHAffinef __fastcall GetPosture(){
		if (Imp) return (SHAffinef&)Imp->GetPosture();
		return SHAffinef();
	}
	void __fastcall SetPosture(SHAffinef a){
		if (Imp) Imp->SetPosture(a);
	}
protected:
	friend class TSHSceneGraphDlg;
public:
	///	�R���X�g���N�^
	__fastcall TSHFrame(TComponent* Owner);
	///	�p���v���p�e�B
	TSHAffinef Posture;
__published:
	///	�p���̃v���p�e�B�̕\��
	__property TSHAffinefDummy* PostureDummy={read=FPostureDummy, write=FPostureDummy};
};
//---------------------------------------------------------------------------
#endif
