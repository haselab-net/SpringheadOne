// DynaLocalHapView.h : CDynaLocalHapView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaLocalHapVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_DynaLocalHapVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <FrameWork/FWAppD3D.h>
#include <FrameWork/FWPointer.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <WinBasis/WBMMTimer.h>
#include <Collision/CDCollisionEngine.h>
#include <Physics/PHContactEngine.h>
#include "HapticRender.h"
#include "HisConfig.h"

#ifdef USE_IHC
#include <Device/DRUsb20Sh4.h>
#endif

class TPlane{
public:
	Vec3f pos;
	Vec3f normal;
	UTRef<PHSolid> solid;
	TPlane(Vec3f p, Vec3f n, PHSolid* s):pos(p), normal(n), solid(s){}
};
class TPlanes:public std::vector<TPlane>{
};

class CLocalHapApp:public Spr::FWAppD3D, public CDCollisionListener{
protected:
	UTRef<PHSolid> soNearSphere;
	UTRef<PHContactEngine> pContactEngine;
	int listenerPos;				///<	���X�i�̈ʒu

public:
	float posScale, forceScale;
	Vec3f hapticOffset;
#ifdef USE_IHC
	DRUsb20Sh4* ihc;
#else
	HapticRender hapticRender;
#endif
	TPlanes planes;
	Vec3f pointer;					///<	�|�C���^�̈ʒu
	float pointerRadius;			///<	�|�C���^�̔��a
	Vec3f planePoint;
	Vec3f planeNormal;
	UTRef<PHSolid> planeSolid;
	float minDist;
	Vec3f receivedForce;

	CLocalHapApp();
	~CLocalHapApp();

	//����̍X�V�����i�V�~�����[�^�j
	void SimulationStep();
	void Step();
	/*�Փ˔��胊�X�i�p�֐�
	  �Փ˕��̂̍ŋߖT�_��FrameRecord���x�N�^�[�ɓo�^  */
	void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	void Load(std::string fn);
	void Draw();

};

class CDynaLocalHapView : public CView {
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CHisConfig dlConfig;
	Spr::WBMMTimer timer;
	CLocalHapApp app;
	UINT simuTimerId, drawTimerId;
	bool bPause;

	std::vector<std::string> fileList;
	int fileListCur;

	CDynaLocalHapView();
	DECLARE_DYNCREATE(CDynaLocalHapView)
// �A�g���r���[�g
public:
	CDynaLocalHapDoc* GetDocument();

// �I�y���[�V����
public:
	void InitHis(std::string hisList, float ps, float fs);
	void ReadRegistry();
	void WriteRegistry();
	void Step();
	void Load(std::string filename);
	void Save(std::string filename);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDynaLocalHapView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CDynaLocalHapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	void LoadList();
	void ThrowSphere(float r);
	//{{AFX_MSG(CDynaLocalHapView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnD3d();
	afx_msg void OnGL();
	afx_msg void OnHisConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DynaLocalHapView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CDynaLocalHapDoc* CDynaLocalHapView::GetDocument()
   { return (CDynaLocalHapDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DynaLocalHapVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
