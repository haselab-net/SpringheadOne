// DynaHapticDoc.cpp : CDynaHapticDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DynaHaptic.h"

#include "DynaHapticDoc.h"
#include "DynaHapticView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticDoc

IMPLEMENT_DYNCREATE(CDynaHapticDoc, CDocument)

BEGIN_MESSAGE_MAP(CDynaHapticDoc, CDocument)
	//{{AFX_MSG_MAP(CDynaHapticDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticDoc クラスの構築/消滅

CDynaHapticDoc::CDynaHapticDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CDynaHapticDoc::~CDynaHapticDoc()
{
}

BOOL CDynaHapticDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDynaHapticDoc シリアライゼーション

void CDynaHapticDoc::Serialize(CArchive& ar)
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
// CDynaHapticDoc クラスの診断

#ifdef _DEBUG
void CDynaHapticDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDynaHapticDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticDoc コマンド

BOOL CDynaHapticDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaHapticView*)pView)->app->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CDynaHapticDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CDynaHapticView*)pView)->app->Save(lpszPathName);
	}   
	return true;
}
