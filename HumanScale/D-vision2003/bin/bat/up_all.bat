call ..\..\config\config.bat


if "%1"=="exec" goto exec
for %%i in (00 01) do %0 exec %%i
goto end
:exec
set HOST=VPC%2
set DEST=\\%HOST%\C\Home\Demo\Project\HumanScale\%ProjectName%
mkdir %DEST%
mkdir %DEST%\Data
mkdir %DEST%\bin
mkdir %DEST%\config
copy ..\..\debug\%ProgramName% %DEST%
copy ..\..\*.dll %DEST%
xcopy ..\..\bin %DEST%\bin /e /c /h
xcopy ..\..\Data %DEST%\Data /e /c /h
xcopy ..\..\config %DEST%\config /e /c /h
:end
