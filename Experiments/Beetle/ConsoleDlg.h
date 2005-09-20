#pragma once


// CConsoleDlg �_�C�A���O

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
	CConsoleDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CConsoleDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CONSOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
