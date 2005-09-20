# Microsoft Developer Studio Project File - Name="VHInteraction" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VHInteraction - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "VHInteraction.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "VHInteraction.mak" CFG="VHInteraction - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "VHInteraction - Win32 MFC Debug" ("Win32 (x86) Application" 用)
!MESSAGE "VHInteraction - Win32 MFC Release" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Experiments/VH/VHInteraction", AAICAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(SPRINGHEAD)\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX"stdafx.h" /FD /GZ /c
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

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

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
# ADD CPP /nologo /G6 /Gr /MD /W3 /GX /Zi /Ox /Ot /Oa /Og /Oi /Ob2 /Gf /I "$(SPRINGHEAD)\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# SUBTRACT CPP /Gy
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

# Name "VHInteraction - Win32 MFC Debug"
# Name "VHInteraction - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FWAppVH.cpp
# End Source File
# Begin Source File

SOURCE=.\HisConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SLCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VHCheckDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\VHInteraction.cpp
# End Source File
# Begin Source File

SOURCE=.\VHInteraction.rc
# End Source File
# Begin Source File

SOURCE=.\VHInteractionDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\VHInteractionView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CySLC.h
# End Source File
# Begin Source File

SOURCE=.\FWAppVH.h
# End Source File
# Begin Source File

SOURCE=.\HisConfig.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SLCamera.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VHCheckDialog.h
# End Source File
# Begin Source File

SOURCE=.\VHInteraction.h
# End Source File
# Begin Source File

SOURCE=.\VHInteractionDoc.h
# End Source File
# Begin Source File

SOURCE=.\VHInteractionView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VHInteraction.ico
# End Source File
# Begin Source File

SOURCE=.\res\VHInteraction.rc2
# End Source File
# Begin Source File

SOURCE=.\res\VHInteractionDoc.ico
# End Source File
# End Group
# Begin Group "DebugLib"

# PROP Default_Filter ".lib"
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Base\BaseMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\SceneGraph\SceneGraphMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\WinBasis\WinBasisMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\FileIO\FileIOMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\ImpD3D\ImpD3DMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Collision\CollisionMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Physics\PhysicsMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Device\DeviceMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\HIS\HISMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Graphics\GraphicsMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\WinDX\WinDXMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\GraphicsGL\GraphicsGLMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Framework\FrameworkMD.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Springhead\src\Creature\CreatureMD.lib

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CySLC.lib
# End Source File
# End Group
# Begin Group "ReleaseLib"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Base\BaseM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\SceneGraph\SceneGraphM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\WinBasis\WinBasisM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\FileIO\FileIOM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\ImpD3D\ImpD3DM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Collision\CollisionM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Physics\PhysicsM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Device\DeviceM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\HIS\HISM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Graphics\GraphicsM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\WinDX\WinDXM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\GraphicsGL\GraphicsGLM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="$(SPRINGHEAD)\src\Framework\FrameworkM.lib"

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Springhead\src\Creature\CreatureM.lib

!IF  "$(CFG)" == "VHInteraction - Win32 MFC Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VHInteraction - Win32 MFC Release"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\block3hap.x
# End Source File
# Begin Source File

SOURCE=.\jenga.x
# End Source File
# Begin Source File

SOURCE=.\PointerTest.x
# End Source File
# Begin Source File

SOURCE=.\TopRotY.x
# End Source File
# Begin Source File

SOURCE=.\TouchTest.x
# End Source File
# End Target
# End Project
