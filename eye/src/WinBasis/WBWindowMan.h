#ifndef WBWINDOWMAN_H
#define WBWINDOWMAN_H
#include "WBWin.h"

namespace Spr{;

///	�E�B���h�E�̑傫�����Ǘ�����
class WBWindowMan{
	protected:
	HWND hWnd;
	WINDOWPLACEMENT* pWndPlaceOld;
	WINDOWPLACEMENT* pMainWndPlaceOld;

	public:
	WBWindowMan();
	~WBWindowMan();
	///	�E�C���h�E�̈ʒu�� pt �傫���� sz �ɂ���
	virtual bool FitWindow(POINT pt, SIZE sz);
	///	�E�B���h�E�̈ʒu�Ƒ傫�����L������
	virtual void SaveWindowPlace();
	///	�Ō�ɋL�^�����ʒu�Ƒ傫���ɖ߂�
	virtual bool RestoreWindowPlace();
	///	�����Ă���E�B���h�E�̃n���h��
	HWND HWnd(){return hWnd;}
	///	�����E�C���h�E�̐ݒ�
	virtual void HWnd(HWND wnd){hWnd = wnd;}
	///	pt, sz ���X�N���[�����W�n�ɕϊ�
	void ClientToScreen(POINT* pt, SIZE* sz);
	///	�E�B���h�E�̃N���C�A���g�̈�̑傫��
	SIZE Size();
};

}	//	namespace Spr

#endif
