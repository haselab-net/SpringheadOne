//---------------------------------------------------------------------------

#include <basepch.h>

#pragma hdrstop

#include "SHRenderer.h"
#pragma resource "*.res"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck �́A��`���ꂽ�R���|�[�l���g���������z�֐����܂�
// ���ۃN���X�ł͂Ȃ����Ƃ��m�F���邽�߂ɒ�`����Ă��܂��B
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
 