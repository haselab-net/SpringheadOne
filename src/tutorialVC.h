#ifndef Spr_TUTORIALVC_H
#define Spr_TUTORIALVC_H

/** @page TutorialVC �`���[�g���A��(Visual C++)

	@section Content �ڎ�
		- @ref Execution
		- @ref EditDataFile
			-# @ref MakeConvex
			-# @ref MakeConcave
			-# @ref SetTrianglePole
			-# @ref TiltFloor
			-# @ref SetSphere
			-# @ref SetLight
			-# @ref SetPhysics
			-# @ref SetCamera
			-# @ref BroadenFloor
			-# @ref SetVessel
			-# @ref AccelSphere
			-# @ref SetTexture
			-# @ref Adjustment
			-# @ref SetPointer
		- @ref SPIDAR
		- @ref NewProject

	@section Execution �r���h���Ď��s���Ă݂�
	�܂��́C����邽�߂Ɋ����̃v���W�F�N�g�i����� Springhead/src/Samples/DynaHaptic�j�����s���Ă݂܂��D
		-# �r���h���s�������Ƃ��Ĉȉ��̂��Ƃ��s���Ă��������D
			- ���ϐ� Springhead �� Springhead ���C���X�g�[�������f�B���N�g����
			�w�肵�ĉ������D<br>
			Windows2000�Ȃ��[�}�C�R���s���[�^]-[�v���p�e�B]-[�ڍ�]-[���ϐ�]-[�V�K]�̃_�C�A���O��
			[�ϐ���] Springhead [�ϐ��l] �C���X�g�[�������f�B���N�g���Ƃ��Ă��������D
			- DirectX8.1��9.0��SDK���C���X�g�[�����Ă��������D
		-# Springhead/src/Samples/DynaHaptic/DynaHaptic.dsw �� Visual C++ �ŊJ���ăr���h���Ă��������D<br>
		�r���h���������Ȃ����͈ȉ��̂��Ƃ��m�F���Ă��������D
			- DirectX SDK�̃C���N���[�h�t�@�C���ƃ��C�u�����t�@�C���̃f�B���N�g���̐ݒ�<br>
			[�c�[��]-[�I�v�V����]-[�f�B���N�g��]�́u�\������f�B���N�g��(S)�v���u�C���N���[�h�t�@�C���v�ɕς��C
			DirectX SDK�̃C���N���[�h�t�@�C���̃f�B���N�g��(��:"C:\DXSDK\INCLUDE")���ݒ肳��Ă��邩�m�F���Ă��������D<br>
			�ݒ肳��Ă��Ȃ���ΐݒ肵�Ă��������D<br>
			�܂��C�����̗D�揇�ʂ��グ�邽�߂Ɂu���v�̃{�^�������x���N���b�N���āC
			��ԏ�ɗ���悤�ɂ��Ă��������D<br>
			���C�u�����t�@�C���ɂ��Ă��C���N���[�h�t�@�C���Ɠ��l�Ƀp�X�̐ݒ���s���Ă�������
			�i�u�\������f�B���N�g��(S)�v���u���C�u�����t�@�C���v�ɕύX����j�D
			- �A�N�e�B�u�v���W�F�N�g�̐ݒ�
			[�r���h]-[�A�N�e�B�u�ȍ\���̐ݒ�]��
			�u�v���W�F�N�g�̍\��]�� "Dynahaptic - Win32 MFC Debug (or Release)" 
			�ɂ��ĉ������D
			- <A href=http://sklab-www.pi.titech.ac.jp/~hase/springhead/>Springhead�̃y�[�W</A>�́u�����̃r���h�v�����Ă��������D
			�����ɂ͂��̓��̌ߑO�R���̎��_��VSS�ɃA�b�v����Ă���v���W�F�N�g�̃r���h���ʂ��\������Ă��܂��D
			�����ɍڂ��Ă��Ȃ��v���W�F�N�g�̃r���h�͒ʂ�Ȃ��Ƃ������ƂɂȂ�܂��̂ŁC�ߋ��̃r���h����������o�[�W�����̃t�@�C�����擾���Ă��Ă��������D
		-# ���s���܂��D<br>
		���s������C[�t�@�C��]-[�J��]��<A href=../../Samples/data/>Springhead/src/Samples/data/</A>�ɂ���f�[�^�t�@�C��(@ref XFile "Direct3D X�t�@�C��")��K���Ƀ��[�h���Ă݂Ă��������D
		���s���̎�ȃL�[����͈ȉ��̂悤�ɂȂ��Ă��܂��D
				- ���F�O�i
				- ���F���
				- ���F����]
				- ���F�E��]
				- S�F���]
				- X�F����]
				- Q�F�����s�ړ�
				- W�F�E���s�ړ�
				- A�F�㕽�s�ړ�
				- Z�F�����s�ړ�
				- R�F���_���Z�b�g
				- F�F�t���X�N���[��
				- G�F��ʂ��t���X�N���[�����猳�ɖ߂�
				- C�FSPIDAR�̃O���b�v�̃L�����u���[�V����
				- �X�y�[�X�FSPIDAR�ɗ͂�Ԃ����Ԃ��Ȃ�����ON-OFF
				- T�F�}���`���f�B�A�^�C�}�[�̋N��
				�i���x�̍����͊o��񎦂��邽�߂ɂ͕K�v�DRelease�ł̂݁j

	@section EditDataFile �f�[�^�t�@�C����ҏW����
	�{�[����]�����Ĕ��ɓ����Ƃ������z���E�̍쐬��ʂ��āC
	X�t�@�C���̕ҏW�̕��@���ȒP�Ɍ��Ă����܂��D<br><br>
	- @ref XFile �Ƃ́H
	- ������
		- rollingBall <A href=../../Samples/data/tutorial/rollingBall.x>Springhead/src/Samples/data/tutorial/rollingBall.x</A>
		- golf <A href=../../Samples/data/tutorial/golf.x>Springhead/src/Samples/data/turorial/golf.x</A>

	�܂��́CrollingBall �������Ă����܂��D

		@subsection MakeConvex �ʕ��̂����
		�܂��C���z���E�ɕK�v�ȃI�u�W�F�N�g�Ƃ��āC
		���C�W�����v��ƂȂ�O�p���C�{�[�������f�����O���܂��D
		.x�`���ŕۑ����邱�Ƃ̂ł��郂�f�����O�\�t�g���g�p���č���Ă��������D<br><br>
		�|�쐬��|
		- <A href=../../Samples/data/tutorial/floor.x>floor.x</A>
		- <A href=../../Samples/data/tutorial/trianglePole.x>trianglePole.x</A>
		- <A href=../../Samples/data/tutorial/sphere.x>sphere.x</A>

		@subsection MakeConcave �����̂����
		���z���E�ɓo�ꂷ�镨�̂ɂ͂����قǍ쐬�����悤�Ȓ����́C���̂悤�ȓʕ��̂ƁC
		���̂悤�ȉ����̂�����܂��D
		Springhead�ł͏Փ˔���ɗp���Ă���A���S���Y���̐�����C
		���̂͂��ׂēʕ��̂łȂ���΂Ȃ�܂���D
		�����ŁC�������Ƃ��ɂ́C��Ǝl���̕ǂ̂T�̒����̂�g�ݍ��킹�č쐬���܂��D<br>
		�쐬��ł́C�e�t���[�� Vessel �̉���
		�q�t���[���Ƃ��� VesselFront,Back,Left,Right,Bottom ��z�u���Ă��܂��D
		�e�t���[���ɂ��� FrameTransformMatrix ��
		�A�t�B���s���]�u�����`�ŋL�q����Ă���C
		�e�t���[������̃��[�J���ϊ���\���Ă��܂��D
		�iVesselFront,Back,left,right �͂��̃��[�J���ϊ����قȂ邾���ŁC
		���̉��ɋL�q����Ă��� Mesh �̕����͓������̂��g�p���Ă��܂��j<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/vessel.x>vessel.x</A> 

		@subsection SetTrianglePole ���ɎO�p����z�u����
		�ł͎��ۂɐ��E������Ă����܂��D
		�܂��C���E�̈�Ԃ̐e�t���[���Ƃ��� frTop ���쐬���C
		���̎q�t���[���ɏ����C�����ď��̎q�t���[���Ƃ��ĎO�p����z�u���܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall1.x>rollingBall1.x</A>
		
		@subsection TiltFloor �����X����
		���́C�����X���܂��D<br>
		�����قǁC�O�p�������̎q�t���[���Ƃ��Ă���̂ŁC
		���t���[���� FrameTransformMatrix ��ҏW���邾���ŁC
		�O�p�����ƌX���邱�Ƃ��ł��܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall2.x>rollingBall2.x</A>

		@subsection SetSphere �{�[����z�u����
		�{�[���� frTop �̎q�t���[���Ƃ��Ĕz�u���܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall3.x>rollingBall3.x</A>

		@subsection SetLight ���C�g��ǉ����� 
		�����܂ł̏�ԂŎ��s���Ă��C
		�Â��Ă��܂茩���Ȃ��̂Ń��C�g��ǉ����܂��D
		��̓I�ɂ́C���C�g�̈ʒu�E�p�������߂邽�߂̃t���[����
		���̃��C�g�̐��������� Light8 �� frTop �̉��ɒǉ����܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall4.x>rollingBall4.x</A>
		
		@subsection SetPhysics �����@����ǉ�����
		�ł͂��悢�扼�z���E�ɕ����@����ǉ����܂��D
		��̓I�ɂ́C
		- SolidContainer : ���z���̂̕����I����
		- GravityEngine : �d�͂̐���
		- PenaltyEngine : �Փ˔���̐���

		��ǉ����邱�ƂɂȂ�܂��D<br>
		���ݍ�肽�����E�̎d�l���C

			- �{�[���������D
			- �d�͂��{�[���ɂ͂��炭�D���ɂ͂͂��炩�Ȃ��D
			- �{�[���Ə��̊ԂŏՓ˂��N����C�Փˌ�{�[���͒��˕Ԃ邪���͌Œ�D

		�Ƃ���ƁCSolidContainer ����� GravityEngine �ɂ̓{�[��������o�^���C
		PenaltyEngine �ɂ́C�{�[���Ə��̏Փ˔�����s���Փˌ�̓{�[�������ɔ��͂�������Ƃ����L�q�����܂��D<br>
		�Ȃ��CPenaltyEngine �ł́C
		�e�t���[���Ƃ̏Փ˔�����s���悤�ɋL�q����΁C
		�ċA�I�Ɏq�t���[���Ƃ̏Փ˔�����s���̂ŁC
		�{�[���Ə��Ƃ̏Փ˔�����L�q���Ă����΁C
		���̎q�t���[���ł���O�p���Ƃ̏Փ˔���������I�ɍs���܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall5.x>rollingBall5.x</A>

		@subsection SetCamera �J������ǉ�
		���̂܂܂ł͉�ʂɉf��͈͂������̂ŁC
		�����Ђ����i�Y�[���A�E�g�����j�ʒu�Ɏ��_�𓮂����܂��D<br>
		���_�̈ʒu��ݒ肷�邽�߂ɂ́CfrTop �̉��� Camera ��ǉ����܂��D
		Camera ���Ȃ��ꍇ�̓f�t�H���g�̃J�����ʒu�ɂȂ�܂��D
		�i�f�t�H���g�̃J�����ݒ�ɂ��Ă� <A href=../../src/Graphics/GRCamera.cpp>Springhead/src/Graphics/GRCamera.cpp</A> �� GRCameraData::InitData() ���Q�Ɓj
		����܂ł̓J�����̐ݒ�����Ă��Ȃ������̂ŁC
		�f�t�H���g�̃J�����ʒu�ɂȂ��Ă����Ƃ������ƂɂȂ�܂��D<br>
		�J�����̈ʒu�̐ݒ�ɂ́C�ۑ��̋@�\���g���ƕ֗��ł��D
		�ۑ����s���ƁC���̂Ƃ��̃J�����̐ݒ��X�t�@�C���ɏ����o���Ă���܂��D
		����āC<br>

		���s �� �L�[����ɂ��C�ӂ̎��_�ʒu�Ɉړ� �� �g���O��ς��āh�ۑ� 
		�� �ۑ�����X�t�@�C���̃J�����ݒ�����̃t�@�C���ɃR�s�[

		�Ƃ����菇�ōs���Ɗy�ł��D
		�ۑ�����ہC�{�[���̈ʒu�Ȃǉ��z���E�̏�Ԃ��ꏏ�ɕۑ����Ă��܂��̂ŁC
		�ʂ̖��O�ŕۑ�����K�v������܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall6.x>rollingBall6.x</A>

		@subsection BroadenFloor �����L�����ă{�[�����ړ�
		���́C�����L�����āC�{�[���������Əォ��]����悤�ɕύX���܂��D
		�����L�����邽�߂ɂ͕��̂̒��_���W���Ǘ����Ă��� Mesh ��ύX���܂��D
		�{�[���̈ʒu�̕ύX�� FrameTransformMatrix �̕��s�ړ�������ύX���čs���܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall7.x>rollingBall7.x</A>

		@subsection SetVessel ����z�u
		�{�[�����L���b�`���锠��z�u���܂��D<br>
		@ref MakeConcave �ō쐬�������t���[����ǉ����C
		�{�[���Ƃ̏Փ˔�����s���悤�� Penalty ���ǉ����܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall8.x>rollingBall8.x</A>

		@subsection AccelSphere �{�[���̏������x��傫������
		���Ƀ{�[��������Ȃ������̂ŁC�{�[���������Ɖ����֔��ł����悤�ɁC
		Solid ��ҏW���ď������x��傫�����܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall9.x>rollingBall9.x</A>

		@subsection SetTexture  �w�i�����C�e�N�X�`����\��D
		���Ƀ{�[��������Ƃ���܂łł����̂ŁC���������ڂ����コ���邽�߂ɁC
		�w�i��t���C�e�N�X�`�����\��܂��D<br>
		�w�i�́C���E�̌���ɑ傫�ȕ��ʃI�u�W�F�N�g��u���č�邱�Ƃɂ��܂��D
		�e�N�X�`���́CMaterial �̒��� TextureFilename �Ŏw�肵�܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/rollingBall.x>rollingBall.x</A>

		����łЂƂ܂� rollingBall �͊����ł��D

		@subsection Adjustment �e�I�u�W�F�N�g�ʒu�̕ύX
		�Â��� rollingBall �����ǂ��� golf ������Ă����܂��D<br>
		�܂��́C���E��golf�p�ɕύX���܂��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/golf1.x>golf1.x</A>

		�s������Ƃ͈ȉ��̒ʂ�ł��D
		- ���̕ύX
				- �X���𐅕��ɂ���
				- �J�b�v����邽�߂ɏ����S��������(���̂����̈�ɂ͌X�΂�����)
				- �W�����v���Ⴍ����
		- ���̏����ʒu�C�������x�̕ύX
		- ���̈ʒu�̕ύX

		@subsection SetPointer �͊o�|�C���^�����
		SPIDAR�̃O���b�v�����ɑΉ�����|�C���^�����܂��D
		�܂��́C�|�C���^�̃t���[�����쐬���܂��D
		�����āC�|�C���^�̕���(Solid,Penalty)��ǉ����܂��D
		�f�[�^�t�@�C�����̍�Ƃ͈ȏ�ŏI���ł��D<br><br>
		�|�쐬��|�@
		- <A href=../../Samples/data/tutorial/golf.x>golf.x</A>
		
		���Ƃ�SPIDAR�̐ݒ蓙�C�v���O������ҏW�����ƂɂȂ�܂��D
		SPIDAR�̐ݒ�ɂ��Ă� @ref SPIDAR ���Q�ƁD

	@section SPIDAR SPIDAR
	�������΂炭���܂���������....

	@section NewProject �V�����v���W�F�N�g�����
	�����p�ɐV�����v���W�F�N�g�����܂��D<br>
	�v���O�����ɕύX�������Ă����ꍇ�́C
	�V�����v���W�F�N�g����邱�Ƃ������߂��܂��D<br>
	����́C�ȒP�ȕ��@�Ƃ������ƂŊ����̃v���W�F�N�g�iSpringhead/src/Samples/DynaHaptic�j��
	�R�s�[���Ė��O�����ύX����Ƃ������@����邱�Ƃɂ��܂��D
		-# VSS���� $/Project/Springhead/src/Samples/DynaHaptic �ȉ����擾����D
		-# DynaHaptic�t�H���_���R�s�[���C
		�K���ȂƂ���iSamples�̉��ł����Ǝv���܂��j�ɓ\��t����D
		-# �\��t�����t�H���_���̂��ׂẴt�@�C���̓ǂݎ���p�̃`�F�b�N���͂����C
		�t�@�C�����y�ъe�t�@�C���̒��ɓo�ꂷ�� "DynaHaptic" ��
		���ׂĐV�����v���W�F�N�g���ɒu������D<br>
		.dsp, dsw, rc, scc�t�@�C���Ȃ񂩂��e�L�X�g�G�f�B�^�ŊJ���Ēu�����Ă��������D		-# �r���h���Ď��s����D
		�r���h�����܂������Ȃ����́C���O�����ׂĕύX����Ă��邩�C
		�܂���@ref Execution�̃r���h�̂Ƃ�����m�F���Ă��������D


*/

#endif