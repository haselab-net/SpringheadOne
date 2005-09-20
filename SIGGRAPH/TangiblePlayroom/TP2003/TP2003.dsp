# Microsoft Developer Studio Project File - Name="TP2003" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TP2003 - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "TP2003.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "TP2003.mak" CFG="TP2003 - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "TP2003 - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "TP2003 - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/SIGGRAPH/TabgiblePlayroom/TP2003", QDEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TP2003 - Win32 Release"

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
# ADD CPP /W3 /GR /GX /Zi /O2 /I "$(LVRM_ROOT)" /I ".\SDL\Include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /nologo /YX
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dsound.lib /nologo /subsystem:console /debug /machine:I386 /out:"TP2003.exe"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "$(LVRM_ROOT)" /I ".\SDL\Include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dsound.lib /nologo /subsystem:console /debug /machine:I386 /out:"TP2003D.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TP2003 - Win32 Release"
# Name "TP2003 - Win32 Debug"
# Begin Group "DebugLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DebugLib\BaseD.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugLib\WinBasisD.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugLib\SceneGraphD.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugLib\RenderD.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugLib\HISD.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugLib\libF77D.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugLib\libI77D.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DebugLib\QuadPRD.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "ReleaseLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ReleaseLib\Base.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ReleaseLib\Device.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ReleaseLib\HIS.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ReleaseLib\Render.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ReleaseLib\SceneGraph.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ReleaseLib\WinBasis.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ReleaseLib\QuadPR.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ReleaseLib\libI77.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ReleaseLib\libF77.lib

!IF  "$(CFG)" == "TP2003 - Win32 Release"

!ELSEIF  "$(CFG)" == "TP2003 - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SDL\lib\SDL.lib
# End Source File
# Begin Source File

SOURCE=.\SDL\lib\SDL_image.lib
# End Source File
# Begin Source File

SOURCE=.\SDL\lib\SDLmain.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\CharacterManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterManager.h
# End Source File
# Begin Source File

SOURCE=.\DispNode.cpp
# End Source File
# Begin Source File

SOURCE=.\DispNode.h
# End Source File
# Begin Source File

SOURCE=.\dsutil.cpp
# End Source File
# Begin Source File

SOURCE=.\dsutil.h
# End Source File
# Begin Source File

SOURCE=.\enumWins.cpp
# End Source File
# Begin Source File

SOURCE=.\environment.cpp
# End Source File
# Begin Source File

SOURCE=.\environment.h
# End Source File
# Begin Source File

SOURCE=.\game.cpp
# End Source File
# Begin Source File

SOURCE=.\game.h
# End Source File
# Begin Source File

SOURCE=.\gameView.cpp
# End Source File
# Begin Source File

SOURCE=.\GameView.h
# End Source File
# Begin Source File

SOURCE=.\graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\graphics.h
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Physics.cpp
# End Source File
# Begin Source File

SOURCE=.\Physics.h
# End Source File
# Begin Source File

SOURCE=.\SceneManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneManager.h
# End Source File
# Begin Source File

SOURCE=.\ResFiles\Script1.rc
# End Source File
# Begin Source File

SOURCE=.\settings.h
# End Source File
# Begin Source File

SOURCE=.\SndClass.cpp
# End Source File
# Begin Source File

SOURCE=.\SndClass.h
# End Source File
# Begin Source File

SOURCE=.\TextureImage.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureImage.h
# End Source File
# End Target
# End Project
