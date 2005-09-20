// QDPRBalanceDoc.cpp : CQDPRBalanceDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QDPRBalance.h"

#include "QDPRBalanceDoc.h"
#include "QDPRBalanceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceDoc

IMPLEMENT_DYNCREATE(CQDPRBalanceDoc, CDocument)

BEGIN_MESSAGE_MAP(CQDPRBalanceDoc, CDocument)
	//{{AFX_MSG_MAP(CQDPRBalanceDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceDoc クラスの構築/消滅

CQDPRBalanceDoc::CQDPRBalanceDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CQDPRBalanceDoc::~CQDPRBalanceDoc()
{
}

BOOL CQDPRBalanceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceDoc シリアライゼーション

void CQDPRBalanceDoc::Serialize(CArchive& ar)
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
// CQDPRBalanceDoc クラスの診断

#ifdef _DEBUG
void CQDPRBalanceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CQDPRBalanceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CQDPRBalanceDoc コマンド

BOOL CQDPRBalanceDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CQDPRBalanceView*)pView)->app.Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CQDPRBalanceDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CQDPRBalanceView*)pView)->app.Save(lpszPathName);
	}   
	return true;
}
