rem Project以下をVSSから取得します

cd ..\..
ss Workfold $/Project .
ss Get $/Project -R > Springhead\test\log\GetVSS.log
cd Springhead\test
