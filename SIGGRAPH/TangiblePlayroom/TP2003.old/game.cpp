#include "environment.h"
#include "Game.h"
#include "settings.h"
#include "SceneManager.h" //debug
#include "GameView.h"
#include <strstream>
#include <direct.h>				// for _chdir()


#define xAI_DEBUG //AIデバッグ中。コンソールに表示します。

//extern int SndPuckGoal;

extern int SndPuckPuck;
extern int SndPenPuck;
extern int SndPuckGoal;
extern int SndRacPen;
extern int SndRacPuck;
extern int SndRacOppPen1;
extern int SndRacOppPen2;

// コンストラクタ
Game::Game(){
	bFront = false;
	initState = 2;  //debugYI ?
	friendPoint = 0;
	enemyPoint = 0;
	matchPoint = 10;

	for(int i=0; i<NCLIENT; ++i) ssock[i].port = PORTBASE+i;
	csock.port = PORTBASE;
	csock.server = "merlin";

	// カメラの位置	床から3M
	/*
	// h				原点からは，3+0.3-0.75
	float camH = 3.0f;
	afCam = Affinef(Rad(-90), 'x', Vec3f(0, camH+0.3f-0.75f, -0.75f));
	afCam = afCam * Affinef(Rad(-90), 'z');
	*/
//	float camH = 3.0f;
	float camH = 3.5f;
	afCam = Affinef(Rad(-90), 'x', Vec3f(0, camH, 0.0f));
	afCam = afCam * Affinef(Rad(-90), 'z');

	// スクリーンの位置
	float scale = 768/1.5f;
	screenPos = Vec3f(0, 0, scale * camH);

	// クリッピングオブジェクトの位置
	afClipR.Pos().X() = 0.5f;
	afClipL.Pos().X() = -0.5f;

	// データファイルの準備
	SYSTEMTIME  STime;
	int fileNameTime;
	fileNameTime = timeGetTime();
	GetLocalTime(&STime);
	sprintf(dataFileName,"%04d%02d%02d_%02d%02d%02d_%010d.txt",
		STime.wYear,STime.wMonth,STime.wDay,
		STime.wHour,STime.wMinute,STime.wSecond,
		fileNameTime);
	chdir("Data");
	dataFile.open(dataFileName);
	cout << "Dataディレクトリに " << dataFileName << " の名称で軌跡を保存します。" <<endl;
	chdir("../");
	dataFile << "milisec,RacketX,RacketY,RacketZ,forceX,forceX,forceX,ForceRatio,FriendPoint,EnemyPoint,haptics,scene" << endl;

}

