# Microsoft Developer Studio Project File - Name="Creature" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Creature - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Creature.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Creature.mak" CFG="Creature - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Creature - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "Creature - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Creature - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "Creature - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/Creature", OOXAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Creature - Win32 Release"

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

!ELSEIF  "$(CFG)" == "Creature - Win32 Debug"

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
# ADD LIB32 /nologo /out:"CreatureD.lib"

!ELSEIF  "$(CFG)" == "Creature - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Creature___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "Creature___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Op /Ob1 /I "$(SPRINGHEAD)\src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"CreatureMD.lib"

!ELSEIF  "$(CFG)" == "Creature - Win32 MFC Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Creature___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "Creature___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /vd0 /GX /Zi /Ox /Ot /Og /Oi /Op /Ob2 /Gf /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# SUBTRACT CPP /Gy /Fr
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"CreatureM.lib"

!ENDIF 

# Begin Target

# Name "Creature - Win32 Release"
# Name "Creature - Win32 Debug"
# Name "Creature - Win32 MFC Debug"
# Name "Creature - Win32 MFC Release"
# Begin Source File

SOURCE=.\CRActionPlanner.cpp
# End Source File
# Begin Source File

SOURCE=.\CRActionPlanner.h
# End Source File
# Begin Source File

SOURCE=.\CRBalance.cpp
# End Source File
# Begin Source File

SOURCE=.\CRBalance.h
# End Source File
# Begin Source File

SOURCE=.\CRBalanceMultiLeg.cpp
# End Source File
# Begin Source File

SOURCE=.\CRBalanceMultiLeg.h
# End Source File
# Begin Source File

SOURCE=.\CRBalanceQuadPR.cpp
# End Source File
# Begin Source File

SOURCE=.\CRBalanceQuadPR.h
# End Source File
# Begin Source File

SOURCE=.\CRBallHuman.cpp
# End Source File
# Begin Source File

SOURCE=.\CRBallHuman.h
# End Source File
# Begin Source File

SOURCE=.\CRBallVirtualHuman.cpp
# End Source File
# Begin Source File

SOURCE=.\CRBallVirtualHuman.h
# End Source File
# Begin Source File

SOURCE=.\CRBeetle.cpp
# End Source File
# Begin Source File

SOURCE=.\CRBeetle.h
# End Source File
# Begin Source File

SOURCE=.\Creature.h
# End Source File
# Begin Source File

SOURCE=.\CRHuman.cpp
# End Source File
# Begin Source File

SOURCE=.\CRHuman.h
# End Source File
# Begin Source File

SOURCE=.\CRPuppet.cpp
# End Source File
# Begin Source File

SOURCE=.\CRPuppet.h
# End Source File
# Begin Source File

SOURCE=.\CRReachingMovment.cpp
# End Source File
# Begin Source File

SOURCE=.\CRReachingMovment.h
# End Source File
# Begin Source File

SOURCE=.\CRReachingMovmentPD.cpp
# End Source File
# Begin Source File

SOURCE=.\CRReachingMovmentPD.h
# End Source File
# Begin Source File

SOURCE=.\CRRegisterLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\CRSupportArea.cpp
# End Source File
# Begin Source File

SOURCE=.\CRSupportArea.h
# End Source File
# Begin Source File

SOURCE=.\CRUser.cpp
# End Source File
# Begin Source File

SOURCE=.\CRUser.h
# End Source File
# Begin Source File

SOURCE=.\CRVH.cpp
# End Source File
# Begin Source File

SOURCE=.\CRVH.h
# End Source File
# Begin Source File

SOURCE=.\CRVirtualHuman.cpp
# End Source File
# Begin Source File

SOURCE=.\CRVirtualHuman.h
# End Source File
# End Target
# End Project
