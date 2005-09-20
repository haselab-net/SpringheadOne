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
# PROP Scc_ProjName ""$/VR", BAAAAAAA"
# PROP Scc_LocalPath ".."
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
# ADD CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /x /d "_DEBUG"
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
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
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
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MFCRelease"
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
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

SOURCE=.\HIDiffCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\HIDiffCalc.h
# End Source File
# Begin Source File

SOURCE=.\HIHapticDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\HIHapticDevice.h
# End Source File
# Begin Source File

SOURCE=.\HIS.h
# End Source File
# Begin Source File

SOURCE=.\HISpidar3Quad.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar3Quad.h
# End Source File
# Begin Source File

SOURCE=.\HISpidar4quad.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar4quad.h
# End Source File
# Begin Source File

SOURCE=.\HISpidar8.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidar8.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarG.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarG.h
# End Source File
# Begin Source File

SOURCE=.\HISpidarMotor.cpp
# End Source File
# Begin Source File

SOURCE=.\HISpidarMotor.h
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
