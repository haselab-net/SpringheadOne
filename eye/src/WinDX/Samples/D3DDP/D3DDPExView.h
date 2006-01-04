// D3DDPExView.h : WXD3DPExView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
#define AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_

#include <WinDX/D3D/WXD3DPDev.h>


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class WXD3DPExView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	WXD3DPExView();
	DECLARE_DYNCREATE(WXD3DPExView)

// �A�g���r���[�g
public:
	WXD3DPExDoc* GetDocument();

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(WXD3DPExView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	WXDDraws ddraws;
	WXD3DPDevice d3ddev;
	virtual ~WXD3DPExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(WXD3DPExView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // D3DDPExView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline WXD3DPExDoc* WXD3DPExView::GetDocument()
   { return (WXD3DPExDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
