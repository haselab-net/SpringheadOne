// vcsampleDlg.h : ヘッダー ファイル
//

#if !defined(AFX_VCSAMPLEDLG_H__09A46B07_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
#define AFX_VCSAMPLEDLG_H__09A46B07_B6F5_11D4_935E_006008B03B0A__INCLUDED_

#include "TrgSet.h"	// ClassView によって追加されました。
#include "DataIn.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVcsampleDlg ダイアログ

class CVcsampleDlg : public CDialog
{
// 構築
public:
	int SampleDatNum;
	CVcsampleDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
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

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CVcsampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
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
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VCSAMPLEDLG_H__09A46B07_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
