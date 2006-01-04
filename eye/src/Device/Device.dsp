# Microsoft Developer Studio Project File - Name="Device" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Device - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Device.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Device.mak" CFG="Device - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Device - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "Device - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Device - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Device - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/Device", MCKAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Device - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Gr /W3 /GX /O2 /Ob2 /I ".." /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Device.lib"

!ELSEIF  "$(CFG)" == "Device - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I ".." /I "$(SPRINGHEAD)\src" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "DEBUG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"DeviceD.lib"

!ELSEIF  "$(CFG)" == "Device - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Device___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "Device___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"DeviceMD.lib"

!ELSEIF  "$(CFG)" == "Device - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Device___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "Device___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /GR- /GX /Zi /Ox /Ot /Og /Oi /Op /Oy- /Ob2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /FR /YX /FD /c
# SUBTRACT CPP /Oa
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"DeviceM.lib"

!ENDIF 

# Begin Target

# Name "Device - Win32 Release"
# Name "Device - Win32 Debug"
# Name "Device - Win32 MFC Debug"
# Name "Device - Win32 MFC Release"
# Begin Source File

SOURCE=.\Device.h
# End Source File
# Begin Source File

SOURCE=.\DRADAPIO.cpp
# End Source File
# Begin Source File

SOURCE=.\DRADAPIO.h
# End Source File
# Begin Source File

SOURCE=.\DRContecIsaCounter.cpp
# End Source File
# Begin Source File

SOURCE=.\DRContecIsaCounter.h
# End Source File
# Begin Source File

SOURCE=.\DRContecIsaDa.cpp
# End Source File
# Begin Source File

SOURCE=.\DRContecIsaDa.h
# End Source File
# Begin Source File

SOURCE=.\DREzUSB.h
# End Source File
# Begin Source File

SOURCE=.\DRNittaForce.cpp
# End Source File
# Begin Source File

SOURCE=.\DRNittaForce.h
# End Source File
# Begin Source File

SOURCE=.\DRRealDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\DRRealDevice.h
# End Source File
# Begin Source File

SOURCE=.\DRRealDevicePool.cpp
# End Source File
# Begin Source File

SOURCE=.\DRRealDevicePool.h
# End Source File
# Begin Source File

SOURCE=.\DRTokin3D.cpp
# End Source File
# Begin Source File

SOURCE=.\DRTokin3D.h
# End Source File
# Begin Source File

SOURCE=.\DRUPP.CPP
# End Source File
# Begin Source File

SOURCE=.\DRUpp.h
# End Source File
# Begin Source File

SOURCE=.\DRUsb20Sh4.cpp
# End Source File
# Begin Source File

SOURCE=.\DRUsb20Sh4.h
# End Source File
# Begin Source File

SOURCE=.\DRUsb20Simple.cpp
# End Source File
# Begin Source File

SOURCE=.\DRUsb20Simple.h
# End Source File
# Begin Source File

SOURCE=.\DRUsbH8Simple.cpp
# End Source File
# Begin Source File

SOURCE=.\DRUsbH8Simple.h
# End Source File
# Begin Source File

SOURCE=.\DRUsbH8SimpleDef.h
# End Source File
# Begin Source File

SOURCE=.\DVADBase.h
# End Source File
# Begin Source File

SOURCE=.\DVCounterBase.h
# End Source File
# Begin Source File

SOURCE=.\DVDABase.h
# End Source File
# Begin Source File

SOURCE=.\DVDeviceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DVDeviceManager.h
# End Source File
# Begin Source File

SOURCE=.\DVForceBase.h
# End Source File
# Begin Source File

SOURCE=.\DVOrientationBase.h
# End Source File
# Begin Source File

SOURCE=.\DVPIOBase.h
# End Source File
# Begin Source File

SOURCE=.\DVVirtualDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\DVVirtualDevice.h
# End Source File
# Begin Source File

SOURCE=.\DVVirtualDevicePool.cpp
# End Source File
# Begin Source File

SOURCE=.\DVVirtualDevicePool.h
# End Source File
# Begin Source File

SOURCE=.\Tuadapio.h
# End Source File
# End Target
# End Project
