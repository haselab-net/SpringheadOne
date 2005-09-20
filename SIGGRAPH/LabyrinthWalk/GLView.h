#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"
#include <HIS/HIStepWalk.h>
#include <HIS/HITurnTable.h>
#include "../Share/WalkThroughTools.h"
#include "EasyDS.h"
#include "EasyDS.h"

//#define USE_SOUND

using namespace std;
using namespace LVRM;

class TGLView: public RDGlutView{
	int mouseX, mouseY;
//	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;
	float lastTableAngle;
	
	DVDeviceManager dev;
	HIStepWalk hiStep;
	HITurnTable hiTurn;
	bool bInitSuccess;

	//	衝突判定
	float radius;					//	アバターの半径
	TClosestPointDetectors cpds;	//	最近傍点の検出器
	TReflectNormalCalculator rnc;	//	反射面の法線の計算機
	TPenaltyCanceler penCancel;		//	ペナルティ解消の計算

	//	アバターの属性
	Affinef afEye;				//	アバターから見た目の位置
	Vec3f vel;					//	アバターの速度
	Vec3f dvel;					//	アバターの速度の変化
	bool bFalling;				//	アバター落下中フラグ
	float fallingTime;			//  アバター落下時間
	
	bool bDraw;
	bool bDrawNormal;
	bool bDrawCollision;

	// DirectSoundクラス
#ifdef USE_SOUND
	CEasyDS *easyds;
#endif 
public:
	Affinef afBody;				//	アバターの位置
	Affinef afInitialBody;		//  アバターの初期位置
	Vec3f VecMyself[3];			//  自分自身の位置
	TGeoNodes allNodes;			//	すべてのノード
	Vec3f clearColor;			//	空の色
	bool bSender;				//	送信側ならば true
	int viewMode;				//  視点のモード (0:アパタ 1:上から視点 2:鳥瞰視点)
	int viewTurn;				//  視点の回転角
	float topHeight;			//  上から視点の高さ
	float topShift;				//  上から視点の中心からのずれ
	float birdsHeight;			//  鳥瞰視点の高さ
	float birdsRadius;			//  鳥瞰視点の角度
	float birdsShift;			//  鳥瞰視点の中心からのずれ
	float cRadius;				//  半径
	bool showInnerData;			//  内部データ(アパタ位置・圧力センサ入力・重心など)を表示するか
	int cSides;				//  角の数
	double PI2;				//  円周角
	float cStep;				//
	Vec3f VecCircle[32];
	Vec3f VecSquare[4];
	Vec3f VecSensor[4];
	Vec3f VecStartPosition[4];

	int nowMode;
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
	void InitCollisionDetector();

	void OutPutSIGGRAPHData();

	std::ofstream *centerGFile;
	int printFileNameTime;
	char printFileName[16];

	std::ofstream *turnDataFile;
	int printTurnDataTime;
	char printTurnData[16];

/*	std::ofstream *dataFileSIGGRAPH;
	int fileNameSIGGRAPHTime;
	char fileSIGGRAPHTime[16];
*/
	char* folderName;
	char folderNameBack[20];

protected:
	float dwLastTime;
	float dwTime;

	bool getSIGGRAPHData;
};

#endif