// 描画
void Game::Draw(){

	Vec3f sLoc; //Sprite用標準表示位置
	Vec3f target;  //目標位置

	sLoc = Vec3f(0.0f, 0.0f, -2.0f);
	// 基本となるカメラとシーングラフを描画
	glPushMatrix();
		Affinef af = afCam.inv();
		glLoadMatrixf(af);
		dnTree->Draw();

	//シーンごとに異なる描画
	switch ( view->sceneManager.GetCurScene() ) {
		case 0://タイトル画面表示
			DrawSprite(0,sLoc); //title.jpg
			break;
		case 1://言語選択画面表示
			DrawSprite(1,sLoc); //language.jpg
			break;
		case 2://立ち位置＆グリップ握り画面表示
			DrawSprite(2,sLoc); //graspx.jpg
			if (env.iStaticFrame>=300) {
				DrawSprite(3,Vec3f(0.0f,0.0f,-1.5f)); //gripJ.jpg
			}
			if (env.iGraspFrame>=10) {
				DrawSprite(4,Vec3f(0.0f,0.0f,-1.5f)); //grip2x.jpg
			}

			if (env.iGraspFrame>=300) {
				env.view->sceneManager.SetScene(3);
			}
			cout << "iGraspFrame " << env.iGraspFrame <<" iStaticFrame " << env.iStaticFrame << endl;
			break;
		case 3://移動,あかいまる
			sLoc.Z() = -2.5f;
			DrawSprite(5,sLoc); //trace1J.jpg
//			cout << user << endl;
//			cout << env.vSpidar << endl;
//			DrawObject2d(0, env.vSpidar.X(),env.vSpidar.Y(),env.vSpidar.Z());
			DrawObject2d(0, user.X(),user.Y(),user.Z());

			target = Vec3f( 0.22, 0.27 , 0.0f);
			cout << "Diff" << (target-user).norm() << " location " << user.X() <<" , " << user.Y() << endl;
			if ((target-user).norm()<0.01) {
				env.view->sceneManager.SetScene(4);
			}
			break;

		case 4://移動,あおいまる
			sLoc.Z() = -2.5f;
			DrawSprite(6,sLoc); //trace2J.jpg
			DrawObject2d(0, user.X(),user.Y(),user.Z());

			target = Vec3f( -0.22, 0.27 , 0.0f);
			cout << "Diff" << (target-user).norm() << " location " << user.X() <<" , " << user.Y() << endl;
			if ((target-user).norm()<0.01) {
				env.view->sceneManager.SetScene(5);
			}
			break;

		case 5:
			break;

		case 9: //映像用キャリブレーション画面
			Grid();
			DrawObject2d(0, user.X(),user.Y(),user.Z());
			break;

		default:
			break;
	
	}


	glPopMatrix();

	
	// 得点を描画
	if (bFront){
		float dist = 5.7f;
		float h = 4.0f;
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
			glLoadMatrixf(Affinef(Rad(0),'z', Vec3f(-dist, h, -16)));
			DrawNumber(friendPoint);
		glPopMatrix();
		glPushMatrix();
			glLoadMatrixf(Affinef(Rad(0),'z' ,Vec3f(dist, h, -16)));
			DrawNumber(enemyPoint);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}




}

// 数字を描画するメソッド
void Game::DrawNumber(int i){
	if (0 <= i && i < numbers.size()){
		numbers[i]->Draw();
	}else{
		glutSolidCube(1);
	}
}


// スプライトを描画するメソッド
void Game::DrawSprite(int i,Vec3f pos){
	switch ( env.view->sceneManager.GetCurScene() ) {
	case 8:
		break;
	default:
	//	if (0 <= i && i < sprites.size()){
		if (0 <= i){
			glPushMatrix();
			glLoadMatrixf(Affinef(Rad(0),'z', pos));
			switch (i) {
				case 2:
					sprite2[env.iLanguage]->Draw();
					break;
				case 4:
					sprite4[env.iLanguage]->Draw();
					break;
				default:
					sprites[i]->Draw();
					break;
			}
//			cout << "DrawSprite = " << i << ", Lang =" << env.iLanguage << " " << sprites[i]->Name() << endl;
			glPopMatrix();
		}
		break;
	}
}

// ポリゴンで作られたオブジェクトを描画するメソッド by AS
void Game::DrawObject2d(int i,GLfloat x , GLfloat y , GLfloat z){
	glPushMatrix();
		glTranslatef(x , y, z);
		experiments[i]->Draw();
	glPopMatrix();
}


// 正面からの視点に変更
void Game::Front(){
	float z = -0.75f;
	afCam = Affinef(5.0f*0.5f, 3.0f*0.5f, z);
	afCam.LookAtGL(Vec3f(0,0,z), Vec3f(0,10,z));
	screenPos = Vec3f(0, 0, 1024);
	bFront = true;
}

