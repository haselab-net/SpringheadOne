// D3DDPExDoc.cpp : WXD3DPExDoc �N���X�̓���̒�`���s���܂��B
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
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WXD3DPExDoc �N���X�̍\�z/����

WXD3DPExDoc::WXD3DPExDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

WXD3DPExDoc::~WXD3DPExDoc()
{
}

BOOL WXD3DPExDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// WXD3DPExDoc �V���A���C�[�[�V����

void WXD3DPExDoc::Serialize(CArchive& ar)
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
// WXD3DPExDoc �N���X�̐f�f

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
// WXD3DPExDoc �R�}���h
