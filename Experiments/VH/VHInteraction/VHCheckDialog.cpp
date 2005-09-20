// VHCheckDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "VHInteraction.h"
#include "VHCheckDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Spr;

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog ダイアログ

VHCheckDialog* vhCheckDialog;

VHCheckDialog::VHCheckDialog(CWnd* pParent /*=NULL*/)
	: CDialog(VHCheckDialog::IDD, pParent)
{
	vhCheckDialog = this;
	bDraw = false;
	bDrawZmp = false;
	bDrawSuArea = false;
	//{{AFX_DATA_INIT(VHCheckDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void VHCheckDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(VHCheckDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


void VHCheckDialog::Draw(Spr::GRRender* render, Spr::CRHuman* crHuman){
	if (!m_hWnd) return;
	CWnd* viewCWnd = GetDlgItem(IDC_VIEWER);	//ピクチャー（VIEWER)の取得
	HWND viewHW = viewCWnd->GetSafeHwnd();		//ウィンドハンドルを取得

	if(viewHW){
		//カメラ設定
		Affinef dlgView,		//このダイアログのカメラ位置
				mainView;		//メインダイアログのカメラ位置

		mainView = render->camera->data.view;	//メインの視点を保存しておく

		//dlgのカメラ姿勢
		dlgView.yz = -1.0f;
		dlgView.yy = 0.0f;
		dlgView.zy = -1.0f;
		dlgView.zz = 0.0f;
		//dlgのカメラの位置
		dlgView.pz = -1.5f;
		//dlgView.px = -crHuman->crBalance.GetTargetZMP().x;
		//dlgView.py = crHuman->crBalance.GetTargetZMP().z;
//		dlgView.px = -crHuman->crBalance.supportArea.coCog.x;
//		dlgView.py = crHuman->crBalance.supportArea.coCog.y;

		render->camera->data.view = dlgView;

		//ビューポートの変更
		render->SetViewport(viewHW);

		render->ClearBuffer();			//	バッファクリア
		render->BeginScene();			//	描画開始

		//////////		描画ゾーン		//////////
		if(bDrawZmp){
//			crHuman->crBalance.DrawZMP(render);
		}
		if(bDrawSuArea){
//			crHuman->crBalance.supportArea.DrawCollisionVetxs(render);
		}

		//////////////////////////////////////////
		render->EndScene();				//	描画終了
		render->Present();				//	表示

		render->camera->data.view = mainView;	//カメラの視点をメインウィンドウに戻す
	}
}

void VHCheckDialog::DrawTest(Spr::GRRender* render, Spr::SGScene* scene){
	if (!m_hWnd) return;
	CWnd* viewCWnd = GetDlgItem(IDC_VIEWER);	//ピクチャー（VIEWER)の取得
	HWND viewHW = viewCWnd->GetSafeHwnd();		//ウィンドハンドルを取得

	if(viewHW){
		//ビューポートの変更
		render->SetViewport(viewHW);

		render->ClearBuffer();			//	バッファクリア
		render->BeginScene();			//	描画開始

		//////////		描画ゾーン		//////////
		render->Render(scene);		//	シーンの描画
		render->EndScene();				//	描画終了
		render->Present();				//	表示
	}
}

BEGIN_MESSAGE_MAP(VHCheckDialog, CDialog)
	//{{AFX_MSG_MAP(VHCheckDialog)
	ON_BN_CLICKED(IDC_CHECK_ZMP, OnCheckZmp)
	ON_BN_CLICKED(IDC_CHECK_SUPPORT_AREA, OnCheckSupportArea)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog メッセージ ハンドラ

void VHCheckDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	bDraw = true;
	//CDialog::OnOK();
}

void VHCheckDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	bDraw = false;
	CDialog::OnCancel();
}


void VHCheckDialog::OnCheckZmp() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CButton* chkZmp = (CButton*)GetDlgItem(IDC_CHECK_ZMP);
	if(chkZmp->GetCheck() == 1){
		bDrawZmp = true;
	}
	else{
		bDrawZmp = false;
	}	
}

void VHCheckDialog::OnCheckSupportArea() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CButton* chkSuArea = (CButton*)GetDlgItem(IDC_CHECK_SUPPORT_AREA);
	if(chkSuArea->GetCheck() == 1){
		bDrawSuArea = true;
	}
	else{
		bDrawSuArea = false;
	}		
}
