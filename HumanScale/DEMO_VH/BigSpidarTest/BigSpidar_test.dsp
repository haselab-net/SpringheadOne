# Microsoft Developer Studio Project File - Name="BigSpidar_test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=BigSpidar_test - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "BigSpidar_test.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "BigSpidar_test.mak" CFG="BigSpidar_test - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "BigSpidar_test - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "BigSpidar_test - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/VR/His/BigSpidar_test", VDAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "$(LVRM_ROOT)" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

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

# Name "BigSpidar_test - Win32 Release"
# Name "BigSpidar_test - Win32 Debug"
# Begin Group "libRelease"

# PROP Default_Filter ".lib"
# Begin Source File

SOURCE="$(LVRM_ROOT)\HIS\HIS.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\Base\Base.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\WinBasis\WinBasis.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\Device\Device.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\HIS\Fortran\QuadPR.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\HIS\Fortran\libI77.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\HIS\Fortran\libF77.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "libDebug"

# PROP Default_Filter ".lib"
# Begin Source File

SOURCE="$(LVRM_ROOT)\Base\BaseD.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\WinBasis\WinBasisD.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\HIS\HISD.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\HIS\Fortran\QuadPRD.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\HIS\Fortran\libI77D.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\HIS\Fortran\libF77D.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(LVRM_ROOT)\Device\DeviceD.lib"

!IF  "$(CFG)" == "BigSpidar_test - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BigSpidar_test - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\bigspidar_test.cpp
# End Source File
# Begin Source File

SOURCE=.\BigSpidar_test.dsw
# End Source File
# End Target
# End Project
