// DynaClockTestDoc.cpp : CDynaClockTestDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DynaClockTest.h"

#include "DynaClockTestDoc.h"
#include "DynaClockTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestDoc

IMPLEMENT_DYNCREATE(CDynaClockTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CDynaClockTestDoc, CDocument)
	//{{AFX_MSG_MAP(CDynaClockTestDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestDoc クラスの構築/消滅

CDynaClockTestDoc::CDynaClockTestDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CDynaClockTestDoc::~CDynaClockTestDoc()
{
}

BOOL CDynaClockTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestDoc シリアライゼーション

void CDynaClockTestDoc::Serialize(CArchive& ar)
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
// CDynaClockTestDoc クラスの診断

#ifdef _DEBUG
void CDynaClockTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDynaClockTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestDoc コマンド

BOOL CDynaClockTestDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaClockTestView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CDynaClockTestDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaClockTestView*)pView)->Save(lpszPathName);
	}   
	return true;
}
