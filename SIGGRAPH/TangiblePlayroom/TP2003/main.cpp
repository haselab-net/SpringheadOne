
// ====================================================
// PenguinHockeyのmain()
// ====================================================

#include "game.h"
#include "SceneManager.h"
#include "gameView.h"
#include "DispNode.h"
#include "physics.h"
#include "environment.h"
#include <Render/RDGlutView.h>

using namespace std;

Environment env; //環境設定クラス
GameView view;

// =====================================================
//	main()
// =====================================================
int main(int argc, char *argv[])
{
	//環境設定ファイル読み込み
	if(argc >= 2){
		env.filename = argv[1];
	}else{
		cout << "起動オプションに環境設定ファイル名が指定されなかったので env.ini を使用します。" << endl;
		env.filename = "env.ini";
	}
	if(env.Load()){
		cout << "環境設定ファイル " << env.filename << " 読み込み完了" <<endl;
	}else{
		CleanupPhysics();
		exit(0);
	}

	//viewを渡す
	env.view = &view;


	//ネットワーク接続の初期化
	if(!view.InitNetwork()){
		cout << "ネットワーク接続の初期化に失敗しました" <<endl;
		return -1;
	}

	if(env.bUseGraphics){ //現状このパラメータは常にTRUEを期待。

		//debugYI ここに
		// GameView view
		// があってもよくない？

		//シーングラフの初期化
		UTRef<TSts> root = LoadWrl("media/world.wrl"); // wrlファイルの読み込み
		InitList(root);
		DispNodeBuilder builder;
		builder.Build(root);
		
		view.game.dnTree = builder.Root();
		view.game.dnTree->Dump(cout);
		view.game.Generation(); // キャラクタの初期化

		if(!env.bClient){
			//debugYI 削除可
			//InitPhysics(false);
			InitHaptics();
		}
		view.sceneManager.SetGameView(&view);
		view.sceneManager.SetScene(0); // 最初のシーンに初期化
		view.Run();
	}	
/*
	else{
		//debugYI 削除可
		//InitPhysics(true); //現状はテストモードのグラフィックスが走る
		
		InitHaptics();

		// テストメードのグラフィックス
		TestView test_view;
		test_view.WindowPosition(10,10, 300, 200);
		test_view.Init("OpenGL Window");
		test_view.Run();
	}
*/
  //env.bUseGraphics
	
	return 0;
}