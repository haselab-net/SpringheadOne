# Microsoft Developer Studio Project File - Name="MFCD3DDyna" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MFCD3DDyna - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "MFCD3DDyna.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "MFCD3DDyna.mak" CFG="MFCD3DDyna - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "MFCD3DDyna - Win32 MFC Debug" ("Win32 (x86) Application" 用)
!MESSAGE "MFCD3DDyna - Win32 MFC Release" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src/SceneGraph/Samples/MFCD3DDyna", UTUAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /Op /Gf /Gy /I "$(SPRINGHEAD)\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
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

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

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
# ADD CPP /nologo /G6 /Gr /MD /W3 /GX /Zi /Ox /Ot /Og /Oi /Op /Oy- /Ob2 /Gf /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /FD /c
# SUBTRACT CPP /Oa /Ow /Gy
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "MFCD3DDyna - Win32 MFC Debug"
# Name "MFCD3DDyna - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MFCD3DDyna.cpp
# End Source File
# Begin Source File

SOURCE=.\MFCD3DDyna.rc
# End Source File
# Begin Source File

SOURCE=.\MFCD3DDynaDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MFCD3DDynaView.cpp
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

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MFCD3DDyna.h
# End Source File
# Begin Source File

SOURCE=.\MFCD3DDynaDoc.h
# End Source File
# Begin Source File

SOURCE=.\MFCD3DDynaView.h
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

SOURCE=.\res\MFCD3DDyna.ico
# End Source File
# Begin Source File

SOURCE=.\res\MFCD3DDyna.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MFCD3DDynaDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "ReleaseLib"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Base\BaseM.lib"

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\SceneGraph\SceneGraphM.lib"

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\WinBasis\WinBasisM.lib"

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\FileIO\FileIOM.lib"

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\ImpD3D\ImpD3DM.lib"

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Collision\CollisionM.lib"

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Physics\PhysicsM.lib"

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Graphics\GraphicsM.lib"

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\WinDX\WinDXM.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\HIS\HISM.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\GraphicsGL\GraphicsGLM.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Framework\FrameworkM.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Creature\CreatureM.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "DebugLib"

# PROP Default_Filter ".lib"
# Begin Source File

SOURCE=..\..\Base\BaseMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SceneGraph\SceneGraphMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\WinBasis\WinBasisMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\FileIO\FileIOMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\ImpD3D\ImpD3DMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Collision\CollisionMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Physics\PhysicsMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Graphics\GraphicsMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\WinDX\WinDXMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\HIS\HISMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\GraphicsGL\GraphicsGLMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Framework\FrameworkMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\Creature\CreatureMD.lib

!IF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "MFCD3DDyna - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