// Gameクラスの初期化
void Game::Generation(){
	Affinef initPenPos[penNum];
	Affinef initPuckPos[puckNum];

	Vec3f ex = Vec3f(1,0,0);			//グローバル座標系の単位ベクトル
	Vec3f ey = Vec3f(0,1,0);
	Vec3f ez = Vec3f(0,0,1);
	

	//debug iniファイルにしたい
/*	initPenPos[0].Pos() = Vec3f(0.35,-0.4,0.35);
	initPenPos[1].Pos() = Vec3f(-0.35,-0.4,-1.0);
	initPenPos[2].Pos() = Vec3f(0.35,-0.4,-1.1);
*/
	initPenPos[0].Pos() = Vec3f(0.35,-0.4,0.35); //green[*]
	initPenPos[1].Pos() = Vec3f(-0.35,-0.4,-1.0);
	initPenPos[2].Pos() = Vec3f(0.35,-0.4,-1.1);

	initPenPos[0].Rot() = Matrix3f(ex,ey,ez);
	initPenPos[1].Rot() = Matrix3f(ex,ey,ez);
	initPenPos[2].Rot() = Matrix3f(ex,ey,ez);

	initPuckPos[0].Pos() = Vec3f(-0.12,0, 0.12);
	initPuckPos[1].Pos() = Vec3f(0.12,0, 0.12);
	initPuckPos[2].Pos() = Vec3f(0.12,0,-0.12);
	initPuckPos[3].Pos() = Vec3f(-0.12,0,-0.12);
	initPuckPos[0].Rot() = Matrix3f(ex,ey,ez);
	initPuckPos[1].Rot() = Matrix3f(ex,ey,ez);
	initPuckPos[2].Rot() = Matrix3f(ex,ey,ez);
	initPuckPos[3].Rot() = Matrix3f(ex,ey,ez);

	UTRef<DispNode> pgframe = DispNode::NodeMap()["PGFrame"];	//緑ペンギン
	UTRef<DispNode> pframe = DispNode::NodeMap()["PFrame"];		//赤ペンギン
	if (pgframe && pframe){
		pgframe->DelFromParent();
		pframe->DelFromParent();
		//penguin[0] = 緑ペンギン
		//penguin[1] = 赤ペンギン１
		//penguin[2] = 赤ペンギン２
		for(int i = 0; i < penNum; i++){
			if(i==0)
				penguin[i].root = pgframe->Copy(i);
			else
				penguin[i].root = pframe->Copy(i);
			dnTree->AddChild(penguin[i].root);
			penguin[i].initPos = initPenPos[i];  //初期位置を設定
			penguin[i].ai = new AIDeffence();	//AI初期化.最初は防衛。
		}
	}else{
		cout << "no Penguins" << endl;
	}

	//パック(雪だるま)
	//pframe再利用しているな
	pframe = DispNode::NodeMap()["PuckFrame"];
	if(pframe){
		pframe->DelFromParent();
		//puck[i]にコピー
		for(int i = 0; i < puckNum; i++){
			puck[i].root = pframe->Copy(i);
			dnTree->AddChild(puck[i].root);
			puck[i].initPos = initPuckPos[i];
		}
	}else{
		cout << "no Puck" << endl;
	}

	//ラケット(自キャラの球体)
	racket.root = DispNode::NodeMap()["RacketFrame"];
	if(racket.root){
		racket.root->DelFromParent();
		dnTree->AddChild(racket.root);
	}else{
		cout << "no Racket" << endl;
	}

	//"Goal"の文字
	goalLogo = DispNode::NodeMap()["GoalFrame"];
	if(goalLogo){
		goalLogo->DelFromParent();
	}else{
		cout << "no GoalLogo" << endl;
	}

	//数字読み込み
//	for(int i=0; i<30; i++){
	for(int i=0; i<11; i++){ //数字は10までしかないので
		char name[256];
		ostrstream(name, sizeof(name)) << "number" << i << '\0';
		UTRef<DispNode> node = DispNode::NodeMap()[name];
		if (node){
			node->DelFromParent();
			numbers.push_back(node);
		}else{
			cout << "no number " << i << endl;
		}
	}

//sprites
	for(int i=0; i<MAX_SPRITES; i++){
		char name[256];
		ostrstream(name, sizeof(name)) << "sprite" << i << '\0';
		UTRef<DispNode> node = DispNode::NodeMap()[name];
		if(node){
			node->DelFromParent();
			sprites.push_back(node);
			cout << "Load Sprite sprites["<<sprites.size()-1<<"]= "<< name << endl;
		}
	}

	//sprite2
	for(int i=0; i<MAX_LANGUAGES; i++){
		char name[256];
		ostrstream(name, sizeof(name)) << "sprite2" << i << '\0';
		UTRef<DispNode> node = DispNode::NodeMap()[name];
		if(node){
			node->DelFromParent();
			sprite2.push_back(node);
			cout << "Load Sprite sprites["<<sprite2.size()-1<<"]= "<< name << endl;
		}
	}
	//sprite4
	for(int i=0; i<MAX_LANGUAGES; i++){
		char name[256];
		ostrstream(name, sizeof(name)) << "sprite4" << i << '\0';
		UTRef<DispNode> node = DispNode::NodeMap()[name];
		if(node){
			node->DelFromParent();
			sprite4.push_back(node);
			cout << "Load Sprite sprites["<<sprite4.size()-1<<"]= "<< name << endl;
		}
	}


	//実験用オブジェクトの読み込み
	for(int i=0; i<1; i++){
		char name[256];
		ostrstream(name, sizeof(name)) << "experiment" << i << '\0';
		UTRef<DispNode> node = DispNode::NodeMap()[name];
		if(node){
			node->DelFromParent();
			experiments.push_back(node);
		}
	}

	
	//debugYI とりあえず試しにペンギンたちを消す
	penguin[0].root->SetVisible(false);
	penguin[1].root->SetVisible(false);
	penguin[2].root->SetVisible(false);
	puck[0].root->SetVisible(false);
	puck[1].root->SetVisible(false);
	puck[2].root->SetVisible(false);
	puck[3].root->SetVisible(false);	
}

