// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "game.h"
#include "gameView.h"
#include "DispNode.h"
#include "physics.h"
#include <Render/RDGlutView.h>

using namespace std;

static GLfloat	spinY = 0;
bool		bUseSound; //サウンドを使用する

//static GLfloat	spinY = 180;/* カメラの初期角度（pan）*/

////////////////////////////////////////////////////////////////////////////////
//	Game Management functions


////////////////////////////////////////////////////////////////////////////////
//	main

int main(int argc, char *argv[])
{
	bool bClient = false;
	bUseSound = false;
	char* serverName = NULL;
	if (argc >= 2){
		if ( argc == 2 ) {
			bUseSound = true;
			printf("Server Mode / UseSound\n");
		}
		if ( argc == 3 ) {
			serverName = argv[1];
			bClient = true;
			bUseSound = true;
			printf("Client Mode (Server:%s) / UseSound\n", serverName);
		}
	} else {
			bUseSound = true;
			printf("Server Mode / UseSound\n");
	}
#ifdef USE_GRAPHICS
	GameView view;
	view.WindowPosition(10,10,300,200);
	if (bClient){
		view.game.mode = Game::CLIENT;
		view.game.csock.server = serverName;
		for(int i=0; i<Game::NCLIENT; ++i){
			view.game.csock.port = Game::PORTBASE + i;
			view.game.csock.Connect();
		}
		if (!view.game.csock.sock.IsConnected()) return -1;
		view.Init("Display Client");
	}else{
		view.game.mode = Game::SERVER;
		view.Init("Game Server");
	}
	

	UTRef<TSts> root = LoadWrl("media/world.wrl");
	InitList(root);

	DispNodeBuilder builder;
	builder.Build(root);
	view.game.dnTree = builder.Root();
	view.game.dnTree->Dump(cout);
	view.game.Generation();

	if (!bClient) InitPhysics(false);
#else
	InitPhysics(true);
#endif

#ifdef USE_GRAPHICS
	view.Run();
#endif
	
	return 0;
}