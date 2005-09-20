#ifndef LVRM_DEVICE_H
#define LVRM_DEVICE_H
/**	@mainpage LVRM Device ���C�u����
@section aim �ړI
	���̃p�b�P�[�W�́C�f�o�C�X�h���C�o�𒊏ۉ����邽�߂̃��C���[�ł��D�A�v���P�[�V��������f�o�C�X�h���C�o�𒼐ڌĂяo���ƁC�f�o�C�X��ύX�����Ƃ��ɁC�A�v���P�[�V�������C�����Ȃ���΂Ȃ�Ȃ��Ȃ�܂��D�����ŁC�Ⴆ��D/A�R���o�[�^�Ȃ�ǂ�D/A�R���o�[�^�ł������悤�ɌĂяo����悤�ɒ��ۉ�����D/A�R���o�[�^�f�o�C�X(�o�[�`�����f�o�C�X)���l���C�A�v���P�[�V�����̓o�[�`�����f�o�C�X���g�p����悤�ɂ��܂��D�o�[�`�����f�o�C�X�͎��ۂ̃f�o�C�X(���A���f�o�C�X)���Ăяo���悤�Ɏ������Ă���܂��D

@section usage �g����
�ȒP�ȃT���v���������܂��D
@verbatim
	DVDeviceManager devMan;				//	�f�o�C�X�Ǘ��N���X�D
	devMan.RPool().Register(new DRAdapio(9));	//	���A���f�o�C�X�̓o�^
	devMan.Init();						//	�f�o�C�X�̏������C������
										//	�o�[�`�����f�o�C�X�����������D
	std::cout << devMan;				//	��Ԃ̕\��
	
	//	�f�o�C�X�Ǘ��N���X����A/D���؂�Ă���
	DVAdBase* ad;
	devMan.VPool().Rent(ad);
	std::cout << ad->Voltage();
@endverbatim
�܂��f�o�C�X�}�l�[�W�����쐬���C���f�o�C�X��o�^���C���������܂��D����ƃo�[�`�����f�o�C�X�����������̂ŁC�A�v���P�[�V�����́C�g�������o�[�`�����f�o�C�X���f�o�C�X�}�l�[�W������؂�Ă��܂��D

����̃f�o�C�X�̓���̋@�\���g�������ꍇ�C��p�̃f�o�C�X�}�l�[�W�����쐬���āC�f�o�C�X��o�^�C���������āC�o�[�`�����f�o�C�X���擾���Ă��������D

�f�o�C�X�}�l�[�W���͕����쐬���Ă���肠��܂���D
*/

#include "DVDeviceManager.h"

#include "DRContecIsaCounter.h"
#include "DRContecIsaDa.h"
#include "DRNittaForce.h"
#include "DRUsbH8Simple.h"

#endif
