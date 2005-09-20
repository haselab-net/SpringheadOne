// HisConfig.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "AccelControl.h"
#include "HisConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHisConfig ダイアログ


CHisConfig::CHisConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CHisConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHisConfig)
	hisList = _T("");
	forceScale = 0.0f;
	posScale = 0.0f;
	timeStep = 0.0f;
	//}}AFX_DATA_INIT
}


void CHisConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHisConfig)
	DDX_Text(pDX, IDC_HIS_LIST, hisList);
	DDX_Text(pDX, IDC_ForceScale, forceScale);
	DDX_Text(pDX, IDC_PosScale, posScale);
	DDX_Text(pDX, IDC_TimeStep, timeStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHisConfig, CDialog)
	//{{AFX_MSG_MAP(CHisConfig)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHisConfig メッセージ ハンドラ
