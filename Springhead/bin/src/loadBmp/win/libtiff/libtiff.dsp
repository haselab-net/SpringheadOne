# Microsoft Developer Studio Project File - Name="libtiff" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libtiff - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "libtiff.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "libtiff.mak" CFG="libtiff - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "libtiff - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "libtiff - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "libtiff - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "libtiff - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libtiff - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\Release"
# PROP Intermediate_Dir "..\..\obj\Release"
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /G6 /MT /w /W0 /GX /O2 /Ob2 /I "." /I "..\..\otherlib\libtiff\libtiff" /I "..\..\otherlib\libjpeg" /I "..\libjpeg" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libtiff - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug"
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /G5 /MTd /w /W0 /GX /Z7 /Od /I "." /I "..\..\otherlib\libtiff\libtiff" /I "..\..\otherlib\libjpeg" /I "..\libjpeg" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libtiff - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libtiff___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "libtiff___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\MFCDebug"
# PROP Intermediate_Dir "..\..\obj\MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /w /W0 /GX /Z7 /Od /I "." /I "..\..\otherlib\libtiff\libtiff" /I "..\..\otherlib\libjpeg" /I "..\libjpeg" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /MDd /w /W0 /GX /Z7 /Od /I "." /I "..\..\otherlib\libtiff\libtiff" /I "..\..\otherlib\libjpeg" /I "..\libjpeg" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libtiff - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libtiff___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "libtiff___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\MFCRelease"
# PROP Intermediate_Dir "..\..\obj\MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /w /W0 /GX /O2 /Ob2 /I "." /I "..\..\otherlib\libtiff\libtiff" /I "..\..\otherlib\libjpeg" /I "..\libjpeg" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /Gr /MD /w /W0 /GX /O2 /Ob2 /I "." /I "..\..\otherlib\libtiff\libtiff" /I "..\..\otherlib\libjpeg" /I "..\libjpeg" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libtiff - Win32 Release"
# Name "libtiff - Win32 Debug"
# Name "libtiff - Win32 MFC Debug"
# Name "libtiff - Win32 MFC Release"
# Begin Source File

SOURCE=..\..\otherlib\libTiff\fax3sm_winnt.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\t4.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_aux.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_close.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_codec.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_color.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_compress.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_dir.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_dir.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_dirinfo.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_dirread.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_dirwrite.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_dumpmode.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_error.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_extension.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_fax3.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_fax3.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_flush.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_getimage.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_jpeg.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_luv.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_lzw.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_next.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_ojpeg.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_open.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_packbits.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_pixarlog.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_predict.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_predict.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_print.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_read.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_strip.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_swab.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_thunder.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_tile.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_version.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_warning.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_write.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tif_zip.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tiff.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tiffcomp.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tiffconf.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tiffio.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tiffiop.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\tiffvers.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libTiff\uvcode.h
# End Source File
# End Target
# End Project
