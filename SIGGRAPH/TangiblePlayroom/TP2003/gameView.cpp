#include "SceneManager.h"
#include "GameView.h"
#include "Physics.h"
#include <fstream>
#include "environment.h"


extern Vec3f	fakeSpidarPos; // マウス入力(デバッグ用)

Game *theGame;	//グローバルゲームポインタ

//debugYI これはGameViewのメンバ変数でよいのでは
//static GLfloat	spinY = 0; // カメラの初期角度(pan) 


GameView::GameView(){
	scale = 1;
	spinX = 0;
//	mouseX = 0;
//	mouseY = 0;
//	mouseButton = -1;
	game.view = this;
	theGame = &game;
	delta = 1.0f;
//	bFullscreen = false;
}

void GameView::LoadCamera(void) {
	ifstream file("viewpoint.txt");
	file >> game.afCam;
	file >> game.screenPos;
	file >> game.screenSize;
	file >> game.afClipL;
	file >> game.afClipR;
	OnSize(game.screenSize.X(), game.screenSize.Y());
}


void GameView::Init(char* title){

	WindowPosition(224, 0, 800, 600);
	RDGlutView::Init(title);


	//debugYI Viewでサウンド関連の初期化するべきか？

	// サウンド関連の初期化
	if ( env.bUseSound ) {
		gssystem = new GameSndSystem;
		gssystem->Init(title);
	}

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);	//もしくはGLUT_CURSOR_NONE 

	
	float	lmamb[] = {.7, .7, .7, 1},
		l0amb[] = {0, 0, 0, 1},
		l0dif[] = {1, 1, 1, 1},
		l0spc[] = {1, 1, 1, 1},
		l0pos[] = {0, 0, 10, 0};
			
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

#ifdef PH_GRAPHIC_LOW_DETAIL
	glShadeModel(GL_FLAT);
#else
	glShadeModel(GL_SMOOTH);
#endif
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmamb);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
			
	glutCreateMenu(MenuFuncEntry);
	glutAddMenuEntry("Zoom In 	[PgUp]", GLUT_KEY_PAGE_UP);
	glutAddMenuEntry("Zoom Out	[PgDn]", GLUT_KEY_PAGE_DOWN);
	glutAddMenuEntry("Initialize ViewPoint	[Home]", GLUT_KEY_HOME);
	glutAddMenuEntry("------------------", 9999);
	glutAddMenuEntry("Full Screen	[F9]", GLUT_KEY_F9);
	glutAddMenuEntry("Screen Snapshot	[F12]", GLUT_KEY_F12);
	glutAddMenuEntry("------------------", 9999);
	glutAddMenuEntry("Quit  	[Esc]", GLUT_KEY_F4);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

// =====================================================
// ネットワーク接続の初期化
//debguYI ネットワークの初期化だけじゃなくて、Init()も呼び出しているので注意
// =====================================================
bool GameView::InitNetwork(){
	
	//ネットワーク接続
	if (env.bClient){
		game.mode = Game::CLIENT;
		game.csock.server = env.nameServer;
		cout << "クライアントモードで " << env.nameServer << " に接続中...";
		for(int i=0; i<Game::NCLIENT; ++i){
			game.csock.port = Game::PORTBASE + i;
			game.csock.Connect();
		}
		if (!game.csock.sock.IsConnected()) {
			cout << "失敗しました！"<< endl <<"プログラムを終了します。" << endl;
			return false;
		} else {
			cout << "接続成功"<< endl;
			Init("Display Client");  // Initの引数はウィンドウタイトル
			return true;
		}
	}else{
		cout << "サーバーモードで起動中...";
		game.mode = Game::SERVER;
		Init("Game Server"); // Initの引数はウィンドウタイトル
		return true;
	}
}


void GameView::OnOpen(){
	memories.graphicRead = memories.graphicShouldRead ? 0 : 1;
}

void GameView::OnIdle(){
	if (!this) return;
	//	game.pen1.afi = penguins.penguinGroup[0].penguinRoot->GetPosture();
	//	cout << game.FPS++ << endl;


	game.Step();
	if ( env.bUseSound ) {// サウンドの処理は必要ならクライアントでも実行する 2003/1/31 AS
		gssystem->CheckMakeSound();	
	}
//	cout << timeGetTime() << endl;
}

