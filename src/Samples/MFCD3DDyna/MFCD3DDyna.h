// MFCD3DDyna.h : MFCD3DDyna アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_MFCD3DVIEWER_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
#define AFX_MFCD3DDyna_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/** @page MFCD3DDyna 物理シミュレータ(VisualC++ MFC SDI版)
	@section MFCD3DDynaAbst 概要
	Springheadライブラリを用いて作成した，物理シミュレータです．フレーム階層
	を持つDirectXファイルをロードして，表示し，物理シミュレーションを行います．
	
	キーボードとマウスで，視点を動かせます．
	力覚ポインタを持つデータファイルでは，力覚ポインタも動かせます．
	名前にD3Dが入っていますが，描画はOpenGLでも行えます．
	
	@section MFCD3DDynaHowToBuild ビルドの方法
	- 環境変数 Springhead に Springheadをインストールしたディレクトリを指定
	  して下さい．Windows2000ならば[マイコンピュータ]-[プロパティ]-[詳細]-[環境変数]-
	  [新規]のダイアログに[変数名] Springhead [変数値] C:\\home\\Project\\Springhead
	  としてください．
	- DirectX8.1のSDKをインストールしてください．
	- <A href=../../Samples/MFCD3DDyna/MFCD3DDyna.dsw>Samples/MFCD3DDyna/MFCD3DDyna.dsw</A>を
	  開いてビルドして実行してください．<br>
		ビルドが成功しない人は以下のことを確認してください．
		- DirectX SDKのインクルードファイルとライブラリファイルのディレクトリの設定<br>
		Visual C++ならば，[ツール]-[オプション]-[ディレクトリ]の「表示するディレクトリ(S)」
		を「インクルードファイル」に変え，DirectX SDKのインクルードファイルのディレクトリ
		（例:"C:\DXSDK\INCLUDE"）が設定されているか確認してください．
		設定されていなければ設定してください．
		また，検索の優先順位を上げるために「↑」のボタンを何度かクリックして，
		一番上に来るようにしてください．<br>
		ライブラリファイルについてもインクルードファイルと同様にパスの設定を行ってください
		（「表示するディレクトリ(S)」を「ライブラリファイル」に変更する）．
	
	- ロードするデータファイル(@ref XFile "Direct3D Xファイル")は
		<A href=../../Samples/data/>Samples/data/</A>にあります．
	
	@section MFCD3DDynaMouseHowToOperate 操作方法
	キーボード操作

	- ↑：前進
	- ↓：後退
	- ←：左回転
	- →：右回転
	- S：上回転
	- X：下回転
	- Q：左平行移動
	- W：右平行移動
	- A：上平行移動
	- Z：下平行移動
	- R：視点リセット
	- F：フルスクリーン
	- G：画面をフルスクリーンから元に戻す
	- スペース：ポインタに力を返すか返さないかのON-OFF
	- T：マルチメディアタイマーの起動（精度を求めるなら必要．Release版のみ）


	マウス操作

	- 左ドラッグ：ポインタをカメラ座標系のx軸，y軸周りに回転
	- 中ドラッグ：ポインタをカメラ座標系のx-z平面で平行移動
	- 右ドラッグ：ポインタをカメラ座標系のx-y平面で平行移動
	- 左右ドラッグ：ポインタをカメラ座標系のx-z平面で平行移動(中ボタンorホイールがないマウス用)
	- 右中ドラッグ：ポインタをカメラ座標系のy-z平面で平行移動
	- ホイール:ポインタをカメラ座標系のz軸方向に平行移動
	- 左ダブルクリック：ポインタの姿勢を初期化
	- 右ダブルクリック：ポインタの位置を初期化
	- Shift + 左ドラッグ：カメラをx軸，y軸周りに回転
	- Shift + 中ドラッグ：カメラをx-z平面で平行移動
	- Shift + 右ドラッグ：カメラをx-y平面で平行移動
	- Shift + 左右ドラッグ：カメラをx-z平面で平行移動(中ボタンorホイールがないマウス用)
	- Shift + 右中ドラッグ：カメラをy-z平面で平行移動
	- Shift + ホイール:カメラをz軸方向に平行移動
	- Shift + 左ダブルクリック：カメラの位置・姿勢を初期化	

	@section MFCD3DDynaPoint 見所
	- CMFCD3DDynaViewクラス<br>
		メンバ変数に，render(レンダラ)，afBody*afHead(視点行列), 
		world(シーングラフのトップフレーム)を加えている．
	- CMFCD3DDynaView::OnInitialUpdate()<br>
		初期化
	- CMFCD3DDynaView::Load()<br>
		Xファイルのロード
	- CMFCD3DDynaView::OnDraw()<br>
		レンダリング処理
	- CMFCD3DDynaView::PreTranslateMessage()<br>
		マウスとキー入力処理．ここでキーとマウスのメッセージを見てしまっています．
	- CMFCD3DDynaDoc::OnOpenDocument()<br>
		ファイルのオープン(単にLoadを呼び出すだけ)
	他の部分はAppWizerdが作ったままになっています．
*/

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaApp:
// このクラスの動作の定義に関しては MFCD3DDyna.cpp ファイルを参照してください。
//
class CMFCD3DDynaApp : public CWinApp
{
public:
	CMFCD3DDynaApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMFCD3DDynaApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CMFCD3DDynaApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MFCD3DDyna_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
