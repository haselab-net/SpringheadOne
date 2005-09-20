# Microsoft Developer Studio Project File - Name="motion_vh" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=motion_vh - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "motion_vh.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "motion_vh.mak" CFG="motion_vh - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "motion_vh - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "motion_vh - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/HumanScale/motion_vh", FSXAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "motion_vh - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "$(LVRM_ROOT)" /I "../../Dyna/SOLID/include" /I "../../Dyna/SOLID/src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dsound.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(LVRM_ROOT)" /I "../../Dyna/SOLID/include" /I "../../Dyna/SOLID/src" /I "$(ATI_GL_SDK_ROOT)\include" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ATIExtensions_d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dsound.lib glut32.lib mpi/mpich.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"$(ATI_GL_SDK_ROOT)\lib"

!ENDIF 

# Begin Target

# Name "motion_vh - Win32 Release"
# Name "motion_vh - Win32 Debug"
# Begin Group "DebugLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\LVRM\Base\BaseD.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Render\RenderD.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\SceneGraph\SceneGraphD.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\WinBasis\WinBasisD.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Dyna\SOLID\SOLIDD.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Device\DeviceD.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\HIS\HISD.lib
# End Source File
# Begin Source File

SOURCE=..\..\LVRM\HIS\Fortran\QuadPRD.lib
# End Source File
# Begin Source File

SOURCE=..\..\LVRM\HIS\Fortran\libI77D.lib
# End Source File
# Begin Source File

SOURCE=..\..\LVRM\HIS\Fortran\libF77D.lib
# End Source File
# End Group
# Begin Group "ReleaseLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Dyna\SOLID\SOLID.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Base\Base.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Device\Device.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Render\Render.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\SceneGraph\SceneGraph.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\WinBasis\WinBasis.lib

!IF  "$(CFG)" == "motion_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "motion_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\HIS\HIS.lib
# End Source File
# Begin Source File

SOURCE=..\..\LVRM\HIS\Fortran\QuadPR.lib
# End Source File
# Begin Source File

SOURCE=..\..\LVRM\HIS\Fortran\libI77.lib
# End Source File
# Begin Source File

SOURCE=..\..\LVRM\HIS\Fortran\libF77.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ball.cpp
# End Source File
# Begin Source File

SOURCE=.\Ball.h
# End Source File
# Begin Source File

SOURCE=.\direxec.cpp
# End Source File
# Begin Source File

SOURCE=.\direxec.h
# End Source File
# Begin Source File

SOURCE=.\GeoNode.cpp
# End Source File
# Begin Source File

SOURCE=.\GeoNode.h
# End Source File
# Begin Source File

SOURCE=.\GLView.cpp
# End Source File
# Begin Source File

SOURCE=.\GLView.h
# End Source File
# Begin Source File

SOURCE=.\joint.cpp
# End Source File
# Begin Source File

SOURCE=.\joint.h
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\mymath.h
# End Source File
# Begin Source File

SOURCE=.\mySoftShadowMapVH.cpp
# End Source File
# Begin Source File

SOURCE=.\mySoftShadowMapVH.h
# End Source File
# Begin Source File

SOURCE=.\tex.h
# End Source File
# End Target
# End Project
