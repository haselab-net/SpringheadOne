# Microsoft Developer Studio Project File - Name="swiftpp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=swiftpp - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "swiftpp.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "swiftpp.mak" CFG="swiftpp - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "swiftpp - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "swiftpp - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/SWIFT++/swift_spidar", ACMAAAAA"
# PROP Scc_LocalPath ".\swift_spidar"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "swiftpp - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I ".\include" /I ".\qhull3.0\src" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "LOOKUP_TABLE_5_625" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "swiftpp - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\include" /I ".\qhull3.0\src" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "LOOKUP_TABLE_5_625" /D "SWIFT_DEBUG" /YX /FD /GZ /c /Tp
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

# Name "swiftpp - Win32 Release"
# Name "swiftpp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\fileio.cpp
# End Source File
# Begin Source File

SOURCE=.\src\lut.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mesh_utils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\object.cpp
# End Source File
# Begin Source File

SOURCE=.\src\pair.cpp
# End Source File
# Begin Source File

SOURCE=.\src\pqueue.cpp
# End Source File
# Begin Source File

SOURCE=.\src\scene.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\SWIFT.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_array.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_boxnode.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_common.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_config.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_fileio.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_front.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_linalg.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_lut.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_mesh.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_mesh_utils.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_object.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_pair.h
# End Source File
# Begin Source File

SOURCE=.\include\SWIFT_pqueue.h
# End Source File
# End Group
# End Target
# End Project
