// TGeoNodeNum.cpp: TGeoNodeNum �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "TGeoNodeNum.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

/* �^�C�}�[�̖��O�� 
nc_t1_2 �� �^�C�}�[��1�̈�(������ꌅ��)��2
nc_t2_4 �� �^�C�}�[��10�̈�(������񌅖�)��4
nc_t3_1 �� �^�C�}�[��100�̈�(������O����)��1
*/

/* ���_�̖��O�� 
nc_s1_2 �� ���_��1�̈�(������ꌅ��)��2
nc_s2_4 �� ���_��10�̈�(������񌅖�)��4
nc_s3_1 �� ���_��100�̈�(������O����)��1
nc_s4_m �� ���_��1000�̈�(������l����)��-(�}�C�i�X)
*/

TGeoNodeNum::TGeoNodeNum()
{
	TGeoNode();
}

TGeoNodeNum::~TGeoNodeNum(){ }

void TGeoNodeNum::Draw()
{
	control();
//	isOffDraw();

	if(!isDraw) return;

	// �^�C�}�[�`�� -->>
	glPushMatrix();	// �ʒu���W�̑ޔ� ( 1��̍��W�n��ޔ� )
	glMultMatrixf( p_TGLView->afBody * afTrans);	// �A�o�^�[����̑��Έʒu�ɕ\��			
	glCallList(displayList);
	glPopMatrix();	// �ʒu���W�̕��A ( 1��̍��W�n�֕��A )
}

void TGeoNodeNum::control(){ }

/*
void TGeoNodeNum::isOffDraw()
{
	if(!isDraw) return;
	if(vn != number){ isDraw = false; }	
}*/

Affinef TGeoNodeNum::setTrans(int digit)
{
	float h = 1.0;			//�����ړ��̉�
	float v = 6.53;			//�����ړ��̏c
	float d = -12;			//�����ړ��̉��s��
	
	switch(digit){
	case 1:
		h += 1.0;
		break;
	case 2:
		h += 0.5;
		break;
	case 3:
		h += 0.0;
		break;
	default:
		h += 0;
		break;
	}

	Affinef t = Affinef(Rad(90), 'x');
	t.Pos() = Vec3f(h,v,d);
	return t;
}

void TGeoNodeNum::InitInstance(string name)
{
	const char *pObjectNameStr = name.c_str();
	digit = atoi(pObjectNameStr+4);

	if(pObjectNameStr[6] == 'm'){ number = 10; }
	else{ number = atoi(pObjectNameStr+6); }

	afTrans = setTrans(digit);
	isDraw = false;
}


//***** TGeoNodeScore *****//
void TGeoNodeScore::InitInstance(string name)
{
	TGeoNodeNum::InitInstance(name);
	afTrans = afTrans * Affinef(3.0, 0, 0);
}

void TGeoNodeScore::control()
{
//***	if(isDraw) return;

	char c = *(p_TGLView->vash_char + 4 - digit);		 
	vn = atoi(&c);

//	std::cout << "vc = " << p_TGLView->vash_char << std::endl;
//	std::cout << "vn = " << vn << std::endl;


//	if(vn == number && !(digit == 4 && vn ==0))	{
	if(vn == number && !( (abs(p_TGLView->vash_val) < pow(10,digit-1)) && digit != 1 ))	{
		if(isDraw == false){
			isDraw = true;
			size = 10;
//			afTrans = afTrans * Affinef(0, 0, size * 0.05); 
			afTrans = afTrans * Affinef(0, size * 0.05, 0); 
		} 
		else {
			if(--size >= 0){
//				afTrans = afTrans * Affinef(0, 0, -0.05);
				afTrans = afTrans * Affinef(0, -0.05, 0); 
			}
		}
	}
	else{ isDraw = false; }
	
//	if((c == '-'  && number == 10){ isDraw = true; }
	if(p_TGLView->vash_val < 0  && c == '-' && number == 10){ isDraw = true; }

}

Affinef TGeoNodeScore::setTrans(int digit)
{
	Affinef af = TGeoNodeNum::setTrans(digit);
	if(digit == 4){
		af = af * Affinef(-0.4, 0, 0);
	}
	return af;
}

//***** TGeoNodeTimer *****//
void TGeoNodeTimer::control()
{
//***	if(isDraw) return;

	char c = *(p_TGLView->timer_char + 3 - digit);		 
	vn = atoi(&c);

//	if(vn == number){ isDraw = true; }
	if(vn == number && !( (abs(p_TGLView->timer_val) < pow(10,digit-1)) && digit != 1 ))	{
		isDraw = true; 
	} 
	else { isDraw = false; }

//	std::cout << "vn = " << p_TGLView->timer_char << std::endl;
//	std::cout << "vn = " << vn << std::endl;
}
