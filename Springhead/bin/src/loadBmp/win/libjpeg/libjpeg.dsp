# Microsoft Developer Studio Project File - Name="libjpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** �ҏW���Ȃ��ł������� **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libjpeg - Win32 MFC Debug
!MESSAGE ����͗L����Ҳ�̧�قł͂���܂���B ������ۼު�Ă�����ނ��邽�߂ɂ� NMAKE ���g�p���Ă��������B
!MESSAGE [Ҳ�̧�ق̴���߰�] ����ނ��g�p���Ď��s���Ă�������
!MESSAGE 
!MESSAGE NMAKE /f "libjpeg.mak".
!MESSAGE 
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "libjpeg.mak" CFG="libjpeg - Win32 MFC Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "libjpeg - Win32 Release" ("Win32 (x86) Static Library" �p)
!MESSAGE "libjpeg - Win32 Debug" ("Win32 (x86) Static Library" �p)
!MESSAGE "libjpeg - Win32 MFC Debug" ("Win32 (x86) Static Library" �p)
!MESSAGE "libjpeg - Win32 MFC Release" ("Win32 (x86) Static Library" �p)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libjpeg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\Release"
# PROP Intermediate_Dir "..\..\obj\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /Gd /MT /W2 /GX /O2 /Ob2 /I "." /I "..\..\otherlib\libjpeg" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W2 /GX /Z7 /Od /I "." /I "..\..\otherlib\libjpeg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libjpeg - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libjpeg___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "libjpeg___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\MFCDebug"
# PROP Intermediate_Dir "..\..\obj\MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /W2 /GX /Z7 /Od /I "." /I "..\..\otherlib\libjpeg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /G5 /MDd /W2 /GX /Z7 /Od /I "." /I "..\..\otherlib\libjpeg" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /YX /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libjpeg - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libjpeg___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "libjpeg___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\MFCRelease"
# PROP Intermediate_Dir "..\..\obj\MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W2 /GX /O2 /Ob2 /I "." /I "..\..\otherlib\libjpeg" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /c
# ADD CPP /nologo /G6 /Gr /MD /W2 /GX /O2 /Ob2 /I "." /I "..\..\otherlib\libjpeg" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /c
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

# Name "libjpeg - Win32 Release"
# Name "libjpeg - Win32 Debug"
# Name "libjpeg - Win32 MFC Debug"
# Name "libjpeg - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcapimin.c
DEP_CPP_JCAPI=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcapistd.c
DEP_CPP_JCAPIS=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jccoefct.c
DEP_CPP_JCCOE=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jccolor.c
DEP_CPP_JCCOL=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcdctmgr.c
DEP_CPP_JCDCT=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\Otherlib\libjpeg\jdct.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jchuff.c
DEP_CPP_JCHUF=\
	"..\..\otherlib\libjpeg\jchuff.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcinit.c
DEP_CPP_JCINI=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcmainct.c
DEP_CPP_JCMAI=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcmarker.c
DEP_CPP_JCMAR=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcmaster.c
DEP_CPP_JCMAS=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jcomapi.c
DEP_CPP_JCOMA=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcparam.c
DEP_CPP_JCPAR=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcphuff.c
DEP_CPP_JCPHU=\
	"..\..\otherlib\libjpeg\jchuff.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcprepct.c
DEP_CPP_JCPRE=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jcsample.c
DEP_CPP_JCSAM=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jctrans.c
DEP_CPP_JCTRA=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdapimin.c
DEP_CPP_JDAPI=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdapistd.c
DEP_CPP_JDAPIS=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdatadst.c
DEP_CPP_JDATA=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdatasrc.c
DEP_CPP_JDATAS=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdcoefct.c
DEP_CPP_JDCOE=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdcolor.c
DEP_CPP_JDCOL=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jddctmgr.c
DEP_CPP_JDDCT=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\Otherlib\libjpeg\jdct.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdhuff.c
DEP_CPP_JDHUF=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\Otherlib\libjpeg\jdhuff.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdinput.c
DEP_CPP_JDINP=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdmainct.c
DEP_CPP_JDMAI=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdmarker.c
DEP_CPP_JDMAR=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdmaster.c
DEP_CPP_JDMAS=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdmerge.c
DEP_CPP_JDMER=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdphuff.c
DEP_CPP_JDPHU=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\Otherlib\libjpeg\jdhuff.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdpostct.c
DEP_CPP_JDPOS=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdsample.c
DEP_CPP_JDSAM=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdtrans.c
DEP_CPP_JDTRA=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jerror.c
DEP_CPP_JERRO=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	"..\..\Otherlib\libjpeg\jversion.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\Jfdctflt.c
DEP_CPP_JFDCT=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\Otherlib\libjpeg\jdct.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jidctflt.c
DEP_CPP_JIDCT=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\Otherlib\libjpeg\jdct.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jidctfst.c
DEP_CPP_JIDCTF=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\Otherlib\libjpeg\jdct.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jidctint.c
DEP_CPP_JIDCTI=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\Otherlib\libjpeg\jdct.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jidctred.c
DEP_CPP_JIDCTR=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\Otherlib\libjpeg\jdct.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jmemmgr.c
DEP_CPP_JMEMM=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\Otherlib\libjpeg\jmemsys.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jmemnobs.c
DEP_CPP_JMEMN=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\Otherlib\libjpeg\jmemsys.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jquant1.c
DEP_CPP_JQUAN=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jquant2.c
DEP_CPP_JQUANT=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jutils.c
DEP_CPP_JUTIL=\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jinclude.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\jchuff.h
# End Source File
# Begin Source File

SOURCE=..\..\Otherlib\libjpeg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\jerror.h
# End Source File
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\jmorecfg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\otherlib\libjpeg\jconfig.vc

!IF  "$(CFG)" == "libjpeg - Win32 Release"

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug"

# Begin Custom Build - Copying jconfig.vc
InputPath=..\..\otherlib\libjpeg\jconfig.vc

"..\..\otherlib\libjpeg\jconfig.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\otherlib\libjpeg\jconfig.vc ..\..\otherlib\libjpeg\jconfig.h

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 MFC Debug"

# Begin Custom Build - Copying jconfig.vc
InputPath=..\..\otherlib\libjpeg\jconfig.vc

"..\..\otherlib\libjpeg\jconfig.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\otherlib\libjpeg\jconfig.vc ..\..\otherlib\libjpeg\jconfig.h

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\libjpeg\jmorecfg.h

!IF  "$(CFG)" == "libjpeg - Win32 Release"

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying jmorecfg.h
InputPath=..\libjpeg\jmorecfg.h

"..\..\otherlib\libjpeg\jmorecfg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy jmorecfg.h ..\..\otherlib\libjpeg\jmorecfg.h

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 MFC Debug"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying jmorecfg.h
InputPath=..\libjpeg\jmorecfg.h

"..\..\otherlib\libjpeg\jmorecfg.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy jmorecfg.h ..\..\otherlib\libjpeg\jmorecfg.h

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 MFC Release"

!ENDIF 

# End Source File
# End Target
# End Project
