#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include <basepch.h>
#include <DesignEditors.hpp>
#include <Dialogs.hpp>//��
#include "SHFrame.h"
#include "SHScene.h"
#include "SHCamera.h"
#include "SHBox.h"
#include "SHPTMPropEdit.h"
#include <SceneGraph/SGScene.h>

using namespace Spr;

#pragma package(smart_init)


//---------------------------------------------------------------------------
// ValidCtrCheck �́A��`���ꂽ�R���|�[�l���g���������z�֐����܂�
// ���ۃN���X�ł͂Ȃ����Ƃ��m�F���邽�߂ɒ�`����Ă��܂��B
//

static inline void ValidCtrCheck(TSHFrame *)
{
	new TSHFrame(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSHFrame::TSHFrame(TComponent* Owner)
	: TSHComponent(Owner), Posture(&GetPosture, &SetPosture)
{
	FPostureDummy = new TSHAffinefDummy(Posture);
	Posture.OnChange = &NotifyChange;
}

//---------------------------------------------------------------------------
namespace Shframe
{
char posture[] = "Posture";
	void __fastcall PACKAGE Register()
	{
			TComponentClass classes[1] = {__classid(TSHFrame)};
		RegisterComponents("SpringHead", classes, 0);
		//	PostureDummy�̃v���p�e�B�G�f�B�^��o�^
		RegisterPropertyEditor(__typeinfo(TSHAffinefDummy), __classid(TSHFrame),
		"PostureDummy", __classid(TAffineProperty<posture>));
	}
}
//---------------------------------------------------------------------------


