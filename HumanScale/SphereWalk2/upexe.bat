copy debug\*.exe .
copy debug\*.exe c:\home\demo\project\humanscale\spherewalk2
if "%1"=="exec" goto exec
for %%i in (00 01 02 03 04 05 06 07 10 11 12 13 14 15 16 17 20 21 22 23 30 31 32 33) do %0 exec %%i
goto end
:exec
set HOST=VPC%2
set DEST=\\%HOST%\C\Home\Demo\Project\HumanScale\SphereWalk2
rem del %DEST%
rem mkdir %DEST%
rem mkdir %DEST%\data
copy SphereWalk.exe %DEST%
rem copy sf* %DEST%
rem xcopy /S data\* %DEST%\data
:end
