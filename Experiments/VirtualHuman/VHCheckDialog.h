#if !defined(AFX_VHCHECKDIALOG_H__85BE0687_4C48_41B8_8C5D_9281C722CBCF__INCLUDED_)
#define AFX_VHCHECKDIALOG_H__85BE0687_4C48_41B8_8C5D_9281C722CBCF__INCLUDED_


//Springhead
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <Graphics/GRMaterial.h>
#include <Physics/PHHuman.h>
#include <Base/Affine.h>
#include <vector>

//VirtualHuman
#include "TBalance.h"
#include "CollisionArea.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VHCheckDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog ダイアログ

class VHCheckDialog : public CDialog
{
// コンストラクション
public:
	VHCheckDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ
	
// ダイアログ データ
	//{{AFX_DATA(VHCheckDialog)
	enum { IDD = IDD_VIEW_ZMP };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(VHCheckDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション

protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(VHCheckDialog)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		
public:
//////////////////////////////////////////////////////
//////////  以下より自作  ////////////////////////////
//////////////////////////////////////////////////////

	void DrawDlg(Spr::GRRender* render, TBalance& balance, Spr::PHHuman* phHuman);		//ダイアログに描画

	//描画するもの
	void DrawZmp(Spr::GRRender* render, TBalance& balance, Spr::PHHuman* phHuman);
	void DrawFootArea(Spr::GRRender* render, TBalance& balance);
	void DrawCollisionArea(CollisionArea& collisionArea);

	void SetNonDepth(Spr::Vec3f* vec){							//ｙ座標をなくす。
		vec->y = 0.0;
	}

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VHCHECKDIALOG_H__85BE0687_4C48_41B8_8C5D_9281C722CBCF__INCLUDED_)
