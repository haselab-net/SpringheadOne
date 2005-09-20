// DynaClockTestView.h : CDynaClockTestView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaClockTestVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_DynaClockTestVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinBasis/WBCriticalSection.h>
#include <WinBasis/WBPreciseTimer.h>
#include <Physics/PhSolid.h>
#include <ImpD3D/D3Render.h>

class CDynaClockTestView : public CView
{
	std::string initialLoadFile;
	std::string loadFileName;
	int height, side;
	bool execFlag;
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CDynaClockTestView();
	DECLARE_DYNCREATE(CDynaClockTestView)

// �A�g���r���[�g
public:

	CDynaClockTestDoc* GetDocument();
	Spr::UTRef<Spr::D3Render> render;
	Spr::UTRef<Spr::SGScene> scene;
	Spr::Affinef afBody, afHead;
	float dt;
	/// ���݂̃J����
	Spr::UTRef<Spr::GRCamera> camera;

	Spr::PHSolid* solid;

	Spr::WBPreciseTimer ptimer;

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDynaClockTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void OnIdle();
	void Step();
	virtual ~CDynaClockTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Load(std::string filename);
	void Save(std::string filename);
	bool CreateBox();
	bool CreateBoxAuto();

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDynaClockTestView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DynaClockTestView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CDynaClockTestDoc* CDynaClockTestView::GetDocument()
   { return (CDynaClockTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DynaClockTestVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
