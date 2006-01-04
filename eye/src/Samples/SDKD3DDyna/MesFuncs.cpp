#include "MesFuncs.h"
using namespace Spr;

LRESULT CALLBACK
MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static MesParams mpObj;
	MesFuncParams mfpObj;
	mfpObj.hWnd = hWnd;
	mfpObj.uMsg = uMsg;
	mfpObj.wParam = wParam;
	mfpObj.lParam = lParam;

	switch( uMsg )
	{
	case WM_CREATE :
		return OnCreate( &mpObj, &mfpObj );
	case WM_PAINT :
		return OnPaint( &mpObj, &mfpObj );
	case WM_ERASEBKGND : // どうもDefWindowProcが窓のBackgroundを消そうとするので
		return 1;	// バックを消さないのってこれでいいの？
	case WM_KEYDOWN :
		return OnKeyDown( &mpObj, &mfpObj );
	case WM_DESTROY :
		return OnDestroy( &mpObj, &mfpObj );
	case WM_TIMER :
		return OnTimer( &mpObj, &mfpObj );
	case WM_DROPFILES :
		return OnDrop( &mpObj, &mfpObj );
	default :
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}

LRESULT OnCreate( MesParams *mp, MesFuncParams *mfp )
{
	memset( mp, 0, sizeof( MesParams ) );	// とりあえず0初期化

	// ステップ更新の時間間隔(0.01sec.)
	mp->dt = 0.01f;

	// 衝突検知関数マップの初期化
//	Spr::CDFramePair::InitCommonPointMap();

	// ファイルドロップは始めは非対応
	mp->bDropOn = FALSE;

	// レンダラの初期化
	mp->render = new D3Render;
	mp->render->device.Create( mfp->hWnd );
	mp->scene = new SGScene;
	mp->scene->SetTimeStep(mp->dt);
	mp->timerID = SetTimer( mfp->hWnd, 1, 10, NULL );	// 10ms = 0.01sec毎のタイマ

	return 0;
}

LRESULT OnPaint( MesParams *mp, MesFuncParams *mfp )
{
	// レンダラが準備できていなかったら何もしない
	if( !mp->render ) return 0;

	static bool bDraw = false;

	mp->render->Render(mp->scene);
	mp->render->EndScene();
	mp->render->Present();
	mp->render->SetViewport(mfp->hWnd);
	mp->render->ClearBuffer();
//	render->device->SetRenderState(D3DRS_AMBIENT, 0x808080);
	mp->render->BeginScene();

//	return 0;
	return DefWindowProc( mfp->hWnd, mfp->uMsg, mfp->wParam, mfp->lParam );
	// DefWindowProcを呼ばないと、永久にWM_PAINTが呼ばれる。なんでだろう？
}

LRESULT OnKeyDown( MesParams *mp, MesFuncParams *mfp )
{
	int	ret = 0;

	switch( mfp->wParam )
	{
	case VK_ESCAPE :	// ESCキーの場合
		PostMessage( mfp->hWnd, WM_CLOSE, 0, 0 ); // ウインドウを閉じる
		break;
	case 'L' :		// Lキーの場合 (ロード)
		// ret = Load( "Block5.x", mp, mfp );
		ret = FileDlg( mp, mfp );
		break;
	case 'R' :		// シーンファイルのリロード
		if( mp->szFileName[0] ){	// すでにファイルロードをしていること
			ret = Load( mp->szFileName, mp, mfp );
		}
		break;
	case 'K' :		// タイマー停止
		KillTimer( mfp->hWnd, mp->timerID );
		break;
	case VK_SPACE :		// ステップ実行
		ret = Step( mp, mfp );
		break;
	case 'T' :		// タイマー再開
		mp->timerID = SetTimer( mfp->hWnd, 1, 10, NULL );	// 10ms = 0.01sec毎のタイマ
		break;

		// カメラの移動
	case 'Q' :
	case 'W' :
	case 'A' :
	case 'Z' :
	case VK_DOWN :
	case VK_UP :
	case VK_LEFT :
	case VK_RIGHT :
		ret = CameraMove( mp, mfp );
		break;

	case 'C' :		// カメラ行列をデバッグウインドウへ表示
		DSTR << "Camera :" << std::endl;
		DSTR << mp->render->camera->data.view << std::endl;
		break;
	case 'D' :		// ドロップ対応のON/OFF
		if( mp->bDropOn ) mp->bDropOn = FALSE;
		else mp->bDropOn = TRUE;
		DragAcceptFiles( mfp->hWnd, mp->bDropOn );
		break;
	}

	return ret;
}

LRESULT	OnDestroy( MesParams *mp, MesFuncParams *mfp )
{
	PostQuitMessage( 0 );

	return 0;
}

LRESULT OnTimer( MesParams *mp, MesFuncParams *mfp )
{
	return Step( mp, mfp );	// 世界を指定時間だけ進める
}

