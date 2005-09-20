// VHCheckDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "vhindependence.h"
#include "VHCheckDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Spr;

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog ダイアログ


VHCheckDialog::VHCheckDialog(CWnd* pParent /*=NULL*/)
	: CDialog(VHCheckDialog::IDD, pParent)
{
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


void VHCheckDialog::Draw(Spr::GRRender* render, Spr::CRVirtualHuman* crVH){
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

		Vec3f center = Vec3f();
		std::vector<Vec3f> supportVtxs = crVH->supportArea.GetSupportArea();
		for(int i = 0; i < supportVtxs.size(); i++){
			center += supportVtxs[i];
		}
		center /= supportVtxs.size();
		dlgView.px = -center.x;
		dlgView.py = center.y;

		render->camera->data.view = dlgView;

		//ビューポートの変更
		render->SetViewport(viewHW);

		render->ClearBuffer();			//	バッファクリア
		render->BeginScene();			//	描画開始

		//////////		描画ゾーン		//////////
		if(bDrawZmp){
			crVH->balanceQDPR.DrawZMP(render);
		}
		if(bDrawSuArea){
			crVH->supportObject[0].DrawCollisionVetxs(render);
			crVH->supportObject[1].DrawCollisionVetxs(render);

			// 足首のZMPを描画
			for(int i=0; i<2; ++i){
				render->SetModelMatrix(Affinef());
				if (i==0) render->SetMaterial(GRMaterialData(Spr::Vec4f(0,1,1,1)));
				else render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,1,1)));
				render->SetLineWidth(10);
				Vec3f v[2];
				if (i==0){
					v[0] = crVH->balanceQDPR.GetAnkleRealZmpR();
					v[1] = crVH->balanceQDPR.GetAnkleGoalZmpR();
				}else{
					v[0] = crVH->balanceQDPR.GetAnkleRealZmpL();
					v[1] = crVH->balanceQDPR.GetAnkleGoalZmpL();
				}
				render->DrawDirect(Spr::GRRender::POINTS, v , v+1);
				render->DrawDirect(Spr::GRRender::LINES, v , v+2);
			}
		}

		//////////////////////////////////////////
		render->EndScene();				//	描画終了
		render->Present();				//	表示

		render->camera->data.view = mainView;	//カメラの視点をメインウィンドウに戻す
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
