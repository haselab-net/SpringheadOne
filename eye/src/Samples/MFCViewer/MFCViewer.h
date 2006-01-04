// MFCViewer.h : MFCViewer �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_MFCD3DVIEWER_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
#define AFX_MFCViewer_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/** @page MFCViewer �����V�~�����[�^(VisualC++ MFC SDI��)
	@section MFCViewerAbst �T�v
	Springhead���C�u������p���č쐬�����C�����V�~�����[�^�ł��D
	�t���[���K�w������DirectX�t�@�C�������[�h���āC�\�����C
	�����V�~�����[�V�������s���܂��D
	���_�𓮂������Ƃ��ł��܂��D
	
	@section MFCViewerHowToBuild �r���h�̕��@
	- ���ϐ� Springhead �� Springhead���C���X�g�[�������f�B���N�g�����w��
	  ���ĉ������DWindows2000�Ȃ��[�}�C�R���s���[�^]-[�v���p�e�B]-[�ڍ�]-[���ϐ�]-
	  [�V�K]�̃_�C�A���O��[�ϐ���] Springhead [�ϐ��l] C:\\home\\Project\\Springhead
	  �Ƃ��Ă��������D
	- DirectX8.1��SDK���C���X�g�[�����Ă��������D
	- <A href=../Samples/MFCViewer/MFCViewer.dsw>Samples/MFCViewer/MFCViewer.dsw</A>��
	  �J���ăr���h���Ď��s���Ă��������D
	
	@section HowToOperate ������@
	������@

	- ���F�O�i
	- ���F���
	- ���F����]
	- ���F�E��]
	- �`�F�㏸
	- �y�F���~
	- �p�F�����s�ړ�
	- �v�F�E���s�ړ�
	- �r�F�������
	- �w�F��������
	- �q�F���_���Z�b�g
	�����c�� 2002/06/26	

	@section MFCViewerPoint ����
	- CMFCViewerView�N���X<br>
		�����o�ϐ��ɁCrender(�����_��)�CafBody*afHead(���_�s��), 
		world(�V�[���O���t�̃g�b�v�t���[��)�������Ă���D
	- CMFCViewerView::OnInitialUpdate()<br>
		������
	- CMFCViewerView::Load()<br>
		X�t�@�C���̃��[�h
	- CMFCViewerView::OnDraw()<br>
		�����_�����O����
	- CMFCViewerView::OnKeyDown()<br>
		�L�[���͂ɂ�鎋�_�ړ�����
	- CMFCViewerDoc::OnOpenDocument()<br>
		�t�@�C���̃I�[�v��(�P��Load���Ăяo������)
	���̕�����AppWizerd��������܂܂ɂȂ��Ă��܂��D
*/

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� MFCViewer.cpp �t�@�C�����Q�Ƃ��Ă��������B
//
class CMFCViewerApp : public CWinApp
{
public:
	CMFCViewerApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMFCViewerApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CMFCViewerApp)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MFCViewer_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
