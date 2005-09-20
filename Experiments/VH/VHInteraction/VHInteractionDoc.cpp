// VHInteractionDoc.cpp : CVHInteractionDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "VHInteraction.h"

#include "VHInteractionDoc.h"
#include "VHInteractionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionDoc

IMPLEMENT_DYNCREATE(CVHInteractionDoc, CDocument)

BEGIN_MESSAGE_MAP(CVHInteractionDoc, CDocument)
	//{{AFX_MSG_MAP(CVHInteractionDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionDoc �N���X�̍\�z/����

CVHInteractionDoc::CVHInteractionDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CVHInteractionDoc::~CVHInteractionDoc()
{
}

BOOL CVHInteractionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVHInteractionDoc �V���A���C�[�[�V����

void CVHInteractionDoc::Serialize(CArchive& ar)
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
// CVHInteractionDoc �N���X�̐f�f

#ifdef _DEBUG
void CVHInteractionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVHInteractionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionDoc �R�}���h

BOOL CVHInteractionDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVHInteractionView*)pView)->app.Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CVHInteractionDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CVHInteractionView*)pView)->app.Save(lpszPathName);
	}   
	return true;
}
