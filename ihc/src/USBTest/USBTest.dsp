# Microsoft Developer Studio Project File - Name="USBTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=USBTest - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "USBTest.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "USBTest.mak" CFG="USBTest - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "USBTest - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "USBTest - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "USBTest - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
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

# Name "USBTest - Win32 Release"
# Name "USBTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\hwsetup.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IHCDebuger.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ihcdevicemanager.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ihcdevicemanagerbase.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ihcmotor.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ihcprinter.c

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ihcspidar4quad.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ihcspidaramp03.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\main.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OutPort.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PhysicsThread.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\qp_prog01.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\setting.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SH4Data.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SH4DataSet.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SH4Matrix.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SH4Vector.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SpidarThread.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TMU1_INTR.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TMUCtrl.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\USB_INTR.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\USBThread.cpp

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\builtin.inl
# End Source File
# Begin Source File

SOURCE=.\ihcdebuger.hpp
# End Source File
# Begin Source File

SOURCE=.\ihcdevicemanager.hpp
# End Source File
# Begin Source File

SOURCE=.\ihcdevicemanagerbase.hpp
# End Source File
# Begin Source File

SOURCE=.\ihcmotor.hpp
# End Source File
# Begin Source File

SOURCE=.\ihcprinter.h
# End Source File
# Begin Source File

SOURCE=.\ihcspidar4quad.hpp
# End Source File
# Begin Source File

SOURCE=.\iodefine.h
# End Source File
# Begin Source File

SOURCE=.\OutPort.h
# End Source File
# Begin Source File

SOURCE=.\packetdef.h
# End Source File
# Begin Source File

SOURCE=.\qp_prog01.h
# End Source File
# Begin Source File

SOURCE=.\regdefs.h
# End Source File
# Begin Source File

SOURCE=.\setting.h
# End Source File
# Begin Source File

SOURCE=.\SH4Data.hpp
# End Source File
# Begin Source File

SOURCE=.\SH4DataSet.hpp
# End Source File
# Begin Source File

SOURCE=.\SH4Matrix.hpp
# End Source File
# Begin Source File

SOURCE=.\SH4Vector.hpp
# End Source File
# Begin Source File

SOURCE=.\spidar.h
# End Source File
# Begin Source File

SOURCE=.\TMU1_INTR.hpp
# End Source File
# Begin Source File

SOURCE=.\TMUCtrl.h
# End Source File
# Begin Source File

SOURCE=.\USB_INTR.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\Makefile

!IF  "$(CFG)" == "USBTest - Win32 Release"

!ELSEIF  "$(CFG)" == "USBTest - Win32 Debug"

# Begin Custom Build
InputPath=.\Makefile

"none.x" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	set PATH=C:\home\hase\C\Project\HDHC\gnutools\sh-elf\bin;c:\cygwin\bin;%PATH% 
	make 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MatrixAsm.src
# End Source File
# End Target
# End Project
