************************************************************************
*            Virtual Human �f�����s�菇 �h�L�������g                   *
*                                        since : 2003.10.28            *
*                                   lastupdate : 2003.10.29            *
************************************************************************

//�S�̃v���N����
�ȉ��̂悤�Ƀ\�[�X�v���O������path��MPI�Ŏ��s�����DEMO�p��Path���Ⴄ�̂Œ��ӂ��Ă��������B

SOURCE:
�@\home\VSS\project\HumanScale\DEMO_VH
Execute file and data�F
�@\home\demo\project\HumanScale\DEMO_VH

�܂��A�ŐV���s�t�@�C�����o�b�`�����ōX�V���Ă��������B
\home\VSS\project\HumanScale\DEMO_VH��upexec.bat�Ƃ����T���v��������܂��̂ŁA���p���Ă��������B

1�jVMSQL   SQL�T�[�o��p�@
   D-vision����DPC03�̏ꏊ�ɐڑ�����Ă��܂��B

2) VMControl �R���g���[���pPC
   DOM�̑���Ɏg�p���܂��B

<toggle�L�[>
/////////////////////////////////////////////
Spidar�֘A����:�@
	b�F�ŏ�����
	c: Calibration, �{�[��������
Animation�֘A:
	Space:�@animation�N���E��~
	Tab: �{�[���𓊂���@	
	f : full screen
	escape: Exit

////////////////////////////////////////////

//TODO BigSpidar�̋N���菇��ǋL


1.�N�����@
�@�A���v��PC��USB2.0�Őڑ�����D
�@�A���v�̃X�C�b�`�n�m����ƍŏ����͂���������D
�@���}�̗l�ɁA�Ў肸�g�ݗ��Ă�D
�@��������������̂ŁA�Ў肪��ɂȂ�悤�ɂ���D

2�D�Ђ����@

big_spidar.htm���Q�l�ɂ��Ă��������B
�iOS��XP�ł͂Ȃ����́uXP Office run�v��Download����K�v������j

*���ӁFBigSpidar��p��LVRM��Device�̒����C����������������܂��B
LVRM\Device\DRUsb20Simple.cpp ��Reset()�̃G���R�[�_�����ݒ�A
�W����outBuffer[2] = 0xFF;�� 0x5A �ɂ��Ă��܂��B

=====================================================================
���N���菇

�܂��AMPI�֘ADLL���Ȃ����́Cmpi folder�Ɂumpich.nt.1.2.5.exe�v������܂��̂ŁA
Install���Ă��������BDLL�� /Sytem32�Ɏ����ICOPY����܂��B


�EStep0

 VPC00�`VPC33�̃}�V���̏��luncher.exe�����s����Ă��Ȃ����Ƃ��m�F����B
 �����Aluncher.exe�����s����Ă���̂Ȃ�ΏI��������B

�EStep1

 VMCONTROL�ŁA�R�}���h�v�����v�g���玟�����s�B

----------------------------------------------
c:
cd \home\demo\project\HumanScale\DEMO_VH
mpirun -logon config-full.txt
----------------------------------------------

mpirun�����s�����account�Apassword���������̂ŁA
account  : mpich
password : mpich
����͂���ƁA�f�����J�n�����B


=====================================================================
���I���菇

�EStep0

 VMCONTROL�ŃA�v���P�[�V�������I��������B(VMCONTROL�ŏI��������ƁAVPC�̊ePC��̃A�v���P�[�V�������I������)

�EStep1

 VPC00�`VPC33��ŁA"�X�^�[�g"��"�v���O����"��"�X�^�[�g�A�b�v"����"lunch.bat"�����s������B
(����D-vision�̃A�v���P�[�V�����͍��̂Ƃ���AVPC���luncher.exe�����s����Ă��邱�Ƃ��K�v�Ȃ̂ŁB�����Ԃ񃁃��h�E�����ǥ��)


=====================================================================
���L�[���t�@�����X

//TODO �����ɃL�[���t�@�����X��ǋL


