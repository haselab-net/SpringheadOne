rem Project以下をVSSから取得します

rem …このファイルはすでに使われていないですか？

cd ..\..
ss Workfold $/Project .
ss Get $/Project -R > Springhead\test\log\GetVSS.log
cd Springhead\test
