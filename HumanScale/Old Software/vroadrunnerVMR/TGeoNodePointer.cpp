// TGeoNodePointer.cpp: TGeoNodePointer �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodePointer.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TGeoNodePointer::TGeoNodePointer()
{
	TGeoNode();
	isDraw = false;
	isAttack = false;
#ifdef ALGORITHM1
	g = -0.05;
#endif
#ifdef ALGORITHM2
	g = -0.06;
	//g = -0.01;
#endif
	dy = 0.0;
	dz = -1.0;
	radius = 4.0;
}

TGeoNodePointer::~TGeoNodePointer()
{
}

void TGeoNodePointer::Draw()
{
	if(!isDraw) return;


	// �ʒu��i�߂�BVASH��Draw�Ŕ��肵�Ă���̂ł����Draw�ŁB
#ifdef ALGORITHM1
	dy += g;
#endif //ALGORITHM1

#ifdef ALGORITHM2
	count++;
	if( count < upcount || count > downcount ){
		dy += g;
	}
#endif

	posture = posture * Affinef(0,dy,dz);
//	Affinef afPos = afStart;// * posture;

	// �n�ʂɗ������������
	if( posture.PosY() < 0 ){
		Hide();
		return;
	}

/*
	// ��苗���i�񂾂������
	if( posture.PosZ() < dz*50.0 ){
		Hide();
		return;
	}
*/
//	const char *pObjectNameStr = this->Name().c_str();
//	if( pObjectNameStr[5] == 's'){
//	}	

	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
//	glLoadIdentity();
	glMultMatrixf( posture );//* afPos  );	// �ʒu�E�p����ύX����B
//	glLoadMatrixf( afPos  );	// �ʒu�E�p����ύX����B
	glCallList(displayList);
	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )
}

void TGeoNodePointer::Step()
{
}

bool TGeoNodePointer::Attack( Affinef &_afStart )
{
	if( isAttack ) return false;
	isDraw = true;
	isAttack = true;
	posture = _afStart;//_afCam * Affinef(0,-2,-1.5); // �������W���J�����ʒu���班������
	dy = 0.35; // �������x
//	posture = Affinef(0,0,0);

	return true;
}

bool TGeoNodePointer::Attack( Affinef &afCam,Affinef &afLaser )
{
	if( isAttack ) return false;

	isDraw = true;
	isAttack = true;

	// �����ʒu�̐ݒ�
#ifdef ALGORITHM1
	afStart = afCam * Affinef(0,-2,-1.0); //������Ƃ��炷;
	Affinef afPoint = afCam * afLaser * Affinef( 0,-2,-1.0);
	afStart.LookAtGL(afPoint.Pos());
	dy = 0.45;
//	posture = Affinef(0,0,0);
#endif //ALGORITHM1

#ifdef ALGORITHM2
	posture.Pos() = afCam.Pos(); //Vec3f( afCam.PosX(),afCam.PosY(),afCam.PosZ() );
	Affinef afPoint = afCam * afLaser;
	posture.LookAtGL( afPoint.Pos() );
	dy = sqrt( -2.0 * (afLaser.PosY()+2.0) * g );
	count = 0;
	upcount = - dy / g;
	downcount = 9;
	posture = posture * Affinef(0,-afLaser.PosY()-2.0,0);
#endif //ALGORITHM2

cout << "posture:" << posture.Pos() << endl;

	return true;
}

Affinef TGeoNodePointer::GetPosition()
{
	return posture;
}

void TGeoNodePointer::Hide()
{
	isDraw = false;
	isAttack = false;
}

float TGeoNodePointer::GetRadius()
{
	return radius;
}

bool TGeoNodePointer::IsDraw()
{
	return isDraw;
}
