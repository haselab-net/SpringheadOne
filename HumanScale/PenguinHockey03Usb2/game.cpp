#include "environment.h"
#include "Game.h"
#include "settings.h"
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


Game::Game(){
	bFront = false;
	initState = 2;
	friendPoint = 0; enemyPoint = 0;
	matchPoint = 10;

	for(int i=0; i<NCLIENT; ++i) ssock[i].port = PORTBASE+i;
	csock.port = PORTBASE;
	csock.server = "merlin";

	//	カメラの位置	床から3M
	//	h				原点からは，3+0.3-0.75
	//	
	float camH = 3.0f;
	afCam = Affinef(Rad(-90), 'x', Vec3f(0, camH+0.3f-0.75f, -0.75f));
	afCam = afCam * Affinef(Rad(-90), 'z');
	
	//	スクリーンの位置
	float scale = 768/1.5f;
	screenPos = Vec3f(0, 0, scale * camH);

	//	クリッピングオブジェクトの位置
	afClipR.Pos().X() = 0.5f;
	afClipL.Pos().X() = -0.5f;

	//データファイルの準備
	SYSTEMTIME  STime;
	int fileNameTime;
	fileNameTime = timeGetTime();
	GetLocalTime(&STime);
	sprintf(dataFileName,"%04d%02d%02d_%02d%02d%02d_%010d.txt",
		STime.wYear,STime.wMonth,STime.wDay,
		STime.wHour,STime.wMinute,STime.wSecond,
		fileNameTime);
	chdir("Data");
//	ofstream dataFile(dataFileName);
	dataFile.open(dataFileName);
	cout << "Dataディレクトリに " << dataFileName << " の名称で軌跡を保存します。" <<endl;
	chdir("../");
//	dataFile << "記録開始," << dataFileName << endl;
	dataFile << "milisec,RacketX,RacketY,RacketZ,forceX,forceX,forceX,ForceRatio,FriendPoint,EnemyPoint,haptics" << endl;
//	dataFile << "記録開始時刻," << timeGetTime() <<std::endl;
//	dataFile.close();



}
void Game::Draw(){
	//	シーングラフ
	glPushMatrix();
		Affinef af = afCam.inv();
		glLoadMatrixf(af);
		dnTree->Draw();
	glPopMatrix();
	//	得点
	if (bFront){
		float dist = 5.7f;
//		float h = -4.0f;
		float h = 4.0f;
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
//			glLoadMatrixf(Affinef(Rad(180),'z', Vec3f(-dist, h, -16)));
			glLoadMatrixf(Affinef(Rad(0),'z', Vec3f(-dist, h, -16)));
			DrawNumber(friendPoint);
		glPopMatrix();
		glPushMatrix();
//			glLoadMatrixf(Affinef(Rad(180),'z' ,Vec3f(dist, h, -16)));
			glLoadMatrixf(Affinef(Rad(0),'z' ,Vec3f(dist, h, -16)));
			DrawNumber(enemyPoint);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
}
void Game::DrawNumber(int i){
	if (0 <= i && i < numbers.size()){
		numbers[i]->Draw();
	}else{
		glutSolidCube(1);
	}
}
void Game::Front(){
	float z = -0.75f;
	afCam = Affinef(5.0f*0.5f, 3.0f*0.5f, z);
	afCam.LookAtGL(Vec3f(0,0,z), Vec3f(0,10,z));
	screenPos = Vec3f(0, 0, 1024);
	bFront = true;
}
void Game::Generation(){
	Affinef initPenPos[penNum];
	Affinef initPuckPos[puckNum];

	Vec3f ex = Vec3f(1,0,0);			//グローバル座標系の単位ベクトル
	Vec3f ey = Vec3f(0,1,0);
	Vec3f ez = Vec3f(0,0,1);
	
	initPenPos[0].Pos() = Vec3f(0.35,-0.4,0.35);
	initPenPos[1].Pos() = Vec3f(-0.35,-0.4,-1.0);
	initPenPos[2].Pos() = Vec3f(0.35,-0.4,-1.1);
	initPenPos[0].Rot() = Matrix3f(ex,ey,ez);
	initPenPos[1].Rot() = Matrix3f(ex,ey,ez);
	initPenPos[2].Rot() = Matrix3f(ex,ey,ez);

	initPuckPos[0].Pos() = Vec3f(-0.12,0,-0.63);
	initPuckPos[1].Pos() = Vec3f(0.12,0,-0.63);
	initPuckPos[2].Pos() = Vec3f(0.12,0,-0.87);
	initPuckPos[3].Pos() = Vec3f(-0.12,0,-0.87);
	initPuckPos[0].Rot() = Matrix3f(ex,ey,ez);
	initPuckPos[1].Rot() = Matrix3f(ex,ey,ez);
	initPuckPos[2].Rot() = Matrix3f(ex,ey,ez);
	initPuckPos[3].Rot() = Matrix3f(ex,ey,ez);

	UTRef<DispNode> pgframe = DispNode::NodeMap()["PGFrame"];	
	UTRef<DispNode> pframe = DispNode::NodeMap()["PFrame"];
	if (pgframe && pframe){
		pgframe->DelFromParent();
		pframe->DelFromParent();
		for(int i = 0; i < penNum; i++){
			if(i==0)
				penguin[i].root = pgframe->Copy(i);
			else
				penguin[i].root = pframe->Copy(i);
			dnTree->AddChild(penguin[i].root);
			penguin[i].initPos = initPenPos[i];
			penguin[i].ai = new AIDeffence();	//AI初期化.最初は防衛。
		}
	}else{
		cout << "no Penguins" << endl;
	}
	
	pframe = DispNode::NodeMap()["PuckFrame"];
	if(pframe){
		pframe->DelFromParent();
		for(int i = 0; i < puckNum; i++){
			puck[i].root = pframe->Copy(i);
			dnTree->AddChild(puck[i].root);
			puck[i].initPos = initPuckPos[i];
		}
	}else{
		cout << "no Puck" << endl;
	}

	racket.root = DispNode::NodeMap()["RacketFrame"];
	if(racket.root){
		racket.root->DelFromParent();
		dnTree->AddChild(racket.root);
	}else{
		cout << "no Racket" << endl;
	}

	goalLogo = DispNode::NodeMap()["GoalFrame"];
	if(goalLogo){
		goalLogo->DelFromParent();
	}else{
		cout << "no GoalLogo" << endl;
	}
	for(int i=0; i<30; i++){
		char name[256];
		ostrstream(name, sizeof(name)) << "number" << i << '\0';
		UTRef<DispNode> node = DispNode::NodeMap()[name];
		if (node){
			node->DelFromParent();
			numbers.push_back(node);
		}
	}
}

void Game::Step(){
	if (mode == SERVER){
		if (CheckMemory()){
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
			AIEngine();
			Judge();

			Record();


			if((friendPoint >= matchPoint) || (enemyPoint >= matchPoint)){
				GameReset();
			}

			if(initState)	{
				InitArrangement();
			}
			Server();
		}
	}else{
		Client();
	}
}

bool Game::CheckMemory(){
	if (memories.graphicRead != memories.graphicShouldRead){
		memories.graphicRead = memories.graphicShouldRead;
		return true;
	}
	return false;
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

HaCommands::HaCmdArray* AIAction::CmdArray(){
	return theGame->cmdArray;
}


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
//	Affinef af2 = puck[target].root->GetGlobalPosture();
//	targetpos = af2.Pos();

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
/* 敵ペンギンが夏バテするコード
	case 13:	//敵ゴール前(浅い)
		targetpos.X() = 0.0f;
		targetpos.Y() =-1.0f*lydown;
		targetpos.Z() =-1.4f;
		neutral = 270.0;
		break;
*/
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

//	cout << "Deffence: len = "<< len << " target " << targetpos <<" chr "<< af.Pos()<<endl;

	inputForce.X() = sub.X() * vel;
	inputForce.Z() = sub.Z() * vel;
/*	if (targetpos.X()< -0.00f) {
		if (af.PosZ() > targetpos.Z()) {
			inputForce.Z() = -0.1f;
		} else {
			inputForce.Z() = 0.1f;
		}
		if (af.PosX() > targetpos.X()) {
			inputForce.X() = -0.1f;
		} else {
			inputForce.X() = 0.1f;
		}
	}*/
	//たとえば回転
//	af.Rot() = af.Rot() * Matrix3f(Rad(5),'y');
	//移動方向に沿った回転(90度足さないと真横を向く)	
	af.Rot() = Matrix3f(Rad(Deg(acos(sub * af.Ez()))+ neutral),'y');
//	af.Rot() = af.Rot() * Matrix3f(Rad(len),'x');
	//コマンド発行
	CmdArray()->push_back(new MoveObject(c->id, af));
	CmdArray()->push_back(new SetVelocity(c->id, inputForce));

}

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



void Game::AIEngine(){
	int i;
	for (i=0;i<penNum;i++) {
		penguin[i].ai->Action(&penguin[i]);
	}
//	penguin[0].ai = new AIDummAttack(); //2003/03/06 夏ばての原因はこれか？
	penguin[0].ai = new AIAttack();
	penguin[1].ai = new AIAttack();
	penguin[2].ai = new AIAttack();

	//AIクラスを利用
//	penguin[1].ai = new AIMoveTo(Vec3f(1,0,0));//
///	penguin[1].ai->Action(&penguin[1]);

	//以下古いコード
//	const int target = 0;
	Vec3f inputForce,targetpos;
/*
	//味方ペンギンのAI
	Affinef af = penguin[0].root->GetGlobalPosture();
	Affinef af2 = puck[0].root->GetGlobalPosture();
	targetpos = af2.Pos();
	if (targetpos.X()< -0.00f) {
//	cout << targetpos << af.Pos()<<endl;
		if (af.PosZ() > targetpos.Z()) {
			inputForce.Z() = -0.1f;
		} else {
			inputForce.Z() = 0.1f;
		}
		if (af.PosX() > targetpos.X()) {
			inputForce.X() = -0.1f;
		} else {
			inputForce.X() = 0.1f;
		}
	}
*/
//	cmdArray->push_back(new MoveObject(penguin[0].id, af));
//	cmdArray->push_back(new SetVelocity(penguin[0].id, inputForce));

//	af.Pos().Z() += 1;
//	cmdArray->push_back(new MoveObject(puck[0].id, af));

}

void Game::Judge(){
	for(int i = 0; i < puckNum; i++){
		if(puck[i].existence){
			if(puck[i].root->GetGlobalPosture().PosY() < -1){
				if(puck[i].root->GetGlobalPosture().PosZ() < 0){
					friendPoint++;
//					SndPuckGoal += 10;
					SndPuckGoal = MAXSNDPARAM;
					GoalLogo(FRIEND);
				}else{
					enemyPoint++;
//					SndPuckGoal += 10;
					SndPuckGoal = MAXSNDPARAM;
					GoalLogo(ENEMY);
				}
				cout << endl << "friendPoint:" << friendPoint << " enemyPoint:" << enemyPoint << endl;
				puck[i].existence = false;
			}
		}else{
			if(puck[i].root->GetGlobalPosture().PosY() > -0.45)
				puck[i].existence = true;
		}
		if(puck[i].root->GetGlobalPosture().PosY() < -10){
			Affinef drop; //ゴール後復活する位置
			drop = puck[i].initPos;
			drop.PosY() = 1.0;
			cmdArray->push_back(new MoveObject(puck[i].id, drop));
			cmdArray->push_back(new SetVelocity(puck[i].id, Vec3f()));
		}
	}
}

void Game::GoalLogo(Side side){
	Vec3f from;
	if (side == FRIEND) from = Vec3f(0, 0, -0.75f - 1.0f);
	else from = Vec3f(0,0, -0.75f + 1.0f);
	if (goalLogo){
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
//	friendPoint = 0;
//	enemyPoint = 0;
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
//	cout << "force =" << dForce.X() << "ratio =" << dRatio << endl;

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
	dataFile << endl;

}
