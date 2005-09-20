// SLCamera.h: SLCamera クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_)
#define AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SceneGraph/SGScene.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
//#include <Base/TMatrix.h>
#include <Base/Affine.h>
#include <Creature/CRHuman.h>
#include <Creature/CRUser.h>
#include "CySLC.h"

namespace Spr{;

class SLCamera{
	class TMarker{
	public:
		Spr::Vec3f pos;	// マーカー位置
		Spr::Vec3f vel;	// マーカー速度
		int state;		// マーカー状態(0:消失, 1:最新, 2:古い)
		int match;		// マッチラベル (-1:無し 0,1,...:ラベル番号)
		std::vector<float> dist;	// 取得したラベル位置と直前のマーカー位置との距離
	
		TMarker(){
			pos = Spr::Vec3f(0,0,0);
			vel = Spr::Vec3f(0,0,0);
			state = 0;
			match = -1;
		};
	};

protected:
	HANDLE lCamera;
	int labelNum;           //ラベリングカメラで取得するラベルの数
	int lastLabelNum;
	enum {labelNumMax = 64};//一応64にしてあるが少なくしても構わないのでは．
	enum {LABEL_NUM = 3};
	SLCOBJECT labelPos[labelNumMax]; //ラベルの情報を格納する

public:
	#define MaxMarkerNum 3			// トラックングマーカーの数

	bool bStep;
	bool bCalib;
	Spr::Vec3f tmpLabelPos[labelNumMax];	// ラベル位置の仮保存
	TMarker marker[MaxMarkerNum];			// マーカー
	int match[labelNumMax];					// ラベルのマーカーとの対応
	int markerNum;							// 取得したラベル数(有効なマーカー数)

	Affinef T;	// 座標変換行列

	SLCamera();
	virtual ~SLCamera();
	bool InitSLC();
	void CloseSLCamera();

	void Init();
	void SetInitMarkerPos(Spr::Vec3f* soPos);
	bool Draw(Spr::GRRender* render);

	void Calib(SLCOBJECT label);
	Spr::Vec3f Convert(SLCOBJECT label);
	void SaveLabelPos();
	void SaveDistance();
	void IncorrectRecognitionCheck();
	//void SearchNeighborhood();

	float CalcTotalCost(int m, float cost, float min);
	void Matching();
	void CaseOfErasure();

	bool Step(Spr::SGScene* scene);

	void SetUserPos(CRUser* user); 
};

}

#endif // !defined(AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_)
