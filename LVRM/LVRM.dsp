# Microsoft Developer Studio Project File - Name="LVRM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=LVRM - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "LVRM.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "LVRM.mak" CFG="LVRM - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "LVRM - Win32 Release" ("Win32 (x86) External Target" 用)
!MESSAGE "LVRM - Win32 Debug" ("Win32 (x86) External Target" 用)
!MESSAGE "LVRM - Win32 MFC Release" ("Win32 (x86) External Target" 用)
!MESSAGE "LVRM - Win32 MFC Debug" ("Win32 (x86) External Target" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/LVRM", BAAAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "LVRM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f LVRM.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "LVRM.exe"
# PROP BASE Bsc_Name "LVRM.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "LVRMLib.bat RELEASE"
# PROP Rebuild_Opt ""
# PROP Target_File "LVRM.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "LVRM - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f LVRM.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "LVRM.exe"
# PROP BASE Bsc_Name "LVRM.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "LVRMLib.bat DEBUG"
# PROP Rebuild_Opt ""
# PROP Target_File "LVRMD.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "LVRM - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MFCRelease"
# PROP BASE Intermediate_Dir "MFCRelease"
# PROP BASE Cmd_Line "VRLib.bat RELEASE"
# PROP BASE Rebuild_Opt ""
# PROP BASE Target_File "Release\LVRM.lib"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MFCRelease"
# PROP Intermediate_Dir "MFCRelease"
# PROP Cmd_Line "LVRMLib.bat MFCRELEASE"
# PROP Rebuild_Opt ""
# PROP Target_File "LVRMM.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "LVRM - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MFCDebug"
# PROP BASE Intermediate_Dir "MFCDebug"
# PROP BASE Cmd_Line "VRLib.bat DEBUG"
# PROP BASE Rebuild_Opt ""
# PROP BASE Target_File "Debug\LVRM.lib"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Cmd_Line "LVRMLib.bat MFCDEBUG"
# PROP Rebuild_Opt ""
# PROP Target_File "LVRMMD.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "LVRM - Win32 Release"
# Name "LVRM - Win32 Debug"
# Name "LVRM - Win32 MFC Release"
# Name "LVRM - Win32 MFC Debug"

!IF  "$(CFG)" == "LVRM - Win32 Release"

!ELSEIF  "$(CFG)" == "LVRM - Win32 Debug"

!ELSEIF  "$(CFG)" == "LVRM - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "LVRM - Win32 MFC Debug"

!ENDIF 

# Begin Source File

SOURCE=.\AllDoc.bat
# End Source File
# Begin Source File

SOURCE=.\archive.bat
# End Source File
# Begin Source File

SOURCE=.\LVRM.doxy
# End Source File
# Begin Source File

SOURCE=.\LVRM.dsw
# End Source File
# Begin Source File

SOURCE=.\LVRM.h
# End Source File
# Begin Source File

SOURCE=.\LVRMDoc.bat
# End Source File
# Begin Source File

SOURCE=.\LVRMLib.bat
# End Source File
# Begin Source File

SOURCE=.\LVRMSamples.dsw
# End Source File
# End Target
# End Project
