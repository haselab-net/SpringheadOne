#if !defined(AFX_VHCHECKDIALOG_H__1489F613_9D1C_4F11_9BA4_5F2F4E8EB94E__INCLUDED_)
#define AFX_VHCHECKDIALOG_H__1489F613_9D1C_4F11_9BA4_5F2F4E8EB94E__INCLUDED_

//Springhead
#include "VHInteraction.h"
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <Graphics/GRMaterial.h>
#include <Creature/CRHuman.h>
#include <Base/Affine.h>
#include <vector>

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
	void Draw(Spr::GRRender* render, Spr::CRHuman* crHuman);
	void DrawTest(Spr::GRRender* render, Spr::SGScene* scene);
	bool IsDraw(){return bDraw;}

// �_�C�A���O �f�[�^
	//{{AFX_DATA(VHCheckDialog)
	enum { IDD = IDD_VH_CHECK };
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

	bool bDraw;			//�`�悷�邩�ۂ�
	bool bDrawZmp;		//ZMP��`�悷�邩�ۂ�
	bool bDrawSuArea;	//����̈��`�悷�邩�ۂ�

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(VHCheckDialog)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCheckZmp();
	afx_msg void OnCheckSupportArea();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern VHCheckDialog* vhCheckDialog;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_VHCHECKDIALOG_H__1489F613_9D1C_4F11_9BA4_5F2F4E8EB94E__INCLUDED_)
