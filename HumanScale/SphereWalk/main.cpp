//	VRML scene graph builder
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "GLView.h"
#include "GeoNode.h"

#include <crtdbg.h>
#include <conio.h>
using namespace LVRM;
using namespace std;

int main(int argc, char *argv[]){
//	ƒƒ‚ƒŠƒŠ[ƒNŒŸ¸—p
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);
	TGLView view(argc > 1);
	view.Init("SphereWalk");	//	GL‚Ì‰Šú‰»‚ğæ‚É‚·‚é‚Ğ‚Â‚æ‚¤‚ ‚è
	view.Run();
	return 0;
}
