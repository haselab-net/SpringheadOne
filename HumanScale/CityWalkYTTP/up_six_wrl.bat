copy debug\*.exe .
if "%1"=="exec" goto exec
for %%i in (00 01 02 03 04 05 06 07 10 11 12 13 14 15 16 17 20 21 22 23 30 31 32 33) do %0 exec %%i
goto end
:exec
set HOST=VPC%2
set DEST=\\%HOST%\C\Home\Demo\Project\HumanScale\CityWalk
mkdir %DEST%
copy 1.wrl %DEST%
copy 2.wrl %DEST%
copy 3.wrl %DEST%
copy 4.wrl %DEST%
copy 5.wrl %DEST%
copy pre.wrl %DEST%
:end
