:@echo off
@echo ���C�u�������}�[�W���܂��DVisualC++�̃��C�u�����Ǘ��c�[�� LIB.EXE �Ƀp�X��ʂ��Ă����Ă��������D

: �r���h�\���ɂ��킹�����C�u�������̃T�t�B�b�N�X��ݒ�
if "%1"=="RELEASE" set EXT=
if "%1"=="DEBUG" set EXT=D
if "%1"=="MFCRELEASE" set EXT=M
if "%1"=="MFCDEBUG" set EXT=MD
if "%2"=="7" set EXT=7%EXT%

: �o�̓t�@�C���Ɠ��̓t�@�C��
set OUTPUT=WinDX%EXT%.lib
set INPUT=%1%2\WinDXS.lib

: DirectX�̃��C�u�����݂̍菈�D
if exist c:\mssdk set DXROOT=c:\mssdk
if exist d:\mssdk set DXROOT=d:\mssdk
if exist c:\dxsdk set DXROOT=c:\dxsdk
if exist d:\dxsdk set DXROOT=d:\dxsdk
if exist c:\dx90sdk set DXROOT=c:\dx90sdk
if exist d:\dx90sdk set DXROOT=d:\dx90sdk

if not "%DXROOT%"=="" set DXLIB=%DXROOT%\lib
if exist %DXLIB%\x86\nul set DXLIB=%DXLIB%\x86

: DirectX�̃o�[�W����
set DXVER=9


if DXVER==9 set DINPUT=dinput.lib
if DXVER==8 set DINPUT=dinput8.lib
set LIBS=ddraw.lib d3d%DXVER%.lib d3dx%DXVER%.lib dsound.lib strmiids.lib Quartz.lib d3dxof.lib %DINPUT% setupapi.lib dxguid.lib

: DirectShow �� BaseClass���C�u����������΂�����ǉ�
if "%EXT%"=="D" if exist %DXROOT%\samples\Multimedia\DirectShow\BaseClasses\Debug\strmbasd.lib set DSHOW=%DXROOT%\samples\Multimedia\DirectShow\BaseClasses\Debug\strmbasd.lib
if "%EXT%"=="" if exist %DXROOT%\samples\Multimedia\DirectShow\BaseClasses\Release\strmbase.lib set DSHOW=%DXROOT%\samples\Multimedia\DirectShow\BaseClasses\Release\strmbase.lib
set LIBS=%LIBS% %DSHOW%

if "%LIBS%"=="" echo ���ϐ��p�������̋󂫂��s�����Ă��܂��D
if "%LIBS%"=="" echo �󂫂𑝂₵�Ă�����x���s���܂��D
if "%LIBS%"=="" echo ���̃��b�Z�[�W�������ĕ\�������ꍇ�́C
if "%LIBS%"=="" echo �R�}���h�v�����v�g�̐ݒ���C�����Ă��������D
if "%LIBS%"=="" command /e:4096 /c%0 %1
if not "%LIBS%"=="" if not "%DXLIB%"=="" LIB /LIBPATH:"%DXLIB%" /OUT:%OUTPUT% %INPUT% %LIBS%
if not "%LIBS%"=="" if "%DXLIB%"=="" LIB /OUT:%OUTPUT% %INPUT% %LIBS%
