set PATH=\\192.168.251.1\VSS\win32;%PATH%
set SSDIR=\\192.168.251.1\VSS
if exist pack del /F /S /Q pack
if not exist pack mkdir pack
cd pack
ss Decloak $/Project/Springhead/src/boost
ss Decloak $/Project/Springhead/src/boost_libs
ss Cloak $/Project/Springhead/src/samples/data/Tachikoma
ss Get $/Project/Springhead -R -W -I-
ss Decloak $/Project/Springhead/src/samples/data/Tachikoma
ss Cloak $/Project/Springhead/src/boost
ss Cloak $/Project/Springhead/src/boost_libs
minitar -c -z9 ../Springhead.tar.gz *.*
cd ..
