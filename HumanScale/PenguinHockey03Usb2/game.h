#ifndef GAME_H
#define GAME_H
#include <Base/Affine.h>
#include <WinBasis/WBSockStr.h>
#include "DispNode.h"
#include "Physics.h"
#include "settings.h"

#include <fstream>


class AIAction : public UTRefCount{
public:
	static HaCommands::HaCmdArray* CmdArray();
	virtual void Action(class Character* c) = 0;
	virtual ~AIAction(){}
};

//ディフェンス用AIルーチン
class AIDeffence : public AIAction {
public :
	virtual void Action(class Character* c);
};

//攻撃用AIルーチン"DummAttack"
class AIDummAttack : public AIAction {
public :
	virtual void Action(class Character* c);
};

//攻撃用AIルーチン
class AIAttack : public AIAction {
public :
	virtual void Action(class Character* c);
};

class Character{
public:
	DispNode* root; // ペンギンのroot
	int id;
	Affinef initPos;
	//以下AIで使用するメンバ
	UTRef<AIAction>	ai;
	int	defaultwait;	//気の長さ
	float	defaultvel;		//基本の速度
	int		wait;			//堪忍袋
	int		puck;			//目標パック
	float	len;			//ターゲットとの距離
	bool	enemy;			//敵ならTRUE
	Character(){			//コンストラクタ
		defaultvel	= 0.5f;
		len			= 100.0f;		//最初は最大の距離であってほしいので
		defaultwait	= 200;			//50ターンは辛抱して待つ.あまり気が短いと、行動が中途半端になる。
		wait		= defaultwait;
		puck		= NUM_PUCK -1;	//最初は何が対象でもかまわないが。
		switch (id) {
		case 12:
			enemy = false;
			break;
		case 13:
			enemy = true;
			break;
		case 14:
			enemy = true;
			break;
		}
	};
};

class Penguin : public Character{
public:
	DispNode* rWing; // ペンギンの羽
	DispNode* lWing;
	DispNode* rFoot; // ペンギンの足
	DispNode* lFoot;
};

class Puck : public Character{
public:
	bool existence;
};

class TSocketReader{
protected:
	std::string buffer;
	std::string cmd;
public:
	WBSockStream sock;
	int port;
	std::string server;
	TSocketReader(){ port = 12000; }
	TSocketReader(int p){ port = p; }
	TSocketReader(const char* s, int p){ server = s; port = p; }

	std::string& Command(){ return cmd; }
	bool Read();
	bool Accept();
	bool Connect();
};


class GoalNode : public DispNode{
public:
	int life;
	GoalNode();
	void Draw();
};

class Game{
public:
	//	カメラの姿勢
	Affinef afCam;
	//	スクリーンの位置
	Vec3f screenPos;
	//	スクリーンのサイズ
	Vec2f screenSize;
	//
	Affinef afClipL, afClipR;
	//	演出 フロントスクリーン用
	bool bFront;

	//	コマンドアレイ
	HaCommands::HaCmdArray* cmdArray;
	UTRef<DispNode> dnTree;
	int friendPoint, enemyPoint, matchPoint;
	int initState;
	enum{
		penNum = 3,
		puckNum = 4,
	};
	Penguin penguin[penNum];
	Puck puck[puckNum];
	Character racket;
	UTRef<DispNode> goalLogo;
	vector< UTRef<DispNode> > numbers;
	enum TRunMode{
		SERVER,
		CLIENT
	} mode;
	enum Side{
		FRIEND, ENEMY
	};

	Game();
	void Draw();
	void DrawNumber(int i);
	void Front();
	void AIEngine();
	void Judge();
	void GoalLogo(Side side);
	void GameReset();
	void Step();
	void SetArrangement();
	void InitArrangement();
	bool CheckMemory();
	void Generation();

	void Record(); //データ保存
	ofstream dataFile; //保存先データファイル
	char dataFileName[255]; //データファイル名
	Vec3f dForce; //記録時間内のSPIDAR出力の総和,env.dForceからもらってくる
	float dRatio;
	~Game() {
		dataFile.close(); //いちおう閉じます
	}

private:
	class GameView* view;
	friend class GameView;
	Vec3f puckPos[puckNum];

	//	TCP/IP通信
	void Server();
	bool Client();
	void Command(std::ostream& sock, std::string& cmd);
public:
	enum { NCLIENT = 30,
		PORTBASE = 12000};
	TSocketReader ssock[NCLIENT];
	TSocketReader csock;
};

//Global
extern Game *theGame;

#endif
