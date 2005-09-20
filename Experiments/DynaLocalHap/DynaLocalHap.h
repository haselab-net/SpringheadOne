// DynaLocalHap.h : DynaLocalHap アプリケーションのメイン ヘッダー ファイル

#if !defined(AFX_MFCD3DVIEWER_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
#define AFX_DynaLocalHap_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/** @page DynaLocalHap 物理シミュレータに力覚インタフェースでさわる．
	@section DynaLocalHapAbst 概要
	HISpidarG6 と 物理シミュレータ のサンプル．
	フレーム階層を持つDirectXファイルをロードして，SPIDARで触れる．
	
	@section DynaLocalHapHowToBuild ビルドの方法
	- 環境変数 Springhead に Springheadをインストールしたディレクトリを指定
	  して下さい．Windows2000ならば[マイコンピュータ]-[プロパティ]-[詳細]-[環境変数]-
	  [新規]のダイアログに[変数名] Springhead [変数値] C:\\home\\Project\\Springhead
	  としてください．
	- DirectX8.1または9のSDKをインストールしてください．
	- <A href=../Samples/DynaLocalHap/DynaLocalHap.dsw>Samples/DynaLocalHap/DynaLocalHap.dsw</A>を
	  開いてビルドして実行してください．
	
	@section DynaLocalHapHowToOperate 操作方法
	サンプルの操作方法
	- ↑：前進
	- ↓：後退
	- ←：左回転
	- →：右回転
	- Ａ：上昇
	- Ｚ：下降
	- Ｑ：左平行移動
	- Ｗ：右平行移動
	- Ｓ：上を向く
	- Ｘ：下を向く
	- Ｒ：視点リセット
	ｂｙ田上 2002/06/26	

	@section DynaLocalHapPoint 見所
	- CDynaLocalHapViewクラス<br>
		メンバ変数に，render(レンダラ)，afBody*afHead(視点行列), 
		world(シーングラフのトップフレーム)を加えている．
	- CDynaLocalHapView::OnInitialUpdate()<br>
		初期化
	- CDynaLocalHapView::Load()<br>
		Xファイルのロード
	- CDynaLocalHapView::OnDraw()<br>
		レンダリング処理
	- CDynaLocalHapView::OnKeyDown()<br>
		キー入力による視点移動処理
	- CDynaLocalHapDoc::OnOpenDocument()<br>
		ファイルのオープン(単にLoadを呼び出すだけ)
	他の部分はAppWizerdが作ったままになっています．
	@include DynaLocalHapView.cpp
*/

/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapApp:
// このクラスの動作の定義に関しては DynaLocalHap.cpp ファイルを参照してください。
//
class CDynaLocalHapApp : public CWinApp
{
public:
	CDynaLocalHapApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDynaLocalHapApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CDynaLocalHapApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DynaLocalHap_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
