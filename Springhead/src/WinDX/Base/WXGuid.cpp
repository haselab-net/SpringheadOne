#pragma hdrstop

#define INITGUID
#include "WXComBase.h"

#if SPR_DXVER==8
# include <d3d8.h>
# include <d3dx8.h>
#elif SPR_DXVER==9
# include <d3d9.h>
# include <d3dx9.h>
#endif

#include <dinput.h>

