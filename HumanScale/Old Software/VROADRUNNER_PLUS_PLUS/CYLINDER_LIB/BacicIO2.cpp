#include "bacicio2.h"
//#include <windows.h>
//#include <windowsx.h>
//#include <stdio.h>
//#include <time.h>
//#include <math.h>
//#include <iostream>
//#include <string>
//#include "usbdio32.h"
//#include "resource.h"


//�Ă��ƍX�V
PIO::~PIO(){
		int s;
		cout<<"�f�X�g���N�^"<<endl;
		
		//�I�����ɂ̓V�����_��S�ĉ�����
		s = dioOutPort( NULL , UnitId , PORT0 , AllDown);
		if(s != 0){
			cout<<"Output Error : Number "<<s<<endl;
		}

		//Unit���N���[�Y
		dioCloseUnit(UnitId);
}

//�����ݒ�
void PIO::Init(USHORT UnitId,USHORT PortNum,USHORT Dir)
{
	int s1,s2;

	s1 = dioOpenUnit(NULL , UnitId);//Unit�I�[�v��
	if(s1 != UnitId){
		cout<<"Unit Open Error : Number"<<s1<<endl;}

	s2 = dioSetDirection( NULL, UnitId , PortNum , Dir);//���o�͕����w��i�|�[�g���Ɓj
	if(s2 != 0){
		cout<<"Direction Error : Number "<<s2<<endl;
		cout<<endl;
	}

	for (int i = 0; i < 3; i++){
		cylinder_Status[i] = false;
		std::cout << "Cylinder" << i << " initialized" << std::endl;
		Output(i+1, 0);
	}
}

//PORT0�̎w��V�����_�ɏo��
void PIO::Output(int CylinderNum2, bool OnOff)
{
	int c1,c2;

	CylinderNum2 = pow(4, CylinderNum2-1);

	switch(OnOff)
	{
	case ON:
//		cout<<"�V�����_"<<(log(CylinderNum2)/log(4))+1<<"���グ�܂��B"<<endl;
		CylinderNum = CylinderNum & (~CylinderNum2);
//		cout<<CylinderNum<<endl;
		c1 = dioOutPort( NULL , UnitId , PORT0 , CylinderNum );
		/*
		if(c1 != 0){
			cout<<"Output Error : Error Number" <<c1<<endl;
			cout<<endl;
		}
		*/
		break;

	case OFF:
//		cout<<"�V�����_"<<(log(CylinderNum2)/log(4))+1<<"�������܂��B"<<endl;
		CylinderNum = CylinderNum | CylinderNum2;
//		cout<<CylinderNum<<endl;
		c2 = dioOutPort( NULL , UnitId , PORT0 , CylinderNum );
		/*
		if(c2 != 0){
			cout<<"Output Error : Error Number "<<c2<<endl;
		}
		*/

		break;
	}
}

//�҂����Ԃ����
int PIO::Wait(int T)
{
	clock_t  s = clock();
    clock_t  c;

    do {
        if ((c = clock()) == (clock_t)-1)       /* �G���[ */
            return (0);
    } while (1000UL * (c - s) / CLOCKS_PER_SEC <= T); 
	return(1);
}

