// SampleProgramDoc.cpp : CSampleProgramDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "SampleProgram.h"

#include "SampleProgramDoc.h"
#include "SampleProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleProgramDoc

IMPLEMENT_DYNCREATE(CSampleProgramDoc, CDocument)

BEGIN_MESSAGE_MAP(CSampleProgramDoc, CDocument)
	//{{AFX_MSG_MAP(CSampleProgramDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleProgramDoc クラスの構築/消滅

CSampleProgramDoc::CSampleProgramDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CSampleProgramDoc::~CSampleProgramDoc()
{
}

BOOL CSampleProgramDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSampleProgramDoc シリアライゼーション

void CSampleProgramDoc::Serialize(CArchive& ar)
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
// CSampleProgramDoc クラスの診断

#ifdef _DEBUG
void CSampleProgramDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSampleProgramDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSampleProgramDoc コマンド

BOOL CSampleProgramDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: この位置に固有の作成用コードを追加してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CSampleProgramView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CSampleProgramDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CSampleProgramView*)pView)->Save(lpszPathName);
	}   
	return true;
}
