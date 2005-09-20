// FWAppVH.h: FWAppVH �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef FWAPPVH_H
#define FWAPPVH_H

#include <Framework/FWAppD3D.h>
#include <Creature/CRVirtualHuman.h>
#include <Base/TVector.h>
#include <Base/Affine.h>

#include "VHCheckDialog.h"
#include "Model.h"
#include "ModelUnder.h"
#include "QDPR.h"
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
	void AppControl(ModelUnder* model);
	void AppControl(Model* model);

	bool bCalc;						// �o�����X����J�n
	
	VHCheckDialog vhDlg;			// �ʃ_�C�A���O�ŕ����I�ɂu�g���`�F�b�N
	PHContactEngine* contactEngine;	// �ڐG�G���W��

	// _no���ǂ���ɕt���邩�ɂ���āA�Б����f���������g���f�������܂�
	UTRef<Model> model_no;			// �Б����f��
	UTRef<ModelUnder> model;		// �����g���f��

	QDPR qdpr;
};

}
#endif // !defined(FWAPPVH_H)
