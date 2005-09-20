# Microsoft Developer Studio Project File - Name="WinBasis" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=WinBasis - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "WinBasis.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "WinBasis.mak" CFG="WinBasis - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "WinBasis - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "WinBasis - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "WinBasis - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "WinBasis - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/WinBasis", WQUAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WinBasis - Win32 Release"

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
# ADD CPP /nologo /Gr /W3 /GX /O2 /Ob2 /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\WinBasisS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリのマージ
PostBuild_Cmds=WinBasisLib.bat RELEASE
# End Special Build Tool

!ELSEIF  "$(CFG)" == "WinBasis - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /Zi /I "$(SPRINGHEAD)\src" /D "_LIB" /D "DEBUG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /YX /FD /I /mssdk/include" /GZ " /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\WinBasisS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリのマージ
PostBuild_Cmds=WinBasisLib.bat DEBUG
# End Special Build Tool

!ELSEIF  "$(CFG)" == "WinBasis - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinBasis___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "WinBasis___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "SDK/include" /I "SDK/Classes/Base" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DEBUG" /YX /FD /I /mssdk/include" /GZ " /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Op /Ob2 /Gf /Gy /I "$(SPRINGHEAD)\src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DEBUG" /D "_AFXDLL" /YX /FD /I /mssdk/include" /GZ " /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\WinBasisS.lib"
# ADD LIB32 /nologo /out:"MFCDebug\WinBasisS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリのマージ
PostBuild_Cmds=WinBasisLib.bat MFCDEBUG
# End Special Build Tool

!ELSEIF  "$(CFG)" == "WinBasis - Win32 MFC Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinBasis___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "WinBasis___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /vd0 /GR- /GX /Zi /Ox /Ot /Og /Oi /Op /Ob2 /Gf /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# SUBTRACT CPP /Oa /Ow /Fr
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\WinBasisS.lib"
# ADD LIB32 /nologo /out:"MFCRelease\WinBasisS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリのマージ
PostBuild_Cmds=WinBasisLib.bat MFCRELEASE
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "WinBasis - Win32 Release"
# Name "WinBasis - Win32 Debug"
# Name "WinBasis - Win32 MFC Debug"
# Name "WinBasis - Win32 MFC Release"
# Begin Source File

SOURCE=.\WBComStream.cpp
# End Source File
# Begin Source File

SOURCE=.\WBComStream.h
# End Source File
# Begin Source File

SOURCE=.\WBCriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\WBDllLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\WBDllLoader.h
# End Source File
# Begin Source File

SOURCE=.\WBDllLoaderImpl.h
# End Source File
# Begin Source File

SOURCE=.\WBMMTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\WBMMTimer.h
# End Source File
# Begin Source File

SOURCE=.\WBPath.cpp
# End Source File
# Begin Source File

SOURCE=.\WBPath.h
# End Source File
# Begin Source File

SOURCE=.\WBPortIO.cpp
# End Source File
# Begin Source File

SOURCE=.\WBPortIO.H
# End Source File
# Begin Source File

SOURCE=.\WBPreciseTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\WBPreciseTimer.h
# End Source File
# Begin Source File

SOURCE=.\WBSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\WBSocket.h
# End Source File
# Begin Source File

SOURCE=.\WBSockStr.cpp
# End Source File
# Begin Source File

SOURCE=.\WBSockStr.h
# End Source File
# Begin Source File

SOURCE=.\WBUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\WBUtility.h
# End Source File
# Begin Source File

SOURCE=.\WBWin.h
# End Source File
# Begin Source File

SOURCE=.\WBWindowMan.cpp
# End Source File
# Begin Source File

SOURCE=.\WBWindowMan.h
# End Source File
# Begin Source File

SOURCE=.\WBWinDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\WBWinDriver.h
# End Source File
# Begin Source File

SOURCE=.\WBWinEnv.h
# End Source File
# Begin Source File

SOURCE=.\WBWinTypes.h
# End Source File
# Begin Source File

SOURCE=.\WinBasis.h
# End Source File
# Begin Source File

SOURCE=.\WinBasisLib.bat
# End Source File
# End Target
# End Project
