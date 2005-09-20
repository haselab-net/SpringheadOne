#if !defined(AFX_VHCHECKDIALOG_H__1489F613_9D1C_4F11_9BA4_5F2F4E8EB94E__INCLUDED_)
#define AFX_VHCHECKDIALOG_H__1489F613_9D1C_4F11_9BA4_5F2F4E8EB94E__INCLUDED_

//Springhead
#include "VHInteraction.h"
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <Graphics/GRMaterial.h>
#include <Creature/CRHuman.h>
#include <Base/Affine.h>
#include <vector>

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
	void Draw(Spr::GRRender* render, Spr::CRHuman* crHuman);
	void DrawTest(Spr::GRRender* render, Spr::SGScene* scene);
	bool IsDraw(){return bDraw;}

// ダイアログ データ
	//{{AFX_DATA(VHCheckDialog)
	enum { IDD = IDD_VH_CHECK };
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

	bool bDraw;			//描画するか否か
	bool bDrawZmp;		//ZMPを描画するか否か
	bool bDrawSuArea;	//安定領域を描画するか否か

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(VHCheckDialog)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCheckZmp();
	afx_msg void OnCheckSupportArea();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern VHCheckDialog* vhCheckDialog;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VHCHECKDIALOG_H__1489F613_9D1C_4F11_9BA4_5F2F4E8EB94E__INCLUDED_)
