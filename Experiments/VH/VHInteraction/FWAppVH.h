// FWAppVH.h: FWAppVH �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef FWAPPVH_H
#define FWAPPVH_H

#include <Framework/FWAppD3D.h>
#include <Creature/CRHuman.h>
#include <Creature/CRUser.h>
#include <Creature/CRPuppet.h>
#include <Creature/CRActionPlanner.h>
#include <SceneGraph/SGScene.h>
#include <WinBasis/WBPreciseTimer.h>


#include "VHCheckDialog.h"
#include "SLCamera.h"

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

	UTRef<SGScene> scene2;
	CRPuppet crPuppet;			// crHuman�̑����crPuppet�Ŏ���
	CRUser crUser;				// ���[�U�[����̓��͂ɂ�鑀��

	CRActionPlanner crPlanner;
	VHCheckDialog vhDlg;		// �ʃ_�C�A���O�ŕ����I�ɂu�g���`�F�b�N

	SLCamera slc;						// ���x�����O�J����
	std::vector<HISpidar4*> spidars;	// SPIDAR4

	WBPreciseTimer timer;
	bool bDrawTest;
	bool bDrawCount;
	bool bDrawState;
};

}
#endif // !defined(AFX_FWAPPVH_H__6DFDEA6F_A8B7_4A78_9FCD_C88C86C2E658__INCLUDED_)
