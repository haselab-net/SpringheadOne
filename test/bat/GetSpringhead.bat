rem Springhead�ȉ���VSS����擾����
rem SVN�ւ̈ڍs�ɔ����Ďg�p����Ȃ��Ȃ�܂� jumius

cd ..
ss Decloak $/Project/Springhead/src/boost
ss Decloak $/Project/Springhead/src/boost_libs
ss Workfold $/Project/Springhead .
ss Get $/Project/Springhead -R > test\log\GetVSS.log
cd test
