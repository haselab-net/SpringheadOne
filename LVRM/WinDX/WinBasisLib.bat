@echo off
@echo ���C�u�������}�[�W���܂��DVisualC++�̃��C�u�����Ǘ��c�[�� LIB.EXE �Ƀp�X��ʂ��Ă����Ă��������D

if "%1"=="RELEASE" set EXT=
if "%1"=="DEBUG" set EXT=D
if "%1"=="MFCRELEASE" set EXT=M
if "%1"=="MFCDEBUG" set EXT=MD

set OUTPUT=WinBasis%EXT%.lib
set INPUT=%1\WinCmpS.lib
if exist c:\mssdk\lib set LIBS=dxguid.lib ddraw.lib d3d8.lib d3drm.lib d3dx.lib dsound.lib winmm.lib ws2_32.lib strmiids.lib
if not exist c:\mssdk\lib set LIBS=dxguid.lib ddraw.lib d3drm.lib dsound.lib winmm.lib ws2_32.lib strmiids.lib

if "%LIBS%"=="" echo ���ϐ��p�������̋󂫂��s�����Ă��܂��D
if "%LIBS%"=="" echo �󂫂𑝂₵�Ă�����x���s���܂��D
if "%LIBS%"=="" echo ���̃��b�Z�[�W�������ĕ\�������ꍇ�́C
if "%LIBS%"=="" echo �R�}���h�v�����v�g�̐ݒ���C�����Ă��������D
if "%LIBS%"=="" command /e:4096 /c%0 %1
if not "%LIBS%"=="" LIB /LIBPATH:"%SDKLIB%" /OUT:%OUTPUT% %INPUT% %LIBS%
