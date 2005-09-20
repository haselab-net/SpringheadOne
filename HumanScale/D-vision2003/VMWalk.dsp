# Microsoft Developer Studio Project File - Name="VMWalk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=VMWalk - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "VMWalk.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "VMWalk.mak" CFG="VMWalk - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "VMWalk - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "VMWalk - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VMWalk - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "$(LVRM_ROOT)" /I "../../Dyna/SOLID/include" /I "../../Dyna/SOLID/src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VMWalk___Win32_Debug"
# PROP BASE Intermediate_Dir "VMWalk___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(LVRM_ROOT)" /I "../../Dyna/SOLID/include" /I "../../Dyna/SOLID/src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dsound.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "VMWalk - Win32 Release"
# Name "VMWalk - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\communication.cpp
# End Source File
# Begin Source File

SOURCE=.\EasyDS.cpp
# End Source File
# Begin Source File

SOURCE=.\GeoNode.cpp
# End Source File
# Begin Source File

SOURCE=.\GLView.cpp
# End Source File
# Begin Source File

SOURCE=.\LabelingCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\stereo.cpp
# End Source File
# Begin Source File

SOURCE=..\Share\WalkThroughTools.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\communication.h
# End Source File
# Begin Source File

SOURCE=.\CySLC.h
# End Source File
# Begin Source File

SOURCE=.\EasyDS.h
# End Source File
# Begin Source File

SOURCE=.\GeoNode.h
# End Source File
# Begin Source File

SOURCE=.\GLView.h
# End Source File
# Begin Source File

SOURCE=.\LabelingCamera.h
# End Source File
# Begin Source File

SOURCE=.\setting.ini
# End Source File
# Begin Source File

SOURCE=.\SLCdevelop.h
# End Source File
# Begin Source File

SOURCE=.\Stereo.h
# End Source File
# Begin Source File

SOURCE=..\Share\WalkThroughTools.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "DebugLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\LVRM\Base\BaseD.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Render\RenderD.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\SceneGraph\SceneGraphD.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\WinBasis\WinBasisD.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Dyna\SOLID\SOLIDD.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Device\DeviceD.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HIS\HISD.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CySLC.lib
# End Source File
# End Group
# Begin Group "ReleaseLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Dyna\SOLID\SOLID.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Base\Base.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Device\Device.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\Render\Render.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\SceneGraph\SceneGraph.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\LVRM\WinBasis\WinBasis.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HIS\HIS.lib

!IF  "$(CFG)" == "VMWalk - Win32 Release"

!ELSEIF  "$(CFG)" == "VMWalk - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
