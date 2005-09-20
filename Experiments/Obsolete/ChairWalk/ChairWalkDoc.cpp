// ChairWalkDoc.cpp : CChairWalkDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "ChairWalk.h"

#include "ChairWalkDoc.h"
#include "ChairWalkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChairWalkDoc

IMPLEMENT_DYNCREATE(CChairWalkDoc, CDocument)

BEGIN_MESSAGE_MAP(CChairWalkDoc, CDocument)
	//{{AFX_MSG_MAP(CChairWalkDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChairWalkDoc クラスの構築/消滅

CChairWalkDoc::CChairWalkDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CChairWalkDoc::~CChairWalkDoc()
{
}

BOOL CChairWalkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChairWalkDoc シリアライゼーション

void CChairWalkDoc::Serialize(CArchive& ar)
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
// CChairWalkDoc クラスの診断

#ifdef _DEBUG
void CChairWalkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChairWalkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChairWalkDoc コマンド

BOOL CChairWalkDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	view->Load(lpszPathName);
	return TRUE;
}

BOOL CChairWalkDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	view->Save(lpszPathName);
	return true;
}
