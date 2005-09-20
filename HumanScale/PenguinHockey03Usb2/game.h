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

//�f�B�t�F���X�pAI���[�`��
class AIDeffence : public AIAction {
public :
	virtual void Action(class Character* c);
};

//�U���pAI���[�`��"DummAttack"
class AIDummAttack : public AIAction {
public :
	virtual void Action(class Character* c);
};

//�U���pAI���[�`��
class AIAttack : public AIAction {
public :
	virtual void Action(class Character* c);
};

class Character{
public:
	DispNode* root; // �y���M����root
	int id;
	Affinef initPos;
	//�ȉ�AI�Ŏg�p���郁���o
	UTRef<AIAction>	ai;
	int	defaultwait;	//�C�̒���
	float	defaultvel;		//��{�̑��x
	int		wait;			//���E��
	int		puck;			//�ڕW�p�b�N
	float	len;			//�^�[�Q�b�g�Ƃ̋���
	bool	enemy;			//�G�Ȃ�TRUE
	Character(){			//�R���X�g���N�^
		defaultvel	= 0.5f;
		len			= 100.0f;		//�ŏ��͍ő�̋����ł����Ăق����̂�
		defaultwait	= 200;			//50�^�[���͐h�����đ҂�.���܂�C���Z���ƁA�s�������r���[�ɂȂ�B
		wait		= defaultwait;
		puck		= NUM_PUCK -1;	//�ŏ��͉����Ώۂł����܂�Ȃ����B
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
	DispNode* rWing; // �y���M���̉H
	DispNode* lWing;
	DispNode* rFoot; // �y���M���̑�
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
	//	�J�����̎p��
	Affinef afCam;
	//	�X�N���[���̈ʒu
	Vec3f screenPos;
	//	�X�N���[���̃T�C�Y
	Vec2f screenSize;
	//
	Affinef afClipL, afClipR;
	//	���o �t�����g�X�N���[���p
	bool bFront;

	//	�R�}���h�A���C
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

	void Record(); //�f�[�^�ۑ�
	ofstream dataFile; //�ۑ���f�[�^�t�@�C��
	char dataFileName[255]; //�f�[�^�t�@�C����
	Vec3f dForce; //�L�^���ԓ���SPIDAR�o�͂̑��a,env.dForce���������Ă���
	float dRatio;
	~Game() {
		dataFile.close(); //�����������܂�
	}

private:
	class GameView* view;
	friend class GameView;
	Vec3f puckPos[puckNum];

	//	TCP/IP�ʐM
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
