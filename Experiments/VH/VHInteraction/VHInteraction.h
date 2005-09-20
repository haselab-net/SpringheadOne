// VHInteraction.h : VHInteraction �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��

#if !defined(AFX_MFCD3DVIEWER_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
#define AFX_VHInteraction_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/** @page VHInteraction �����V�~�����[�^�ɗ͊o�C���^�t�F�[�X�ł����D
	@section VHInteractionAbst �T�v
	HISpidarG6 �� �����V�~�����[�^ �̃T���v���D
	�t���[���K�w������DirectX�t�@�C�������[�h���āCSPIDAR�ŐG���D
	
	@section VHInteractionHowToBuild �r���h�̕��@
	- ���ϐ� Springhead �� Springhead���C���X�g�[�������f�B���N�g�����w��
	  ���ĉ������DWindows2000�Ȃ��[�}�C�R���s���[�^]-[�v���p�e�B]-[�ڍ�]-[���ϐ�]-
	  [�V�K]�̃_�C�A���O��[�ϐ���] Springhead [�ϐ��l] C:\\home\\Project\\Springhead
	  �Ƃ��Ă��������D
	- DirectX8.1�܂���9��SDK���C���X�g�[�����Ă��������D
	- <A href=../Samples/VHInteraction/VHInteraction.dsw>Samples/VHInteraction/VHInteraction.dsw</A>��
	  �J���ăr���h���Ď��s���Ă��������D
	
	@section VHInteractionHowToOperate ������@
	�T���v���̑�����@
	- [UP]:		�O�i
	- [DOWN]:	���
	- [LEFT]:	����]
	- [RIGHT]:	�E��]
	- A:	�㏸
	- Z:	���~
	- Q:	�����s�ړ�
	- W:	�E���s�ړ�
	- S:	�������
	- X:	��������
	- R:	�V�[���̃����[�h�i���Z�b�g�j
	- D:	���͂̒�(multi media timer �ƕ��p�s��)
	- M:	Multi media timer ���g�p
	- T:	�ʏ�̃^�C�}�[(WM_TIMER)���g�p
	- C:	�L�����u���[�V����
	@section �q���[�}���C���^�t�F�[�X�̐ݒ�ɂ���
	- �ݒ���e�̓��W�X�g���ɕۑ�����܂��D
	- Position Scale:	SPIDAR�̓�����scale�{�g�債�āC�o�[�`�������E�ɔ��f���܂��D
	- Force Scale:		�o�[�`�������E�ł̗͂�1/scale�ɏk�����āC�͊o�񎦂��܂��D
	- Time step:		�f�[�^�t�@�C���Ɏ��ԍ��݂��L����Ă��Ȃ��ꍇ�C���̎��ԍ��݂ŃV�~�����[�V�������܂��D
	- Human interface list:	�g�p����q���[�}���C���^�t�F�[�X���w�肵�܂��D
		�N���X���ɑΉ�����C���^�t�F�[�X������ׂ�ƁC
		���������C���^�t�F�[�X��͊o�|�C���^�ɏ��Ɋ��蓖�Ă܂��D
		
		�C���^�t�F�[�X����:
		<pre>
SpidarG6X
SpidarG6X

Spidar4
Spidar4 reverse

SpidarG6C
SpidarG6C

Mouse
		</pre>
		����Ȋ����Ŏw�肵�܂��D
	
	@section VHInteractionPoint ����
	- CVHInteractionView�N���X<br>
		CVHInteractionView::app ���t���[�����[�N�ł���C�قƂ�ǂ̏������s���Ă���D
	- CVHInteractionView::OnInitialUpdate()<br>
		������
	- CVHInteractionView::Load()<br>
		X�t�@�C���̃��[�h
	- CVHInteractionView::OnDraw()<br>
		�����_�����O����
	- CVHInteractionView::OnKeyDown()<br>
		�L�[���͂ɂ�鎋�_�ړ�����
	- CVHInteractionDoc::OnOpenDocument()<br>
		�t�@�C���̃I�[�v��(�P��Load���Ăяo������)
	- CVHInteractionView::OnD3d();
		D3D�Ń����_�����O
	- CVHInteractionView::OnGL();
		OpenGL�Ń����_�����O
	- CVHInteractionView::OnHisConfig();
		�q���[�}���C���^�t�F�[�X�̐ݒ�
	@include VHInteractionView.cpp
*/

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionApp:
//
class CVHInteractionApp : public CWinApp
{
public:
	CVHInteractionApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CVHInteractionApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CVHInteractionApp)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_VHInteraction_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
