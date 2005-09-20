// SLCamera.h: SLCamera クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_)
#define AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>
#include <Base/Affine.h>
#include "CySLC.h"

#define PI 3.14159265358979
#define MarkerNumMax 10

class SLCamera{

struct TMarker{
	Spr::Vec3f pos;	// マーカー位置
	Spr::Vec3f vel;	// マーカー速度
//	int state;		// マーカー状態
//	int match;		// マッチラベル (-1:無し 0,1,...:ラベル番号)
//	float dist[MarkerNumMax];	// ラベルとの距離
};

protected:
	HANDLE lCamera;
	int labelNum;           //ラベリングカメラで取得するラベルの数
	int lastLabelNum;
	enum {labelNumMax = 64};//一応64にしてあるが少なくしても構わないのでは．
	enum {LABEL_NUM = 3};
	SLCOBJECT labelPos[labelNumMax]; //ラベルの情報を格納する

public:
	TMarker marker[MarkerNumMax];	// マーカー
	int markerNum;
//	int minIndex[labelNumMax];
//	int maxLabelNum;
//	float minCost;

	SLCamera();
	virtual ~SLCamera();
	bool InitSLC();
	void CloseSLCamera();

	bool Draw(Spr::GRRender* render);
	Spr::Vec3f Convert(SLCOBJECT label);
//	void Matching(int lnum);
//	float CalcTotalCost(int lnum, int l, float cost);
//	void SetPosition(float dt);
	void SetMarkerPos();
	bool Step(float dt);
};

#endif // !defined(AFX_SLCAMERA_H__5A46C8FA_724B_454F_830F_6729780FF313__INCLUDED_)