LRESULT	OnDrop( MesParams *mp, MesFuncParams *mfp )
{
	HDROP	hDrop;
	int		nFiles;

	hDrop = ( HDROP ) mfp->wParam;
	nFiles = DragQueryFile( hDrop, (UINT) -1, NULL, 0 );
	if( nFiles == 1 ){		// 落としたファイルが1個のとき
		DragQueryFile( hDrop, 0, mp->szFileName, MAX_PATH );	// ファイル名のロード
		DragFinish( hDrop );	// ドロップ領域の開放

		return Load( mp->szFileName, mp, mfp );	// 選択されたファイルのロード
	}
	// 複数個落としたときは、何もしない
	DragFinish( hDrop );	// ドロップ領域の開放
	return 0;
}

LRESULT FileDlg( MesParams *mp, MesFuncParams *mfp )
{
	char szCurrentDir[ MAX_PATH ];
	bool bIniDirSet = false;
	OPENFILENAME ofn;
	BOOL bRet;

	if( mp->szFileName[0] == 0 ){	// まだファイル名が設定されていなければ
		// 初期ディレクトリの設定 (カレント)
		GetCurrentDirectory( MAX_PATH, szCurrentDir );
		bIniDirSet = true;	// 初期ディレクトリをセットする
	}

	memset( &ofn, 0, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = mfp->hWnd;
	ofn.lpstrFilter = NULL;
	ofn.lpstrFile = mp->szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = ( bIniDirSet )? szCurrentDir : NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	bRet = GetOpenFileName( &ofn );	// ファイルを開くコモンダイアログ

	if( bRet == 0 ) return 0;	// キャンセル選択時は、何もせずに戻る

	return Load( mp->szFileName, mp, mfp );	// 選択されたファイルのロード
}

LRESULT Load( const char *filename, MesParams *mp, MesFuncParams *mfp )
{
	// シーングラフが準備できてなければ何もしない
	if( !mp->scene ) return 0;

	mp->scene->Clear();
	mp->scene->GetRenderers().Add(mp->render);

//	MessageBox( mfp->hWnd, filename, "Load File", MB_OK );

	FILoadScene loader;
	REGISTER_LOADER(&loader, SceneGraph);
	REGISTER_LOADER(&loader, ImpD3D);
	REGISTER_LOADER(&loader, Collision);
	REGISTER_LOADER(&loader, Physics);
	REGISTER_LOADER(&loader, Graphics);

	D3FileDoc fileDoc;
	fileDoc.Load(filename);
	loader.Load(mp->scene, &fileDoc);

	GRCamera* cam = NULL;
	mp->scene->GetRenderers().GetObjects().Find(cam);
	if (!cam) mp->scene->GetRenderers().Set(new GRCamera);

	return 0;
}

LRESULT	Step( MesParams *mp, MesFuncParams *mfp )
{
	DWORD t = GetTickCount();

	// double delta = 10;

	mp->scene->ClearForce();
	mp->scene->GenerateForce();
	/*
	if(mp->scene->FindObject("soBlock5")){
		mp->solid = (PHSolid*)mp->scene->FindObject("soBlock5");
		if(mp->solid->GetVelocity().X() < 0.5){
			mp->solid->AddForce(Vec3f(delta,0,0));
		}
		if(mp->solid->GetVelocity().X() > 0.5){
			mp->solid->AddForce(Vec3f(-delta,0,0));
		}
	}
	*/
	mp->scene->Integrate();

	InvalidateRect( mfp->hWnd, NULL, TRUE );

	return 0;
}

LRESULT CameraMove( MesParams *mp, MesFuncParams *mfp )
{
	Affinef afCam;
	float df = 1.0f ; //移動変化量

	afCam = mp->render->camera->data.view.inv();	// カメラ行列を取得

	switch( mfp->wParam ){
	case 'Q' :		// X方向左
		afCam.Pos() = afCam.Pos() - afCam.Ex() * df;
		break;
	case 'W' :		// X方向右
		afCam.Pos() = afCam.Pos() + afCam.Ex() * df;
		break;
	case 'Z' :		// Y方向下
		afCam.Pos() = afCam.Pos() - afCam.Ey() * df;
		break;
	case 'A' :		// Y方向上
		afCam.Pos() = afCam.Pos() + afCam.Ey() * df;
		break;
	case VK_DOWN :	// Z方向(奥行)手前
		afCam.Pos() = afCam.Pos() - afCam.Ez() * df;
		break;
	case VK_UP :	// Z方向(奥行)奥
		afCam.Pos() = afCam.Pos() + afCam.Ez() * df;
		break;
	case VK_LEFT :	// 左方向回転
		afCam = afCam * Affinef::Rot(Radf(-5),'y');
		break;
	case VK_RIGHT :	// 右方向回転
		afCam = afCam * Affinef::Rot(Radf(5),'y');
		break;
	}

	mp->render->camera->data.view = afCam.inv();	// 変更後のカメラ行列を適用

	InvalidateRect( mfp->hWnd, NULL, TRUE );		// 再描画要求

	return 0;
}
