REM CommandInterpreter: $(COMSPEC)
if exist c:\mssdk\lib set SDKLIB=c:\mssdk\lib
if exist c:\dxsdk\lib set SDKLIB=c:\dxsdk\lib
echo SDKLIB=%SDKLIB%

set BCBSDK=%SDKLIB%\bcb
set BCBLIB=C:\Progra~1\Borland\CBuilder6\Lib
set LOGFILE=WinDX8BCBLib.log

if exist %LOGFILE% del %LOGFILE%
if not exist %BCBSDK% mkdir %BCBSDK%
for %%i in (%SDKLIB%\*.lib) do if not exist %BCBSDK%\%%~ni%%~xi coff2omf -lib:ms %%i %BCBSDK%\%%~ni%%~xi >> %LOGFILE%

