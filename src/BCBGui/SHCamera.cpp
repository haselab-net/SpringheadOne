#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include <basepch.h>


#include "SHCamera.h"
#include "SHScene.h"
#include "SHBox.h"
#include <SceneGraph/SGScene.h>

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck �́A��`���ꂽ�R���|�[�l���g���������z�֐����܂�
// ���ۃN���X�ł͂Ȃ����Ƃ��m�F���邽�߂ɒ�`����Ă��܂��B
//
using namespace Spr;
static inline void ValidCtrCheck(TSHCamera *)
{
    new TSHCamera(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSHCamera::TSHCamera(TComponent* Owner)
	: TSHComponent(Owner)
{
}
//---------------------------------------------------------------------------
namespace Shcamera
{
    void __fastcall PACKAGE Register()
    {
         TComponentClass classes[1] = {__classid(TSHCamera)};
		 RegisterComponents("SpringHead", classes, 0);
    }
}
//---------------------------------------------------------------------------


