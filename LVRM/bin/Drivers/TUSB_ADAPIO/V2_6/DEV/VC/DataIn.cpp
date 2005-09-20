// DataIn.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "vcsample.h"
#include "DataIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataIn ダイアログ


CDataIn::CDataIn(CWnd* pParent /*=NULL*/)
	: CDialog(CDataIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataIn)
	m_Data = _T("");
	m_Message = _T("");
	//}}AFX_DATA_INIT
}


void CDataIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataIn)
	DDX_Text(pDX, IDC_DATA, m_Data);
	DDX_Text(pDX, IDC_MESSAGE1, m_Message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataIn, CDialog)
	//{{AFX_MSG_MAP(CDataIn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataIn メッセージ ハンドラ

void CDataIn::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);//変数にダイアログ値を取り込む
	if( !HexCheck(m_Data) )//閾値のデータのチェック(HEX値でなければならない)
	{
		AfxMessageBox("閾値の値が不正です");
		return;
	}
	sscanf(m_Data,"%x",&DataNum);//m_ThLevelをint変数に取り込む
	CDialog::OnOK();
}


//文字列にHEX(0-9,a-e,A-E)以外の文字がないかチェックする関数
BOOL CDataIn::HexCheck(const char *str)
{
	int len;//文字列長

	len = strlen(str);

	for( int i = 0 ; i < len ; i++ )
	{
		switch( str[i] )
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			continue;
		default:
			return FALSE;//HEX文字以外の文字があった
		}
	}
	return TRUE;
}