void Game::Step(){
//本当はこの辺にシーマネージャーの更新を呼びたいが
	if (mode == SERVER){

		if (memories.graphicRead != memories.graphicShouldRead){

			memories.graphicRead = memories.graphicShouldRead;

			//コマンドバッファの設定
			if (commands.hapticRead){
				int hsr = commands.hapticShouldRead+1;
				if (hsr >= 3) hsr = 0;
				commands.hapticShouldRead = hsr;
				commands.hapticRead = false;
			}
			int curCmdBufId = commands.hapticShouldRead+1;
			if (curCmdBufId >= 3) curCmdBufId = 0;
			cmdArray = &commands[curCmdBufId];

			SetArrangement();

			// ===============================================
			// ゲームモードの時(ここから)
			//debug とりあえず、グリッド表示モードのときもゲームモードにする
			if(view->sceneManager.GetCurScene() == 9 || view->sceneManager.GetCurScene() == 8){
				AIEngine();
				Judge();

//				Record();

				if((friendPoint >= matchPoint) || (enemyPoint >= matchPoint)){					
					view->sceneManager.EventFunc(0);
					GameReset(); // これは、SceneMangerから呼びたい
				}
			}

			Record(); //全部のシーンでデータ記録

			//debugYI このようなゲームの制御はSceneMangerから呼び出す
			// sceneManager.SetScene(6);
			//↓これはSceneManagerから呼び出したい
			/*
			if(initState)	{
				InitArrangement();
			}*/

			Server();
		}
	}else{
		Client();
	}
}

void Game::SetArrangement(){
	for(int i = 0; i < penNum; i++){
		penguin[i].root->SetGtoLPosture(memories[memories.graphicRead].pen[i].GetPosture());
		penguin[i].id = memories[memories.graphicRead].pen[i].objectId;
	}
	for(int i = 0; i < puckNum; i++){
		puck[i].root->SetGtoLPosture(memories[memories.graphicRead].puck[i].GetPosture());
		puck[i].id = memories[memories.graphicRead].puck[i].objectId;
	}
	racket.root->SetGtoLPosture(memories[memories.graphicRead].racket.GetPosture());
	racket.id = memories[memories.graphicRead].racket.objectId;

	// 再描画
	// (view->OnDraw()が呼び出される)
	view->Invalidate();
}

