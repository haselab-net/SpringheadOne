// vcsampleDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_VCSAMPLEDLG_H__09A46B07_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
#define AFX_VCSAMPLEDLG_H__09A46B07_B6F5_11D4_935E_006008B03B0A__INCLUDED_

#include "TrgSet.h"	// ClassView �ɂ���Ēǉ�����܂����B
#include "DataIn.h"	// ClassView �ɂ���Ēǉ�����܂����B
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVcsampleDlg �_�C�A���O

class CVcsampleDlg : public CDialog
{
// �\�z
public:
	int SampleDatNum;
	CVcsampleDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CVcsampleDlg)
	enum { IDD = IDD_VCSAMPLE_DIALOG };
	CButton	m_DevOpen;
	CButton	m_DevClose;
	CButton	m_GetStatus;
	CButton	m_GetDatas;
	CButton	m_AnalogTrg;
	CButton	m_DigitalTrg;
	CButton	m_SingleSample;
	CButton	m_DacOut2;
	CButton	m_DacOut1;
	CButton	m_ClockStop;
	CButton	m_ClockOut;
	CButton	m_WordWrite;
	CButton	m_WordRead;
	CButton	m_LowWrite;
	CButton	m_HighWrite;
	CButton	m_HighRead;
	CButton	m_LowRead;
	CButton	m_HighDir;
	CButton	m_LowDir;
	CComboBox	m_IdNum;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CVcsampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CVcsampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLowdir();
	afx_msg void OnDevopen();
	afx_msg void OnDevclose();
	afx_msg void OnHighdir();
	afx_msg void OnClose();
	afx_msg void OnLowwrite();
	afx_msg void OnHighwrite();
	afx_msg void OnLowread();
	afx_msg void OnHighread();
	afx_msg void OnWordwrite();
	afx_msg void OnWordread();
	afx_msg void OnClockout();
	afx_msg void OnClockstop();
	afx_msg void OnDacout1();
	afx_msg void OnDacout2();
	afx_msg void OnSinglesample();
	afx_msg void OnDigitaltrg();
	afx_msg void OnAnalogtrg();
	afx_msg void OnGetstatus();
	afx_msg void OnGetdatas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Button_Disable();
	void Button_Enable();
	CTrgSet TRGSET;
	CDataIn DATAIN;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_VCSAMPLEDLG_H__09A46B07_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
