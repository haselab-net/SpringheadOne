# Microsoft Developer Studio Project File - Name="Physics" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Physics - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Physics.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Physics.mak" CFG="Physics - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Physics - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "Physics - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Physics - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Physics - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/Physics", OOXAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Physics - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /W3 /vd0 /GX /Ox /Ot /Oa /Og /Oi /Ob2 /Gf /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# SUBTRACT CPP /Ow /Os /Gy
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Physics - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "$(SPRINGHEAD)\src" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "DEBUG" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"PhysicsD.lib"

!ELSEIF  "$(CFG)" == "Physics - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Physics___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "Physics___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Op /Ob2 /I "$(SPRINGHEAD)\src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"PhysicsMD.lib"

!ELSEIF  "$(CFG)" == "Physics - Win32 MFC Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Physics___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "Physics___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /vd0 /GX /Zi /Ox /Ot /Og /Oi /Op /Ob2 /Gf /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# SUBTRACT CPP /Oa /Ow /Gy /Fr
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"PhysicsM.lib"

!ENDIF 

# Begin Target

# Name "Physics - Win32 Release"
# Name "Physics - Win32 Debug"
# Name "Physics - Win32 MFC Debug"
# Name "Physics - Win32 MFC Release"
# Begin Source File

SOURCE=.\PHChangeObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PHChangeObject.h
# End Source File
# Begin Source File

SOURCE=.\PHContactEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\PHContactEngine.h
# End Source File
# Begin Source File

SOURCE=.\PHConvexPair.cpp
# End Source File
# Begin Source File

SOURCE=.\PHConvexPair.h
# End Source File
# Begin Source File

SOURCE=.\PHForceField.cpp
# End Source File
# Begin Source File

SOURCE=.\PHForceField.h
# End Source File
# Begin Source File

SOURCE=.\PHJoint.cpp
# End Source File
# Begin Source File

SOURCE=.\PHJoint.h
# End Source File
# Begin Source File

SOURCE=.\PHJoint1D.cpp
# End Source File
# Begin Source File

SOURCE=.\PHJoint1D.h
# End Source File
# Begin Source File

SOURCE=.\PHJointControl.cpp
# End Source File
# Begin Source File

SOURCE=.\PHJointControl.h
# End Source File
# Begin Source File

SOURCE=.\PHJointMulti.cpp
# End Source File
# Begin Source File

SOURCE=.\PHJointMulti.h
# End Source File
# Begin Source File

SOURCE=.\PHJointPid.cpp
# End Source File
# Begin Source File

SOURCE=.\PHJointPid.h
# End Source File
# Begin Source File

SOURCE=.\PHMagneticEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\PHMagneticEngine.h
# End Source File
# Begin Source File

SOURCE=.\PHPenalty.cpp
# End Source File
# Begin Source File

SOURCE=.\PHPenalty.h
# End Source File
# Begin Source File

SOURCE=.\PHRegisterLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\PHSolid.cpp
# End Source File
# Begin Source File

SOURCE=.\PHSolid.h
# End Source File
# Begin Source File

SOURCE=.\PHSpring.cpp
# End Source File
# Begin Source File

SOURCE=.\PHSpring.h
# End Source File
# Begin Source File

SOURCE=.\PHStickEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\PHStickEngine.h
# End Source File
# Begin Source File

SOURCE=.\PHWarp.cpp
# End Source File
# Begin Source File

SOURCE=.\PHWarp.h
# End Source File
# Begin Source File

SOURCE=.\PHWater.cpp
# End Source File
# Begin Source File

SOURCE=.\PHWater.h
# End Source File
# Begin Source File

SOURCE=.\PHWaterContact.cpp
# End Source File
# Begin Source File

SOURCE=.\PHWaterContact.h
# End Source File
# Begin Source File

SOURCE=.\physics.h
# End Source File
# End Target
# End Project
