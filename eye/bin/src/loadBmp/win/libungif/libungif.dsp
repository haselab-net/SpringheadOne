# Microsoft Developer Studio Project File - Name="libungif" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** �ҏW���Ȃ��ł������� **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libungif - Win32 MFC Debug
!MESSAGE ����͗L����Ҳ�̧�قł͂���܂���B ������ۼު�Ă�����ނ��邽�߂ɂ� NMAKE ���g�p���Ă��������B
!MESSAGE [Ҳ�̧�ق̴���߰�] ����ނ��g�p���Ď��s���Ă�������
!MESSAGE 
!MESSAGE NMAKE /f "libungif.mak".
!MESSAGE 
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "libungif.mak" CFG="libungif - Win32 MFC Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "libungif - Win32 Release" ("Win32 (x86) Static Library" �p)
!MESSAGE "libungif - Win32 Debug" ("Win32 (x86) Static Library" �p)
!MESSAGE "libungif - Win32 MFC Debug" ("Win32 (x86) Static Library" �p)
!MESSAGE "libungif - Win32 MFC Release" ("Win32 (x86) Static Library" �p)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/giflib-4.1.0/Lib", THCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libungif - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\Release"
# PROP Intermediate_Dir "..\..\obj\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W2 /GX /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_OPEN_BINARY" /D "HAVE_IO_H" /D CDECL=__cdecl /D open=_open /D close=_close /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libungif - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W2 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_LIB" /D "_OPEN_BINARY" /D "HAVE_IO_H" /D CDECL=__cdecl /D open=_open /D close=_close /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libungif - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libungif___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "libungif___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\MFCDebug"
# PROP Intermediate_Dir "..\..\obj\MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W2 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_LIB" /D "_OPEN_BINARY" /D "HAVE_IO_H" /D CDECL=__cdecl /D open=_open /D close=_close /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W2 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_LIB" /D "_OPEN_BINARY" /D "HAVE_IO_H" /D CDECL=__cdecl /D open=_open /D close=_close /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libungif - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libungif___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "libungif___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\MFCRelease"
# PROP Intermediate_Dir "..\..\obj\MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W2 /GX /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_OPEN_BINARY" /D "HAVE_IO_H" /D CDECL=__cdecl /D open=_open /D close=_close /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /Gr /MD /W2 /GX /O2 /Ob2 /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_OPEN_BINARY" /D "HAVE_IO_H" /D CDECL=__cdecl /D open=_open /D close=_close /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libungif - Win32 Release"
# Name "libungif - Win32 Debug"
# Name "libungif - Win32 MFC Debug"
# Name "libungif - Win32 MFC Release"
# Begin Source File

SOURCE=..\..\otherlib\libungif\dev2gif.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\dgif_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\egif_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\getarg.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\getarg.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\gif_err.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\gif_font.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\gif_lib.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\gif_lib_private.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\gifalloc.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\qprintf.c
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libungif\quantize.c
# End Source File
# End Target
# End Project
