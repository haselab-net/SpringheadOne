# Microsoft Developer Studio Project File - Name="LVRMRender" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LVRMRender - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "LVRMRender.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "LVRMRender.mak" CFG="LVRMRender - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "LVRMRender - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "LVRMRender - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "LVRMRender - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "LVRMRender - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/VR", BAAAAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LVRMRender - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\LVRMRenderS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリの結合
PostBuild_Cmds=RenderLib.bat RELEASE
# End Special Build Tool

!ELSEIF  "$(CFG)" == "LVRMRender - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Render___Win32_Debug"
# PROP BASE Intermediate_Dir "Render___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "$(SPRINGHEAD)\src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\LVRMRenderS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリの結合
PostBuild_Cmds=RenderLib.bat DEBUG
# End Special Build Tool

!ELSEIF  "$(CFG)" == "LVRMRender - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Render___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "Render___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(SPRINGHEAD)\src" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\RenderS.lib"
# ADD LIB32 /nologo /out:"MFCDebug\RenderS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリの結合
PostBuild_Cmds=RenderLib.bat MFCDEBUG
# End Special Build Tool

!ELSEIF  "$(CFG)" == "LVRMRender - Win32 MFC Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Render___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "Render___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MFCRelease"
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\RenderS.lib"
# ADD LIB32 /nologo /out:"MFCRelease\RenderS.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=ライブラリの結合
PostBuild_Cmds=RenderLib.bat MFCRELEASE
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "LVRMRender - Win32 Release"
# Name "LVRMRender - Win32 Debug"
# Name "LVRMRender - Win32 MFC Debug"
# Name "LVRMRender - Win32 MFC Release"
# Begin Group "glaux"

# PROP Default_Filter ""
# End Group
# Begin Group "glut"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\LVRMRender.h
# End Source File
# Begin Source File

SOURCE=.\RDGlauxView.cpp
# End Source File
# Begin Source File

SOURCE=.\RDGlauxView.h
# End Source File
# Begin Source File

SOURCE=.\RDGlutView.cpp
# End Source File
# Begin Source File

SOURCE=.\RDGlutView.h
# End Source File
# Begin Source File

SOURCE=.\RDView.cpp
# End Source File
# Begin Source File

SOURCE=.\RDView.h
# End Source File
# Begin Source File

SOURCE=.\RenderLib.bat
# End Source File
# End Target
# End Project
