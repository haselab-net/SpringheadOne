rem Springhead以下をVSSから取得する

cd ..
ss Decloak $/Project/Springhead/src/boost
ss Decloak $/Project/Springhead/src/boost_libs
ss Workfold $/Project/Springhead .
ss Get $/Project/Springhead -R > test\log\GetVSS.log
cd test
