#ifndef	_MESFUNCS_H_
#define	_MESFUNCS_H_

#include <WinBasis/WBWin.h>
//#include <tchar.h>
#include <WinBasis/WBCriticalSection.h>
#include <Physics/PhSolid.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Collision/CDMesh.h>
#include <ImpD3D/D3Render.h>
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <ImpD3D/D3Mesh.h>
#include <Graphics/GRCamera.h>


// Window関数で共通のパラメータ
typedef struct tagMesParams {
	char szFileName[ MAX_PATH ];		// 現在読み込んでいるシーンのパス＆ファイル名
	Spr::UTRef<Spr::D3Render> render;	// レンダラー
	Spr::UTRef<Spr::SGScene> scene;		// シーングラフ
	Spr::Affinef afBody, afHead;
	UINT timerID;
	float dt;				// ステップ更新間隔
//	Spr::UTRef<Spr::SGCamera> camera;	// 現在のカメラ
	Spr::PHSolid* solid;
	BOOL bDropOn;			// ファイルドロップを受け付けるか
} MesParams;

// Window関数で呼び出しごとに使うパラメータ
typedef struct tagMesFuncParams {
	HWND	hWnd;
	UINT	uMsg;
	WPARAM	wParam;
	LPARAM	lParam;
} MesFuncParams;

LRESULT CALLBACK
MainWndProc( HWND hWnd, UINT uMsg,
			WPARAM wParam, LPARAM lParam); // メインのウインドウ関数
LRESULT	OnCreate( MesParams *mp, MesFuncParams *mfp );	// 初期化関数
LRESULT	OnPaint( MesParams *mp, MesFuncParams *mfp );	// 画面描画
LRESULT	OnKeyDown( MesParams *mp, MesFuncParams *mfp );	// キーを押したとき
LRESULT	OnDestroy( MesParams *mp, MesFuncParams *mfp );	// 終了時の関数
LRESULT	OnTimer( MesParams *mp, MesFuncParams *mfp );	// タイマー関数
LRESULT	OnDrop( MesParams *mp, MesFuncParams *mfp );	// ファイルドロップ関数

LRESULT FileDlg( MesParams *mp, MesFuncParams *mfp );	// シーンファイルのオープンダイアログ
LRESULT	Load( const char *filename, MesParams *mp, MesFuncParams *mfp );	// シーンのロード

LRESULT	Step( MesParams *mp, MesFuncParams *mfp );	// 世界を指定時間だけ進める
LRESULT CameraMove( MesParams *mp, MesFuncParams *mfp );	// キーボードによるカメラ操作

#endif // _MESFUNCS_H_
