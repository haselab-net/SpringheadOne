// EtcDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SampleProgram.h"
#include "EtcDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EtcDialog ダイアログ


EtcDialog::EtcDialog(CWnd* pParent /*=NULL*/)
	: CDialog(EtcDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(EtcDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void EtcDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EtcDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EtcDialog, CDialog)
	//{{AFX_MSG_MAP(EtcDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EtcDialog メッセージ ハンドラ
