# Microsoft Developer Studio Project File - Name="Springhead" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=Springhead - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Springhead.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Springhead.mak" CFG="Springhead - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Springhead - Win32 Release" ("Win32 (x86) External Target" 用)
!MESSAGE "Springhead - Win32 Debug" ("Win32 (x86) External Target" 用)
!MESSAGE "Springhead - Win32 MFC Release" ("Win32 (x86) External Target" 用)
!MESSAGE "Springhead - Win32 MFC Debug" ("Win32 (x86) External Target" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Springhead/src", WPUAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "Springhead - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f Springhead.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Springhead.exe"
# PROP BASE Bsc_Name "Springhead.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "SpringheadLib.bat RELEASE"
# PROP Rebuild_Opt ""
# PROP Target_File "Springhead.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Springhead - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f Springhead.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Springhead.exe"
# PROP BASE Bsc_Name "Springhead.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "SpringheadLib.bat DEBUG"
# PROP Rebuild_Opt ""
# PROP Target_File "SpringheadD.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Springhead - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MFCRelease"
# PROP BASE Intermediate_Dir "MFCRelease"
# PROP BASE Cmd_Line "VRLib.bat RELEASE"
# PROP BASE Rebuild_Opt ""
# PROP BASE Target_File "Release\Springhead.lib"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MFCRelease"
# PROP Intermediate_Dir "MFCRelease"
# PROP Cmd_Line "SpringheadLib.bat MFCRELEASE"
# PROP Rebuild_Opt ""
# PROP Target_File "SpringheadM.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Springhead - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MFCDebug"
# PROP BASE Intermediate_Dir "MFCDebug"
# PROP BASE Cmd_Line "VRLib.bat DEBUG"
# PROP BASE Rebuild_Opt ""
# PROP BASE Target_File "Debug\Springhead.lib"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Cmd_Line "SpringheadLib.bat MFCDEBUG"
# PROP Rebuild_Opt ""
# PROP Target_File "SpringheadMD.lib"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Springhead - Win32 Release"
# Name "Springhead - Win32 Debug"
# Name "Springhead - Win32 MFC Release"
# Name "Springhead - Win32 MFC Debug"

!IF  "$(CFG)" == "Springhead - Win32 Release"

!ELSEIF  "$(CFG)" == "Springhead - Win32 Debug"

!ELSEIF  "$(CFG)" == "Springhead - Win32 MFC Release"

!ELSEIF  "$(CFG)" == "Springhead - Win32 MFC Debug"

!ENDIF 

# Begin Source File

SOURCE=.\Springhead.doxy
# End Source File
# Begin Source File

SOURCE=.\Springhead.h
# End Source File
# Begin Source File

SOURCE=.\SpringheadDoc.bat
# End Source File
# Begin Source File

SOURCE=.\SpringheadLib.bat
# End Source File
# End Target
# End Project
