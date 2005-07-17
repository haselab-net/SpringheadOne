if exist pack\springhead rmdir /S /Q pack\springhead
if not exist pack mkdir pack
cd pack

svn export svn+ssh://sprsvn/export/spr/svn/repository/Springhead/trunk ./Springhead

rmdir /S /Q springehead\src\samples\data\Tachikoma

minitar -c -z9 ../Springhead.tar.gz *.*
cd ..
pause
