rem Springhead以下をVSSから取得する
rem SVNへの移行に伴って使用されなくなります jumius

cd ..
ss Decloak $/Project/Springhead/src/boost
ss Decloak $/Project/Springhead/src/boost_libs
ss Workfold $/Project/Springhead .
ss Get $/Project/Springhead -R > test\log\GetVSS.log
cd test
