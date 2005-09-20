// TGeoNodeVashPoint.cpp: TGeoNodeVashPoint �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodeVashPoint.h"
#include "GLView.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TGeoNodeVashPoint::TGeoNodeVashPoint()
{
	isDraw = false;
}

TGeoNodeVashPoint::~TGeoNodeVashPoint()
{

}

void TGeoNodeVashPoint::Draw()
{
	if(!isDraw) return;

	static int count = 0;
	if(count > 50){
		isDraw = false;
		count = 0;
	}
	count++;

	//
	Affinef af(Rad(90),'x');
	af = 0.004*af;
	af.Pos() = Vec3f(0,0,-0.1);
	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
	glMultMatrixf( this->p_TGLView->afBody * af);	// �ʒu�E�p����ύX����B
	glCallList(displayList);
	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )

	//
}
