@echo off
if "%1"=="RELEASE" set SUFFIX=
if "%1"=="DEBUG" set SUFFIX=D
if "%1"=="MFCRELEASE" set SUFFIX=M
if "%1"=="MFCDEBUG" set SUFFIX=MD

set OUTPUT=LVRM%SUFFIX%.lib

set LIBS=Base\Base%SUFFIX%.lib Device\Device%SUFFIX%.lib HIS\HIS%SUFFIX%.lib
set LIBS=%LIBS% Render\Render%SUFFIX%.lib
set LIBS=%LIBS% WinBasis\WinBasis%SUFFIX%.lib
set LIBS=%LIBS% HIS\Fortran\LIBF77%SUFFIX%.lib
set LIBS=%LIBS% HIS\Fortran\LIBI77%SUFFIX%.lib
set LIBS=%LIBS% HIS\Fortran\QuadPR%SUFFIX%.lib

LIB -OUT:%OUTPUT% %LIBS%>Lib.log
