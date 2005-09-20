// QDPRBalance.h : QDPRBalance アプリケーションのメイン ヘッダー ファイル

#if !defined(AFX_MFCD3DVIEWER_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
#define AFX_QDPRBalance_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/** @page QDPRBalance 物理シミュレータに力覚インタフェースでさわる．
	@section QDPRBalanceAbst 概要
	HISpidarG6 と 物理シミュレータ のサンプル．
	フレーム階層を持つDirectXファイルをロードして，SPIDARで触れる．
	
	@section QDPRBalanceHowToBuild ビルドの方法
	- 環境変数 Springhead に Springheadをインストールしたディレクトリを指定
	  して下さい．Windows2000ならば[マイコンピュータ]-[プロパティ]-[詳細]-[環境変数]-
	  [新規]のダイアログに[変数名] Springhead [変数値] C:\\home\\Project\\Springhead
	  としてください．
	- DirectX8.1または9のSDKをインストールしてください．
	- <A href=../Samples/QDPRBalance/QDPRBalance.dsw>Samples/QDPRBalance/QDPRBalance.dsw</A>を
	  開いてビルドして実行してください．
	
	@section QDPRBalanceHowToOperate 操作方法
	サンプルの操作方法
	- [UP]:		前進
	- [DOWN]:	後退
	- [LEFT]:	左回転
	- [RIGHT]:	右回転
	- A:	上昇
	- Z:	下降
	- Q:	左平行移動
	- W:	右平行移動
	- S:	上を向く
	- X:	下を向く
	- R:	シーンのリロード（リセット）
	- D:	反力の提示(multi media timer と併用不可)
	- M:	Multi media timer を使用
	- T:	通常のタイマー(WM_TIMER)を使用
	- C:	キャリブレーション
	@section ヒューマンインタフェースの設定について
	- 設定内容はレジストリに保存されます．
	- Position Scale:	SPIDARの動きをscale倍拡大して，バーチャル世界に反映します．
	- Force Scale:		バーチャル世界での力を1/scaleに縮小して，力覚提示します．
	- Time step:		データファイルに時間刻みが記されていない場合，この時間刻みでシミュレーションします．
	- Human interface list:	使用するヒューマンインタフェースを指定します．
		クラス名に対応するインタフェース名を並べると，
		見つかったインタフェースを力覚ポインタに順に割り当てます．
		
		インタフェース名は:
		<pre>
SpidarG6X
SpidarG6X

Spidar4
Spidar4 reverse

SpidarG6C
SpidarG6C

Mouse
		</pre>
		こんな感じで指定します．
	
	@section QDPRBalancePoint 見所
	- CQDPRBalanceViewクラス<br>
		CQDPRBalanceView::app がフレームワークであり，ほとんどの処理を行っている．
	- CQDPRBalanceView::OnInitialUpdate()<br>
		初期化
	- CQDPRBalanceView::Load()<br>
		Xファイルのロード
	- CQDPRBalanceView::OnDraw()<br>
		レンダリング処理
	- CQDPRBalanceView::OnKeyDown()<br>
		キー入力による視点移動処理
	- CQDPRBalanceDoc::OnOpenDocument()<br>
		ファイルのオープン(単にLoadを呼び出すだけ)
	- CQDPRBalanceView::OnD3d();
		D3Dでレンダリング
	- CQDPRBalanceView::OnGL();
		OpenGLでレンダリング
	- CQDPRBalanceView::OnHisConfig();
		ヒューマンインタフェースの設定
	@include QDPRBalanceView.cpp
*/

/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceApp:
//
class CQDPRBalanceApp : public CWinApp
{
public:
	CQDPRBalanceApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CQDPRBalanceApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CQDPRBalanceApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_QDPRBalance_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
