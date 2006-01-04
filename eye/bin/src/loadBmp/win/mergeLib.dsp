# Microsoft Developer Studio Project File - Name="mergeLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=mergeLib - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "mergeLib.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "mergeLib.mak" CFG="mergeLib - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "mergeLib - Win32 Release" ("Win32 (x86) Generic Project" 用)
!MESSAGE "mergeLib - Win32 Debug" ("Win32 (x86) Generic Project" 用)
!MESSAGE "mergeLib - Win32 MFC Debug" ("Win32 (x86) Generic Project" 用)
!MESSAGE "mergeLib - Win32 MFC Release" ("Win32 (x86) Generic Project" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "mergeLib - Win32 Release"

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

!ELSEIF  "$(CFG)" == "mergeLib - Win32 Debug"

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

!ELSEIF  "$(CFG)" == "mergeLib - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mergeLib___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "mergeLib___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "mergeLib___Win32_MFC_Debug"
# PROP Intermediate_Dir "mergeLib___Win32_MFC_Debug"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "mergeLib - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mergeLib___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "mergeLib___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "mergeLib___Win32_MFC_Release"
# PROP Intermediate_Dir "mergeLib___Win32_MFC_Release"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "mergeLib - Win32 Release"
# Name "mergeLib - Win32 Debug"
# Name "mergeLib - Win32 MFC Debug"
# Name "mergeLib - Win32 MFC Release"
# Begin Source File

SOURCE=.\mergeLib.bat

!IF  "$(CFG)" == "mergeLib - Win32 Release"

# Begin Custom Build
InputPath=.\mergeLib.bat

"libPaint.lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(InputPath) RELEASE

# End Custom Build

!ELSEIF  "$(CFG)" == "mergeLib - Win32 Debug"

# Begin Custom Build
InputPath=.\mergeLib.bat

"libPaintD.lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(InputPath) DEBUG

# End Custom Build

!ELSEIF  "$(CFG)" == "mergeLib - Win32 MFC Debug"

# Begin Custom Build
InputPath=.\mergeLib.bat

"libPaintMD.lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(InputPath) MFCDEBUG

# End Custom Build

!ELSEIF  "$(CFG)" == "mergeLib - Win32 MFC Release"

# Begin Custom Build
InputPath=.\mergeLib.bat

"libPaintM.lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(InputPath) MFCRELEASE

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