void Game::InitArrangement(){
	
	friendPoint = 0;
	enemyPoint = 0;

	cout << "init" << endl;
	for(int i = 0; i < penNum; i++){
		penguin[i].id = memories[memories.graphicRead].pen[i].objectId;
		cmdArray->push_back(new MoveObject(penguin[i].id, penguin[i].initPos));
	}
	for(int i = 0; i < puckNum; i++){
		puck[i].id = memories[memories.graphicRead].puck[i].objectId;
		puck[i].existence = true;
		cmdArray->push_back(new MoveObject(puck[i].id, puck[i].initPos));
	}
	racket.id = memories[memories.graphicRead].racket.objectId;

	Affinef friendPenPos = penguin[0].initPos;
	friendPenPos.Ex() = friendPenPos.Ex() * -1;
	friendPenPos.Ez() = friendPenPos.Ez() * -1;
	cmdArray->push_back(new MoveObject(penguin[0].id, friendPenPos));
	
//	commands.hapticRead = false;
//	commands.hapticShouldRead = curCmdBufId;

	if (initState > 0) initState--;
	view->Invalidate();
}

void Game::AIEngine(){
	int i;
	for (i=0;i<penNum;i++) {
		penguin[i].ai->Action(&penguin[i]);
	}

	penguin[0].ai = new AIAttack();
	penguin[1].ai = new AIAttack();
	penguin[2].ai = new AIAttack();
}


// ゴールの判定
void Game::Judge(){
	for(int i = 0; i < puckNum; i++){
		if(puck[i].existence){

			// ゴールの判定(Yが負になったら)
			if(puck[i].root->GetGlobalPosture().PosY() < -1){

				cout << "goal" << endl;

				// 味方側ゴール
				if(puck[i].root->GetGlobalPosture().PosZ() < 0){
					friendPoint++;
					SndPuckGoal = MAXSNDPARAM;
					GoalLogo(FRIEND);
				
				// 敵側ゴール
				}else{
					enemyPoint++;
					SndPuckGoal = MAXSNDPARAM;
					GoalLogo(ENEMY);
				}
				
				cout << endl << "friendPoint:" << friendPoint << " enemyPoint:" << enemyPoint << endl;
				puck[i].existence = false;
			}
		}else{

			// 降ってきたpuckが床に到着
			if(puck[i].root->GetGlobalPosture().PosY() > -0.45)
				puck[i].existence = true;
		}

		// 穴に落ちたpuckが復活するタイミング
		if(puck[i].root->GetGlobalPosture().PosY() < -10){
			Affinef drop; //ゴール後復活する位置
			drop = puck[i].initPos;
			drop.PosY() = 1.0;
			cmdArray->push_back(new MoveObject(puck[i].id, drop));
			cmdArray->push_back(new SetVelocity(puck[i].id, Vec3f()));
		}

	}
}

// "GOAL"を描画
void Game::GoalLogo(Side side){
	Vec3f from;
//	if(side == FRIEND) from = Vec3f(0, 0, -0.75f - 1.0f);
//	else from = Vec3f(0,0, -0.75f + 1.0f);
	if(side == FRIEND) from = Vec3f(0, 0, -0.7f);
	else from = Vec3f(0, 0, 0.7f);
	
	if(goalLogo){
		UTRef<DispNode> node;
		static int suffix;
		
		if (bFront){

			node = new DispNode;
			UTRef<DispNode> goal = new GoalNode;
			goal->AddChild(goalLogo->Copy(suffix++));
			node->AddChild(goal);
			Affinef af(Rad(90), 'y', from);
			node->SetPosture(af);
		}else{
			node = new GoalNode;
			node->AddChild(goalLogo->Copy(suffix++));
			Affinef af;
			af.Pos() = from;
			node->SetPosture(af);
		}
		dnTree->AddChild(node);
	}
}

void Game::GameReset(){
	if(friendPoint == enemyPoint){
		cout << "Draw!!" << endl;
	}
	else if(friendPoint > enemyPoint){
		cout << "You Win!!" << endl;
	}
	else{
		cout << "You Lose!!" << endl;
	}

	initState = 1;
}

