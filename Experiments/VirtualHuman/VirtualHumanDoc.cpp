// VirtualHumanDoc.cpp : CVirtualHumanDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "VirtualHuman.h"

#include "VirtualHumanDoc.h"
#include "VirtualHumanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanDoc

IMPLEMENT_DYNCREATE(CVirtualHumanDoc, CDocument)

BEGIN_MESSAGE_MAP(CVirtualHumanDoc, CDocument)
	//{{AFX_MSG_MAP(CVirtualHumanDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanDoc �N���X�̍\�z/����

CVirtualHumanDoc::CVirtualHumanDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CVirtualHumanDoc::~CVirtualHumanDoc()
{
}

BOOL CVirtualHumanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanDoc �V���A���C�[�[�V����

void CVirtualHumanDoc::Serialize(CArchive& ar)
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
// CVirtualHumanDoc �N���X�̐f�f

#ifdef _DEBUG
void CVirtualHumanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVirtualHumanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanDoc �R�}���h

BOOL CVirtualHumanDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVirtualHumanView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CVirtualHumanDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVirtualHumanView*)pView)->Save(lpszPathName);
	}   
	return true;
}
