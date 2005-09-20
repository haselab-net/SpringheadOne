#ifndef LDSIMULATOR_H
#define LDSIMULATOR_H

#include <FrameWork/FWAppD3D.h>
#include <FrameWork/FWPointer.h>
#include <Physics/PHContactEngine.h>
#include <SceneGraph/SGScene.h>
#include <vector>
#include "LDRenderer.h"
#include <Device/DRUsb20Sh4.h>
#include "Matrix6.h"
#include "Vector6.h"

namespace Spr{;
class LDSimulator:public FWAppD3D, CDCollisionListener{
//	FWAppD3D* pApp;
	PHContactEngine::FrameRecord* pFrameRecord;			// 最近傍物体のFrameRecord
	PHContactEngine::FrameRecord* pPrevFrameRecord;		// 一回前のFrameRecord
	PHContactEngine::FrameRecord* pDSFrameRecord;		// DummySphereのFrameRecord
	PHContactEngine::FrameRecord* pPointerFrameRecord;	// 力覚ポインタのFrameRecord
	PHContactEngine::FrameRecord* pPointer2FrameRecord;	// 力覚ポインタのFrameRecord

	DRUsb20Sh4* ihc;
	LDRenderer* pLDRenderer;


	float k, b;						// 物体のばね係数とダンパ係数
	double mass_inv;				// 物体の質量の逆数

	Vec6d velocities;				// 物体の速度・角速度を持つ6次元ベクトル
	Vec3d pos, normal;				// 物体の位置、法線ベクトル
	Vec3d prevPos;					// 一回前の位置
	Vec3d center;					// 物体の質量中心
	Vec3d f_external;				// 物体にかかる外力
	Vec3d torque_ex;				// 物体のトルク
	Vec3d p;						// 質量中心から作用点へのベクトル
	Vec3d impulse;					// 物体が受けた力積
	Vec6d displacement;				// 物体の移動量（位置・回転）

	Matrix6d Iinv;					// 慣性行列の逆行列
	Matrix3d rotation;				// 回転行列(物体座標系->グローバル座標系)
	Matrix3d matP;					// 外積の行列表示
	
	Vec3d pointer;					// 力覚ポインタの位置

	float simuDT;
	bool bChanged;					// 送信する物体が変わったかどうかのチェック
	bool calc_ld;
	bool bExperiment;

	std::vector<Vec3d> vNormals;
	std::vector<Vec3d> vClosestPoints;
	std::vector<PHContactEngine::FrameRecord*> vFrameRecords;

protected:
	int listenerPos;				///<	リスナの位置
	
public:
	FWPointer6D* device;

	LDSimulator();
	~LDSimulator();

	//パラメータ初期化
	void ResetParameters();
	//毎回の更新処理（シミュレータ）
	void Step();
	//ローカルエリアの物理更新
	void IntegrateLocalDynamics();
	//ローカルエリアの設定
	void DefineLocalArea();
	//最近傍点の探索（vector要素の添え字を返す)
	int FindClosestPoint();
	//LocalDynamicsプロセスに情報送信
	void Send();
	//LocalDynamicsプロセスから情報受信
	void Receive();
	//IHCを使った場合の通信用関数
	void IHCSend();
	void IHCReceive();
	/*衝突判定リスナ用関数
	  衝突物体の最近傍点とFrameRecordをベクターに登録  */
	void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	// Listener追加
	void AddListener();
	void Load(std::string fn);
	bool OnKeyDown(unsigned int key);

	bool GetCalcLD() { return calc_ld; }
	void SetCalcLD(bool  b) { calc_ld = b;}
	void SetBExperiment(bool b){ bExperiment = b; }
	void SetSimuDT(float t) { simuDT = t; }
//	void SetFWApp(FWAppD3D* p){ pApp = p; }
	void SetLDRenderer(LDRenderer* p){ pLDRenderer = p; }
	void FindDSFrameRecord();
	void SetIHC(DRUsb20Sh4* i){ ihc = i; }
};

} // Spr

#endif //LDSIMULATOR_H