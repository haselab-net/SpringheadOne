// VirtualHuman.h : VirtualHuman �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_VirtualHuman_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
#define AFX_VirtualHuman_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/** @page VirtualHuman �����V�~�����[�^�Ƀ}�E�X�ł����D
	@section VirtualHumanAbst �T�v
	�}�E�X�ƕ����V�~�����[�^�̃T���v���D
	�}�E�X��p���ĉ��z���̂𓮂������Ƃ��ł���D
	
	@section VirtualHumanHowToBuild �r���h�̕��@
	- ���ϐ� Springhead �� Springhead���C���X�g�[�������f�B���N�g�����w��
	  ���ĉ������DWindows2000�Ȃ��[�}�C�R���s���[�^]-[�v���p�e�B]-[�ڍ�]-[���ϐ�]-
	  [�V�K]�̃_�C�A���O��[�ϐ���] Springhead [�ϐ��l] C:\\home\\Project\\Springhead
	  �Ƃ��Ă��������D
	- DirectX8.1��SDK���C���X�g�[�����Ă��������D
	- <A href=../Samples/VirtualHuman/VirtualHuman.dsw>Samples/VirtualHuman/VirtualHuman.dsw</A>��
	  �J���ăr���h���Ď��s���Ă��������D<br>
		�r���h���������Ȃ��l�͈ȉ��̂��Ƃ��m�F���Ă��������D
					- DirectX SDK�̃C���N���[�h�t�@�C���ƃ��C�u�����t�@�C���̃f�B���N�g���̐ݒ�<br>
		Visual C++�Ȃ�΁C[�c�[��]-[�I�v�V����]-[�f�B���N�g��]�́u�\������f�B���N�g		��(S)�v���u�C���N���[�h�t�@�C���v�ɕς��CDirectX SDK�̃C���N���[�h�t�@�C����		�f�B���N�g���i��:"C:\DXSDK\INCLUDE"�j���ݒ肳��Ă��邩�m�F���Ă��������D
		�ݒ肳��Ă��Ȃ���ΐݒ肵�Ă��������D
		�܂��C�����̗D�揇�ʂ��グ�邽�߂Ɂu���v�̃{�^�������x���N���b�N���āC
		��ԏ�ɗ���悤�ɂ��Ă��������D<br>
		���C�u�����t�@�C���ɂ��Ă��C���N���[�h�t�@�C���Ɠ��l�Ƀp�X�̐ݒ���s���Ă�		�������i�u�\������f�B���N�g��(S)�v���u���C�u�����t�@�C���v�ɕύX����j�D
	
	- ���[�h����f�[�^�t�@�C��(@ref XFile "Direct3D X�t�@�C��")��<A href=../../Samples/data/>Samples/data/</A>�ɂ���܂��D

	
	@section VirtualHumanHowToOperate ������@
	�L�[�{�[�h����

	- ���F�O�i
	- ���F���
	- ���F����]
	- ���F�E��]
	- S�F���]
	- X�F����]
	- Q�F�����s�ړ�
	- W�F�E���s�ړ�
	- A�F�㕽�s�ړ�
	- Z�F�����s�ړ�
	- R�F���_���Z�b�g
	- F�F�t���X�N���[��
	- G�F��ʂ��t���X�N���[�����猳�ɖ߂�
	- �X�y�[�X�F�|�C���^�ɗ͂�Ԃ����Ԃ��Ȃ�����ON-OFF
	- T�F�}���`���f�B�A�^�C�}�[�̋N���i���x�����߂�Ȃ�K�v�DRelease�ł̂݁j


	�}�E�X����

	- ���h���b�O�F�|�C���^���J�������W�n��x���Cy������ɉ�]
	- ���h���b�O�F�|�C���^���J�������W�n��x-z���ʂŕ��s�ړ�
	- �E�h���b�O�F�|�C���^���J�������W�n��x-y���ʂŕ��s�ړ�
	- ���E�h���b�O�F�|�C���^���J�������W�n��x-z���ʂŕ��s�ړ�(���{�^��or�z�C�[�����Ȃ��}�E�X�p)
	- �E���h���b�O�F�|�C���^���J�������W�n��y-z���ʂŕ��s�ړ�
	- �z�C�[��:�|�C���^���J�������W�n��z�������ɕ��s�ړ�
	- ���_�u���N���b�N�F�|�C���^�̎p����������
	- �E�_�u���N���b�N�F�|�C���^�̈ʒu��������
	- Shift + ���h���b�O�F�J������x���Cy������ɉ�]
	- Shift + ���h���b�O�F�J������x-z���ʂŕ��s�ړ�
	- Shift + �E�h���b�O�F�J������x-y���ʂŕ��s�ړ�
	- Shift + ���E�h���b�O�F�J������x-z���ʂŕ��s�ړ�(���{�^��or�z�C�[�����Ȃ��}�E�X�p)
	- Shift + �E���h���b�O�F�J������y-z���ʂŕ��s�ړ�
	- Shift + �z�C�[��:�J������z�������ɕ��s�ړ�
	- Shift + ���_�u���N���b�N�F�J�����̈ʒu�E�p����������	

	
	@section VirtualHumanPoint ����
	- CVirtualHumanView�N���X<br>
		�����o�ϐ��ɁCrender(�����_��)�CafBody*afHead(���_�s��), 
		world(�V�[���O���t�̃g�b�v�t���[��)�������Ă���D
	- CVirtualHumanView::OnInitialUpdate()<br>
		������
	- CVirtualHumanView::Load()<br>
		X�t�@�C���̃��[�h
	- CVirtualHumanView::OnDraw()<br>
		�����_�����O����
	- CVirtualHumanView::OnKeyDown()<br>
		�L�[���͂ɂ�鎋�_�ړ�����
	- CVirtualHumanDoc::OnOpenDocument()<br>
		�t�@�C���̃I�[�v��(�P��Load���Ăяo������)
	���̕�����AppWizerd��������܂܂ɂȂ��Ă��܂��D
*/

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� VirtualHuman.cpp �t�@�C�����Q�Ƃ��Ă��������B
//
class CVirtualHumanApp : public CWinApp
{
public:
	CVirtualHumanApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CVirtualHumanApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CVirtualHumanApp)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_VirtualHuman_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
