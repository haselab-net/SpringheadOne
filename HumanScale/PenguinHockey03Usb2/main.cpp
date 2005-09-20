// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "game.h"
#include "gameView.h"
#include "DispNode.h"
#include "physics.h"
#include "environment.h"
#include <Render/RDGlutView.h>

using namespace std;

static GLfloat	spinY = 0;

//static GLfloat	spinY = 180;/* カメラの初期角度（pan）*/

Environment env; //環境設定クラス
GameView view;


////////////////////////////////////////////////////////////////////////////////
//	Game Management functions


////////////////////////////////////////////////////////////////////////////////

//ネットワーク接続の初期化
bool InitNetworks()	{
	//ネットワーク接続
	if (env.bClient){
		view.game.mode = Game::CLIENT;
		view.game.csock.server = env.nameServer;
		cout << "クライアントモードで " << env.nameServer << " に接続中...";
		for(int i=0; i<Game::NCLIENT; ++i){
			view.game.csock.port = Game::PORTBASE + i;
			view.game.csock.Connect();
		}
		if (!view.game.csock.sock.IsConnected()) {
			cout << "失敗しました！"<< endl <<"プログラムを終了します。" << endl;
			return false;
		} else {
			cout << "接続成功"<< endl;
			view.Init("Display Client");
			return true;
		}
	}else{
		cout << "サーバーモードで起動中...";
		view.game.mode = Game::SERVER;
		view.Init("Game Server");
		return true;
	}
}


//	main

int main(int argc, char *argv[])
{
	//環境設定ファイル読み込み
	if (argc >= 2){
		env.filename = argv[1];
	} else {
		cout << "起動オプションに環境設定ファイル名が指定されなかったので env.txt を使用します。" << endl;
		env.filename = "env.txt";
	}
	if (env.Load()) {
		cout << "環境設定ファイル " << env.filename << " 読み込み完了" <<endl;
	} else {
		exit(0);	
	}

	//ネットワーク接続の初期化
	if (!InitNetworks()) {
		return -1;
	}

	if (env.bUseGraphics) { //現状このパラメータは常にTRUEを期待。
//		GameView view;//グローバル化
		view.WindowPosition(10,10,300,200);
		//過去ではここにネットワーク接続の初期化が存在。
		//シーンの初期化
		UTRef<TSts> root = LoadWrl("media/world.wrl");
		InitList(root);
		DispNodeBuilder builder;
		builder.Build(root);
		view.game.dnTree = builder.Root();
		view.game.dnTree->Dump(cout);
		view.game.Generation(); //初期位置

		if (!env.bClient) {
//			InitPhysics(false);
			InitHaptics();
		}

		view.Run();

//#else
	} else {
		InitPhysics(true); //現状はテストモードのグラフィックスが走る
	}	//env.bUseGraphics
//#endif
	
	return 0;
}