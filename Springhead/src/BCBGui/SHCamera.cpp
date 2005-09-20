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
// ValidCtrCheck は、定義されたコンポーネントが純粋仮想関数を含む
// 抽象クラスではないことを確認するために定義されています。
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


