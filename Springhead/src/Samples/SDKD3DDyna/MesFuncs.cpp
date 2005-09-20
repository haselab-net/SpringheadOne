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
	case WM_ERASEBKGND : // �ǂ���DefWindowProc������Background���������Ƃ���̂�
		return 1;	// �o�b�N�������Ȃ��̂��Ă���ł����́H
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
	memset( mp, 0, sizeof( MesParams ) );	// �Ƃ肠����0������

	// �X�e�b�v�X�V�̎��ԊԊu(0.01sec.)
	mp->dt = 0.01f;

	// �Փˌ��m�֐��}�b�v�̏�����
//	Spr::CDFramePair::InitCommonPointMap();

	// �t�@�C���h���b�v�͎n�߂͔�Ή�
	mp->bDropOn = FALSE;

	// �����_���̏�����
	mp->render = new D3Render;
	mp->render->device.Create( mfp->hWnd );
	mp->scene = new SGScene;
	mp->scene->SetTimeStep(mp->dt);
	mp->timerID = SetTimer( mfp->hWnd, 1, 10, NULL );	// 10ms = 0.01sec���̃^�C�}

	return 0;
}

LRESULT OnPaint( MesParams *mp, MesFuncParams *mfp )
{
	// �����_���������ł��Ă��Ȃ������牽�����Ȃ�
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
	// DefWindowProc���Ă΂Ȃ��ƁA�i�v��WM_PAINT���Ă΂��B�Ȃ�ł��낤�H
}

LRESULT OnKeyDown( MesParams *mp, MesFuncParams *mfp )
{
	int	ret = 0;

	switch( mfp->wParam )
	{
	case VK_ESCAPE :	// ESC�L�[�̏ꍇ
		PostMessage( mfp->hWnd, WM_CLOSE, 0, 0 ); // �E�C���h�E�����
		break;
	case 'L' :		// L�L�[�̏ꍇ (���[�h)
		// ret = Load( "Block5.x", mp, mfp );
		ret = FileDlg( mp, mfp );
		break;
	case 'R' :		// �V�[���t�@�C���̃����[�h
		if( mp->szFileName[0] ){	// ���łɃt�@�C�����[�h�����Ă��邱��
			ret = Load( mp->szFileName, mp, mfp );
		}
		break;
	case 'K' :		// �^�C�}�[��~
		KillTimer( mfp->hWnd, mp->timerID );
		break;
	case VK_SPACE :		// �X�e�b�v���s
		ret = Step( mp, mfp );
		break;
	case 'T' :		// �^�C�}�[�ĊJ
		mp->timerID = SetTimer( mfp->hWnd, 1, 10, NULL );	// 10ms = 0.01sec���̃^�C�}
		break;

		// �J�����̈ړ�
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

	case 'C' :		// �J�����s����f�o�b�O�E�C���h�E�֕\��
		DSTR << "Camera :" << std::endl;
		DSTR << mp->render->camera->data.view << std::endl;
		break;
	case 'D' :		// �h���b�v�Ή���ON/OFF
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
	return Step( mp, mfp );	// ���E���w�莞�Ԃ����i�߂�
}

LRESULT	OnDrop( MesParams *mp, MesFuncParams *mfp )
{
	HDROP	hDrop;
	int		nFiles;

	hDrop = ( HDROP ) mfp->wParam;
	nFiles = DragQueryFile( hDrop, (UINT) -1, NULL, 0 );
	if( nFiles == 1 ){		// ���Ƃ����t�@�C����1�̂Ƃ�
		DragQueryFile( hDrop, 0, mp->szFileName, MAX_PATH );	// �t�@�C�����̃��[�h
		DragFinish( hDrop );	// �h���b�v�̈�̊J��

		return Load( mp->szFileName, mp, mfp );	// �I�����ꂽ�t�@�C���̃��[�h
	}
	// �������Ƃ����Ƃ��́A�������Ȃ�
	DragFinish( hDrop );	// �h���b�v�̈�̊J��
	return 0;
}

LRESULT FileDlg( MesParams *mp, MesFuncParams *mfp )
{
	char szCurrentDir[ MAX_PATH ];
	bool bIniDirSet = false;
	OPENFILENAME ofn;
	BOOL bRet;

	if( mp->szFileName[0] == 0 ){	// �܂��t�@�C�������ݒ肳��Ă��Ȃ����
		// �����f�B���N�g���̐ݒ� (�J�����g)
		GetCurrentDirectory( MAX_PATH, szCurrentDir );
		bIniDirSet = true;	// �����f�B���N�g�����Z�b�g����
	}

	memset( &ofn, 0, sizeof( OPENFILENAME ) );
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.hwndOwner = mfp->hWnd;
	ofn.lpstrFilter = NULL;
	ofn.lpstrFile = mp->szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = ( bIniDirSet )? szCurrentDir : NULL;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	bRet = GetOpenFileName( &ofn );	// �t�@�C�����J���R�����_�C�A���O

	if( bRet == 0 ) return 0;	// �L�����Z���I�����́A���������ɖ߂�

	return Load( mp->szFileName, mp, mfp );	// �I�����ꂽ�t�@�C���̃��[�h
}

LRESULT Load( const char *filename, MesParams *mp, MesFuncParams *mfp )
{
	// �V�[���O���t�������ł��ĂȂ���Ή������Ȃ�
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
	float df = 1.0f ; //�ړ��ω���

	afCam = mp->render->camera->data.view.inv();	// �J�����s����擾

	switch( mfp->wParam ){
	case 'Q' :		// X������
		afCam.Pos() = afCam.Pos() - afCam.Ex() * df;
		break;
	case 'W' :		// X�����E
		afCam.Pos() = afCam.Pos() + afCam.Ex() * df;
		break;
	case 'Z' :		// Y������
		afCam.Pos() = afCam.Pos() - afCam.Ey() * df;
		break;
	case 'A' :		// Y������
		afCam.Pos() = afCam.Pos() + afCam.Ey() * df;
		break;
	case VK_DOWN :	// Z����(���s)��O
		afCam.Pos() = afCam.Pos() - afCam.Ez() * df;
		break;
	case VK_UP :	// Z����(���s)��
		afCam.Pos() = afCam.Pos() + afCam.Ez() * df;
		break;
	case VK_LEFT :	// ��������]
		afCam = afCam * Affinef::Rot(Radf(-5),'y');
		break;
	case VK_RIGHT :	// �E������]
		afCam = afCam * Affinef::Rot(Radf(5),'y');
		break;
	}

	mp->render->camera->data.view = afCam.inv();	// �ύX��̃J�����s���K�p

	InvalidateRect( mfp->hWnd, NULL, TRUE );		// �ĕ`��v��

	return 0;
}
