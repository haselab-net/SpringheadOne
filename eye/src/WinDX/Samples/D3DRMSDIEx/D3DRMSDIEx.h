// D3DRMSDIEx.h : SDIDDRAW �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_SDIDDRAW_H__D98D6024_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
#define AFX_SDIDDRAW_H__D98D6024_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� D3DRMSDIEx.cpp �t�@�C�����Q�Ƃ��Ă��������B
//
class WXD3RMSDIExView;

class WXD3RMSDIExApp : public CWinApp
{
public:
	WXD3RMSDIExApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(WXD3RMSDIExApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	WXD3RMSDIExView* view;

	//{{AFX_MSG(WXD3RMSDIExApp)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SDIDDRAW_H__D98D6024_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
