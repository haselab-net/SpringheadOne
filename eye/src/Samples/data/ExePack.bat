copy ..\DynaHaptic\MFCRelease\DynaHaptic.exe .
del /S /F /Y ..\..\..\pack\ExePack
mkdir ..\..\..\pack\ExePack
xcopy /I /S /Y /EXCLUDE:ExePack.exclude . ..\..\..\pack\ExePack
cd ..\..\..\pack\ExePack
minitar -c -z9 ../SpringheadExePack.tgz *.*
