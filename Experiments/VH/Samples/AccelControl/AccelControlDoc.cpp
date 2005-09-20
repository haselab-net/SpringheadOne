// AccelControlDoc.cpp : CAccelControlDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "AccelControl.h"

#include "AccelControlDoc.h"
#include "AccelControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccelControlDoc

IMPLEMENT_DYNCREATE(CAccelControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CAccelControlDoc, CDocument)
	//{{AFX_MSG_MAP(CAccelControlDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccelControlDoc クラスの構築/消滅

CAccelControlDoc::CAccelControlDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CAccelControlDoc::~CAccelControlDoc()
{
}

BOOL CAccelControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAccelControlDoc シリアライゼーション

void CAccelControlDoc::Serialize(CArchive& ar)
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
// CAccelControlDoc クラスの診断

#ifdef _DEBUG
void CAccelControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAccelControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAccelControlDoc コマンド

BOOL CAccelControlDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CAccelControlView*)pView)->app->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CAccelControlDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CAccelControlView*)pView)->app->Save(lpszPathName);
	}   
	return true;
}
