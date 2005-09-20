// BeetleDoc.cpp : CBeetleDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Beetle.h"

#include "BeetleDoc.h"
#include "BeetleView.h"


/////////////////////////////////////////////////////////////////////////////
// CBeetleDoc

IMPLEMENT_DYNCREATE(CBeetleDoc, CDocument)

BEGIN_MESSAGE_MAP(CBeetleDoc, CDocument)
	//{{AFX_MSG_MAP(CBeetleDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBeetleDoc クラスの構築/消滅

CBeetleDoc::CBeetleDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CBeetleDoc::~CBeetleDoc()
{
}

BOOL CBeetleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBeetleDoc シリアライゼーション

void CBeetleDoc::Serialize(CArchive& ar)
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
// CBeetleDoc クラスの診断

#ifdef _DEBUG
void CBeetleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBeetleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBeetleDoc コマンド

BOOL CBeetleDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CBeetleView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CBeetleDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CBeetleView*)pView)->Save(lpszPathName);
	}   
	return true;
}
