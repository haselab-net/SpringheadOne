set PATH=\\192.168.251.1\VSS\win32;%PATH%
set SSDIR=\\192.168.251.1\VSS
if exist pack del /F /S /Q pack
if not exist pack mkdir pack
cd pack
rem ss Decloak $/Project/Springhead/src/boost
rem ss Decloak $/Project/Springhead/src/boost_libs
rem ss Cloak $/Project/Springhead/src/samples/data/Tachikoma
rem ss Get $/Project/Springhead -R -W -I-
rem ss Decloak $/Project/Springhead/src/samples/data/Tachikoma
rem ss Cloak $/Project/Springhead/src/boost
rem ss Cloak $/Project/Springhead/src/boost_libs

svn export svn+ssh://sprserv/export/spr/svn/repository/Springhead/trunk ./Springhead

minitar -c -z9 ../Springhead.tar.gz *.*
cd ..
