//---------------------------------------------------------------------------

#ifndef SHCameraH
#define SHCameraH
//---------------------------------------------------------------------------
#include "SHComponent.h"
#include <Graphics/GRCamera.h>

//---------------------------------------------------------------------------
class PACKAGE TSHCamera : public TSHComponent{
	DEFINE_OBJECT_IMP(Spr::GRCamera);
public:
	__fastcall TSHCamera(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
