// Balance.h : Balance アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_MFCD3DDYNAMOUSE_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
#define AFX_MFCD3DDYNAMOUSE_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/** @page Balance 物理シミュレータにマウスでさわる．
	@section BalanceAbst 概要
	マウスと物理シミュレータのサンプル．
	マウスでポインタとなる物体を動かすことができる．
	
	@section BalanceHowToBuild ビルドの方法
	- 環境変数 Springhead に Springheadをインストールしたディレクトリを指定
	  して下さい．Windows2000ならば[マイコンピュータ]-[プロパティ]-[詳細]-[環境変数]-
	  [新規]のダイアログに[変数名] Springhead [変数値] C:\\home\\Project\\Springhead
	  としてください．
	- DirectX8.1のSDKをインストールしてください．
	- <A href=../Samples/Balance/Balance.dsw>Samples/Balance/Balance.dsw</A>を
	  開いてビルドして実行してください．<br>
		ビルドが成功しない人は以下のことを確認してください．
					- DirectX SDKのインクルードファイルとライブラリファイルのディレクトリの設定<br>
		Visual C++ならば，[ツール]-[オプション]-[ディレクトリ]の「表示するディレクト		リ(S)」を「インクルードファイル」に変え，DirectX SDKのインクルードファイルの		ディレクトリ（例:"C:\DXSDK\INCLUDE"）が設定されているか確認してください．
		設定されていなければ設定してください．
		また，検索の優先順位を上げるために「↑」のボタンを何度かクリックして，
		一番上に来るようにしてください．<br>
		ライブラリファイルについてもインクルードファイルと同様にパスの設定を行ってく		ださい（「表示するディレクトリ(S)」を「ライブラリファイル」に変更する）．
	
	- ロードするデータファイル(@ref XFile "Direct3D Xファイル")は<A href=../../Samples/data/>Samples/data/</A>にあります．

	
	@section BalanceHowToOperate 操作方法
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

	@section BalancePoint 見所
	- CBalanceViewクラス<br>
		メンバ変数に，render(レンダラ)，afBody*afHead(視点行列), 
		world(シーングラフのトップフレーム)を加えている．
	- CBalanceView::OnInitialUpdate()<br>
		初期化
	- CBalanceView::Load()<br>
		Xファイルのロード
	- CBalanceView::OnDraw()<br>
		レンダリング処理
	- CBalanceView::OnKeyDown()<br>
		キー入力による視点移動処理
	- CBalanceDoc::OnOpenDocument()<br>
		ファイルのオープン(単にLoadを呼び出すだけ)
	他の部分はAppWizerdが作ったままになっています．
*/

/////////////////////////////////////////////////////////////////////////////
// CBalanceApp:
// このクラスの動作の定義に関しては Balance.cpp ファイルを参照してください。
//
class CBalanceApp : public CWinApp
{
public:
	CBalanceApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CBalanceApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CBalanceApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MFCD3DDYNAMOUSE_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
