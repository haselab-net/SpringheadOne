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

enum modeSelect{
	hReceive,
		bReceive,
		hSend,
		bSend
}nowMode;

int main(int argc, char *argv[]){
//	ÉÅÉÇÉäÉäÅ[ÉNåüç∏óp
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);
	if (argc <= 1) {
		fprintf(stderr, "usage:	%s VRML2.0_File\n", argv[0]);
		return 1;
	}
//	TGLView view(false);
//	if (argc > 2){
//		TGLView vi(true);
//		if (*argv[argc - 1] == 'b'){
//			TGLView vi(false);
//			vi.birdsView = true;
//		}
//		view = vi;
//	}
	nowMode = hReceive;
	if (argc > 2){
		nowMode = bSend;
		if (*argv[argc - 1] == '0'){
			nowMode = hReceive;
		}
		if (*argv[argc - 1] == '1'){
			nowMode = bReceive;
		}
		if (*argv[argc - 1] == '2'){
			nowMode = hSend;
		}
		if (*argv[argc - 1] == '3'){
			nowMode = bSend;
		}
	}
	TGLView view(nowMode);
	view.Init("BoxGarden");	//	GLÇÃèâä˙âªÇêÊÇ…Ç∑ÇÈÇ–Ç¬ÇÊÇ§Ç†ÇË
	{
		std::cout << "Loading " << argv[1] << "...";
		UTRef<TSts> root = LoadWrl(argv[1]);
		std::cout << "done." << std::endl;
		std::cout << "Initializing loaded tree ...";
		InitList(root);
		std::cout << "done." << std::endl;
		std::cout << "Building TGeoNode ... ";
		TGeoNodeBuilder builder;
		if (argc > 3) builder.DontDisplay();
		builder.Build(root);
		view.allNodes = builder.allNodes;
		view.clearColor = builder.clearColor;
		std::cout << "done." << std::endl;
	}
	view.InitCollisionDetector();
	view.Run();
	return 0;
}
