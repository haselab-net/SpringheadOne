// D3DRMSDIExDoc.cpp : WXD3RMSDIExDoc �N���X�̓���̒�`���s���܂��B
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
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExDoc �N���X�̍\�z/����

WXD3RMSDIExDoc::WXD3RMSDIExDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

WXD3RMSDIExDoc::~WXD3RMSDIExDoc()
{
}

BOOL WXD3RMSDIExDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExDoc �V���A���C�[�[�V����

void WXD3RMSDIExDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExDoc �N���X�̐f�f

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
// WXD3RMSDIExDoc �R�}���h
