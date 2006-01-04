# Microsoft Developer Studio Project File - Name="D3DDPEx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=D3DDPEx - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "D3DDPEx.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "D3DDPEx.mak" CFG="D3DDPEx - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "D3DDPEx - Win32 MFC Debug" ("Win32 (x86) Application" 用)
!MESSAGE "D3DDPEx - Win32 MFC Release" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/personal/hase/VR/LVRM/Samples/D3DDPEx", LKAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "D3DDPEx - Win32 MFC Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "D3DDPEx___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "D3DDPEx___Win32_MFC_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX"stdafx.h" /FD /I /mssdk/include" " /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "../../.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX"stdafx.h" /FD /I /mssdk/include" " /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib ddraw.lib d3dx.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "D3DDPEx - Win32 MFC Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "D3DDPEx___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "D3DDPEx___Win32_MFC_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ENDIF 

# Begin Target

# Name "D3DDPEx - Win32 MFC Debug"
# Name "D3DDPEx - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\D3DDPEx.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DDPEx.rc
# End Source File
# Begin Source File

SOURCE=.\D3DDPExDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\D3DDPExView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\D3DDPEx.h
# End Source File
# Begin Source File

SOURCE=.\D3DDPExDoc.h
# End Source File
# Begin Source File

SOURCE=.\D3DDPExView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\D3DDPEx.ico
# End Source File
# Begin Source File

SOURCE=.\res\D3DDPEx.rc2
# End Source File
# Begin Source File

SOURCE=.\res\D3DDPExDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "ReleaseLib"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=..\..\..\WinBasis\WinBasisM.lib

!IF  "$(CFG)" == "D3DDPEx - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "D3DDPEx - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\WinDXM.lib

!IF  "$(CFG)" == "D3DDPEx - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "D3DDPEx - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Base\BaseM.lib

!IF  "$(CFG)" == "D3DDPEx - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "D3DDPEx - Win32 MFC Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "DebugLib"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=..\..\..\Base\BaseMD.lib

!IF  "$(CFG)" == "D3DDPEx - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "D3DDPEx - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\WinDXMD.lib

!IF  "$(CFG)" == "D3DDPEx - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "D3DDPEx - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\WinBasis\WinBasisMD.lib

!IF  "$(CFG)" == "D3DDPEx - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "D3DDPEx - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
