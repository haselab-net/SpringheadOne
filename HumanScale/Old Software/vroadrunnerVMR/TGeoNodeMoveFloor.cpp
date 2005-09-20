// TGeoNodeMoveFloor.cpp: TGeoNodeMoveFloor �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodeMoveFloor.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TGeoNodeMoveFloor::TGeoNodeMoveFloor()
{
	TGeoNode();
}

TGeoNodeMoveFloor::~TGeoNodeMoveFloor()
{

}

//void TGeoNodeNum::InitInstance(string name)
void TGeoNodeMoveFloor::InitInstance(string name)
{
	const char *pObjectNameStr = name.c_str();
	floorNo = atoi(pObjectNameStr+5);

//	T = 300;  // ����
//	reverseT = T+100;
	bMove = true;

	// �����ʒu����шړ����̐ݒ�
	switch(floorNo) {
	case 1:
		x = 613.579;
		y = 48.9368;
		z = -1;
//		z += -195.0993;
		dist = -195.0993;
//		dist = 0;
		T = 200;
		stopT = 100;
		break;
	case 2:
		x = 665.118;
		y = 49.2368;
		z = -155.845;
		dist = 165.0;
//		x += 165.0;
//		dist = 0;
		T = 300;
		stopT = 0;
		break;
	case 3:
		x = 885.05;
//		x = 890.05;
		y = 49.2436;
		z = -108;
//		y += 32.4;
		dist = 32.4;
//		dist = 0;
		T = 300;
		stopT = 0;
		break;
	default:
		break;
	}

	reverseT = T + stopT + 100;
	vel = dist / T;  // ���x
	
	floorDirection = 1;
//	floorTime = 0;

}

void TGeoNodeMoveFloor::Draw()
{
//	static int floorTime = 0;

	// ��ڂ̈ړ�����^�񒆂̓��Ŏ~�߂�
	if(++floorTime > T/2 && floorTime <= T/2 + stopT) {
		bMove = false;
	} else { 
		bMove = true;
	}

	//	floorTime�����݂ɒ��������U�~�߂āA���̌㏰�̓��������𔽓]
	if(floorTime >= T + stopT){
		if( floorTime >= reverseT ){

#ifdef _OKADA_TEST_APP
			std::cout << "T_TGeoNodeMove:" << T << " , reverseT_TGeoNodeMove:" << reverseT << std::endl;
			std::cout << "floorTime_TGeoNodeMove:" << floorTime << std::endl;
#endif	// _OKADA_TEST_APP			
			
			//floorDirection *= -1;
			vel *= -1;
			floorTime = 0;
			bMove = true;
		}else{
			bMove = false;
		}
	}

	Affinef afTrans = Affinef(x, y, z);
	if( bMove ){
		mv += vel;
	}

	switch(floorNo){
	case 1:
		afTrans = afTrans * Affinef(0, 0, mv);
		break;
	case 2:
		afTrans = afTrans * Affinef(mv, 0, 0) ;//* Affinef(Rad(90), 'y') ;
		break;
	case 3:
		afTrans = afTrans * Affinef(0, mv, 0);
		break;
	}

	// �������`�� -->>
	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
	glMultMatrixf(afTrans);	// �A�o�^�[����̑��Έʒu�ɕ\��			
	glCallList(displayList);
	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )

//************************
//	std::cout << "TGeoNodeDraw" <<std::endl;

}
