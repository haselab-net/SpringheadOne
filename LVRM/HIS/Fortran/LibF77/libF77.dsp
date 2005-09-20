# Microsoft Developer Studio Project File - Name="libF77" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libF77 - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "libF77.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "libF77.mak" CFG="libF77 - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "libF77 - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "libF77 - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "libF77 - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "libF77 - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/Experiments/DynaLocalHap/Fortran", MUBCAAAA"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libF77 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libF77 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../libF77D.lib"

!ELSEIF  "$(CFG)" == "libF77 - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MFC Debug"
# PROP BASE Intermediate_Dir "MFC Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MFCDebug"
# PROP Intermediate_Dir "MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\libF77MD.lib"

!ELSEIF  "$(CFG)" == "libF77 - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MFC Release"
# PROP BASE Intermediate_Dir "MFC Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MFCRelease"
# PROP Intermediate_Dir "MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\libF77M.lib"

!ENDIF 

# Begin Target

# Name "libF77 - Win32 Release"
# Name "libF77 - Win32 Debug"
# Name "libF77 - Win32 MFC Debug"
# Name "libF77 - Win32 MFC Release"
# Begin Source File

SOURCE=.\ABORT_.C
# End Source File
# Begin Source File

SOURCE=.\C_ABS.C
# End Source File
# Begin Source File

SOURCE=.\C_cos.c
# End Source File
# Begin Source File

SOURCE=.\C_DIV.C
# End Source File
# Begin Source File

SOURCE=.\C_exp.c
# End Source File
# Begin Source File

SOURCE=.\C_log.c
# End Source File
# Begin Source File

SOURCE=.\C_sin.c
# End Source File
# Begin Source File

SOURCE=.\C_sqrt.c
# End Source File
# Begin Source File

SOURCE=.\CABS.C
# End Source File
# Begin Source File

SOURCE=.\D_ABS.C
# End Source File
# Begin Source File

SOURCE=.\D_ACOS.C
# End Source File
# Begin Source File

SOURCE=.\D_ASIN.C
# End Source File
# Begin Source File

SOURCE=.\D_ATAN.C
# End Source File
# Begin Source File

SOURCE=.\D_ATN2.C
# End Source File
# Begin Source File

SOURCE=.\D_CNJG.C
# End Source File
# Begin Source File

SOURCE=.\D_COS.C
# End Source File
# Begin Source File

SOURCE=.\D_COSH.C
# End Source File
# Begin Source File

SOURCE=.\D_DIM.C
# End Source File
# Begin Source File

SOURCE=.\D_EXP.C
# End Source File
# Begin Source File

SOURCE=.\D_IMAG.C
# End Source File
# Begin Source File

SOURCE=.\D_INT.C
# End Source File
# Begin Source File

SOURCE=.\D_LG10.C
# End Source File
# Begin Source File

SOURCE=.\D_LOG.C
# End Source File
# Begin Source File

SOURCE=.\D_MOD.C
# End Source File
# Begin Source File

SOURCE=.\D_NINT.C
# End Source File
# Begin Source File

SOURCE=.\D_PROD.C
# End Source File
# Begin Source File

SOURCE=.\D_SIGN.C
# End Source File
# Begin Source File

SOURCE=.\D_SIN.C
# End Source File
# Begin Source File

SOURCE=.\D_SINH.C
# End Source File
# Begin Source File

SOURCE=.\D_SQRT.C
# End Source File
# Begin Source File

SOURCE=.\D_TAN.C
# End Source File
# Begin Source File

SOURCE=.\D_TANH.C
# End Source File
# Begin Source File

SOURCE=.\DERF_.C
# End Source File
# Begin Source File

SOURCE=.\DERFC_.C
# End Source File
# Begin Source File

SOURCE=.\EF1ASC_.C
# End Source File
# Begin Source File

SOURCE=.\EF1CMC_.C
# End Source File
# Begin Source File

SOURCE=.\ERF_.C
# End Source File
# Begin Source File

SOURCE=.\ERFC_.C
# End Source File
# Begin Source File

SOURCE=.\F2C.H
# End Source File
# Begin Source File

SOURCE=.\FIO.H
# End Source File
# Begin Source File

SOURCE=.\FMT.H
# End Source File
# Begin Source File

SOURCE=.\FP.H
# End Source File
# Begin Source File

SOURCE=.\GETARG_.C
# End Source File
# Begin Source File

SOURCE=.\GETENV_.C
# End Source File
# Begin Source File

SOURCE=.\H_ABS.C
# End Source File
# Begin Source File

SOURCE=.\H_DIM.C
# End Source File
# Begin Source File

SOURCE=.\H_DNNT.C
# End Source File
# Begin Source File

SOURCE=.\H_INDX.C
# End Source File
# Begin Source File

SOURCE=.\H_LEN.C
# End Source File
# Begin Source File

SOURCE=.\H_MOD.C
# End Source File
# Begin Source File

SOURCE=.\H_NINT.C
# End Source File
# Begin Source File

SOURCE=.\H_SIGN.C
# End Source File
# Begin Source File

SOURCE=.\HL_GE.C
# End Source File
# Begin Source File