void Game::Server(){
	if (mode != SERVER) return;
	for(int i=0; i<NCLIENT; i++){
		if (ssock[i].Accept()){
			if (ssock[i].Read()) Command(ssock[i].sock, ssock[i].Command());
		}
	}
}
bool Game::Client(){
	if (mode != CLIENT) return false;
	if (csock.sock.IsConnected()){
		static bFirst = true;
		if (bFirst){
			csock.sock << 'S' << '\0';
			csock.sock << 'S' << '\0';
			csock.sock << 'S' << '\0';
			csock.sock << 'S' << '\0';
			csock.sock.flush();
			bFirst = false;
		}
		if (csock.Read()){
			Command(csock.sock, csock.Command());
			csock.sock.flush();
		}
	}else{
		csock.port ++;
		if (csock.port >= PORTBASE + NCLIENT) csock.port = PORTBASE;
	}
	return true;
}

void Game::Command(std::ostream& sock, std::string& cmd){
	if (cmd[0] == 'S'){
		sock << 'R';
		for(int i = 0; i < penNum; i++){
			sock << penguin[i].root->GetGlobalPosture();
		}
		for(int i = 0; i < puckNum; i++){
			sock << puck[i].root->GetGlobalPosture();
		}
		sock << racket.root->GetGlobalPosture();
		sock << friendPoint << " " << enemyPoint;

		sock << " " << SndPuckPuck;
		sock << " " << SndPenPuck;
		sock << " " << SndPuckGoal;
		sock << " " << SndRacPen;
		sock << " " << SndRacPuck;
		sock << " " << SndRacOppPen1;
		sock << " " << SndRacOppPen2;

		sock << '\0';
		sock.flush();
	}
	if (cmd[0] == 'R'){
		cout <<"RcvCmd:"<< cmd << endl;
		std::istrstream is(cmd.c_str(), cmd.length());
		is.get();
		Affinef af;
		for(int i = 0; i < penNum; i++){
			is >> af;
			penguin[i].root->SetGlobalPosture(af);
		}
		for(int i = 0; i < puckNum; i++){
			is >> af;
			puck[i].root->SetGlobalPosture(af);
		}
		is >> af;
		racket.root->SetGlobalPosture(af);


		int point=0;
		is >> point;
		if (point != friendPoint){
//			GoalLogo(FRIEND);
			friendPoint = point;
		}
		is >> point;
		if (point != enemyPoint){
//			GoalLogo(ENEMY);
			enemyPoint = point;
		}

		is >> SndPuckPuck;
		is >> SndPenPuck;
		is >> SndPuckGoal;
		is >> SndRacPen;
		is >> SndRacPuck;
		is >> SndRacOppPen1;
		is >> SndRacOppPen2;

/*		cout << "SndReceive: " <<SndPuckPuck<<SndPenPuck<<SndPuckGoal<<SndRacPen<<SndRacPuck
			<<SndRacOppPen1<<SndRacOppPen2<<endl;
*/
		view->Invalidate();

		sock << "S" << '\0'; //送信要求
		sock.flush();
	}
}

//行動データのセーブ
//Pentium2 環境では40tickぐらいでしか回ってこない
void Game::Record() {
	//	milisec,RacketX,RacketY,RacketZ,force,...
	//	cout << GetTickCount() <<endl;

	//手に伝えている力。二次計画法を通す前。
	dForce = env.dForce;
	dRatio = env.dRatio;
	env.dForce.clear();
	env.dRatio = 0.0f;

	dataFile << timeGetTime() << ",";
	dataFile << racket.root->GetGlobalPosture().PosX() <<  ",";
	dataFile << racket.root->GetGlobalPosture().PosY() <<  ",";
	dataFile << racket.root->GetGlobalPosture().PosZ() <<  ",";
	dataFile << dForce.X() << ",";
	dataFile << dForce.Y() << ",";
	dataFile << dForce.Z() << ",";
	dataFile << dRatio     << ",";
	dataFile << friendPoint << "," << enemyPoint << ",";
	dataFile << env.bHaptics << "," ;
	dataFile << env.view->sceneManager.GetCurScene() << "," ;
	dataFile << endl;
}

// ----------------------------------------------------------------
GoalNode::GoalNode(){
	life = 100;
}

