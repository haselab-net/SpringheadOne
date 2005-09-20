# Microsoft Developer Studio Project File - Name="DynaClockTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DynaClockTest - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "DynaClockTest.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "DynaClockTest.mak" CFG="DynaClockTest - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "DynaClockTest - Win32 MFC Debug" ("Win32 (x86) Application" 用)
!MESSAGE "DynaClockTest - Win32 MFC Release" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/SceneGraph/Samples/DynaClockTest", UTUAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MFC Debug"
# PROP BASE Intermediate_Dir "MFC Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(SPRINGHEAD)" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /Op /I "$(SPRINGHEAD)\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MFC Release"
# PROP BASE Intermediate_Dir "MFC Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MFCRelease"
# PROP Intermediate_Dir "MFCRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "$(SPRINGHEAD)" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /vd0 /GX /Zi /Ox /Ot /Og /Oi /Op /Ob2 /Gf /I "." /I "$(SPRINGHEAD)/src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Oa /Ow /Gy /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "DynaClockTest - Win32 MFC Debug"
# Name "DynaClockTest - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DynaClockTest.cpp
# End Source File
# Begin Source File

SOURCE=.\DynaClockTest.rc
# End Source File
# Begin Source File

SOURCE=.\DynaClockTestDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DynaClockTestView.cpp
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

SOURCE=.\DynaClockTest.h
# End Source File
# Begin Source File

SOURCE=.\DynaClockTestDoc.h
# End Source File
# Begin Source File

SOURCE=.\DynaClockTestView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PreciseTimer.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DynaClockTest.ico
# End Source File
# Begin Source File

SOURCE=.\res\DynaClockTest.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DynaClockTestDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "DebugLib"

# PROP Default_Filter ".lib"
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Base\BaseMD.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\SceneGraph\SceneGraphMD.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\WinBasis\WinBasisMD.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\WinDX\WinDX8MD.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\FileIO\FileIOMD.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\ImpD3D\ImpD3DMD.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Collision\CollisionMD.lib

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Physics\PhysicsMD.lib

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "ReleaseLib"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Base\BaseM.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\SceneGraph\SceneGraphM.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\WinBasis\WinBasisM.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\WinDX\WinDX8M.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\FileIO\FileIOM.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\ImpD3D\ImpD3DM.lib"

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Collision\CollisionM.lib

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Physics\PhysicsM.lib

!IF  "$(CFG)" == "DynaClockTest - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "DynaClockTest - Win32 MFC Release"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\col.x
# End Source File
# Begin Source File

SOURCE=.\col2.x
# End Source File
# End Target
# End Project
