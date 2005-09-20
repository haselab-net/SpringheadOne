// FWAppVH.h: FWAppVH クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef FWAPPVH_H
#define FWAPPVH_H

#include <Framework/FWAppD3D.h>
#include <Creature/CRVirtualHuman.h>
//#include <Creature/CRVH.h>
#include <Base/TVector.h>
#include <Base/Affine.h>
#include <WinBasis/WBPreciseTimer.h>
#include <iostream>
#include "VHCheckDialog.h"

namespace Spr{;

class FWAppVH : public FWAppD3D  
{
public:
	FWAppVH();
	virtual ~FWAppVH();

	void Step();	
	void Load(std::string fn);
	void Draw();
	bool OnKeyDown(unsigned int nChar);
	void DrawDlg();
	void AppControl(CRVirtualHuman* crVH);

	void DataOut();

	bool bCalc;						// バランス制御開始
	bool balance;
	
	UTRef<CRVirtualHuman> crVH;		// VHエンジンクラス（制御,身体設定）
	//UTRef<CRVH> crVH;		// VHエンジンクラス（制御,身体設定）
	VHCheckDialog vhDlg;			// 別ダイアログで部分的にＶＨをチェック
	PHContactEngine* contactEngine;	// 接触エンジン
	WBPreciseTimer timer;
};

}
#endif // !defined(FWAPPVH_H)
