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
	///	コンストラクタ
	__fastcall TSHFrame(TComponent* Owner);
	///	姿勢プロパティ
	TSHAffinef Posture;
__published:
	///	姿勢のプロパティの表示
	__property TSHAffinefDummy* PostureDummy={read=FPostureDummy, write=FPostureDummy};
};
//---------------------------------------------------------------------------
#endif
