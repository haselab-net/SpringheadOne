// LocalDynamicsDoc.cpp : CLocalDynamicsDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "LocalDynamics.h"

#include "LocalDynamicsDoc.h"
#include "LocalDynamicsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsDoc

IMPLEMENT_DYNCREATE(CLocalDynamicsDoc, CDocument)

BEGIN_MESSAGE_MAP(CLocalDynamicsDoc, CDocument)
	//{{AFX_MSG_MAP(CLocalDynamicsDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsDoc クラスの構築/消滅

CLocalDynamicsDoc::CLocalDynamicsDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CLocalDynamicsDoc::~CLocalDynamicsDoc()
{
}

BOOL CLocalDynamicsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsDoc シリアライゼーション

void CLocalDynamicsDoc::Serialize(CArchive& ar)
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
// CLocalDynamicsDoc クラスの診断

#ifdef _DEBUG
void CLocalDynamicsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLocalDynamicsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsDoc コマンド

BOOL CLocalDynamicsDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CLocalDynamicsView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CLocalDynamicsDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CLocalDynamicsView*)pView)->Save(lpszPathName);
	}   
	return true;
}
