#ifndef Spr_XFILE_H
#define Spr_XFILE_H

/** @page XFile X�t�@�C��

	@section WhatXFile X�t�@�C���Ƃ�
	�uX�t�@�C���v�Ƃ́C�t�@�C������ .x �g���q���t�����t�@�C�����w���C
	Direct3D�ɂ����郂�f���f�[�^�̕W���t�H�[�}�b�g�ł��D<br>
	3D�̌`��f�[�^�C�}�e���A���C�e�N�X�`���Ȃǂ��e���v���[�g�Ƃ��Ē�`����C
	����Ƀ��[�U�ɂ���Ď��R�Ƀe���v���[�g�̒�`�̊g�����s�����Ƃ��ł��܂��D
	
	@section XFileHowToEdit �ҏW���@
	- 3D�̌`��f�[�^�C�}�e���A���Ȃ�<br>
	3ds max �� Metasequoia �Ȃǂ�X�t�@�C�����o�͂ł���s�̂̃\�t�g���g���ĕҏW���܂��D

	- �����C�͊o<br>
	X�t�@�C�����������Ȃǂ̃e�L�X�g�G�f�B�^�ŊJ���āC	
	�f�[�^�𒼐ړ��͂��ĕҏW���܂��D<br><br>

	@subsection template �e�f�[�^�^�̋L�q�e���v���[�g
	- X�t�@�C���W���̃e���v���[�g�ɂ��Ă�<A href=http://www.microsoft.com/japan/msdn/library/default.asp?url=/japan/msdn/library/ja/jpdx8_c/hh/directx8_c/_dx_x_file_templates_graphicsxof.asp>Microsoft DirectX 8.0 X�t�@�C�� �e���v���[�g</A>�ɏ����Ă���܂��D

	- Springhead�Ŋg����`�����e���v���[�g�ɂ��ẮC
	<A href=../ImpD3D/SGD3D.cpp>SGD3D.cpp</A>�̒���D3FileDoc::RegisterTemplates()�ɁC<br>
	@verbatim

	"template �^�� {\n"
	"�L�[\n"
	"�^ �f�[�^�̖��O;\n"
	"      :           " 
	"      :           " 
	"[�Q��]\n"
	"}\n"@endverbatim<br>
	�̂悤�Ȍ`�Œ�`����Ă��܂��D
	��Ƃ��āC���̃I�u�W�F�N�g�̃f�[�^�����e���v���[�g Solid �ł́C
	�ȉ��̂悤�Ȓ�`�ɑ΂��C<br>
	@verbatim

 <���̃f�[�^�̒�`����>

	"template Solid {\n"
	" <A9C20358-EDEB-4c0b-97AB-37D330B6BFA4>\n"
	" FLOAT mass;\n"
	" Matrix3x3 inertia;\n"
	" Vector velocity;\n"
	" Vector angularVelocity;\n"
	" Vector center;\n"
	" [Frame]\n"
	"}\n"@endverbatim<br>
	X�t�@�C���ł́C<br>
	@verbatim

 <X�t�@�C���ł̍��̃f�[�^�̋L�q>

    Solid soSpinTop{
      2.5;                    # ����
      3.0, 0.0, 0.0,              
      0.0, 1.0, 0.0,          # �����e���\��
      0.0, 0.0, 3.0;;
      0.0; 0.0; 0.0;;         # �������x
      0.01; 16.0; 0.0;;       # �����p���x
      0.0; 0.1; 0.0;;         # �d�S�i���b�V���̌��_����̑��Έʒu�j
      {frSpinTop}             # �t���[���ւ̎Q��
                              # (�����̃t���[�����c���[�Ƃ��Ď��ꍇ��
                              # �g�b�v�t���[�����w���j
    }@endverbatim<br>
	�̂悤�ɋL�q����܂��D

	- �T���v���t�@�C���Ƃ��ăR�}(<A href=../../Samples/data/TopRotYwithComment.x>TopRotYwithComment.x</A>)�ɃR�����g��t���܂����̂ł��Q�Ƃ��������D<br>

*/

#endif