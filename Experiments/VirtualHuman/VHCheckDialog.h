#if !defined(AFX_VHCHECKDIALOG_H__85BE0687_4C48_41B8_8C5D_9281C722CBCF__INCLUDED_)
#define AFX_VHCHECKDIALOG_H__85BE0687_4C48_41B8_8C5D_9281C722CBCF__INCLUDED_


//Springhead
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <Graphics/GRMaterial.h>
#include <Physics/PHHuman.h>
#include <Base/Affine.h>
#include <vector>

//VirtualHuman
#include "TBalance.h"
#include "CollisionArea.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VHCheckDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// VHCheckDialog �_�C�A���O

class VHCheckDialog : public CDialog
{
// �R���X�g���N�V����
public:
	VHCheckDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	
// �_�C�A���O �f�[�^
	//{{AFX_DATA(VHCheckDialog)
	enum { IDD = IDD_VIEW_ZMP };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(VHCheckDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(VHCheckDialog)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		
public:
//////////////////////////////////////////////////////
//////////  �ȉ���莩��  ////////////////////////////
//////////////////////////////////////////////////////

	void DrawDlg(Spr::GRRender* render, TBalance& balance, Spr::PHHuman* phHuman);		//�_�C�A���O�ɕ`��

	//�`�悷�����
	void DrawZmp(Spr::GRRender* render, TBalance& balance, Spr::PHHuman* phHuman);
	void DrawFootArea(Spr::GRRender* render, TBalance& balance);
	void DrawCollisionArea(CollisionArea& collisionArea);

	void SetNonDepth(Spr::Vec3f* vec){							//�����W���Ȃ����B
		vec->y = 0.0;
	}

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_VHCHECKDIALOG_H__85BE0687_4C48_41B8_8C5D_9281C722CBCF__INCLUDED_)
