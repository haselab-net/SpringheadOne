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
#include <WinBasis/WBSockStr.h>
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

	MyView myView;
	myView.Init("MySceneGraph");

	myView.Run();
	return 0;
}
