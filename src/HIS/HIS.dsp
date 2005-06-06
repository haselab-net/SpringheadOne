# Microsoft Developer Studio Project File - Name="HIS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=HIS - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "HIS.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "HIS.mak" CFG="HIS - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "HIS - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "HIS - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "HIS - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "HIS - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/HIS", WTBBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HIS - Win32 Release"

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
# ADD LIB32 /nologo /out:"HIS.lib"

!ELSEIF  "$(CFG)" == "HIS - Win32 Debug"

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
# ADD LIB32 /nologo /out:"HISD.lib"

!ELSEIF  "$(CFG)" == "HIS - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "HIS___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "HIS___Win32_MFC_Debug"
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
# ADD BASE LIB32 /nologo /out:"HISD.lib"
# ADD LIB32 /nologo /out:"HISMD.lib"

!ELSEIF  "$(CFG)" == "HIS - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HIS___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "HIS___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /GR- /GX /Zi /Ox /Ot /Og /Oi /Op /Oy- /Ob2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# SUBTRACT CPP /Oa
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"HIS.lib"
# ADD LIB32 /nologo /out:"HISM.lib"

!ENDIF 

# Begin Target

# Name "HIS - Win32 Release"
# Name "HIS - Win32 Debug"
# Name "HIS - Win32 MFC Debug"
# Name "HIS - Win32 MFC Release"
# Begin Source File

SOURCE=.\HIBase.cpp
# End Source File
# Begin Source File

SOURCE=.\HIBase.h
# End Source File
# Begin Source File

SOURCE=.\HIHapticDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\HIHapticDevice.h
# End Source File
# Begin Source File

SOURCE=.\HIMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\HIMouse.h
# End Source File
# Begin Source File

SOURCE=.\HIOrientation.cpp
# End Source File
# Begin Source File

SOURCE=.\HIOrientation.h
# End Source File
# Begin Source File

SOURCE=.\HIRUIBase.h
# End Source File
# Begin Source File

SOURCE=.\HIRUIKuma.cpp
# End Source File
# Begin Source File

SOURCE=.\HIRUIKuma.h
# End Source File
# Begin Source File

SOURCE=.\HIS.h
# End Source File
# Begin Source File

SOURCE=.\HISpidar2.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar2.H
# End Source File
# Begin Source File

SOURCE=.\HISpidar3.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar3.h
# End Source File
# Begin Source File

SOURCE=.\HISpidar4.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar4.h
# End Source File
# Begin Source File

SOURCE=.\HISpidar4Angle.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar4Angle.h
# End Source File
# Begin Source File

SOURCE=.\HISpidar4D.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar4D.h
# End Source File
# Begin Source File

SOURCE=.\HISpidar8.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar8.h
# End Source File
# Begin Source File

SOURCE=.\HISpidar8MotorTest.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar8MotorTest.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarCalc3DOF.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarCalc3DOF.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarCalc6DOF.cpp
# End Source File
# Begin Source File

SOURCE=.\HISPidarCalc6DOF.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarCalcBase.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarCalcBase.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarCanoe.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarCanoe.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6C.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6C.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6Jun.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6Jun.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6O.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6O.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6WB.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6WB.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6X.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6X.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6X2.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6X2.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6X3.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarG6X3.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarI7.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarI7.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarMotor.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarMotor.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarMotorAngle.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarMotorAngle.h
# End Source File
# Begin Source File

SOURCE=.\HIStepWalk.cpp
# End Source File
# Begin Source File

SOURCE=.\HIStepWalk.h
# End Source File
# Begin Source File

SOURCE=.\HITurnTable.cpp
# End Source File
# Begin Source File

SOURCE=.\HITurnTable.h
# End Source File
# End Target
# End Project
