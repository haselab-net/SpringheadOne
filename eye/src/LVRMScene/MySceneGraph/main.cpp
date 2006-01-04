//	VRML scene graph builder
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "GLView.h"
#include "MyNode.h"

#include <crtdbg.h>
#include <conio.h>
using namespace Spr;
using namespace std;

int main(int argc, char *argv[]){
//	メモリリーク検査用
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);

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
		std::cout << "Building TMyNode ... ";
		TMyNodeBuilder builder;
		builder.Build(root);
		view.allNodes = builder.allNodes;
		view.clearColor = builder.clearColor;
		std::cout << "done." << std::endl;
		//	視点情報
		view.fov = fieldOfView;
		view.afCam.Pos() = Vec3f(viewpos);
		view.afCam.Rot() = Matrix3f::Rot(vieworient[3], Vec3f(vieworient));
		Affinef af = (Affinef&)viewMatrix;
		view.afCam = af * view.afCam;
	}
	view.Run();
	return 0;
}
