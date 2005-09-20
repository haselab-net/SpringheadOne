call ..\config\config.bat

if "%1"=="exec" goto exec
for %%i in (00 01) do %0 exec %%i
goto end
:exec
set HOST=VPC%2
echo > \\%HOST%\C\Home\Demo\Project\HumanScale\%ProjectName%\stop
:end
