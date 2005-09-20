//---------------------------------------------------------------------------

#include <basepch.h>

#pragma hdrstop

#include "SHRenderer.h"
#pragma resource "*.res"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck は、定義されたコンポーネントが純粋仮想関数を含む
// 抽象クラスではないことを確認するために定義されています。
//

static inline void ValidCtrCheck(TSHRenderer *)
{
	new TSHRenderer(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSHRenderer::TSHRenderer(TComponent* Owner)
	: TComponent(Owner)
{
}
//---------------------------------------------------------------------------
namespace Shrenderer
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TSHRenderer)};
		RegisterComponents("SpringHead", classes, 0);
	}
}
//---------------------------------------------------------------------------
 