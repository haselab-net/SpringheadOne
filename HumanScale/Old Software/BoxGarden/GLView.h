#ifndef TGLView_H
#define TGLView_H

#include <vector>
#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include "communication.h"
#include <HIS/HIStepWalk.h>
#include <HIS/HITurnTable.h>
#include "../Share/WalkThroughTools.h"

using namespace LVRM;

class TGLView: public RDGlutView{
	float nMouseX, nMouseY;
	Vec4f worldMouse;
	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	float lastTableAngle;
	Vec3f mouseVector;			//  マウスの移動ベクトル
	bool bBox;					//  バウンディングボックスの表示／非表示
	bool avatarHit;				//  アバタにヒットしているかどうか
	bool avatarDMove;			//  アバタを瞬間移動するかどうか
	bool objectHit;				//  オブジェクトにヒットしているかどうか
	bool objectDMove;			//  オブジェクトを瞬間移動するかどうか

	DVDeviceManager dev;
	HIStepWalk hiStep;
	HITurnTable hiTurn;
	bool bInitSuccess;
	float birdsHight;
	float birdX, birdZ;

	//	衝突判定
	float radius;					//	アバターの半径
	TClosestPointDetectors cpds;	//	最近傍点の検出器
	TReflectNormalCalculator rnc;	//	反射面の法線の計算機
	TPenaltyCanceler penCancel;		//	ペナルティ解消の計算

	//	アバターの属性
	Affinef afEye;				//	アバターから見た目の位置
	Vec3f vel;					//	アバターの速度
	Vec3f dvel;					//	アバターの速度の変化
	Vec4f planeNormal;			//  平面の法線ベクトル
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;
	Vec4f worldNear, worldFar;
	Vec4f collisionPoint;
	bool bHistorySave;				//  軌跡記録用フラグ
	bool bHistoryDraw;				//  軌跡表示フラグ
	bool bHistoryToFile;			//  軌跡ファイル書きだしフラグ//
	typedef std::vector<Vec3f> History;		//  軌跡保存用
	History historyData;
	typedef std::vector<DWORD> Times;		//  時間保存用
	Times currentTime;
public:
	float mouseX, mouseY;
	Affinef afProjection;
	Affinef afCam;
	Affinef afTmpB;
	TGeoNode* hitNode;				//  当たり判定に引っかかったノードのポインタ格納用
	int windowWidth, windowHeight;	//  ウィンドウの縦と横のドット数
	Affinef afBody;					//	アバターの位置
	Affinef afBird;					//  鳥瞰視点
	Vec3f VecMyself[3];				//  自分自身の位置
	Vec3f VecTest[4];				//  テスト用の四角形
	TGeoNodes allNodes;				//	すべてのノード
	Vec3f clearColor;				//	空の色
	bool bSender;					//	送信側ならば true
	bool birdsView;					//  鳥瞰視点ならばtrue
	int nowMode;
	float screenNear;				//  レンダリングボリュームの前面位置
	float screenFar;				//  レンダリングボリュームの背面位置
	int hitID;						//  当たったボックスのID
	bool firstDraw;					//  一度目の描画かどうかの判定用
	CmdSocket cmdSocket;
	TGLView(int nowMode);
	GLdouble bEye[3], bTarget[3], bUp[3];
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	void OnJoystick(unsigned int button, int x, int y, int z);
	bool OnTimer();
	void OnIdle();
	void Step();
	void SetParam();
	bool LoadSettings();
	void LightSettings();
	void InitCollisionDetector();
	void Collision();
	void DataSave();
protected:
	float dwLastTime;
	float dwTime;
};

#endif