//�V�����_�̓��쐧��
/* ������v���O����
   ����͂��邪�A�璷�E�E�E
void PIO::Control3(float angle, bool active)
{
	//�V�����_�̏グ�������p�x�ɂ�茈�߂�i���ʓ��͂�����V�����_�̔ԍ��j
	//active��false�Ȃ�A���ʂɂ���
	if(!active){
		if(cylinder_Status[0]){
			cylinder_Status[0] = false;
			Output(1, 0);
		}
		if(cylinder_Status[1]){
			cylinder_Status[1] = false;
			Output(2, 0);
		}
		if(cylinder_Status[2]){
			cylinder_Status[2] = false;
			Output(3, 0);
		}

	}
	//�O��������(2,3)
	else if(fabs(angle) <= PI/6){
		if(cylinder_Status[0]){
			cylinder_Status[0] = false;
			Output(1, 0);
		}
		if(!cylinder_Status[1]){
			cylinder_Status[1] = true;
			Output(2, 1);
		}
		if(!cylinder_Status[2]){
			cylinder_Status[2] = true;
			Output(3, 1);
		}
	}
	//����������(1)
	else if(fabs(angle) > PI * 5 / 6){
		if(!cylinder_Status[0]){
			cylinder_Status[0] = true;
			Output(1, 1);
		}
		if(cylinder_Status[1]){
			cylinder_Status[1] = false;
			Output(2, 0);
		}
		if(cylinder_Status[2]){
			cylinder_Status[2] = false;
			Output(3, 0);
		}
	}
	//���O��������(3)
	else if(PI/6 < angle && angle <= PI/2){
		if(cylinder_Status[0]){
			cylinder_Status[0] = false;
			Output(1, 0);
		}
		if(cylinder_Status[1]){
			cylinder_Status[1] = false;
			Output(2, 0);
		}
		if(!cylinder_Status[2]){
			cylinder_Status[2] = true;
			Output(3, 1);
		}
	}
	//������������(1,3)
	else if(PI/2 < angle && angle <= PI * 5 / 6){
		if(!cylinder_Status[0]){
			cylinder_Status[0] = true;
			Output(1, 1);
		}
		if(cylinder_Status[1]){
			cylinder_Status[1] = false;
			Output(2, 0);
		}
		if(!cylinder_Status[2]){
			cylinder_Status[2] = true;
			Output(3, 1);
		}
	}
	//�E�O��������(2)
	else if(-PI/2 <= angle && angle < -PI/6){
		if(cylinder_Status[0]){
			cylinder_Status[0] = false;
			Output(1, 0);
		}
		if(!cylinder_Status[1]){
			cylinder_Status[1] = true;
			Output(2, 1);
		}
		if(cylinder_Status[2]){
			cylinder_Status[2] = false;
			Output(3, 0);
		}
	}
	//�E��날����(1,2)
	else{
		if(!cylinder_Status[0]){
			cylinder_Status[0] = true;
			Output(1, 1);
		}
		if(!cylinder_Status[1]){
			cylinder_Status[1] = true;
			Output(2, 1);
		}
		if(cylinder_Status[2]){
			cylinder_Status[2] = false;
			Output(3, 0);
		}
	}
}
*/

void PIO::Control3(float angle, bool active, bool isJump)
{
	//�V�����_�̏グ�������p�x�ɂ�茈�߂�i���ʓ��͂�����V�����_�̔ԍ��j
	//active��false�Ȃ�A���ʂɂ���
	//isJump : �W�����v���̐���
	if(isJump){
		Output(1, 1);
		Output(2, 1);
		Output(3, 1);
	}
//	else if(!isCollision){}
	else if(!active){
		Output(1, 0);
		Output(2, 0);
		Output(3, 0);
	}
	//�O��������(2,3)
	else if(fabs(angle) <= PI/6){
		Output(1, 0);
		Output(2, 1);
		Output(3, 1);
	}
	//����������(1)
	else if(fabs(angle) > PI * 5 / 6){
		Output(1, 1);
		Output(2, 0);
		Output(3, 0);
	}
	//���O��������(3)
	else if(PI/6 < angle && angle <= PI/2){
		Output(1, 0);
		Output(2, 0);
		Output(3, 1);
	}
	//������������(1,3)
	else if(PI/2 < angle && angle <= PI * 5 / 6){
		Output(1, 1);
		Output(2, 0);
		Output(3, 1);
	}
	//�E�O��������(2)
	else if(-PI/2 <= angle && angle < -PI/6){
		Output(1, 0);
		Output(2, 1);
		Output(3, 0);
	}
	//�E��날����(1,2)
	else{
		Output(1, 1);
		Output(2, 1);
		Output(3, 0);
	}
}

/*
int main()
{
	PIO dio;
	int s;
	char s2[256];

	//cout<<"�R���X�g���N�^�̊m�F"<<endl;
	//dio.report();

	cout<<"�����ݒ�"<<endl;
	dio.Init(0x00,PORT0,PORT_Output);

	while(1){
		//�w��V�����_��ON
		cout<<"�V�����_�ԍ�(1�`4) (0�ŏI��)"<<endl;
		cin>>s;
		s=pow(4,s-1);
		if(s==0){break;}
	
		cout<<"OnOff����"<<endl;
		cin>>s2;
		if(s2=="on"){
			dio.Output(s,1);
			dio.Wait(WaitTime);
		}
		else if(s2=="off"){
			dio.Output(s,0);
			dio.Wait(WaitTime);
		}
		else{
			cout<<"error �ē���"<<endl;
		}
	}
	return 0;
}
*/
