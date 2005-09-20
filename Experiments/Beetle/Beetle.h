// Beetle.h : Beetle アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_MFCD3DVIEWER_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
#define AFX_Beetle_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/** @page Beetle 物理シミュレータ(VisualC++ MFC SDI版)
	@section BeetleAbst 概要
	Springheadライブラリを用いて作成した，物理シミュレータです．
	フレーム階層を持つDirectXファイルをロードして，表示し，
	物理シミュレーションを行います．
	視点を動かすことができます．
	
	@section BeetleHowToBuild ビルドの方法
	- 環境変数 Springhead に Springheadをインストールしたディレクトリを指定
	  して下さい．Windows2000ならば[マイコンピュータ]-[プロパティ]-[詳細]-[環境変数]-
	  [新規]のダイアログに[変数名] Springhead [変数値] C:\\home\\Project\\Springhead
	  としてください．
	- DirectX8.1のSDKをインストールしてください．
	- <A href=../Samples/Beetle/Beetle.dsw>Samples/Beetle/Beetle.dsw</A>を
	  開いてビルドして実行してください．
	
	@section HowToOperate 操作方法
	操作方法

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

	@section BeetlePoint 見所
	- CBeetleViewクラス<br>
		メンバ変数に，render(レンダラ)，afBody*afHead(視点行列), 
		world(シーングラフのトップフレーム)を加えている．
	- CBeetleView::OnInitialUpdate()<br>
		初期化
	- CBeetleView::Load()<br>
		Xファイルのロード
	- CBeetleView::OnDraw()<br>
		レンダリング処理
	- CBeetleView::OnKeyDown()<br>
		キー入力による視点移動処理
	- CBeetleDoc::OnOpenDocument()<br>
		ファイルのオープン(単にLoadを呼び出すだけ)
	他の部分はAppWizerdが作ったままになっています．
*/

/////////////////////////////////////////////////////////////////////////////
// CBeetleApp:
// このクラスの動作の定義に関しては Beetle.cpp ファイルを参照してください。
//
class CBeetleApp : public CWinApp
{
public:
	CBeetleApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CBeetleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CBeetleApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_Beetle_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
