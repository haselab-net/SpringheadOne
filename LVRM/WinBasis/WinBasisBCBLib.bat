REM CommandInterpreter: $(COMSPEC)
@echo off
set SDKLIB=c:\mssdk\lib
set BCBSDK=%SDKLIB%\bcb
set BCBLIB=C:\Progra~1\Borland\CBuilder5\Lib

if not exist %BCBSDK% mkdir %BCBSDK%
for %%i in (%SDKLIB%\*.lib) do if not exist %BCBSDK%\%%~ni%%~xi coff2omf -lib:st %%i %BCBSDK%\%%~ni%%~xi > NUL

del WinCmpBCB.lib
TLIB /P128 WinCmpBCB.lib +WinCmpSBCB.lib +%BCBSDK%\ddraw.lib +%BCBSDK%\d3d8.lib +%BCBSDK%\dsound.lib +%BCBLIB%\ws2_32.lib

del *.~???
