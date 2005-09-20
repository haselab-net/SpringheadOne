// VirtualHumanDoc.cpp : CVirtualHumanDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "VirtualHuman.h"

#include "VirtualHumanDoc.h"
#include "VirtualHumanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanDoc

IMPLEMENT_DYNCREATE(CVirtualHumanDoc, CDocument)

BEGIN_MESSAGE_MAP(CVirtualHumanDoc, CDocument)
	//{{AFX_MSG_MAP(CVirtualHumanDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanDoc クラスの構築/消滅

CVirtualHumanDoc::CVirtualHumanDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CVirtualHumanDoc::~CVirtualHumanDoc()
{
}

BOOL CVirtualHumanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanDoc シリアライゼーション

void CVirtualHumanDoc::Serialize(CArchive& ar)
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
// CVirtualHumanDoc クラスの診断

#ifdef _DEBUG
void CVirtualHumanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVirtualHumanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanDoc コマンド

BOOL CVirtualHumanDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVirtualHumanView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CVirtualHumanDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVirtualHumanView*)pView)->Save(lpszPathName);
	}   
	return true;
}
