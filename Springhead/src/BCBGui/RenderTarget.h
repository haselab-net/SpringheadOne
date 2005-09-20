//---------------------------------------------------------------------------

#ifndef RenderTargetH
#define RenderTargetH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>

#include <ImpD3D/SGD3D.h>
//---------------------------------------------------------------------------
class PACKAGE TRenderTarget : public TWinControl
{
private:
protected:
	Spr::UTRef<Spr::SGRenderD3D> render;
	Spr::UTRef<Spr::SGWorld> world;
	Spr::Affinef afBody, afHead;
public:
	__fastcall TRenderTarget(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#endif
