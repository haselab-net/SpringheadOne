# Microsoft Developer Studio Project File - Name="WinDX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=WinDX - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "WinDX.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "WinDX.mak" CFG="WinDX - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "WinDX - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "WinDX - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "WinDX - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "WinDX - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WinDX - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\WinDXS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリのマージ
PostBuild_Cmds=WinDXLib.bat RELEASE
# End Special Build Tool

!ELSEIF  "$(CFG)" == "WinDX - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DEBUG" /YX /FD /I /mssdk/include" /GZ " /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\WinDXS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリのマージ
PostBuild_Cmds=WinDXLib.bat DEBUG
# End Special Build Tool

!ELSEIF  "$(CFG)" == "WinDX - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDX___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "WinDX___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "SDK/include" /I "SDK/Classes/Base" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DEBUG" /YX /FD /I /mssdk/include" /GZ " /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "DEBUG" /D "_AFXDLL" /YX /FD /I /mssdk/include" /GZ " /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\WinDXS.lib"
# ADD LIB32 /nologo /out:"MFCDebug\WinDXS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリのマージ
PostBuild_Cmds=WinDXLib.bat MFCDEBUG
# End Special Build Tool

!ELSEIF  "$(CFG)" == "WinDX - Win32 MFC Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinDX___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "WinDX___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\WinDXS.lib"
# ADD LIB32 /nologo /out:"MFCRelease\WinDXS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリのマージ
PostBuild_Cmds=WinDXLib.bat MFCRELEASE
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "WinDX - Win32 Release"
# Name "WinDX - Win32 Debug"
# Name "WinDX - Win32 MFC Debug"
# Name "WinDX - Win32 MFC Release"
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Base\WXCallback.h
# End Source File
# Begin Source File

SOURCE=.\Base\WXComBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\WXComBase.h
# End Source File
# Begin Source File

SOURCE=.\Base\WXError.cpp
# End Source File
# Begin Source File

SOURCE=.\Base\WXError.h
# End Source File
# End Group
# Begin Group "D3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\D3D\WXD3Affine.h
# End Source File
# Begin Source File

SOURCE=.\D3D\WXD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\D3D\WXD3D.h
# End Source File
# End Group
# Begin Group "DDraw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DDraw\WXDDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\DDraw\WXDDraw.h
# End Source File
# Begin Source File

SOURCE=.\DDraw\WXDDSurf.cpp
# End Source File
# Begin Source File

SOURCE=.\DDraw\WXDDSurf.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\WinDXLib.bat
# End Source File
# End Target
# End Project
