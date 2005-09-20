# Microsoft Developer Studio Project File - Name="Qhull" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Qhull - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Qhull.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Qhull.mak" CFG="Qhull - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Qhull - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "Qhull - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Dyna/Wrl2PRML v2.0", KUWAAAAA"
# PROP Scc_LocalPath "..\wrl2prml v2.0"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Qhull - Win32 Release"

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

!ELSEIF  "$(CFG)" == "Qhull - Win32 Debug"

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

# Name "Qhull - Win32 Release"
# Name "Qhull - Win32 Debug"
# Begin Source File

SOURCE=.\src\geom.c
# End Source File
# Begin Source File

SOURCE=.\src\geom.h
# End Source File
# Begin Source File

SOURCE=.\src\geom2.c
# End Source File
# Begin Source File

SOURCE=.\src\global.c
# End Source File
# Begin Source File

SOURCE=.\src\io.c
# End Source File
# Begin Source File

SOURCE=.\src\io.h
# End Source File
# Begin Source File

SOURCE=.\src\mem.c
# End Source File
# Begin Source File

SOURCE=.\src\mem.h
# End Source File
# Begin Source File

SOURCE=.\src\merge.c
# End Source File
# Begin Source File

SOURCE=.\src\merge.h
# End Source File
# Begin Source File

SOURCE=.\src\poly.c
# End Source File
# Begin Source File

SOURCE=.\src\poly.h
# End Source File
# Begin Source File

SOURCE=.\src\poly2.c
# End Source File
# Begin Source File

SOURCE=.\src\qhull.c
# End Source File
# Begin Source File

SOURCE=.\src\qhull.h
# End Source File
# Begin Source File

SOURCE=.\src\qhull_a.h
# End Source File
# Begin Source File

SOURCE=.\src\qset.c
# End Source File
# Begin Source File

SOURCE=.\src\qset.h
# End Source File
# Begin Source File

SOURCE=.\src\stat.c
# End Source File
# Begin Source File

SOURCE=.\src\stat.h
# End Source File
# Begin Source File

SOURCE=.\src\user.c
# End Source File
# Begin Source File

SOURCE=.\src\user.h
# End Source File
# End Target
# End Project
