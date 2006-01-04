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


// Window�֐��ŋ��ʂ̃p�����[�^
typedef struct tagMesParams {
	char szFileName[ MAX_PATH ];		// ���ݓǂݍ���ł���V�[���̃p�X���t�@�C����
	Spr::UTRef<Spr::D3Render> render;	// �����_���[
	Spr::UTRef<Spr::SGScene> scene;		// �V�[���O���t
	Spr::Affinef afBody, afHead;
	UINT timerID;
	float dt;				// �X�e�b�v�X�V�Ԋu
//	Spr::UTRef<Spr::SGCamera> camera;	// ���݂̃J����
	Spr::PHSolid* solid;
	BOOL bDropOn;			// �t�@�C���h���b�v���󂯕t���邩
} MesParams;

// Window�֐��ŌĂяo�����ƂɎg���p�����[�^
typedef struct tagMesFuncParams {
	HWND	hWnd;
	UINT	uMsg;
	WPARAM	wParam;
	LPARAM	lParam;
} MesFuncParams;

LRESULT CALLBACK
MainWndProc( HWND hWnd, UINT uMsg,
			WPARAM wParam, LPARAM lParam); // ���C���̃E�C���h�E�֐�
LRESULT	OnCreate( MesParams *mp, MesFuncParams *mfp );	// �������֐�
LRESULT	OnPaint( MesParams *mp, MesFuncParams *mfp );	// ��ʕ`��
LRESULT	OnKeyDown( MesParams *mp, MesFuncParams *mfp );	// �L�[���������Ƃ�
LRESULT	OnDestroy( MesParams *mp, MesFuncParams *mfp );	// �I�����̊֐�
LRESULT	OnTimer( MesParams *mp, MesFuncParams *mfp );	// �^�C�}�[�֐�
LRESULT	OnDrop( MesParams *mp, MesFuncParams *mfp );	// �t�@�C���h���b�v�֐�

LRESULT FileDlg( MesParams *mp, MesFuncParams *mfp );	// �V�[���t�@�C���̃I�[�v���_�C�A���O
LRESULT	Load( const char *filename, MesParams *mp, MesFuncParams *mfp );	// �V�[���̃��[�h

LRESULT	Step( MesParams *mp, MesFuncParams *mfp );	// ���E���w�莞�Ԃ����i�߂�
LRESULT CameraMove( MesParams *mp, MesFuncParams *mfp );	// �L�[�{�[�h�ɂ��J��������

#endif // _MESFUNCS_H_
