// BalanceDoc.cpp : CBalanceDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Balance.h"

#include "BalanceDoc.h"
#include "BalanceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBalanceDoc

IMPLEMENT_DYNCREATE(CBalanceDoc, CDocument)

BEGIN_MESSAGE_MAP(CBalanceDoc, CDocument)
	//{{AFX_MSG_MAP(CBalanceDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBalanceDoc クラスの構築/消滅

CBalanceDoc::CBalanceDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CBalanceDoc::~CBalanceDoc()
{
}

BOOL CBalanceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBalanceDoc シリアライゼーション

void CBalanceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください。
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBalanceDoc クラスの診断

#ifdef _DEBUG
void CBalanceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBalanceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBalanceDoc コマンド

BOOL CBalanceDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CBalanceView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CBalanceDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CBalanceView*)pView)->Save(lpszPathName);
	}   
	return true;
}
