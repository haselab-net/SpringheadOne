// SceneFileList.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "dynahaptic.h"
#include "SceneFileList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneFileList �_�C�A���O


CSceneFileList::CSceneFileList(CWnd* pParent /*=NULL*/)
	: CDialog(CSceneFileList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSceneFileList)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CSceneFileList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSceneFileList)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSceneFileList, CDialog)
	//{{AFX_MSG_MAP(CSceneFileList)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneFileList ���b�Z�[�W �n���h��
