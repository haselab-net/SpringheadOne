// SampleProgramDoc.cpp : CSampleProgramDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "SampleProgram.h"

#include "SampleProgramDoc.h"
#include "SampleProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleProgramDoc

IMPLEMENT_DYNCREATE(CSampleProgramDoc, CDocument)

BEGIN_MESSAGE_MAP(CSampleProgramDoc, CDocument)
	//{{AFX_MSG_MAP(CSampleProgramDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleProgramDoc �N���X�̍\�z/����

CSampleProgramDoc::CSampleProgramDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CSampleProgramDoc::~CSampleProgramDoc()
{
}

BOOL CSampleProgramDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSampleProgramDoc �V���A���C�[�[�V����

void CSampleProgramDoc::Serialize(CArchive& ar)
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
// CSampleProgramDoc �N���X�̐f�f

#ifdef _DEBUG
void CSampleProgramDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSampleProgramDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSampleProgramDoc �R�}���h

BOOL CSampleProgramDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CSampleProgramView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CSampleProgramDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CSampleProgramView*)pView)->Save(lpszPathName);
	}   
	return true;
}
