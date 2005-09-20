// DynaLocalHapDoc.cpp : CDynaLocalHapDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DynaLocalHap.h"

#include "DynaLocalHapDoc.h"
#include "DynaLocalHapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapDoc

IMPLEMENT_DYNCREATE(CDynaLocalHapDoc, CDocument)

BEGIN_MESSAGE_MAP(CDynaLocalHapDoc, CDocument)
	//{{AFX_MSG_MAP(CDynaLocalHapDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapDoc クラスの構築/消滅

CDynaLocalHapDoc::CDynaLocalHapDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CDynaLocalHapDoc::~CDynaLocalHapDoc()
{
}

BOOL CDynaLocalHapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapDoc シリアライゼーション

void CDynaLocalHapDoc::Serialize(CArchive& ar)
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
// CDynaLocalHapDoc クラスの診断

#ifdef _DEBUG
void CDynaLocalHapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDynaLocalHapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapDoc コマンド

BOOL CDynaLocalHapDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaLocalHapView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CDynaLocalHapDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaLocalHapView*)pView)->Save(lpszPathName);
	}   
	return true;
}
