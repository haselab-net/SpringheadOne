# Microsoft Developer Studio Project File - Name="qhull" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=qhull - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "qhull.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "qhull.mak" CFG="qhull - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "qhull - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "qhull - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/SWIFT++/swift_spidar", ACMAAAAA"
# PROP Scc_LocalPath ".\swift_spidar"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "qhull - Win32 Release"

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
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "qhull - Win32 Debug"

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
LINK32=link.exe -lib
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "qhull - Win32 Release"
# Name "qhull - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\qhull3.0\src\geom.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\geom2.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\global.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\io.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\mem.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\merge.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\poly.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\poly2.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\qhull.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\qset.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\stat.c
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\user.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\qhull3.0\src\geom.h
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\io.h
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\mem.h
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\merge.h
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\poly.h
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\qhull.h
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\qhull_a.h
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\qset.h
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\stat.h
# End Source File
# Begin Source File

SOURCE=.\qhull3.0\src\user.h
# End Source File
# End Group
# End Target
# End Project
