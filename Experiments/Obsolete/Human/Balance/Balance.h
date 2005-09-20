// Balance.h : Balance �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_MFCD3DDYNAMOUSE_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
#define AFX_MFCD3DDYNAMOUSE_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/** @page Balance �����V�~�����[�^�Ƀ}�E�X�ł����D
	@section BalanceAbst �T�v
	�}�E�X�ƕ����V�~�����[�^�̃T���v���D
	�}�E�X�Ń|�C���^�ƂȂ镨�̂𓮂������Ƃ��ł���D
	
	@section BalanceHowToBuild �r���h�̕��@
	- ���ϐ� Springhead �� Springhead���C���X�g�[�������f�B���N�g�����w��
	  ���ĉ������DWindows2000�Ȃ��[�}�C�R���s���[�^]-[�v���p�e�B]-[�ڍ�]-[���ϐ�]-
	  [�V�K]�̃_�C�A���O��[�ϐ���] Springhead [�ϐ��l] C:\\home\\Project\\Springhead
	  �Ƃ��Ă��������D
	- DirectX8.1��SDK���C���X�g�[�����Ă��������D
	- <A href=../Samples/Balance/Balance.dsw>Samples/Balance/Balance.dsw</A>��
	  �J���ăr���h���Ď��s���Ă��������D<br>
		�r���h���������Ȃ��l�͈ȉ��̂��Ƃ��m�F���Ă��������D
					- DirectX SDK�̃C���N���[�h�t�@�C���ƃ��C�u�����t�@�C���̃f�B���N�g���̐ݒ�<br>
		Visual C++�Ȃ�΁C[�c�[��]-[�I�v�V����]-[�f�B���N�g��]�́u�\������f�B���N�g		��(S)�v���u�C���N���[�h�t�@�C���v�ɕς��CDirectX SDK�̃C���N���[�h�t�@�C����		�f�B���N�g���i��:"C:\DXSDK\INCLUDE"�j���ݒ肳��Ă��邩�m�F���Ă��������D
		�ݒ肳��Ă��Ȃ���ΐݒ肵�Ă��������D
		�܂��C�����̗D�揇�ʂ��グ�邽�߂Ɂu���v�̃{�^�������x���N���b�N���āC
		��ԏ�ɗ���悤�ɂ��Ă��������D<br>
		���C�u�����t�@�C���ɂ��Ă��C���N���[�h�t�@�C���Ɠ��l�Ƀp�X�̐ݒ���s���Ă�		�������i�u�\������f�B���N�g��(S)�v���u���C�u�����t�@�C���v�ɕύX����j�D
	
	- ���[�h����f�[�^�t�@�C��(@ref XFile "Direct3D X�t�@�C��")��<A href=../../Samples/data/>Samples/data/</A>�ɂ���܂��D

	
	@section BalanceHowToOperate ������@
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

	@section BalancePoint ����
	- CBalanceView�N���X<br>
		�����o�ϐ��ɁCrender(�����_��)�CafBody*afHead(���_�s��), 
		world(�V�[���O���t�̃g�b�v�t���[��)�������Ă���D
	- CBalanceView::OnInitialUpdate()<br>
		������
	- CBalanceView::Load()<br>
		X�t�@�C���̃��[�h
	- CBalanceView::OnDraw()<br>
		�����_�����O����
	- CBalanceView::OnKeyDown()<br>
		�L�[���͂ɂ�鎋�_�ړ�����
	- CBalanceDoc::OnOpenDocument()<br>
		�t�@�C���̃I�[�v��(�P��Load���Ăяo������)
	���̕�����AppWizerd��������܂܂ɂȂ��Ă��܂��D
*/

/////////////////////////////////////////////////////////////////////////////
// CBalanceApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� Balance.cpp �t�@�C�����Q�Ƃ��Ă��������B
//
class CBalanceApp : public CWinApp
{
public:
	CBalanceApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CBalanceApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CBalanceApp)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MFCD3DDYNAMOUSE_H__76565EAB_E58D_4F61_A6A0_E8BB59F3208B__INCLUDED_)
