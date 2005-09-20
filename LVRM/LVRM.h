#ifndef LVRM_LVRM_H
#define LVRM_LVRM_H
/**	@mainpage Lesser VR Module library
@section background �w�i�ƖړI
LVRM���C�u�����́C���H�卲�����ō����VR�V�X�e���̃\�t�g�E�G�A�����o�C�X���āC���C�u�������\�ȕ��������C�u�����ɂ������̂ł��DVR�̃��C�u�����ɂ� VR Juggler ��HIP �Ȃǂ��낢�날��܂����C�����̃��C�u�����́C���̃��C�u�����S�����g����VR�A�v���P�[�V��������邱�Ƃ�O��Ƃ��Ă���C�܂ݐH���͋�����܂���D

���̃��C�u�����ł́C�ł������܂ݐH�������₷���悤�ɁC�������ċ@�\�����Ȃ��p�b�P�[�W�̏W���Ƃ��Ď�������悤�ɂ��Ă��܂��D

Unix�̍l�����ɁCless is more �Ƃ������̂�����܂��D����́C�@�\�����Ȃ������ȃV�X�e���̕����C���@�\�ȋ���V�X�e�������������₷���C�ė��p�ł���̂ŁC�D��Ă���Ƃ������Ƃ�����킵�Ă��܂��DLVRM�Ƃ������O�́CLesser Virtual Reality Module �̓������ł��D�@�\�I�ɂ͗���Ă��ď����ȃ��W���[���Ń��C�u�������\�����邱�ƂŁC�������₷�����p���₷�����C�u������ڎw���Ă��܂��D

@section lib ���C�u�����̍\��
@subsection structure �p�b�P�[�W
LVRM���C�u�����͈ȉ��̃p�b�P�[�W(����̃N���X���W�܂������C�u����)����\������܂��D
 <table>
  <tr>
   <td> �p�b�P�[�W�� </td>
   <td> �@�\ </td>
   <td> ���C�u�����t�@�C���̏o����ꏊ </td>
  </tr> <tr>
   <td> <a href = "../Base/html/index.html"> Base </a> </td>
   <td> ��{�����C�s��E�x�N�g���C�f�o�b�O�x���C���[�e�B���e�B </td>
   <td> LVRM/Base/Base.lib </td>
  </tr> <tr>
   <td> <a href = "../SceneGraph/html/index.html"> SceneGraph </a> </td>
   <td> VRML97�����[�h���C�V�[���O���t���\�z���� </td>
   <td> LVRM/SceneGraph/SceneGraph.lib </td>
  </tr> <tr>
   <td> <a href = "../Render/html/index.html"> Render </a> </td>
   <td> OpenGL�A�v���p�̃t���[�����[�N </td>
   <td> LVRM/Render/Render.lib </td>
  </tr> <tr>
   <td> <a href = "../Device/html/index.html"> Device </a> </td>
   <td> D/A�EA/D�E�J�E���^�{�[�h�Ȃǂ̃f�o�C�X�̒��ۉ� </td>
   <td> LVRM/Device/Device.lib </td>
  </tr> <tr>
   <td> <a href = "../HIS/html/index.html"> Haptic </a> </td>
   <td> �͊o�񎦑[�u(SPIDAR)�̐��� </td>
   <td> LVRM/HIS/HIS.lib </td>
  </tr> <tr>
   <td> <a href = "../WinBasis/html/index.html"> WinBasis </a> </td>
   <td> Windows�̊�{�@�\�̃��b�p�[ </td>
   <td> LVRM/WinBasis/WinBasis.lib </td>
  </tr> <tr>
   <td> <a href = "../WinDX/html/index.html"> WinDX </a> </td>
   <td> DirectX�̋@�\�̃��b�p�[ </td>
   <td> LVRM/WinDX/WinDX.lib </td>
  </tr> <tr>
   <td> LVRM </a> </td>
   <td> �S�p�b�P�[�W����ɂ܂Ƃ߂����� </td>
   <td> LVRM/LVRM.lib </td>
  </tr>
 </table>

@subsection depend �p�b�P�[�W�̈ˑ��֌W
����N���XA���ʂ̃N���XB�̃|�C���^�������Ă���ꍇ�ȂǁCA��B���Ȃ���Ύg���܂���D���̂悤�ȏ󋵂�A��B�Ɉˑ����Ă���ƌ����܂��D�p�b�P�[�W�ɂ͂������̃N���X������܂����C����p�b�P�[�W�ɓ����Ă���N���X�͂��̃p�b�P�[�W�̒��̕ʂ̃N���X�Ɉˑ����Ă��邱�Ƃ������ł��D

�܂��C����p�b�P�[�WA�̃N���X���ʂ̃p�b�P�[�WB�̃N���X�Ɉˑ����Ă��邱�Ƃ�����܂��D�Ⴆ�΁CDevice�p�b�P�[�W�̃N���X��Base�p�b�P�[�W��WinBasis�p�b�P�[�W�̃N���X�Ɉˑ����Ă���̂ŁCBase��WinBasis�Ȃ��ł͎g���܂���D�t�ɁCBase�̃N���X�́CBase�p�b�P�[�W����������Ύg�����Ƃ͂ł��܂��D

�p�b�P�[�W�̈ˑ��֌W��}�ɂ���Ǝ��̂悤�ɂȂ�܂��D
 <ul>
  <li> Base
  <ul>
    <li> WinBasis
    <ul>
      <li> Device
      <ul>
        <li> Haptic
      </ul>
    </ul>
    <li> SceneGraph
    <li> Render
  </ul>
 </ul>

@section usage �g����
�r���h�ł����������Ă��������D�r���h�ɂ�VisualC++���g�����Ƃ�z�肵�Ă��܂��Dgcc��bcc32��bcb�ł��\�z�ł��Ȃ����Ƃ͂Ȃ��ł����Cmakefile�������ō��K�v������܂��Dbcb�̃v���W�F�N�g�t�@�C���̓����e�i���X���Ă��܂���D

@subsection build ���̍���
<ol>
  <li> �\�[�X�c���[�̓��� <br>
       VSS�� $/Project/LVRM �ȉ����擾����Ȃǂ��āC�\�[�X�c���[������Ă��������D
  <li> ���ϐ��̐ݒ�
    <li> ���ϐ���ݒ肵�܂�<br>
    ���ϐ� LVRM_ROOT �� LVRM���擾�����f�B���N�g����ݒ肵�ĉ���
    ���D�Ⴆ�Ύ��̂悤�Ȑݒ�����邱�ƂɂȂ�܂��D<br>
    <ul>
      <li>Win98: ����1�s��Autoexec.bat�ɉ�����<br>
         set LMVR_ROOT=C:\Home\C\Project\LVRM
      <li>Win2000: ���ϐ��Ɏ��̐ݒ������<br>
         �}�C�R���s���[�^�|�v���p�e�B�|�ڍׁ|���ϐ��|�V�K��I�ԁD<br>
         ���ϐ��� LVRM_ROOT �� �l C:\Home\C\Project\LVRM ��ݒ肷��D
    </ul>
</ol>

@subsection app �A�v���P�[�V�����̍���
<ol>
  <li> �R���\�[���A�v���̃v���W�F�N�g�̍쐬<br>
       ����Windows�炵���A�v���P�[�V��������肽���ꍇ�ȊO�́C
       �R���\�[���A�v�����쐬���邱�Ƃ������߂��܂��D
       OpenGL��GLUT����Ďg�p����ꍇ���R���\�[���A�v�����œK�ł��D
    <ol>
      <li> [�t�@�C��]-[�V�K�쐬]-(�v���W�F�N�g)-(Win32 Console Application) ��I�����C�K���ȃv���W�F�N�g
           ���쐬����D
      <li> (��̃v���W�F�N�g)��I��(�I��)���N���b�N����D
      <li> [�\��]-[���[�N�X�y�[�X]��I�сC���[�N�X�y�[�X(Class View �� File View)��\������D
      <li> (FileView)��I�сC�v���W�F�N�g�Ƀ\�[�X�t�@�C����ǉ�����D<br>
              �v���O�����͒ǉ������\�[�X�t�@�C���� main() �֐�����n�܂�D
    </ol>
  <li> �R���\�[���A�v���̃C���N���[�h�t�@�C���ƃ��C�u�����̐ݒ�<br>
          LVRM�̃N���X���g�p���邽�߂ɂ́C�C���N���[�h�t�@�C�����C���N���[�h���C
          ���C�u�������v���W�F�N�g�ɒǉ����Ȃ���΂Ȃ�܂���D
    <ol>
      <li> [�v���W�F�N�g]-[�ݒ�]-(�ݒ�̑Ώ�:�S�Ă̍\��)-(C/C++)-(�J�e�S���F�v���v���Z�b�T)
           ��I�����C�C���N���[�h�t�@�C���̃p�X�ɁC<br>
           , \$(LVRM_ROOT)<br>
           �Ƃ������������ǉ�����D
      <li> �v���W�F�N�g�Ɏg�p����p�b�P�[�W�Ƃ��̃p�b�P�[�W���ˑ�����p�b�P�[�W�̃��C�u������
         �ǉ�����D���̂Ƃ��CDebug�ł�Release�łŃ��C�u�������قȂ�̂Œ��ӂ���D
         �Ⴆ�΁CBase�p�b�P�[�W�́CDebug�ł�BaseD.lib�CRelease�ł�Base.lib�ƂȂ�D��̓I�ɂ�
         �ȉ��̐ݒ������D
        <ol>
          <li> �v���W�F�N�g�ɁCDebug�ŁCRelease�ŗ����̃��C�u������ǉ�����D
          <li> [�v���W�F�N�g]-[�ݒ�]-(�ݒ�̑Ώ�:Win32 Debug ��) ��I�����C�ǉ��������C�u������
             �I������D
          <li> (���)-(���̃t�@�C�����r���h���Ȃ�)�`�F�b�N�{�b�N�X���g���āC
             Debug�łł�Debug�ł̃��C�u�����CRelease�łł�Release�ł̃��C�u�������r���h�����
             �悤�ɐݒ肷��D
        </ol>
    </ol>
  <li> �C���N���[�h�t�@�C���̃C���N���[�h�Ɩ��O��Ԃ̐ݒ�<br>
     �����̏����\�[�X�t�@�C���Ń��C�u�����̃N���X���g�����߂ɂ́C���C�u�����̃w�b�_�t�@�C��
     ���C���N���[�h���C���O��Ԃ𓱓����Ȃ���΂Ȃ�܂���D
    <ol>
      <li> �w�b�_�̃C���N���[�h
        �g�p�������N���X���܂܂��w�b�_�t�@�C���� LVRM ����̑��΃p�X�ŃC���N���[�h���܂��D
        �Ⴆ��<br>
        #include <Base/Affine.h><br>
        �Ƃ��܂��D
      <li> ���O��Ԃ̓���<br>
        LVRM�̃N���X�͖��O���LVRM�̒��Ő錾����Ă��܂��D
        �����ŁC�C���N���[�h�̌�Ŏ��̂悤�ɂ��܂��D<br>
        using namespace LVRM;<br>
        ��������ƁCAffinef�N���X���g����悤�ɂȂ�܂��D
        ����������ɁCAffinef �̂Ƃ���𖼑O��ԕt���� LVRM::Affinef �Ə���
        �Ă�OK�ł��D
    </ol>
</ol>

@subsection libfile lib�t�@�C���̖����Ǝg����
C��C++�̃\�[�X���R���p�C������ƃI�u�W�F�N�g�t�@�C��(�t�@�C�����̍Ōオ.obj �� .o �̃t�@�C��)���ł��܂��D����������N�����(�Ȃ���)���s�t�@�C��(�t�@�C�����̍Ōオ.exe�̃t�@�C��)���ł��܂��D

���C�u�����t�@�C��(lib�t�@�C���C�t�@�C�����̍Ōオ.lib�̃t�@�C��)�́C���s�t�@�C���������ɁC�I�u�W�F�N�g�t�@�C�������̂܂܂P�ɂ܂Ƃ߂����̂ł��D���C�u�����t�@�C��������Ă����ƁC�I�u�W�F�N�g�t�@�C�����ЂƂЂƂ����N����K�v���Ȃ��̂ŁC�v���W�F�N�g�̐ݒ肪�y�ɂȂ�܂��D

LVRM�́C�p�b�P�[�W���Ƃ�Lib�t�@�C����p�ӂ��Ă��܂��D�eLib�t�@�C���́C�f�o�b�O��/�����[�X�ŁCMFC�̂���/�Ȃ� �ňႤ�t�@�C���ɂȂ�܂��D�����̃��C�u������ Base.lib BaseD.lib BaseM.lib BaseMD.lib �̂悤�ɖ��O��ς��āC�e�p�b�P�[�W�̃��[�g�m�[�h�ɂł��܂��D(�Ⴆ�΁CC:\Home\C\Project\LVRM\Base\Base.lib �ɂł��܂��D)

�����̃p�b�P�[�W���̃��C�u�������P�ɂ܂Ƃ߂����̂�LVRM.lib(D,M,MD������܂�)�ł��D����́CLVRM�̃f�B���N�g��(�Ⴆ��C:\Home\C\Project\LVRM\LVRM.lib)�ɂł��܂��D�����I�u�W�F�N�g���܂܂�郉�C�u�����������N����ƃG���[�ɂȂ�܂��DLVRM.lib�������N������CBase.lib�Ȃǂ������N���Ă͂����܂���D

@subsection doc �h�L�������g�����菇
�}�j���A���́C�h�L�������g�����c�[�� doxygen �Ő����ł��܂��D<br>
�}�j���A���̐������@:
  <ol>
    <li> LVRM�̃C���X�g�[��������D
    <li> LVRM/AllDoc.bat �����s����D
    <li> LVRM/html/index.html �Ƀh�L�������g�̃g�b�v�y�[�W�����������D
  </ol>

@section file �t�@�C���E�t�H���_�\��
LVRM�̃\�[�X�c���[�ɂ́CC++�̃\�[�X�ȊO�Ƀr���h�ɕK�v�ȃc�[����f�o�C�X�h���C�o�̃o�C�i�����܂܂�Ă��܂��D
<dl>
  <dt>LVRM/bin
  <dd>make flex bison �Ȃǂ̃r���h�ɕK�v�ȃc�[��������܂��D
     <table><tr><td>flex</td><td>
     SceneGraph�p�b�P�[�W��VRML���[�_�[�́Clex�Ƃ��������̓G���W�������c�[����p���Ă��܂��Dflex��GNU���񋟂���lex�ł��D
     </td></tr><tr><td>bison</td><td>
     SceneGraph�p�b�P�[�W��VRML���[�_�[�́Cyacc�Ƃ����\����̓G���W�������c�[����p���Ă��܂��Dbison��GNU���񋟂���yacc�ł��D
     </td></tr><tr><td>make</td><td>
     make �� �t�@�C���̍����ƈˑ��֌W�������Ă���t�@�C�� makefile ��ǂ݁C
     �ˑ��֌W�ɏ]���ăt�@�C���𐶐�����c�[���ł��D
     flex��bison�𓮂������߂Ɏg�p���Ă��܂��D
     </td></tr><tr><td>doxygen</td><td>
     doxygen��C++�Ȃǂ̃\�[�X�R�[�h���烊�t�@�����X�}�j���A��
     �𐶐�����c�[���ł��D���̃h�L�������g��doxygen���g�p���Đ������Ă��܂��D
      @ref doc �����Q�Ƃ��������D
     </td></tr></table>
  <dt>LVRM/bin/drivers
  <dd>�f�o�C�X�h���C�o������܂��D�Ĕz�z���ł��Ȃ����̂�����܂��D
  �������O���Ɍ��J����ꍇ�́C���̕����������K�v������܂��D
</dl>
  
@section license �z�z�E�g�p����
  <ul>
    <li>LVRM/SceneGraph/Wrl2View �ȉ��� GPL�Ŕz�z����Ă��� Wrl2View �Ƃ���VRML2 Viewer �����ς������̂Ȃ̂ŁCGPL�ł��D
    <li>LVRM/bin �ȉ��̃o�C�i���́C�R���p�C���ɕK�v�ȃc�[���ƃn�[�h�E�F�A�̃h���C�o�ł��DGIVEIO.SYS �̔z�z�E�g�p�����͕s���ł��D���̑��̃c�[��
    ��GPL�ł��D
    <li>���̂ق��͈ȉ��̂Ƃ���Ƃ��܂��D�Ĕz�z����ꍇ�́C����҂̏����E�A��������ρE�폜���Ȃ��ł��������D���I�ȉ�ЂŃ\�t�g�E�G�A�������l�ɂ����R�Ɏg�p�ł���悤�ɂ������̂ŁCGPL�ELGPL�łȂ����C�Z���X�Ŕz�z���܂��D�������GPL�ELGPL�ɉ��ς��čĔz�z���Ă��������Ă����\�ł��D���̃\�[�X�R�[�h��p���������𔭕\����ꍇ�ɂ́C�Q�l������URL���܂߂Ă��������D
  </ul>

@section contanct �A����
���J�� ���� <hase@hi.pi.titech.ac.jp>
�����H�Ƒ�w �����H�w������          ��226-8503 ���l�s�΋撷�Óc�� 4259
�����E���r������

*/

#include "Base/Base.h"
#include "Device/Device.h"
#include "HIS/Haptic.h"
#include "Render/Render.h"
#include "Render/Render.h"

#endif