void GoalNode::Draw(){
	DispNode::Draw();
	life--;
	if (life <= 0){
		DelFromParent();
	}
	posture = posture * Affinef(Rad(10), 'x');
	posture.PosY() += 0.005f;
	if (life > 80) posture.Rot() *= 1.1;
	else posture.Pos().Y() += 0.02f;
}
// ----------------------------------------------------------------

HaCommands::HaCmdArray* AIAction::CmdArray(){
	return theGame->cmdArray;
}

// ----------------------------------------------------------------

//防衛アクション
void AIDeffence::Action(Character *c)
{
	Vec3f inputForce,targetpos,sub;
	float	vel;	//移動速度
	float	len;	//ターゲットとの距離
	float	neutral;	//ニュートラル方向(自分=90度,敵=270度)
	vel = 0.5f;

	//自陣のゴール前を守ります
	Affinef af = c->root->GetGlobalPosture();

	//キャラクタによって防衛位置は異なるので...
	switch (c->id) {
		cout << "debug ID = " << c->id << endl;
	case 12:	//自分ゴール前
		targetpos.X() = 0.0f;
		targetpos.Y() =-1.0f*lydown;
		targetpos.Z() = 0.2f;
		neutral = 90.0;
		break;
	case 13:	//夏バテ対策
		targetpos.X() = 0.0f;
		targetpos.Y() =-1.0f*lydown;
		targetpos.Z() = 0.2f;
		neutral = 90.0;
		break;
	case 14:	//敵ゴール前(深い)
		targetpos.X() = 0.0f;
		targetpos.Y() =-1.0f*lydown;
		targetpos.Z() =-1.6f;
		neutral = 270.0;
		break;
	default:
		cout << "ID = " << c->id << endl;
		targetpos.X() = 0.0f;
		targetpos.Y() =-1.0f*lydown;
		targetpos.Z() = 0.0f;
	}
	sub = targetpos - af.Pos();
	//距離算出
	len = sqrt(sub.X() * sub.X() + sub.Y() * sub.Y() + sub.Z() * sub.Z());
	//正規化
	sub = sub / len;

	inputForce.X() = sub.X() * vel;
	inputForce.Z() = sub.Z() * vel;
	//移動方向に沿った回転(90度足さないと真横を向く)	
	af.Rot() = Matrix3f(Rad(Deg(acos(sub * af.Ez()))+ neutral),'y');
	//コマンド発行
	CmdArray()->push_back(new MoveObject(c->id, af));
	CmdArray()->push_back(new SetVelocity(c->id, inputForce));

}

// ----------------------------------------------------------------

//攻撃アクション"DummAttack"
void AIDummAttack::Action(Character *c)
{
	Vec3f inputForce,targetpos,sub;
	float	newlen;	//ターゲットとの距離

	//Puckを追いかけます
	Affinef af = c->root->GetGlobalPosture();
	Affinef af2 = theGame->puck[c->puck].root->GetGlobalPosture();
	targetpos = af2.Pos();
	sub = targetpos - af.Pos();
	//目標物との距離算出
	newlen = sqrt(sub.X() * sub.X() + sub.Y() * sub.Y() + sub.Z() * sub.Z());

	//自殺点対応
	if (!c->enemy) {
//		cout << "Z="<< sub.Z() << endl;
//		af.Ez() * sub;
	}

	//距離比較
	if (newlen < c->len) {	//距離が縮んだ！
		c->len = newlen;
	} else {
		c->wait--;
		if (c->wait<0) {	//堪忍袋の緒が切れた
			c->puck--;		//目標変更
				if (c->puck<0) {
					c->puck = NUM_PUCK -1;
				}
			c->wait = c->defaultwait;
#ifdef AI_DEBUG
			cout << "DummAttack:target[" << c->id << "] changed -> " << c->puck << endl ;
#endif
		}
	}
	//正規化
	sub = sub / newlen;
	//与える力を算出
	inputForce.X() = sub.X() * c->defaultvel;
	inputForce.Y() = sub.Y() * c->defaultvel;
	inputForce.Z() = sub.Z() * c->defaultvel;
	//コマンド発行
	CmdArray()->push_back(new InputForce(c->id, inputForce*8, af * Vec3f(0,0.75,0.5) ));
}

