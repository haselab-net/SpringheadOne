# Microsoft Developer Studio Project File - Name="SPIDAR_AngleAxis" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SPIDAR_AngleAxis - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "SPIDAR_AngleAxis.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "SPIDAR_AngleAxis.mak" CFG="SPIDAR_AngleAxis - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SPIDAR_AngleAxis - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "SPIDAR_AngleAxis - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/LVRM/HIS/Samples/SPIDAR_AngleAxis", CCABAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "$(LVRM_ROOT)" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(LVRM_ROOT)" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SPIDAR_AngleAxis - Win32 Release"
# Name "SPIDAR_AngleAxis - Win32 Debug"
# Begin Group "lib_Release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Base\Base.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Device\Device.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\WinBasis\WinBasis.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Fortran\QuadPR.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Fortran\libI77.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Fortran\libF77.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Render\Render.lib
# End Source File
# Begin Source File

SOURCE=..\..\HIS.lib
# End Source File
# End Group
# Begin Group "lib_Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Fortran\QuadPRD.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Fortran\libI77D.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Fortran\libF77D.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\HISD.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Base\BaseD.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Device\DeviceD.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\WinBasis\WinBasisD.lib

!IF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SPIDAR_AngleAxis - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Render\RenderD.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\glut_test.cpp
# End Source File
# Begin Source File

SOURCE=.\glut_test.h
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Target
# End Project
