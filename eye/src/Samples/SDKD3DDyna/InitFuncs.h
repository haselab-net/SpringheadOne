#ifndef	_INITFUNCS_H_
#define	_INITFUNCS_H_

#include <windows.h>
//#include <tchar.h>

typedef struct tagWindowParams {
	// �E�C���h�E�V�X�e���n
	HWND		hWnd;				// �E�C���h�E �n���h��
	HINSTANCE	hInst;				// �C���X�^���X
	LPTSTR		lpszCmdParam;		// �R�}���h���C��
	int			nCmdShow;			// �\�����[�h
	char		szWinName[128];		// �E�C���h�E�N���X��
	char		szWinTitle[128];	// �E�C���h�E�̃^�C�g��

} WindowParams;

// �A�v���P�[�V�����������n
void	InitParams( WindowParams *wp,
				   HINSTANCE hInst,
				   LPTSTR lpszCmdParam,
				   int nCmdShow);	// �\���̂̃����o�̏�����
BOOL	RegWinClass( WindowParams *wp );	// �E�C���h�E�N���X�̓o�^
BOOL	WindowSetup( WindowParams *wp );	// �E�C���h�E�̍쐬��
int		MesLoop();					// ���b�Z�[�W���[�v

#ifndef _MESFUNCS_H_
LRESULT CALLBACK
MainWndProc( HWND hwnd, UINT uMsg,
			WPARAM wParam, LPARAM lParam); // ���C���̃E�C���h�E�֐�
#endif

#endif // _INITFUNCS_H_
