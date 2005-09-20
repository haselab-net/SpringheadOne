// SceneFileList.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dynahaptic.h"
#include "SceneFileList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneFileList ダイアログ


CSceneFileList::CSceneFileList(CWnd* pParent /*=NULL*/)
	: CDialog(CSceneFileList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSceneFileList)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CSceneFileList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSceneFileList)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSceneFileList, CDialog)
	//{{AFX_MSG_MAP(CSceneFileList)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneFileList メッセージ ハンドラ
