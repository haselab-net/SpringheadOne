// DDrawSDIExDoc.cpp : WXDDrawSDIExDoc �N���X�̓���̒�`���s���܂��B
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
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExDoc �N���X�̍\�z/����

WXDDrawSDIExDoc::WXDDrawSDIExDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

WXDDrawSDIExDoc::~WXDDrawSDIExDoc()
{
}

BOOL WXDDrawSDIExDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExDoc �V���A���C�[�[�V����

void WXDDrawSDIExDoc::Serialize(CArchive& ar)
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
// WXDDrawSDIExDoc �N���X�̐f�f

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
// WXDDrawSDIExDoc �R�}���h
