// FWAppVH.h: FWAppVH クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef FWAPPVH_H
#define FWAPPVH_H

#include <Framework/FWAppD3D.h>
#include <Creature/CRVirtualHuman.h>
#include <Base/TVector.h>
#include <Base/Affine.h>

#include "VHCheckDialog.h"
#include "Model.h"
#include "ModelUnder.h"
#include "QDPR.h"
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
	void AppControl(ModelUnder* model);
	void AppControl(Model* model);

	bool bCalc;						// バランス制御開始
	
	VHCheckDialog vhDlg;			// 別ダイアログで部分的にＶＨをチェック
	PHContactEngine* contactEngine;	// 接触エンジン

	// _noをどちらに付けるかによって、片足モデルか下半身モデルかきまる
	UTRef<Model> model_no;			// 片足モデル
	UTRef<ModelUnder> model;		// 下半身モデル

	QDPR qdpr;
};

}
#endif // !defined(FWAPPVH_H)
