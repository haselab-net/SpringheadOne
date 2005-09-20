// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "MyNode.h"
#include "MyView.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <crtdbg.h>
#include <iostream>
#include <conio.h>
#include "communication.h"
using namespace LVRM;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
//	main

int main(int argc, char *argv[])
{
//	ÉÅÉÇÉäÉäÅ[ÉNåüç∏óp
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);
	fprintf(stderr, "wrl2view 1.3.6:       VRML2.0 File Viewer	Copyright (c)1999 Mamoru Shiroki\n");
	fprintf(stderr, "wrl2view++ 1.3.6+0.1: Copyright (c)2001 Modified by Shoichi Hasegawa\n");


	if (argc <= 1) {
		fprintf(stderr, "usage:	%s VRML2.0_File\n", argv[0]);
		return 1;
	}
	MyView myView;
	myView.Init("MySceneGraph");

	UTRef<TSts> root = LoadWrl(argv[1]);
	std::cout << "Loaded\n";
	InitList(root);
	std::cout << "Initialized\n";

	MyNodeBuilder builder;
	builder.Build(root);
	myView.dnTree = builder.Root();
	std::cout << "Builded\n";

	myView.bSender = argc > 2;
	myView.Run();
	return 0;
}
