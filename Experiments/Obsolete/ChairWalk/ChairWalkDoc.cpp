// ChairWalkDoc.cpp : CChairWalkDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "ChairWalk.h"

#include "ChairWalkDoc.h"
#include "ChairWalkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChairWalkDoc

IMPLEMENT_DYNCREATE(CChairWalkDoc, CDocument)

BEGIN_MESSAGE_MAP(CChairWalkDoc, CDocument)
	//{{AFX_MSG_MAP(CChairWalkDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChairWalkDoc �N���X�̍\�z/����

CChairWalkDoc::CChairWalkDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CChairWalkDoc::~CChairWalkDoc()
{
}

BOOL CChairWalkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChairWalkDoc �V���A���C�[�[�V����

void CChairWalkDoc::Serialize(CArchive& ar)
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
// CChairWalkDoc �N���X�̐f�f

#ifdef _DEBUG
void CChairWalkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChairWalkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChairWalkDoc �R�}���h

BOOL CChairWalkDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	view->Load(lpszPathName);
	return TRUE;
}

BOOL CChairWalkDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	view->Save(lpszPathName);
	return true;
}
