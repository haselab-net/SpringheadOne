// VHCheckDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "VirtualHuman.h"
#include "VHCheckDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern Spr::GRMaterialData Mat(Spr::Vec4f c);

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog ダイアログ


VHCheckDialog::VHCheckDialog(CWnd* pParent /*=NULL*/)
	: CDialog(VHCheckDialog::IDD, pParent)
{
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

void VHCheckDialog::DrawDlg(Spr::GRRender* render, TBalance& balance,Spr::PHHuman* phHuman){

	if (!m_hWnd) return;
	CWnd* viewCWnd = GetDlgItem(IDC_VIEWER);	//ピクチャー（VIEWER)の取得
	HWND viewHW = viewCWnd->GetSafeHwnd();		//ウィンドハンドルを取得

	if(viewHW){

		//カメラ設定
		Spr::Affinef dlgView,		//このダイアログのカメラ位置
					mainView;		//メインダイアログのカメラ位置
					
		mainView = render->camera->data.view;	//メインの視点を保存しておく

		//dlgのカメラ位置の設定
		dlgView.yz = -1.0f;
		dlgView.yy = 0.0f;
		dlgView.zy = -1.0f;
		dlgView.zz = 0.0f;
		dlgView.pz = -1.0f;
		render->camera->data.view = dlgView;

		//ビューポートの変更
		render->SetViewport(viewHW);

		render->ClearBuffer();			//	バッファクリア
		render->BeginScene();			//	描画開始

		//DrawFootArea(render,balance);
		DrawCollisionArea(balance.collisionArea);
		DrawZmp(render,balance,phHuman);

		render->EndScene();				//	描画終了
		render->Present();				//	表示

		render->camera->data.view = mainView;	//カメラの視点をメインウィンドウに戻す
	}

}

void VHCheckDialog::DrawFootArea(Spr::GRRender* render, TBalance& balance){
		Spr::Vec3f* supportPoint = balance.GetFootPoint();
		Spr::Vec3f supportArea[9];

		//線を描画するため、一つ増やす。
		for(int i = 0; i < 8; ++i){
			supportArea[i] = supportPoint[i];
		}
		supportArea[8] = supportArea[0];

		//メインウィンド座標をこのダイアログ座標に変換するため
		Spr::Vec3f center;
		for(int i = 0; i < 8; ++i){
			center += supportArea[i];
		}
		center = center/8;
		for(int i = 0; i < 9; ++i){
			supportArea[i] -= center;
			SetNonDepth(&supportArea[i]);
		}

		//安定領域の描画
		render->SetLineWidth(6);
		render->SetMaterial(Mat(Spr::Vec4f(1,0,1,1)));
		for(int i=0; i<8; ++i){
			render->DrawDirect(Spr::GRRender::POINTS, &supportArea[i] , &supportArea[i]+1);
			render->DrawDirect(Spr::GRRender::LINES, &supportArea[i], &supportArea[i+2]);
		}

		//ZMPの描画
		render->SetMaterial(Mat(Spr::Vec4f(1,0,0,1)));
		render->SetLineWidth(3);
		Spr::Vec3f v = balance.GetZmp();
		v -= center;
		SetNonDepth(&v);
		render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);

		//原点の描画
		Spr::Vec3f zero;
		render->DrawDirect(Spr::GRRender::POINTS, &zero , &zero+1);

}

void VHCheckDialog::DrawCollisionArea(CollisionArea& collisionArea){
	collisionArea.DrawCollisionVetxs();
}

void VHCheckDialog::DrawZmp(Spr::GRRender* render, TBalance& balance, Spr::PHHuman* phHuman){
	balance.DrawZmp(phHuman,render);
}

BEGIN_MESSAGE_MAP(VHCheckDialog, CDialog)
	//{{AFX_MSG_MAP(VHCheckDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog メッセージ ハンドラ
