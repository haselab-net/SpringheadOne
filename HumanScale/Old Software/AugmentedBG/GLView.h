#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"
#include "HIS/HIStepWalk.h"
#include "HIS/HITurnTable.h"
#include "../Share/WalkThroughTools.h"
#include "LabelingCamera.h"
#include "Stereo.h"

using namespace LVRM;

class TGLView: public RDGlutView{
	float nMouseX, nMouseY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	float lastTableAngle;
	int first[2], second[2];	//  マウス座標の履歴
	Vec3f mouseVector;			//  マウスの移動ベクトル
	bool bBox;					//  バウンディングボックスの表示／非表示
	bool avatarHit;				//  アバタにヒットしているかどうか
	bool objectHit;				//  オブジェクトにヒットしているかどうか

	DVDeviceManager dev;
	HIStepWalk hiStep;
	HITurnTable hiTurn;
	bool bInitSuccess;
	float birdX, birdZ;

	//	衝突判定
	float radius;						//	アバターの半径
	float hand_radius;					//	手の半径　追加：赤羽
	TClosestPointDetectors cpds;		//	最近傍点の検出器
	TClosestPointDetectors LHand_cpds;	//	左手最近傍点の検出器　追加：赤羽
	TClosestPointDetectors RHand_cpds;	//	右手最近傍点の検出器　追加：赤羽
	TReflectNormalCalculator rnc;		//	反射面の法線の計算機
	TPenaltyCanceler penCancel;			//	ペナルティ解消の計算

	//	アバターの属性
	Affinef afEye;				//	アバターから見た目の位置
	Vec3f vel;					//	アバターの速度
	Vec3f dvel;					//	アバターの速度の変化
	bool bFalling;				//	アバター落下中フラグ
	float fallingTime;			//  アバター落下時間
	Affinef bird;				//  鳥瞰カメラ制御用
	Affinef quarter;			//  クォータービュー？？カメラ制御用
	Vec4f planeNormal;			//  平面の法線ベクトル
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;
	Vec4f worldNear, worldFar;
	Vec4f collisionPoint;
	
	bool bHistorySave;						//  軌跡記録用フラグ
	bool bHistoryDraw;						//  軌跡表示フラグ
	bool bHistoryToFile;					//  軌跡ファイル書きだしフラグ//
	typedef std::vector<Vec3f> History;		//  軌跡保存用
	History historyData;
	typedef std::vector<DWORD> Times;		//  時間保存用
	Times currentTime;
	bool bTest;								//  テスト用暗幕設定

public:
	Vec3f avatarMoveVector;			//  アバターの移動ベクトル
	Vec3f objectMoveVector;			//  物体の移動ベクトル
	bool bZoom;						//  ズーム中ならtrue
	Affinef beforeRotateAfBody;		//  マウスで回転する前のアバターの姿勢
	bool nowRotateBirdCam;			//  鳥瞰カメラの回転中ならtrue
	float birdCamRotateAngle;		//  鳥瞰カメラの回転角度
	float tmpElevation[2];
	float elevationHeight;			//  鳥瞰視点の高度
	float elevationDistance;		//  鳥瞰視点の距離
	DVOrientationBase* elevationSensor;
	float startElevationAngle;
	float viewElevation;
	Affinef afLHand;				//  アバターの左手の位置　追加：赤羽
	Affinef afRHand;				//  アバターの右手の位置　追加：赤羽
	int tmpMouseY[2];				//  マウス位置の履歴
	int mouseX, mouseY;				//  スクリーン上のマウスの位置
	int startMouseY;				//  右クリックしたときのマウスの位置
	Affinef afProjection;
	Affinef afCam;
	Affinef afQuarter;				//  クオータービュー？用のカメラ
	Affinef afTmpB;
	bool nowLeftBottunOn;			//  現在マウスの左ボタンがonならtrue
	bool nowRightBottunOn;			//  現在マウスの右ボタンがonならtrue
	int windowWidth, windowHeight;	//  ウィンドウの縦と横のドット数
	Affinef afBody;					//	アバターの位置
	Affinef afInitialBody;			//  アバターの初期位置
	Affinef afBird;					//  鳥瞰視点
	Affinef afBirdRot;				//  鳥瞰視点カメラの回転制御用
	Vec3f VecMyself[3];				//  自分自身の位置
	Vec3f VecTest[4];				//  テスト用の四角形
	TGeoNodes allNodes;				//	すべてのノード
	Vec3f clearColor;				//	空の色
	bool bSender;					//	送信側ならば true
	bool birdsView;					//  鳥瞰視点ならばtrue
	bool quarterView;				//  クオータービューならばtrue
	bool quarterViewM;
	int viewMode;					//  視点のモード (0:アパタ 1:上から視点 2:鳥瞰視点)
	int viewTurn;					//  視点の回転角
	float topHeight;				//  上から視点の高さ
	float topShift;					//  上から視点の中心からのずれ
	float birdsHeight;				//  鳥瞰視点の高さ
	float birdsRadius;				//  鳥瞰視点の角度
	float birdsShift;				//  鳥瞰視点の中心からのずれ
	float cRadius;					//  半径
	bool showInnerData;				//  内部データ(アパタ位置・圧力センサ入力・重心など)を表示するか
	int cSides;						//  角の数
	double PI2;						//  円周角
	float cStep;					//
	Vec3f VecCircle[32];
	Vec3f VecSquare[4];
	Vec3f VecSensor[4];
	Vec3f VecStartPosition[4];
	bool nowElevation;

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
	void Move();
	void CamRotate();
	void InitElevation();
	void NowElevation();
	void InitElevationM();
	void NowElevationM();
	void InverseTransform();
protected:
	float dwLastTime;
	float dwTime;

//	float jumpPAve;  //ジャンプする力測定用
	float jumpRate;
	int jumpFlag;
	int numSensor;

	bool getSIGGRAPHData;

	SLCamera slcamera;//ステレオラベリングカメラ

	StereoV stVision;//ステレオ視用
	Affinef afC;

	Vec3f labelP[2];

};

#endif

