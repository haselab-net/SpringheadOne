#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"
#include "HIS/HIStepWalk.h"		//#include <HIS/HIStepWalk.h>
#include "HIS/HITurnTable.h"	//#include <HIS/HITurnTable.h>

#include "Seesaw.h"		// シーソーのために使う
#include "MoveFloor.h"  // 動く床のために使う
#include "JumpZone.h"	// ジャンプゾーンのために使う
#include "UdpRecv/udprecv.h" // レーザーポインタの受信に使う
#include "TGeoNodePointer.h" // レーザーポインタノード

#include "../Share/WalkThroughTools.h"
#include "cylinder_lib/BacicIO2.h"

using namespace LVRM;
using namespace vroadrunner;

#define TIMER_SIZE 3 // timer表示の桁数
#define VASH_SIZE 4 // vash表示の桁数

#define SOUND_ATTACK 0
#define SOUND_COIN 1
#define SOUND_JUMP 2
#define SOUND_WATER 3
#ifdef USE_SOUND
#include "EasyDS/EasyDS.h"
#endif

class TGLView: public RDGlutView{
	int mouseX, mouseY;
	int spinX, spinY; //??? 使っていない
	Button mouseButton;
	double setting[6]; //?? 利用目的がよくわからない -> プロジェクターの設置位置に関する設定値ではないか？
	Affinef R; //?? よくわからない
	double fov_v; //? glutPerspective()　のy軸方向の(段階の)視野角。

	float lastTableAngle; //　ターンテーブルの角度履歴
	float changedAngle;   //  ターンテーブルの角度変化分
	int first[2], second[2];	//  マウス座標の履歴
	Vec3f mouseVector;			//  マウスの移動ベクトル
	bool hitFlag;				//  当たり判定用
	bool bBox;					//  バウンディングボックスの表示／非表示 //?? バウンディングボックスとは？

	DVDeviceManager dev;
	HIStepWalk hiStep;
	HITurnTable hiTurn;
	bool bInitSuccess;
	float birdsHight;

#ifdef USE_SOUND
	CEasyDS *easyds;
#endif

	//	衝突判定
public:
	float radius;					//	アバターの半径
protected:
	TClosestPointDetectors cpds;	//	最近傍点の検出器

#ifdef _SPIDAR_TEST
	float hand_radius;//	手の半径
#endif //_SPIDAR_TEST

	TReflectNormalCalculator rnc;	//	反射面の法線の計算機
	TPenaltyCanceler penCancel;		//	ペナルティ解消の計算

	bool bGravity;		//	重力を加えるかどうか（動く床で使用）

	//	アバターの属性
	Affinef afEye;				//	アバターから見た目の位置
	Vec3f vel;					//	アバターの速度
	Vec3f dvel;					//	アバターの速度の変化
	Vec4f avatarNormal;			//  アバターの法線ベクトル
	Affinef afCam;				//  カメラの位置
	bool bLastIsJump;			//  前回のStepのisJumpを保存
	bool bFallingPond;			//  池に落ちているときにtrue

	bool bDraw; //?? 描画要求をするかどうか
	bool bDrawNormal;
	bool bDrawCollision;
	Vec4f worldNear, worldFar;
	Vec4f collisionPoint;


#ifdef _KATSU_TEST_APP
	//	シーソー
	CSeesaw aSeesaw[8];
#endif //_KATSU_TEST_APP

//#ifdef _OKADA_TEST_APP
	//　動く床
	CMoveFloor aMoveFloor, bMoveFloor, cMoveFloor;
//#endif //_OKADA_TEST_APP

//#ifdef _OKADA_TEST_APP
	//  ジャンプゾーン
	CJumpZone aJumpZone, bJumpZone, cJumpZone, dJumpZone, eJumpZone, fJumpZone, gJumpZone;
//#endif //_OKADA_TEST_APP

	//cylinder
	PIO pio;

	// レーザーポインタ
	float laserX,laserY; //ポインタの座標(画面のX,Y)
	bool laserCaptured; //ポインタが点灯しているか
	bool lastLaserCaptured; // 前回の更新時にポインタが点灯していたか
	CUdpRecv *udpRecv;
	Affinef afLaser; // カメラ位置から見たポインタの座標 afPoint = afCam * afLaser;みたいに使う。
	Vec3f vecLaser; // ワールド座標で見た、カメラ位置からポインタ位置へのベクトル
	TGeoNodePointer *pLaserNode;

public:
	void PlaySound( int SoundNum );
	void FallPond(); // 池に落ちたときにアバタ―を戻す
	GLfloat bodyHeight;
	int GameTimerSize;
	char GameCommentStr[126];
	void DrawVashVal();
	bool LaserAttack();
	~TGLView();
	bool isGameStart;
	void GameStart();
	bool isGameOver;
	void GameOver();
	void AddTimerVal(int val = 1);
	void GetTimerChar();
	void GetVashChar();
	void AddVashVal(int val);

	int vash_val;
	char vash_char[VASH_SIZE+1]; //3桁

	int timer_val;
	char timer_char[TIMER_SIZE+1]; //4桁
	DWORD timer_start;

	bool isSpotLight;
	void SpotLightSetting();
	void LightSetting();
	TGeoNode* hitNode;			//  当たり判定に引っかかったノードのポインタ格納用
	int windowWidth, windowHeight;	//  ウィンドウの縦と横のドット数
	
#ifdef _SPIDAR_TEST
	Affinef afLHand;			//  アバターの左手の位置　追加：赤羽
	Affinef afRHand;			//  アバターの右手の位置　追加：赤羽
#endif //_SPIDAR_TEST


	Affinef afBody;				//	アバターの位置
	Affinef afBird;				//  鳥瞰視点
	Vec3f VecMyself[3];			//  自分自身の位置
#ifdef _DEBUG
	Vec3f VecTest[4];			//  テスト用の四角形
#endif //_DEBUG
	TGeoNodes allNodes;			//	すべてのノード
	Vec3f clearColor;			//	空の色
	bool bSender;				//	送信側ならば true
	bool birdsView;				//  鳥瞰視点ならばtrue
	int nowMode;
	float screenNear;			//  レンダリングボリュームの前面位置
	float screenFar;			//  レンダリングボリュームの背面位置
	CmdSocket cmdSocket;
	TGLView(int nowMode);
	GLdouble bEye[3], bTarget[3], bUp[3];
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	//void OnJoystick(unsigned int button, int x, int y, int z);
	bool OnTimer();
	void OnIdle();
	void Step();
	//void SetParam();
	//bool LoadSettings();
	void InitCollisionDetector();
	//bool CollisionMouse();
	bool showInnerData;
	Vec3f VecSensor[4];
	Vec3f VecStartPosition[4];
	
	TGeoNodePointer* GetLaserNode(){ return pLaserNode; }

protected:
	float dwLastTime;
	float dwTime;
	float playerAngle; //プレーヤーの正面方向を追従
};

#endif
