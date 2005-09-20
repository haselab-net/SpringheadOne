del /S /Q Pack
mkdir Pack
cd Pack
call ..\..\LVRM\archive.bat
mkdir LVRM
cd LVRM
lha32 x ..\..\..\LVRM\LVRM.lzh
cd ..

mkdir Dyna
cd Dyna
mkdir SOLID
cd SOLID
copy ..\..\..\SOLID\*.* .
mkdir doc
copy ..\..\..\SOLID\doc\*.* .\doc\
mkdir include
copy ..\..\..\SOLID\include\*.* .\include\
mkdir sample
copy ..\..\..\SOLID\sample\*.* .\sample\
mkdir src
copy ..\..\..\SOLID\src\*.* .\src\
cd ..

mkdir WrlWalk
cd WrlWalk
copy ..\..\..\WrlWalk\*.* .
cd ..\..
lha32 u -rx Dyna.lzh
