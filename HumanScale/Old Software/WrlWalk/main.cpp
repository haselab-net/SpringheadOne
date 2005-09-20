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
//	メモリリーク検査用
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);
	fprintf(stderr, "vrml2loader 1.3.6+0.1: Copyright (c)2001 Shoichi Hasegawa\n");

	if (argc <= 1) {
		fprintf(stderr, "usage:	%s VRML2.0_File\n", argv[0]);
		return 1;
	}

	TGLView view;
	view.Init("WrlWalk");	//	GLの初期化を先にするひつようあり
	{
		std::cout << "Loading " << argv[1] << "...";
		UTRef<TSts> root = LoadWrl(argv[1]);
		std::cout << "done." << std::endl;
		std::cout << "Initializing loaded tree ...";
		InitList(root);
		std::cout << "done." << std::endl;
		std::cout << "Building TGeoNode ... ";
		TGeoNodeBuilder builder;
		builder.Build(root);
		view.allNodes = builder.allNodes;
		view.clearColor = builder.clearColor;
		std::cout << "done." << std::endl;
	}
	view.InitCollisionDetector();
	view.Run();
	return 0;
}
