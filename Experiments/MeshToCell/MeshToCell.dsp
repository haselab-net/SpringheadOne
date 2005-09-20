# Microsoft Developer Studio Project File - Name="MeshToCell" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MeshToCell - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "MeshToCell.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "MeshToCell.mak" CFG="MeshToCell - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "MeshToCell - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "MeshToCell - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/Collision/Samples/MeshToCell", VOXAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

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
# ADD CPP /nologo /Gr /W3 /GX /O2 /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "$(SPRINGHEAD)\src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
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

# Name "MeshToCell - Win32 Release"
# Name "MeshToCell - Win32 Debug"
# Begin Group "DebugLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Base\BaseD.lib"

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\FileIO\FileIOD.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\Collision\CollisionD.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\ImpD3D\ImpD3DD.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\WinDX\WinDXD.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\WinBasis\WinBasisD.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\SceneGraph\SceneGraphD.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "ReleaseLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Springhead\src\SceneGraph\SceneGraph.lib
# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\Collision\Collision.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\WinBasis\WinBasis.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\WinDX\WinDX.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\ImpD3D\ImpD3D.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Springhead\src\FileIO\FileIO.lib

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Base\Base.lib"

!IF  "$(CFG)" == "MeshToCell - Win32 Release"

!ELSEIF  "$(CFG)" == "MeshToCell - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Target
# End Project
