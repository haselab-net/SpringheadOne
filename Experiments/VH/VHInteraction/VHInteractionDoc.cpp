// VHInteractionDoc.cpp : CVHInteractionDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "VHInteraction.h"

#include "VHInteractionDoc.h"
#include "VHInteractionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionDoc

IMPLEMENT_DYNCREATE(CVHInteractionDoc, CDocument)

BEGIN_MESSAGE_MAP(CVHInteractionDoc, CDocument)
	//{{AFX_MSG_MAP(CVHInteractionDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionDoc クラスの構築/消滅

CVHInteractionDoc::CVHInteractionDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CVHInteractionDoc::~CVHInteractionDoc()
{
}

BOOL CVHInteractionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVHInteractionDoc シリアライゼーション

void CVHInteractionDoc::Serialize(CArchive& ar)
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
// CVHInteractionDoc クラスの診断

#ifdef _DEBUG
void CVHInteractionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVHInteractionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionDoc コマンド

BOOL CVHInteractionDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVHInteractionView*)pView)->app.Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CVHInteractionDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVHInteractionView*)pView)->app.Save(lpszPathName);
	}   
	return true;
}
