@echo off
@echo ���C�u�������}�[�W���܂��DVisualC++�̃��C�u�����Ǘ��c�[�� LIB.EXE �Ƀp�X��ʂ��Ă����Ă��������D

if "%1"=="RELEASE" set EXT=
if "%1"=="DEBUG" set EXT=D
if "%1"=="MFCRELEASE" set EXT=M
if "%1"=="MFCDEBUG" set EXT=MD
if "%2"=="7" set EXT=7%EXT%

set OUTPUT=Springhead%EXT%.lib
set INPUT=Base/Base%EXT%.lib Collision/Collision%EXT%.lib Device/Device%EXT%.lib
set INPUT=%INPUT% FileIO/FileIO%EXT%.lib Graphics/Graphics%EXT%.lib HIS/HIS%EXT%.lib
set INPUT=%INPUT% ImpD3D/ImpD3D%EXT%.lib Physics/Physics%EXT%.lib SceneGraph/SceneGraph%EXT%.lib
set INPUT=%INPUT% WinBasis/WinBasis%EXT%.lib WinDX/WinDX%EXT%.lib Framework/Framework%EXT%.lib
set INPUT=%INPUT% GraphicsGL/GraphicsGL%EXT%.lib

if "%INPUT%"=="" echo ���ϐ��p�������̋󂫂��s�����Ă��܂��D
if "%INPUT%"=="" echo �󂫂𑝂₵�Ă�����x���s���܂��D
if "%INPUT%"=="" echo ���̃��b�Z�[�W�������ĕ\�������ꍇ�́C
if "%INPUT%"=="" echo �R�}���h�v�����v�g�̐ݒ���C�����Ă��������D
if "%INPUT%"=="" command /e:4096 /c%0 %1
if not "%INPUT%"=="" LIB /OUT:%OUTPUT% %INPUT%
