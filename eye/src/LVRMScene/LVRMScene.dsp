# Microsoft Developer Studio Project File - Name="LVRMScene" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LVRMScene - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "LVRMScene.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "LVRMScene.mak" CFG="LVRMScene - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "LVRMScene - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "LVRMScene - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "LVRMScene - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "LVRMScene - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Spr/LVRMScene", DRKAAAAA"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LVRMScene - Win32 Release"

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
# ADD CPP /nologo /W3 /GR /GX /O2 /I "$(SPRINGHEAD)\src" /I "Wrl2View/vc" /I "libJPEG" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"LVRMScene.lib"

!ELSEIF  "$(CFG)" == "LVRMScene - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GR /GX /Zi /Od /I "$(SPRINGHEAD)\src" /I "Wrl2View/vc" /I "libJPEG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"LVRMSceneD.lib"

!ELSEIF  "$(CFG)" == "LVRMScene - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MFCDebug"
# PROP BASE Intermediate_Dir "MFCDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I ".." /I "Wrl2View/vc" /I "libJPEG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(SPRINGHEAD)\src" /I "Wrl2View/vc" /I "libJPEG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"LVRMSceneMD.lib"

!ELSEIF  "$(CFG)" == "LVRMScene - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MFCRelease"
# PROP BASE Intermediate_Dir "MFCRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MFCRelease"
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "$(SPRINGHEAD)/src" /I "Wrl2View/vc" /I "libJPEG" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"LVRMSceneM.lib"

!ENDIF 

# Begin Target

# Name "LVRMScene - Win32 Release"
# Name "LVRMScene - Win32 Debug"
# Name "LVRMScene - Win32 MFC Debug"
# Name "LVRMScene - Win32 MFC Release"
# Begin Group "libJPEG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libJpeg\jcapimin.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcapistd.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jccoefct.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jccolor.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jchuff.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcinit.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcmainct.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcmarker.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcmaster.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcomapi.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcparam.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcphuff.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcprepct.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jcsample.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jctrans.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdapimin.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdapistd.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdatadst.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdcolor.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdct.h
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdhuff.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdinput.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdmainct.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdmarker.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdmaster.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdmerge.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdphuff.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdpostct.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdsample.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jdtrans.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jerror.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jerror.h
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jfdctint.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jidctflt.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jidctfst.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jidctint.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jidctred.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jmemansi.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jquant1.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jquant2.c
# End Source File
# Begin Source File

SOURCE=.\libJpeg\jutils.c
# End Source File
# End Group
# Begin Group "Wrl2View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Wrl2View\makelist.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\node.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\node.h
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\parser.y

!IF  "$(CFG)" == "LVRMScene - Win32 Release"

# Begin Custom Build
InputPath=.\Wrl2View\parser.y

"Wrl2View\parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Wrl2View\YaccLex.bat

# End Custom Build

!ELSEIF  "$(CFG)" == "LVRMScene - Win32 Debug"

# Begin Custom Build
InputPath=.\Wrl2View\parser.y

"Wrl2View\parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Wrl2View\YaccLex.bat

# End Custom Build

!ELSEIF  "$(CFG)" == "LVRMScene - Win32 MFC Debug"

# Begin Custom Build
InputPath=.\Wrl2View\parser.y

"Wrl2View\parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Wrl2View\YaccLex.bat

# End Custom Build

!ELSEIF  "$(CFG)" == "LVRMScene - Win32 MFC Release"

# Begin Custom Build
InputPath=.\Wrl2View\parser.y

"Wrl2View\parser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Wrl2View\YaccLex.bat

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Wrl2View\scanner.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\scanner.l

!IF  "$(CFG)" == "LVRMScene - Win32 Release"

# Begin Custom Build
InputPath=.\Wrl2View\scanner.l

"Wrl2View\scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Wrl2View\YaccLex.bat

# End Custom Build

!ELSEIF  "$(CFG)" == "LVRMScene - Win32 Debug"

# Begin Custom Build
InputPath=.\Wrl2View\scanner.l

"Wrl2View\scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Wrl2View\YaccLex.bat

# End Custom Build

!ELSEIF  "$(CFG)" == "LVRMScene - Win32 MFC Debug"

# Begin Custom Build
InputPath=.\Wrl2View\scanner.l

"Wrl2View\scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Wrl2View\YaccLex.bat

# End Custom Build

!ELSEIF  "$(CFG)" == "LVRMScene - Win32 MFC Release"

# Begin Custom Build
InputPath=.\Wrl2View\scanner.l

"Wrl2View\scanner.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	Wrl2View\YaccLex.bat

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Wrl2View\tokendef.h
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\type.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\type.h
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\type_imp.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\type_imp.h
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\type_tex.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\typeinit.cpp
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\vc\unistd.h
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\Wrl2View.h
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\YaccLex.bat
# End Source File
# Begin Source File

SOURCE=.\Wrl2View\YaccLex.mak
# End Source File
# End Group
# End Target
# End Project
