// FWAppVH.h: FWAppVH �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef FWAPPVH_H
#define FWAPPVH_H

#include <Framework/FWAppD3D.h>
#include <Creature/CRVirtualHuman.h>
//#include <Creature/CRVH.h>
#include <Base/TVector.h>
#include <Base/Affine.h>
#include <WinBasis/WBPreciseTimer.h>
#include <iostream>
#include "VHCheckDialog.h"

namespace Spr{;

class FWAppVH : public FWAppD3D  
{
public:
	FWAppVH();
	virtual ~FWAppVH();

	void Step();	
	void Load(std::string fn);
	void Draw();
	bool OnKeyDown(unsigned int nChar);
	void DrawDlg();
	void AppControl(CRVirtualHuman* crVH);

	void DataOut();

	bool bCalc;						// �o�����X����J�n
	bool balance;
	
	UTRef<CRVirtualHuman> crVH;		// VH�G���W���N���X�i����,�g�̐ݒ�j
	//UTRef<CRVH> crVH;		// VH�G���W���N���X�i����,�g�̐ݒ�j
	VHCheckDialog vhDlg;			// �ʃ_�C�A���O�ŕ����I�ɂu�g���`�F�b�N
	PHContactEngine* contactEngine;	// �ڐG�G���W��
	WBPreciseTimer timer;
};

}
#endif // !defined(FWAPPVH_H)
