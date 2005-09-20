// D3DRMSDIExDoc.cpp : WXD3RMSDIExDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "D3DRMSDIEx.h"

#include "D3DRMSDIExDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExDoc

IMPLEMENT_DYNCREATE(WXD3RMSDIExDoc, CDocument)

BEGIN_MESSAGE_MAP(WXD3RMSDIExDoc, CDocument)
	//{{AFX_MSG_MAP(WXD3RMSDIExDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExDoc クラスの構築/消滅

WXD3RMSDIExDoc::WXD3RMSDIExDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

WXD3RMSDIExDoc::~WXD3RMSDIExDoc()
{
}

BOOL WXD3RMSDIExDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExDoc シリアライゼーション

void WXD3RMSDIExDoc::Serialize(CArchive& ar)
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
// WXD3RMSDIExDoc クラスの診断

#ifdef _DEBUG
void WXD3RMSDIExDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void WXD3RMSDIExDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExDoc コマンド
