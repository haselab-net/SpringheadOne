#pragma once


// CConsoleDlg ダイアログ

class CConsoleDlg : public CDialog
{
	DECLARE_DYNAMIC(CConsoleDlg)

	float	fYawLF, fPitchLF, fKneeLF;
	float	fYawLM, fPitchLM, fKneeLM;
	float	fYawLB, fPitchLB, fKneeLB;
	float	fYawRF, fPitchRF, fKneeRF;
	float	fYawRM, fPitchRM, fKneeRM;
	float	fYawRB, fPitchRB, fKneeRB;
	float	fYawNeck, fPitchNeck;
	float	fTail;
	float	fYawP, fPitchP, fKneeP, fNeckP, fTailP;
	float	fYawI, fPitchI, fKneeI, fNeckI, fTailI;
	float	fYawD, fPitchD, fKneeD, fNeckD, fTailD;
	float	fYawF, fPitchF, fRollF;
	float	fYawM, fPitchM, fRollM;
	float	fYawB, fPitchB, fRollB;
public:
	CConsoleDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CConsoleDlg();

// ダイアログ データ
	enum { IDD = IDD_CONSOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
