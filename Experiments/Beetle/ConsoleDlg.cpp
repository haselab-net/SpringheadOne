// ConsoleDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Beetle.h"
#include "ConsoleDlg.h"

extern bool g_bShowingConsole;

// CConsoleDlg ダイアログ

IMPLEMENT_DYNAMIC(CConsoleDlg, CDialog)
CConsoleDlg::CConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConsoleDlg::IDD, pParent)
{
}

CConsoleDlg::~CConsoleDlg()
{
}

void CConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDE_YAWLF, fYawLF); DDX_Text(pDX, IDE_PITCHLF, fPitchLF); DDX_Text(pDX, IDE_KNEELF, fKneeLF);
	DDX_Text(pDX, IDE_YAWLM, fYawLM); DDX_Text(pDX, IDE_PITCHLM, fPitchLM); DDX_Text(pDX, IDE_KNEELM, fKneeLM);
	DDX_Text(pDX, IDE_YAWLB, fYawLB); DDX_Text(pDX, IDE_PITCHLB, fPitchLB); DDX_Text(pDX, IDE_KNEELB, fKneeLB);
	DDX_Text(pDX, IDE_YAWRF, fYawRF); DDX_Text(pDX, IDE_PITCHRF, fPitchRF); DDX_Text(pDX, IDE_KNEERF, fKneeRF);
	DDX_Text(pDX, IDE_YAWRM, fYawRM); DDX_Text(pDX, IDE_PITCHRM, fPitchRM); DDX_Text(pDX, IDE_KNEERM, fKneeRM);
	DDX_Text(pDX, IDE_YAWRB, fYawRB); DDX_Text(pDX, IDE_PITCHRB, fPitchRB); DDX_Text(pDX, IDE_KNEERB, fKneeRB);
	DDX_Text(pDX, IDE_YAWNECK, fYawNeck); DDX_Text(pDX, IDE_PITCHNECK, fPitchNeck);
	DDX_Text(pDX, IDE_TAIL, fTail);
	DDX_Text(pDX, IDE_YAWP, fYawP); DDX_Text(pDX, IDE_PITCHP, fPitchP); DDX_Text(pDX, IDE_KNEEP, fKneeP); DDX_Text(pDX, IDE_NECKP, fNeckP); DDX_Text(pDX, IDE_TAILP, fTailP);
	DDX_Text(pDX, IDE_YAWI, fYawI); DDX_Text(pDX, IDE_PITCHI, fPitchI); DDX_Text(pDX, IDE_KNEEI, fKneeI); DDX_Text(pDX, IDE_NECKI, fNeckI); DDX_Text(pDX, IDE_TAILI, fTailI);
	DDX_Text(pDX, IDE_YAWD, fYawD); DDX_Text(pDX, IDE_PITCHD, fPitchD); DDX_Text(pDX, IDE_KNEED, fKneeD); DDX_Text(pDX, IDE_NECKD, fNeckD); DDX_Text(pDX, IDE_TAILD, fTailD);
	DDX_Text(pDX, IDE_YAWF, fYawF); DDX_Text(pDX, IDE_PITCHF, fPitchF); DDX_Text(pDX, IDE_ROLLF, fRollF);
	DDX_Text(pDX, IDE_YAWM, fYawM); DDX_Text(pDX, IDE_PITCHM, fPitchM); DDX_Text(pDX, IDE_ROLLM, fRollM);
	DDX_Text(pDX, IDE_YAWB, fYawB); DDX_Text(pDX, IDE_PITCHB, fPitchB); DDX_Text(pDX, IDE_ROLLB, fRollB);
}


BEGIN_MESSAGE_MAP(CConsoleDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////
// CConsoleDlg メッセージ ハンドラ

void CConsoleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	g_bShowingConsole = false;
}

///////////////////////////////////////////////////////////////////////

