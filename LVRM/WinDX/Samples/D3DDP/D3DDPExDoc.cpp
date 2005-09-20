// D3DDPExDoc.cpp : WXD3DPExDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "D3DDPEx.h"

#include "D3DDPExDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WXD3DPExDoc

IMPLEMENT_DYNCREATE(WXD3DPExDoc, CDocument)

BEGIN_MESSAGE_MAP(WXD3DPExDoc, CDocument)
	//{{AFX_MSG_MAP(WXD3DPExDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WXD3DPExDoc クラスの構築/消滅

WXD3DPExDoc::WXD3DPExDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

WXD3DPExDoc::~WXD3DPExDoc()
{
}

BOOL WXD3DPExDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// WXD3DPExDoc シリアライゼーション

void WXD3DPExDoc::Serialize(CArchive& ar)
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
// WXD3DPExDoc クラスの診断

#ifdef _DEBUG
void WXD3DPExDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void WXD3DPExDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// WXD3DPExDoc コマンド
