# Microsoft Developer Studio Project File - Name="BCBGui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BCBGui - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "BCBGui.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "BCBGui.mak" CFG="BCBGui - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "BCBGui - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "BCBGui - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/BCBGui", LOVAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BCBGui - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BCBGui - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "BCBGui - Win32 Release"
# Name "BCBGui - Win32 Debug"
# Begin Source File

SOURCE=.\SHAffine.cpp
# End Source File
# Begin Source File

SOURCE=.\SHAffine.h
# End Source File
# Begin Source File

SOURCE=.\SHAffineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SHAffineDlg.h
# End Source File
# Begin Source File

SOURCE=.\SHBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SHBox.h
# End Source File
# Begin Source File

SOURCE=.\SHCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\SHCamera.h
# End Source File
# Begin Source File

SOURCE=.\SHComponent.cpp
# End Source File
# Begin Source File

SOURCE=.\SHComponent.h
# End Source File
# Begin Source File

SOURCE=.\SHFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\SHFrame.h
# End Source File
# Begin Source File

SOURCE=.\SHLight.cpp
# End Source File
# Begin Source File

SOURCE=.\SHLight.h
# End Source File
# Begin Source File

SOURCE=.\SHPropEdit.h
# End Source File
# Begin Source File

SOURCE=.\SHPTMPropEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SHPTMPropEdit.h
# End Source File
# Begin Source File

SOURCE=.\SHRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\SHRenderer.h
# End Source File
# Begin Source File

SOURCE=.\SHScene.cpp
# End Source File
# Begin Source File

SOURCE=.\SHScene.h
# End Source File
# Begin Source File

SOURCE=.\SHSceneDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SHSceneDlg.h
# End Source File
# Begin Source File

SOURCE=.\SHSceneGraphDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SHSceneGraphDlg.h
# End Source File
# Begin Source File

SOURCE=.\SHTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\SHTreeView.h
# End Source File
# Begin Source File

SOURCE=.\SHVector.cpp
# End Source File
# Begin Source File

SOURCE=.\SHVector.h
# End Source File
# Begin Source File

SOURCE=.\SpringheadGui.cpp
# End Source File
# End Target
# End Project
