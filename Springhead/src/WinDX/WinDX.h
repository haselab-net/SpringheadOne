/**	@page WinDX WinDXƒ‰ƒCƒuƒ‰ƒŠ



*/
#include "D3D/WXD3D.h"
#if SPR_DXVER == 9
# include <d3d9.h>
# include <d3dx9.h>
# include <d3dx9mesh.h>
#elif SPR_DXVER == 8
# include <d3d8.h>
# include <d3dx8.h>
# include <d3dx8mesh.h>
#else
# error
#endif