void GameView::OnDraw(){

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//	game.Draw();
	env.view->sceneManager.Loop(); //シーンマネージャによる更新と描画

	Flip();

#ifdef _DEBUG
	Sleep(10);
#endif
}
void GameView::RotateView(){
		glScalef(scale, scale, scale);
		glRotatef(spinX, 1, 0, 0);
		glRotatef(spinY, 0, 1, 0);
	}
void GameView::OnSize(int width, int height){
	game.screenSize.X() = width;
	game.screenSize.Y() = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(180/M_PI * fieldOfView, (GLfloat)width / height, 0.01, 100);
	///	スクリーンの位置,	スクリーンのサイズ,	フロントクリッピング, バッククリッピング
	Affinef afProj(game.screenPos, game.screenSize, 0.1f, 100.0f);
	glLoadMatrixf(afProj);
	glMatrixMode(GL_MODELVIEW);
}
void GameView::OnMouse(Button button, bool state, int x, int y){
	cout << "bt:" << button << " st:" << state << "x:" << x << "y:" << y << endl;
	int i;
	for (i=1; i<MOUSE_SAMPLE; i++) {
		env.mousez[i] = env.mousez[i-1];
	}
	switch(button){
	case 1://GLUT_LEFT_BUTTON:
		env.mousez[0] -= 1;
		if (env.mousez[0]<-20) env.mousez[0] = -20;
		break;
	case 3://GLUT_RIGHT_BUTTON:
		env.mousez[0] += 1;
		if (env.mousez[0]>+20) env.mousez[0] = 20;
		break;
	}
	cout << "mouseZ ="<<env.mousez[0] << endl;
}

void GameView::OnMouseMove(bool on, int x, int y){
	if (env.iSpidar_type == Env_NO_SPIDAR) {
		int i;
		for (i=1; i<MOUSE_SAMPLE; i++) {
			env.mousex[i] = env.mousex[i-1];
			env.mousey[i] = env.mousey[i-1];
		}
		env.mousex[0] = x;
		env.mousey[0] = y;
	}
//	mouseX = x;	mouseY = y;
}

void GameView::OnKeyboard(int code, int x, int y){

		// どのシーンでも同じキーバインドのものは↓に書く
		switch(code){
		case 0x1b:				//	終了
			CleanupPhysics();
			Exit();
			break;

		case 'f':				//	フルスクリーンモード切り替え
			if (env.bFullscreen) {
				glutReshapeWindow(400,300);
				env.bFullscreen = false;
			} else {
				glutFullScreen();
				env.bFullscreen = true;
			}
			break;

		case 'h':				//	力覚モード切り替え
			if (env.bHaptics) {
				env.bHaptics = false;
			} else {
				env.bHaptics = true;
			}
			break;

		case 't':				//	床面張り付きモードOFF
				env.bTallNone = false;
			break;
		case 'T':				//	床面張り付きモードON
				env.bTallNone = true;
			break;

			
		case 'c':
			CalibSpidar();
			break;


		case 'L':
			LoadCamera();
			break;

		case 'l':
			arrowMode = MOVE_CLIPL;		//	クリッピングオブジェクト
			break;
		case 'm':
			arrowMode = MOVE_CAM;		//	カメラの平行移動
			break;
		case 'P':				//	正面モード
			game.Front();
			OnSize(game.screenSize.X(), game.screenSize.Y());
			break;
		case 'd':				//	デバッグ用ノードのダンプ
			game.dnTree->Dump(std::cout);
			break;
		case 'r':
			arrowMode = MOVE_CLIPR;		//	クリッピングオブジェクト
			break;
		case 'S':{
			ofstream file("viewpoint.txt");
			file << game.afCam;
			file << game.screenPos;
			file << game.screenSize;
			file << game.afClipL;
			file << game.afClipR;
				 }
			break;
		case 's':
			arrowMode = MOVE_SCREEN;	//	スクリーンの移動
			break;
		case 'z':
			arrowMode = ZOOMROTATE_CAM;	//	カメラを軸周りに回転させる．
			break;

		case '.':{
			SYSTEMTIME  STime;
			int fileNameTime;
			char dataFileName[255];
			fileNameTime = timeGetTime();
			GetLocalTime(&STime);
			sprintf(dataFileName,"%04d%02d%02d_%010d.txt",STime.wYear,STime.wMonth,STime.wDay,fileNameTime);
			ofstream dataFile(dataFileName);
			dataFile << "Now Setting Data..." << timeGetTime() <<std::endl;
			dataFile.close();

				 }
			break;
		
	    // 微調整用パラメータの設定		
		case '1': delta = 0.001f; break;
		case '2': delta = 0.003f; break;
		case '3': delta = 0.01f;  break;
		case '4': delta = 0.03f;  break;
		case '5': delta = 0.1f;   break;
		case '6': delta = 0.3f;   break;
		case '7': delta = 1.0f;   break;
		case '8': delta = 3.0f;   break;
		case '9': delta = 10.0f;  break;

		default: 
			MenuFunc(-code);
			break;
		}
		
		// SceneManagerにキー入力を渡す
		sceneManager.KeyPressed(code);

}


