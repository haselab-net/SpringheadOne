// DynaHapticView.h : CDynaHapticView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaHapticVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_DynaHapticVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ImpD3D/SGD3D.h>
#include <Collision/CDDetector.h>
#include <WinBasis/WBCriticalSection.h>
#include <WinBasis/WBMMTimer.h>
#include <HIS/HISpidar4.h>
#include <Device/DRUsb20Simple.h>
#include <Physics/PHSolid.h>

class CDynaHapticView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CDynaHapticView();
	DECLARE_DYNCREATE(CDynaHapticView)

// �A�g���r���[�g
public:
	CDynaHapticDoc* GetDocument();
	Spr::UTRef<Spr::D3Render> render;
	Spr::UTRef<Spr::SGScene> scene;
	Spr::Affinef afBody, afHead;
	float dt;
	UINT wmTimerID;

	Spr::WBMMTimer timer;
	/// ���݂̃J����
	Spr::UTRef<Spr::SGCamera> camera;

	Spr::DVDeviceManager devMan;
	Spr::HISpidar4 spidar;
	Spr::HISpidar4 spidar2;
	Spr::UTRef<Spr::PHSolid> pointer;
	Spr::UTRef<Spr::PHSolid> pointer2;

	bool bMinForce;

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDynaHapticView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void OnIdle();
	void Step();
	virtual ~CDynaHapticView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Load(const char* filename);
	void Save(const char* filename);

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDynaHapticView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DynaHapticView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CDynaHapticDoc* CDynaHapticView::GetDocument()
   { return (CDynaHapticDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DynaHapticVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
