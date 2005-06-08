# Microsoft Developer Studio Project File - Name="SceneGraph" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SceneGraph - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "SceneGraph.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "SceneGraph.mak" CFG="SceneGraph - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SceneGraph - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "SceneGraph - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "SceneGraph - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "SceneGraph - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/SceneGraph", DTUAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SceneGraph - Win32 Release"

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

!ELSEIF  "$(CFG)" == "SceneGraph - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "$(SPRINGHEAD)\src" /D "_LIB" /D "DEBUG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /YX /FD /I /mssdk/include" /GZ " /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"SceneGraphD.lib"

!ELSEIF  "$(CFG)" == "SceneGraph - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SceneGraph___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "SceneGraph___Win32_MFC_Debug"
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
# ADD BASE LIB32 /nologo /out:"Debug\SceneGraphS.lib"
# ADD LIB32 /nologo /out:"SceneGraphMD.lib"

!ELSEIF  "$(CFG)" == "SceneGraph - Win32 MFC Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SceneGraph___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "SceneGraph___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /vd0 /GX /Zi /Ox /Ot /Og /Oi /Op /Ob0 /Gf /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# SUBTRACT CPP /Oa /Ow /Gy /Fr
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\SceneGraphS.lib"
# ADD LIB32 /nologo /out:"SceneGraphM.lib"

!ENDIF 

# Begin Target

# Name "SceneGraph - Win32 Release"
# Name "SceneGraph - Win32 Debug"
# Name "SceneGraph - Win32 MFC Debug"
# Name "SceneGraph - Win32 MFC Release"
# Begin Source File

SOURCE=.\SceneGraph.h
# End Source File
# Begin Source File

SOURCE=.\SGBehaviorEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\SGBehaviorEngine.h
# End Source File
# Begin Source File

SOURCE=.\SGEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\SGEngine.h
# End Source File
# Begin Source File

SOURCE=.\SGFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\SGFrame.h
# End Source File
# Begin Source File

SOURCE=.\SGObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SGObject.h
# End Source File
# Begin Source File

SOURCE=.\SGRegisterLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\SGRenderEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\SGRenderEngine.h
# End Source File
# Begin Source File

SOURCE=.\SGScene.cpp
# End Source File
# Begin Source File

SOURCE=.\SGScene.h
# End Source File
# End Target
# End Project
