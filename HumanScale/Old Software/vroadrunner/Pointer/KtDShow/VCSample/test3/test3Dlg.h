// test3Dlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_TEST3DLG_H__A529982D_BCB4_4313_A4AF_440313197711__INCLUDED_)
#define AFX_TEST3DLG_H__A529982D_BCB4_4313_A4AF_440313197711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../KtDShow.h"
/////////////////////////////////////////////////////////////////////////////
// CTest3Dlg �_�C�A���O

class CTest3Dlg : public CDialog
{
// �\�z
public:
	CTest3Dlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTest3Dlg)
	enum { IDD = IDD_TEST3_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTest3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTest3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonCapture();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonStart();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	KtDShowCapture* dshow;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TEST3DLG_H__A529982D_BCB4_4313_A4AF_440313197711__INCLUDED_)
