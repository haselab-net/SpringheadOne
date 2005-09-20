call LVRMDoc.bat %1
cd Base
call BaseDoc.bat %1
cd ..\WinCmp
call WinCmpDoc.bat %1
cd ..\Device
call DeviceDoc.bat %1
cd ..\HIS
call HapticDoc.bat %1
cd ..\Render
call RenderDoc.bat %1
cd ..\SceneGraph
call SceneGraphDoc.bat %1
cd ..\WinBasis
call WinBasisDoc.bat %1
