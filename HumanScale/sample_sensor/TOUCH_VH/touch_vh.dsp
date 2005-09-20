# Microsoft Developer Studio Project File - Name="touch_vh" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=touch_vh - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "touch_vh.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "touch_vh.mak" CFG="touch_vh - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "touch_vh - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "touch_vh - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/HumanScale/touch_vh", FSXAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "touch_vh - Win32 Release"

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

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(LVRM_ROOT)" /I "../../Dyna/SOLID/include" /I "../../Dyna/SOLID/src" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dsound.lib glut32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "touch_vh - Win32 Release"
# Name "touch_vh - Win32 Debug"
# Begin Group "DebugLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\LVRM\Base\BaseD.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Render\RenderD.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\SceneGraph\SceneGraphD.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\WinBasis\WinBasisD.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Dyna\SOLID\SOLIDD.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Device\DeviceD.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

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

!IF  "$(CFG)" == "touch_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Base\Base.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Device\Device.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Render\Render.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\SceneGraph\SceneGraph.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\WinBasis\WinBasis.lib

!IF  "$(CFG)" == "touch_vh - Win32 Release"

!ELSEIF  "$(CFG)" == "touch_vh - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

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
# Begin Source File

SOURCE=..\..\LVRM\HIS\HIS.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\ball.cpp
# End Source File
# Begin Source File

SOURCE=.\Ball.h
# End Source File
# Begin Source File

SOURCE=.\cyberglove.cpp
# End Source File
# Begin Source File

SOURCE=.\cyberglove.h
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

SOURCE=.\serial.cpp
# End Source File
# Begin Source File

SOURCE=.\serial.h
# End Source File
# Begin Source File

SOURCE=.\tex.h
# End Source File
# End Target
# End Project
