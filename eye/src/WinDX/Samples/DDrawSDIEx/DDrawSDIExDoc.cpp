// DDrawSDIExDoc.cpp : WXDDrawSDIExDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DDrawSDIEx.h"

#include "DDrawSDIExDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExDoc

IMPLEMENT_DYNCREATE(WXDDrawSDIExDoc, CDocument)

BEGIN_MESSAGE_MAP(WXDDrawSDIExDoc, CDocument)
	//{{AFX_MSG_MAP(WXDDrawSDIExDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExDoc クラスの構築/消滅

WXDDrawSDIExDoc::WXDDrawSDIExDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

WXDDrawSDIExDoc::~WXDDrawSDIExDoc()
{
}

BOOL WXDDrawSDIExDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExDoc シリアライゼーション

void WXDDrawSDIExDoc::Serialize(CArchive& ar)
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
// WXDDrawSDIExDoc クラスの診断

#ifdef _DEBUG
void WXDDrawSDIExDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void WXDDrawSDIExDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExDoc コマンド
