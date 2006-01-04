// MFCViewerDoc.cpp : CMFCViewerDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "MFCViewer.h"

#include "MFCViewerDoc.h"
#include "MFCViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerDoc

IMPLEMENT_DYNCREATE(CMFCViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CMFCViewerDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerDoc クラスの構築/消滅

CMFCViewerDoc::CMFCViewerDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CMFCViewerDoc::~CMFCViewerDoc()
{
}

BOOL CMFCViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMFCViewerDoc シリアライゼーション

void CMFCViewerDoc::Serialize(CArchive& ar)
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
// CMFCViewerDoc クラスの診断

#ifdef _DEBUG
void CMFCViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerDoc コマンド

BOOL CMFCViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CMFCViewerView*)pView)->app.Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CMFCViewerDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CMFCViewerView*)pView)->app.Save(lpszPathName);
	}   
	return true;
}
