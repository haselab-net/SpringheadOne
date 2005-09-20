// VHIndependenceDoc.cpp : CVHIndependenceDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "VHIndependence.h"

#include "VHIndependenceDoc.h"
#include "VHIndependenceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVHIndependenceDoc

IMPLEMENT_DYNCREATE(CVHIndependenceDoc, CDocument)

BEGIN_MESSAGE_MAP(CVHIndependenceDoc, CDocument)
	//{{AFX_MSG_MAP(CVHIndependenceDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVHIndependenceDoc クラスの構築/消滅

CVHIndependenceDoc::CVHIndependenceDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CVHIndependenceDoc::~CVHIndependenceDoc()
{
}

BOOL CVHIndependenceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVHIndependenceDoc シリアライゼーション

void CVHIndependenceDoc::Serialize(CArchive& ar)
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
// CVHIndependenceDoc クラスの診断

#ifdef _DEBUG
void CVHIndependenceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVHIndependenceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVHIndependenceDoc コマンド

BOOL CVHIndependenceDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVHIndependenceView*)pView)->app.Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CVHIndependenceDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVHIndependenceView*)pView)->app.Save(lpszPathName);
	}   
	return true;
}
