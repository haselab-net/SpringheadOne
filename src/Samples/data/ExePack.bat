del ..\SpringheadExePack.tar.gz
copy ..\..\..\..\Experiments\Beetle\MFCRelease\Beetle.exe .
mkdir beetle
xcopy /Y /S ..\..\..\..\Experiments\Beetle\data beetle
copy ..\MFCD3DDyna\MFCRelease\MFCD3DDyna.exe .
minitar -c -z9 ../SpringheadExePack.tar.gz *.*
