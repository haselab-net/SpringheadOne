// SLCSampleDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_SLCSAMPLEDLG_H__6E5C838D_8E85_462E_9B5E_4A37F26BAF25__INCLUDED_)
#define AFX_SLCSAMPLEDLG_H__6E5C838D_8E85_462E_9B5E_4A37F26BAF25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSLCSampleDlg �_�C�A���O

class CSLCSampleDlg : public CDialog
{
// �\�z
public:
	CSLCSampleDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSLCSampleDlg)
	enum { IDD = IDD_SLCSAMPLE_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSLCSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSLCSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnButtonCamcfg();
	afx_msg void OnButtonVidcfg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hSLC_device;	//��SLC API �n���h��
	BOOL OnSlcDataGet(WPARAM wParam,LPARAM lParam);	//��SLC API �C�x���g�n���h��
	BOOL OnSlcDataGetDeb(WPARAM wParam,LPARAM lParam);	//��SLC API �C�x���g�n���h��
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SLCSAMPLEDLG_H__6E5C838D_8E85_462E_9B5E_4A37F26BAF25__INCLUDED_)
