@echo off
@echo ���C�u�������}�[�W���܂��DVisualC++�̃��C�u�����Ǘ��c�[�� LIB.EXE �Ƀp�X��ʂ��Ă����Ă��������D

: �r���h�\���ɂ��킹�����C�u�������̃T�t�B�b�N�X��ݒ�
if "%1"=="RELEASE" set EXT=
if "%1"=="DEBUG" set EXT=D
if "%1"=="MFCRELEASE" set EXT=M
if "%1"=="MFCDEBUG" set EXT=MD
if "%2"=="7" set EXT=7%EXT%

: �o�̓t�@�C���Ɠ��̓t�@�C��
set OUTPUT=libPaint%EXT%.lib
set LIBPATH=..\lib\%1%2
set LIBS=paintlib.lib libjpeg.lib libpng.lib libtiff.lib libungif.lib paintlib.lib

if "%LIBS%"=="" echo ���ϐ��p�������̋󂫂��s�����Ă��܂��D
if "%LIBS%"=="" echo �󂫂𑝂₵�Ă�����x���s���܂��D
if "%LIBS%"=="" echo ���̃��b�Z�[�W�������ĕ\�������ꍇ�́C
if "%LIBS%"=="" echo �R�}���h�v�����v�g�̐ݒ���C�����Ă��������D
if "%LIBS%"=="" command /e:4096 /c%0 %1
if not "%LIBS%"=="" LIB /LIBPATH:"%LIBPATH%" /OUT:%OUTPUT% %LIBS%
