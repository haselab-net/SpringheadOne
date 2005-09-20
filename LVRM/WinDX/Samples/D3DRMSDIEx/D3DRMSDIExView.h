// D3DRMSDIExView.h : WXD3RMSDIExView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
#define AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_

#include <LVRM/DDraw/WXDDraw.h>
#include <LVRM/WXD3D/WXRM.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class WXD3RMSDIExView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	WXD3RMSDIExView();
	DECLARE_DYNCREATE(WXD3RMSDIExView)

// �A�g���r���[�g
public:
	WXD3RMSDIExDoc* GetDocument();

// �I�y���[�V����
public:
	WXRM rm;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(WXD3RMSDIExView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	WXDDraws ddraws;
	WXD3Device d3dDev;
	WXD3Vec2 mouseLDrag, mouseRDrag;
	Affinef now, head;
	Vec3f vel, accel;
	void BuildScene();
	void OnIdle();
	virtual ~WXD3RMSDIExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(WXD3RMSDIExView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // D3DRMSDIExView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline WXD3RMSDIExDoc* WXD3RMSDIExView::GetDocument()
   { return (WXD3RMSDIExDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
