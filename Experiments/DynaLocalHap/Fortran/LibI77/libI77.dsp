# Microsoft Developer Studio Project File - Name="libI77" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libI77 - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "libI77.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "libI77.mak" CFG="libI77 - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "libI77 - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "libI77 - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "libI77 - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "libI77 - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Experiments/DynaLocalHap/Fortran", MUBCAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libI77 - Win32 Release"

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

!ELSEIF  "$(CFG)" == "libI77 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libI77___Win32_Debug"
# PROP BASE Intermediate_Dir "libI77___Win32_Debug"
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
# ADD LIB32 /nologo /out:"../libI77D.lib"

!ELSEIF  "$(CFG)" == "libI77 - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MFC Debug"
# PROP BASE Intermediate_Dir "MFC Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\libI77MD.lib"

!ELSEIF  "$(CFG)" == "libI77 - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MFC Release"
# PROP BASE Intermediate_Dir "MFC Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MFCRelease"
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\libI77M.lib"

!ENDIF 

# Begin Target

# Name "libI77 - Win32 Release"
# Name "libI77 - Win32 Debug"
# Name "libI77 - Win32 MFC Debug"
# Name "libI77 - Win32 MFC Release"
# Begin Source File

SOURCE=.\backspace.c
# End Source File
# Begin Source File

SOURCE=.\CLOSE.C
# End Source File
# Begin Source File

SOURCE=.\DFE.C
# End Source File
# Begin Source File

SOURCE=.\DOLIO.C
# End Source File
# Begin Source File

SOURCE=.\DUE.C
# End Source File
# Begin Source File

SOURCE=.\ENDFILE.C
# End Source File
# Begin Source File

SOURCE=.\ERR.C
# End Source File
# Begin Source File

SOURCE=.\F2C.H
# End Source File
# Begin Source File

SOURCE=.\FIO.H
# End Source File
# Begin Source File

SOURCE=.\FMT.C
# End Source File
# Begin Source File

SOURCE=.\FMT.H
# End Source File
# Begin Source File

SOURCE=.\FMTLIB.C
# End Source File
# Begin Source File

SOURCE=.\FP.H
# End Source File
# Begin Source File

SOURCE=.\IIO.C
# End Source File
# Begin Source File

SOURCE=.\ILNW.C
# End Source File
# Begin Source File

SOURCE=.\INQUIRE.C
# End Source File
# Begin Source File

SOURCE=.\LIO.H
# End Source File
# Begin Source File

SOURCE=.\LREAD.C
# End Source File
# Begin Source File

SOURCE=.\LWRITE.C
# End Source File
# Begin Source File

SOURCE=.\OPEN.C
# End Source File
# Begin Source File

SOURCE=.\RAWIO.H
# End Source File
# Begin Source File

SOURCE=.\RDFMT.C
# End Source File
# Begin Source File

SOURCE=.\REWIND.C
# End Source File
# Begin Source File

SOURCE=.\RSFE.C
# End Source File
# Begin Source File

SOURCE=.\RSLI.C
# End Source File
# Begin Source File

SOURCE=.\RSNE.C
# End Source File
# Begin Source File

SOURCE=.\SFE.C
# End Source File
# Begin Source File

SOURCE=.\SUE.C
# End Source File
# Begin Source File

SOURCE=.\TYPESIZE.C
# End Source File
# Begin Source File

SOURCE=.\UIO.C
# End Source File
# Begin Source File

SOURCE=.\UTIL.C
# End Source File
# Begin Source File

SOURCE=.\Version.c
# End Source File
# Begin Source File

SOURCE=.\WREF.C
# End Source File
# Begin Source File

SOURCE=.\WRTFMT.C
# End Source File
# Begin Source File

SOURCE=.\WSFE.C
# End Source File
# Begin Source File

SOURCE=.\WSLE.C
# End Source File
# Begin Source File

SOURCE=.\WSNE.C
# End Source File
# Begin Source File

SOURCE=.\XWSNE.C
# End Source File
# End Target
# End Project