// ----------------------------------------------------------------

//攻撃アクション(開発中)
void AIAttack::Action(Character *c)
{
	Vec3f inputForce,targetpos,sub;
	float	newlen;	//ターゲットとの距離

	//Puckを追いかけます
	Affinef af = c->root->GetGlobalPosture();
	Affinef af2 = theGame->puck[c->puck].root->GetGlobalPosture();
	targetpos = af2.Pos();
	sub = targetpos - af.Pos();
	//目標物との距離算出
	newlen = sqrt(sub.X() * sub.X() + sub.Y() * sub.Y() + sub.Z() * sub.Z());
	//距離比較
	if (newlen < c->len) {	//距離が縮んだ！
		c->len = newlen;
	} else {
		c->wait--;
		if (c->wait<0) {	//堪忍袋の緒が切れた
			c->puck--;		//目標変更
				if (c->puck<0) {
					c->puck = NUM_PUCK -1;
				}
			c->wait = c->defaultwait;
#ifdef AI_DEBUG
			cout << "target[" << c->id << "] changed -> " << c->puck << endl ;
#endif
		}
	}
	//正規化
	sub = sub / newlen;
	//与える力を算出
	inputForce.X() = sub.X() * c->defaultvel;
	inputForce.Y() = sub.Y() * c->defaultvel;
	inputForce.Z() = sub.Z() * c->defaultvel;
	//コマンド発行
	CmdArray()->push_back(new InputForce(c->id, inputForce*8, af * Vec3f(0,0.75,0.5) ));
}

// ----------------------------------------------------------------

bool TSocketReader::Accept(){
	if (!sock.IsConnected()){
		if (!sock.IsListening()) sock.Listen(port);
		sock.Accept();
		return false;
	}
	return true;
}
bool TSocketReader::Connect(){
	if (!sock.IsConnected()){
		sock.Connect(server.c_str(), port);
		return false;
	}
	return true;
}
bool TSocketReader::Read(){
	int found = buffer.find('\0');
	if (found != buffer.npos){
		cmd = buffer.substr(0, found);
		buffer = buffer.substr(found+1);
		return true;
	}
	if (sock.Avail() > 0){
		char buf[1025];
		int len = std::min((int)sock.Avail(), (int)sizeof(buf)-1);
		sock.read(buf, len);
		buffer.append(buf, len);
	}
	return false;
}

// ----------------------------------------------------------------

void Game::Grid(void) {
	float i,c;

	c = 0.8f;
	glEnable( GL_BLEND );
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f(0,1,0,0.5));
	glMaterialfv(GL_FRONT, GL_DIFFUSE , Vec4f(0,1,0,0.5));
	glMaterialfv(GL_FRONT, GL_AMBIENT , Vec4f(0,1,0,0.5));

	for (i = -c; i <=c ; i += 0.1f ) {
	   glBegin(GL_LINE_LOOP);
		glVertex3fv(Vec3f(-c,0,-c));
		glVertex3fv(Vec3f( i,0,-c));
		glVertex3fv(Vec3f( i,0, c));
		glVertex3fv(Vec3f(-c,0, c));
	   glEnd();
	   glBegin(GL_LINE_LOOP);
		glVertex3fv(Vec3f(-c,0,-c));
		glVertex3fv(Vec3f(-c,0, i));
		glVertex3fv(Vec3f( c,0, i));
		glVertex3fv(Vec3f( c,0,-c));
	   glEnd();
	 }
	//原点を示す三角
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f(0,0,1,0.5));
	glMaterialfv(GL_FRONT, GL_DIFFUSE , Vec4f(0,0,1,0.5));
	glMaterialfv(GL_FRONT, GL_AMBIENT , Vec4f(0,0,1,0.5));
    glBegin(GL_LINE_LOOP);
 	 glVertex3fv(Vec3f(    0, 0,   0));
	 glVertex3fv(Vec3f( 0.05, 0, 0.1));
	 glVertex3fv(Vec3f(-0.05, 0, 0.1));
	glEnd();


	glDisable( GL_BLEND );

}



