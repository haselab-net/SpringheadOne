#include <Device/DVDeviceManager.h>
#include <Device/DRNittaForce.h>
#include <conio.h>
#include <iostream>

using namespace Spr;

void main(){
	
	DVDeviceManager devMan;	//	�f�o�C�X�Ǘ�����N���X�D
	
	//	�f�o�C�X�Ǘ��N���X�Ƀf�o�C�X��o�^
	
#if 0	//	IO�|�[�g�𒼐ڃA�N�Z�X����ꍇ�F
	//	�����Ń{�[�h�̃A�h���X��ݒ�(�}�j���A�������Ē����Ă�������)
	devMan.RPool().Register(new DRNittaForce(0x230));
#else	//	jr3.dll ���g���ꍇ�D�{�[�h�ԍ����w�肷��D
	devMan.RPool().Register(new DRNittaForce(0));
#endif
	//	�f�o�C�X�̏������ƌ��ʂ̕\��
	devMan.Init();
	std::cout << devMan;
	
	//	�f�o�C�X���񋟂���@�\(Virtual Device)����C�̓Z���T��T��
//	DVForceBase* sensor = (DVForceBase*)devMan.VPool().Rent("Force");
	DVForceBase* sensor;
	devMan.VPool().Rent(sensor);
	//	���������Z���T�̂����ŏ��̕����g�p�D
	if (sensor){
		while(!kbhit()){
			Vec3f f,t;	//	�͂ƃg���N
			sensor->GetForce6(f,t);
			std::cout << f << t << std::endl;
		}
		getch();
	}
}
