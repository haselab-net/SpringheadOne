@echo off
@echo ���C�u�������}�[�W���܂��DVisualC++�̃��C�u�����Ǘ��c�[�� LIB.EXE �Ƀp�X��ʂ��Ă����Ă��������D

: �r���h�\���ɂ��킹�����C�u�������̃T�t�B�b�N�X��ݒ�
if "%1"=="RELEASE" set EXT=
if "%1"=="DEBUG" set EXT=D
if "%1"=="MFCRELEASE" set EXT=M
if "%1"=="MFCDEBUG" set EXT=MD

: �o�̓t�@�C���Ɠ��̓t�@�C��
set OUTPUT=Collision%EXT%.lib

set LIBS=%1\Collision.lib
: SOLID\SOLID%EXT%.lib Qhull\%1\Qhull.lib

if "%LIBS%"=="" echo ���ϐ��p�������̋󂫂��s�����Ă��܂��D
if "%LIBS%"=="" echo �󂫂𑝂₵�Ă�����x���s���܂��D
if "%LIBS%"=="" echo ���̃��b�Z�[�W�������ĕ\�������ꍇ�́C
if "%LIBS%"=="" echo �R�}���h�v�����v�g�̐ݒ���C�����Ă��������D
if "%LIBS%"=="" command /e:4096 /c%0 %1

if not "%LIBS%"=="" LIB /OUT:%OUTPUT% %LIBS%
