#if !defined(AFX_DATAIN_H__09A46B0F_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
#define AFX_DATAIN_H__09A46B0F_B6F5_11D4_935E_006008B03B0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataIn.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDataIn �_�C�A���O

class CDataIn : public CDialog
{
// �R���X�g���N�V����
public:
	int DataNum;
	BOOL HexCheck(const char * str);
	CDataIn(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDataIn)
	enum { IDD = IDD_DATAIN };
	CString	m_Data;
	CString	m_Message;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDataIn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDataIn)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DATAIN_H__09A46B0F_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