SOURCE=.\HL_GT.C
# End Source File
# Begin Source File

SOURCE=.\HL_LE.C
# End Source File
# Begin Source File

SOURCE=.\HL_LT.C
# End Source File
# Begin Source File

SOURCE=.\I_ABS.C
# End Source File
# Begin Source File

SOURCE=.\I_DIM.C
# End Source File
# Begin Source File

SOURCE=.\I_DNNT.C
# End Source File
# Begin Source File

SOURCE=.\I_INDX.C
# End Source File
# Begin Source File

SOURCE=.\I_LEN.C
# End Source File
# Begin Source File

SOURCE=.\I_MOD.C
# End Source File
# Begin Source File

SOURCE=.\I_NINT.C
# End Source File
# Begin Source File

SOURCE=.\I_SIGN.C
# End Source File
# Begin Source File

SOURCE=.\IARGC_.C
# End Source File
# Begin Source File

SOURCE=.\L_GE.C
# End Source File
# Begin Source File

SOURCE=.\L_GT.C
# End Source File
# Begin Source File

SOURCE=.\L_LE.C
# End Source File
# Begin Source File

SOURCE=.\L_LT.C
# End Source File
# Begin Source File

SOURCE=.\LIO.H
# End Source File
# Begin Source File

SOURCE=.\MAIN.C
# End Source File
# Begin Source File

SOURCE=.\POW_CI.C
# End Source File
# Begin Source File

SOURCE=.\POW_DD.C
# End Source File
# Begin Source File

SOURCE=.\POW_DI.C
# End Source File
# Begin Source File

SOURCE=.\POW_HH.C
# End Source File
# Begin Source File

SOURCE=.\POW_II.C
# End Source File
# Begin Source File

SOURCE=.\Pow_qq.c
# End Source File
# Begin Source File

SOURCE=.\POW_RI.C
# End Source File
# Begin Source File

SOURCE=.\POW_ZI.C
# End Source File
# Begin Source File

SOURCE=.\POW_ZZ.C
# End Source File
# Begin Source File

SOURCE=.\R_ABS.C
# End Source File
# Begin Source File

SOURCE=.\R_ACOS.C
# End Source File
# Begin Source File

SOURCE=.\R_ASIN.C
# End Source File
# Begin Source File

SOURCE=.\R_ATAN.C
# End Source File
# Begin Source File

SOURCE=.\R_ATN2.C
# End Source File
# Begin Source File

SOURCE=.\R_CNJG.C
# End Source File
# Begin Source File

SOURCE=.\R_COS.C
# End Source File
# Begin Source File

SOURCE=.\R_COSH.C
# End Source File
# Begin Source File

SOURCE=.\R_DIM.C
# End Source File
# Begin Source File

SOURCE=.\R_EXP.C
# End Source File
# Begin Source File

SOURCE=.\R_IMAG.C
# End Source File
# Begin Source File

SOURCE=.\R_INT.C
# End Source File
# Begin Source File

SOURCE=.\R_LG10.C
# End Source File
# Begin Source File

SOURCE=.\R_LOG.C
# End Source File
# Begin Source File

SOURCE=.\R_MOD.C
# End Source File
# Begin Source File

SOURCE=.\R_NINT.C
# End Source File
# Begin Source File

SOURCE=.\R_SIGN.C
# End Source File
# Begin Source File

SOURCE=.\R_SIN.C
# End Source File
# Begin Source File

SOURCE=.\R_SINH.C
# End Source File
# Begin Source File

SOURCE=.\R_SQRT.C
# End Source File
# Begin Source File

SOURCE=.\R_TAN.C
# End Source File
# Begin Source File

SOURCE=.\R_TANH.C
# End Source File
# Begin Source File

SOURCE=.\RAWIO.H
# End Source File
# Begin Source File

SOURCE=.\S_CAT.C
# End Source File
# Begin Source File

SOURCE=.\S_CMP.C
# End Source File
# Begin Source File

SOURCE=.\S_COPY.C
# End Source File
# Begin Source File

SOURCE=.\S_PAUS.C
# End Source File
# Begin Source File

SOURCE=.\S_RNGE.C
# End Source File
# Begin Source File

SOURCE=.\S_STOP.C
# End Source File
# Begin Source File

SOURCE=.\SIG_DIE.C
# End Source File
# Begin Source File

SOURCE=.\SIGNAL_.C
# End Source File
# Begin Source File

SOURCE=.\SYSTEM_.C
# End Source File
# Begin Source File

SOURCE=.\VERSION.C
# End Source File
# Begin Source File

SOURCE=.\Z_ABS.C
# End Source File
# Begin Source File

SOURCE=.\Z_COS.C
# End Source File
# Begin Source File

SOURCE=.\Z_DIV.C
# End Source File
# Begin Source File

SOURCE=.\Z_EXP.C
# End Source File
# Begin Source File

SOURCE=.\Z_LOG.C
# End Source File
# Begin Source File

SOURCE=.\Z_SIN.C
# End Source File
# Begin Source File

SOURCE=.\Z_SQRT.C
# End Source File
# End Target
# End Project
