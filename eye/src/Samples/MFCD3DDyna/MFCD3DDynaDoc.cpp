// MFCD3DDynaDoc.cpp : CMFCD3DDynaDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "MFCD3DDyna.h"

#include "MFCD3DDynaDoc.h"
#include "MFCD3DDynaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaDoc

IMPLEMENT_DYNCREATE(CMFCD3DDynaDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCD3DDynaDoc, CDocument)
	//{{AFX_MSG_MAP(CMFCD3DDynaDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaDoc �N���X�̍\�z/����

CMFCD3DDynaDoc::CMFCD3DDynaDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CMFCD3DDynaDoc::~CMFCD3DDynaDoc()
{
}

BOOL CMFCD3DDynaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaDoc �V���A���C�[�[�V����

void CMFCD3DDynaDoc::Serialize(CArchive& ar)
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
// CMFCD3DDynaDoc �N���X�̐f�f

#ifdef _DEBUG
void CMFCD3DDynaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCD3DDynaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaDoc �R�}���h

BOOL CMFCD3DDynaDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CMFCD3DDynaView*)pView)->Load(lpszPathName);
	}   
	return TRUE;
}

BOOL CMFCD3DDynaDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL){
		CView* pView = GetNextView(pos);
		((CMFCD3DDynaView*)pView)->Save(lpszPathName);
	}   
	return true;
}