void GameView::OnSpecialKey(int code, int x, int y){
	
	int	mdf = glutGetModifiers();
	if(mdf & GLUT_ACTIVE_CTRL){
		switch (code){
		case GLUT_KEY_UP:
			MenuFunc(GLUT_KEY_PAGE_UP);
			break;
		case GLUT_KEY_DOWN:
			MenuFunc(GLUT_KEY_PAGE_DOWN);
			break;
		}
	}else{
		switch (code){
		case GLUT_KEY_UP:
			arrowKey.Y() += delta;
			OnArrowKey();
			break;
		case GLUT_KEY_DOWN:
			arrowKey.Y() -= delta;
			OnArrowKey();
			break;
		case GLUT_KEY_LEFT:
			arrowKey.X() -= delta;
			OnArrowKey();
			break;
		case GLUT_KEY_RIGHT:
			arrowKey.X() += delta;
			OnArrowKey();
			break;
		default:
			MenuFunc(code);
			break;
		}
	}
	
	// SceneManagerにキー入力を渡す
	//debugYI
	//sceneManager->KeyPressed(code);

}
void GameView::MenuFuncEntry(int key){
	if (TheRender()) ((GameView*)TheRender())->MenuFunc(key);
}
void GameView::MenuFunc(int key){
	switch (key) {
	case GLUT_KEY_UP:
		spinX -= 5;
		break;
	case GLUT_KEY_DOWN:
		spinX += 5;
		break;
	case GLUT_KEY_LEFT:
		spinY -= 5;
		break;
	case GLUT_KEY_RIGHT:
		spinY += 5;
		break;
	case GLUT_KEY_PAGE_UP:
		scale *= 1.05;
		break;
	case GLUT_KEY_PAGE_DOWN:
		scale /= 1.05;
		break;
	case GLUT_KEY_HOME:
		spinX = spinY = 0;
		scale = 1;
		break;
	case GLUT_KEY_F9:
		glutFullScreen();
		break;
	case GLUT_KEY_F4:
	case 0x1b:
		CleanupPhysics();
		exit(0);
		break;
	default:
		return;
	}
	glutPostRedisplay();
}
void GameView::OnArrowKey(){
	switch(arrowMode){
	case MOVE_CAM:
		game.afCam.Pos().X() += arrowKey.X() / 30.0f;
		game.afCam.Pos().Z() += arrowKey.Y() / 30.0f;
		break;
	case ZOOMROTATE_CAM:
		game.afCam = game.afCam * Affinef(Rad(arrowKey.X()), 'z');
//		game.afCam.Pos().Y() += arrowKey.Y() / 30.0f;
		game.screenPos.Z() += arrowKey.Y() * 10.0f;
		OnSize(game.screenSize.X(), game.screenSize.Y());
		break;
	case MOVE_CLIPL:
		game.afClipL.Pos().X() += arrowKey.X() / 30.0f;
		game.afClipL = game.afClipL * Affinef(Rad(arrowKey.Y()), 'z');
		break;
	case MOVE_CLIPR:
		game.afClipR.Pos().X() += arrowKey.X() / 30.0f;
		game.afClipR = game.afClipR * Affinef(Rad(arrowKey.Y()), 'z');
		break;
	case MOVE_SCREEN:
		game.screenPos.X() += arrowKey.X()*100;
		game.screenPos.Y() += arrowKey.Y()*100;
		OnSize(game.screenSize.X(), game.screenSize.Y());
		break;
	}
	arrowKey = Vec2f();
}
