call vcvars32.bat

cd ..\..\..\boost_libs\regex\build
nmake -fvc7.mak
nmake -fvc7.mak install
