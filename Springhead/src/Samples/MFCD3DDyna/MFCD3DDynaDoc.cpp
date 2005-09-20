// MFCD3DDynaDoc.cpp : CMFCD3DDynaDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "MFCD3DDyna.h"

#include "MFCD3DDynaDoc.h"
#include "MFCD3DDynaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaDoc

IMPLEMENT_DYNCREATE(CMFCD3DDynaDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCD3DDynaDoc, CDocument)
	//{{AFX_MSG_MAP(CMFCD3DDynaDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaDoc クラスの構築/消滅

CMFCD3DDynaDoc::CMFCD3DDynaDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CMFCD3DDynaDoc::~CMFCD3DDynaDoc()
{
}

BOOL CMFCD3DDynaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaDoc シリアライゼーション

void CMFCD3DDynaDoc::Serialize(CArchive& ar)
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
// CMFCD3DDynaDoc クラスの診断

#ifdef _DEBUG
void CMFCD3DDynaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCD3DDynaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaDoc コマンド

BOOL CMFCD3DDynaDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CMFCD3DDynaView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CMFCD3DDynaDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CMFCD3DDynaView*)pView)->Save(lpszPathName);
	}   
	return true;
}
