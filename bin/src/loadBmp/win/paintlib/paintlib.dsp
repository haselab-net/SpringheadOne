# Microsoft Developer Studio Project File - Name="paintlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=paintlib - Win32 MFC Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "paintlib.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "paintlib.mak" CFG="paintlib - Win32 MFC Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "paintlib - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "paintlib - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "paintlib - Win32 MFC Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE "paintlib - Win32 MFC Release" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/paintlib", CAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "paintlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\Release"
# PROP Intermediate_Dir "..\..\obj\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /MT /W3 /GR /GX /O2 /Ob2 /I "." /I "..\..\common" /I "..\..\common\filter" /I "..\..\otherlib\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /I "..\..\otherlib\libungif" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"plstdpch.h" /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\Debug"
# PROP Intermediate_Dir "..\..\obj\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /Gi /GR /GX /ZI /Od /I "." /I "..\..\common" /I "..\..\common\filter" /I "..\libtiff" /I "..\..\otherlib\libtiff\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /I "..\..\otherlib\libungif\lib" /I "..\..\otherlib\libcurl\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"plstdpch.h" /Yd /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "paintlib - Win32 MFC Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "paintlib___Win32_MFC_Debug"
# PROP BASE Intermediate_Dir "paintlib___Win32_MFC_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\MFCDebug"
# PROP Intermediate_Dir "..\..\obj\MFCDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /W3 /Gi /GR /GX /ZI /Od /I "." /I "..\..\common" /I "..\..\common\filter" /I "..\libtiff" /I "..\..\otherlib\libtiff\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /I "..\..\otherlib\libungif\lib" /I "..\..\otherlib\libcurl\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"plstdpch.h" /Yd /c
# ADD CPP /nologo /G5 /MDd /W3 /Gi /GR /GX /ZI /Od /I "." /I "..\..\common" /I "..\..\common\filter" /I "..\libtiff" /I "..\..\otherlib\libtiff\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /I "..\..\otherlib\libungif\lib" /I "..\..\otherlib\libcurl\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"plstdpch.h" /Yd /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "paintlib - Win32 MFC Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "paintlib___Win32_MFC_Release"
# PROP BASE Intermediate_Dir "paintlib___Win32_MFC_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\MFCRelease"
# PROP Intermediate_Dir "..\..\obj\MFCRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GR /GX /O2 /Ob2 /I "..\..\otherlib\libcurl\include" /I "." /I "..\..\common" /I "..\..\common\filter" /I "..\libtiff" /I "..\..\otherlib\libtiff\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /I "..\..\otherlib\libungif\lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"plstdpch.h" /c
# ADD CPP /nologo /G6 /Gr /MD /W3 /GR /GX /O2 /Ob2 /I "..\..\otherlib\libcurl\include" /I "." /I "..\..\common" /I "..\..\common\filter" /I "..\libtiff" /I "..\..\otherlib\libtiff\libtiff" /I "..\libjpeg" /I "..\..\otherlib\libjpeg" /I "..\..\otherlib\libpng" /I "..\..\otherlib\zlib" /I "..\..\otherlib\libungif\lib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"plstdpch.h" /c
# ADD BASE RSC /l 0x407
# ADD RSC /l 0x407 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "paintlib - Win32 Release"
# Name "paintlib - Win32 Debug"
# Name "paintlib - Win32 MFC Debug"
# Name "paintlib - Win32 MFC Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Group "Filter."

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\Filter\PLFilter.cpp
DEP_CPP_PLFIL=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\planybmp.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterContrast.cpp
DEP_CPP_PLFILT=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterContrast.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterCrop.cpp
DEP_CPP_PLFILTE=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterCrop.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterFlip.cpp
DEP_CPP_PLFILTER=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterFlip.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterGetAlpha.cpp
DEP_CPP_PLFILTERG=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterGetAlpha.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterGrayscale.cpp
DEP_CPP_PLFILTERGR=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterGrayscale.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterIntensity.cpp
DEP_CPP_PLFILTERI=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterIntensity.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterLightness.cpp
DEP_CPP_PLFILTERL=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterLightness.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterMirror.cpp
DEP_CPP_PLFILTERM=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterMirror.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterQuantize.cpp
DEP_CPP_PLFILTERQ=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterQuantize.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResize.cpp
DEP_CPP_PLFILTERR=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeBilinear.cpp
DEP_CPP_PLFILTERRE=\
	"..\..\common\Filter\pl2PassScale.h"\
	"..\..\common\Filter\plcontribdefs.h"\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\Filter\PLFilterResizeBilinear.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeBox.cpp
DEP_CPP_PLFILTERRES=\
	"..\..\common\Filter\pl2PassScale.h"\
	"..\..\common\Filter\plcontribdefs.h"\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\Filter\PLFilterResizeBox.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeGaussian.cpp
DEP_CPP_PLFILTERRESI=\
	"..\..\common\Filter\pl2PassScale.h"\
	"..\..\common\Filter\plcontribdefs.h"\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\Filter\PLFilterResizeGaussian.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeHamming.cpp
DEP_CPP_PLFILTERRESIZ=\
	"..\..\common\Filter\pl2PassScale.h"\
	"..\..\common\Filter\plcontribdefs.h"\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterResize.h"\
	"..\..\common\Filter\PLFilterResizeHamming.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterThreshold.cpp
DEP_CPP_PLFILTERT=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterThreshold.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plfiltervideoinvert.cpp
DEP_CPP_PLFILTERV=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterVideoInvert.h"\
	"..\..\common\Filter\plhsvconvert.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plhsvconvert.cpp
DEP_CPP_PLHSV=\
	"..\..\common\Filter\plhsvconvert.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# End Group
# Begin Group "Win."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\plddhelper.cpp
DEP_CPP_PLDDH=\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	".\plddhelper.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plddrawbmp.cpp
DEP_CPP_PLDDR=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\plddhelper.h"\
	".\plddrawbmp.h"\
	
# End Source File
# Begin Source File

SOURCE=.\pldibsect.cpp
DEP_CPP_PLDIB=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\pldibsect.h"\
	".\plwinbmp.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plressrc.cpp
DEP_CPP_PLRES=\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	".\plressrc.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plStreamSink.cpp
DEP_CPP_PLSTR=\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	".\plStreamSink.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plwemfdec.cpp
DEP_CPP_PLWEM=\
	"..\..\Common\config.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\plwemfdec.h"\
	".\plwinbmp.h"\
	
# End Source File
# Begin Source File

SOURCE=.\plwinbmp.cpp
DEP_CPP_PLWIN=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\plwinbmp.h"\
	
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\common\jmemdest.cpp
DEP_CPP_JMEMD=\
	"..\..\common\jmemdest.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\jmemsrc.cpp
DEP_CPP_JMEMS=\
	"..\..\Common\jmemsrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\planybmp.cpp
DEP_CPP_PLANY=\
	"..\..\common\planybmp.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\planydec.cpp
DEP_CPP_PLANYD=\
	"..\..\Common\config.h"\
	"..\..\common\planydec.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpdec.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plgifdec.h"\
	"..\..\common\pliff85.h"\
	"..\..\common\pliff85dec.h"\
	"..\..\common\pljpegdec.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpcx.h"\
	"..\..\common\plpcxdec.h"\
	"..\..\common\plpgm.h"\
	"..\..\common\plpgmdec.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpictdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpngdec.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plppm.h"\
	"..\..\common\plppmdec.h"\
	"..\..\common\plpsddec.h"\
	"..\..\common\plsgidec.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\pltga.h"\
	"..\..\common\pltgadec.h"\
	"..\..\common\pltiffdec.h"\
	"..\..\common\plwindefs.h"\
	"..\..\common\qdraw.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	"..\..\otherlib\libpng\png.h"\
	"..\..\otherlib\libpng\pngconf.h"\
	"..\..\otherlib\zLib\zconf.h"\
	"..\..\otherlib\zLib\zlib.h"\
	".\plwemfdec.h"\
	
NODEP_CPP_PLANYD=\
	"..\..\common\tiff.h"\
	"..\..\common\tiffio.h"\
	"..\..\otherlib\libpng\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plbitmap.cpp
DEP_CPP_PLBIT=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\Filter\PLFilterQuantize.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	

!IF  "$(CFG)" == "paintlib - Win32 Release"

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# SUBTRACT CPP /X

!ELSEIF  "$(CFG)" == "paintlib - Win32 MFC Debug"

# SUBTRACT BASE CPP /X
# SUBTRACT CPP /X

!ELSEIF  "$(CFG)" == "paintlib - Win32 MFC Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpbase.cpp
DEP_CPP_PLBMP=\
	"..\..\common\Filter\PLFilter.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpdec.cpp
DEP_CPP_PLBMPD=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpdec.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\plwindefs.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpenc.cpp
DEP_CPP_PLBMPE=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpenc.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicenc.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\plwindefs.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpinfo.cpp
DEP_CPP_PLBMPI=\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pldatasink.cpp
DEP_CPP_PLDAT=\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pldatasrc.cpp
DEP_CPP_PLDATA=\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plprognot.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plexcept.cpp
DEP_CPP_PLEXC=\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plexif.cpp
DEP_CPP_PLEXI=\
	"..\..\Common\config.h"\
	"..\..\common\plcountedpointer.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plexif.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plfilesink.cpp
DEP_CPP_PLFILE=\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesink.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plfilesrc.cpp
DEP_CPP_PLFILES=\
	"..\..\Common\config.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plgifdec.cpp
DEP_CPP_PLGIF=\
	"..\..\common\planybmp.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plgifdec.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
NODEP_CPP_PLGIF=\
	"..\..\common\gif_lib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pliff85dec.cpp
DEP_CPP_PLIFF=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\pliff85.h"\
	"..\..\common\pliff85dec.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pljpegdec.cpp
DEP_CPP_PLJPE=\
	"..\..\Common\jmemsrc.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\plcountedpointer.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plexif.h"\
	"..\..\common\pljpegdec.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	"..\libjpeg\jmorecfg.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pljpegenc.cpp
DEP_CPP_PLJPEG=\
	"..\..\common\jmemdest.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\plcountedpointer.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plexif.h"\
	"..\..\common\pljpegenc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicenc.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	"..\libjpeg\jmorecfg.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plmemsink.cpp
DEP_CPP_PLMEM=\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plmemsink.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plmemsrc.cpp
DEP_CPP_PLMEMS=\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plmemsrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\PLObject.cpp
DEP_CPP_PLOBJ=\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpcxdec.cpp
DEP_CPP_PLPCX=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpcx.h"\
	"..\..\common\plpcxdec.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpgmdec.cpp
DEP_CPP_PLPGM=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpgm.h"\
	"..\..\common\plpgmdec.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpicdec.cpp
DEP_CPP_PLPIC=\
	"..\..\Common\config.h"\
	"..\..\common\planybmp.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesrc.h"\
	"..\..\common\plmemsrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	".\plressrc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpicenc.cpp
DEP_CPP_PLPICE=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesink.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpicenc.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpictdec.cpp
DEP_CPP_PLPICT=\
	"..\..\Common\config.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\pljpegdec.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\ploptable.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpictdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\qdraw.h"\
	"..\..\otherlib\libjpeg\jconfig.h"\
	"..\..\otherlib\libjpeg\jerror.h"\
	"..\..\otherlib\libjpeg\jmorecfg.h"\
	"..\..\otherlib\libjpeg\jpegint.h"\
	"..\..\otherlib\libjpeg\jpeglib.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpngdec.cpp
DEP_CPP_PLPNG=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpngdec.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libpng\png.h"\
	"..\..\otherlib\libpng\pngconf.h"\
	"..\..\otherlib\zLib\zconf.h"\
	"..\..\otherlib\zLib\zlib.h"\
	
NODEP_CPP_PLPNG=\
	"..\..\otherlib\libpng\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpngenc.cpp
DEP_CPP_PLPNGE=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicenc.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpngenc.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\otherlib\libpng\png.h"\
	"..\..\otherlib\libpng\pngconf.h"\
	"..\..\otherlib\zLib\zconf.h"\
	"..\..\otherlib\zLib\zlib.h"\
	
NODEP_CPP_PLPNGE=\
	"..\..\otherlib\libpng\alloc.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plppmdec.cpp
DEP_CPP_PLPPM=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plppm.h"\
	"..\..\common\plppmdec.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plpsddec.cpp
DEP_CPP_PLPSD=\
	"..\..\Common\config.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\plfilesrc.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixel8.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plpsddec.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plsgidec.cpp
DEP_CPP_PLSGI=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plsgidec.h"\
	"..\..\common\plstdpch.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\plstdpch.cpp
DEP_CPP_PLSTD=\
	"..\..\common\pldebug.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plstdpch.h"\
	
# ADD CPP /Yc"plstdpch.h"
# End Source File
# Begin Source File

SOURCE=..\..\common\pltgadec.cpp
DEP_CPP_PLTGA=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\pltga.h"\
	"..\..\common\pltgadec.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pltiffdec.cpp
DEP_CPP_PLTIF=\
	"..\..\Common\config.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasrc.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicdec.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\pltiffdec.h"\
	"..\..\Common\tif_msrc.h"\
	
NODEP_CPP_PLTIF=\
	"..\..\common\tiff.h"\
	"..\..\common\tiffio.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pltiffenc.cpp
DEP_CPP_PLTIFF=\
	"..\..\Common\config.h"\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicenc.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\pltiffenc.h"\
	"..\..\Common\tif_msrc.h"\
	
NODEP_CPP_PLTIFF=\
	"..\..\common\tiffio.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\common\pltiffencex.cpp
DEP_CPP_PLTIFFE=\
	"..\..\common\plbitmap.h"\
	"..\..\common\plbmpbase.h"\
	"..\..\common\plbmpinfo.h"\
	"..\..\common\pldatasink.h"\
	"..\..\common\pldebug.h"\
	"..\..\common\plexcept.h"\
	"..\..\common\PLObject.h"\
	"..\..\common\plpaintlibdefs.h"\
	"..\..\common\plpicenc.h"\
	"..\..\common\plpixel16.h"\
	"..\..\common\plpixel24.h"\
	"..\..\common\plpixel32.h"\
	"..\..\common\plpixeldefs.h"\
	"..\..\common\plpoint.h"\
	"..\..\common\plstdpch.h"\
	"..\..\common\pltiffenc.h"\
	"..\..\common\pltiffencex.h"\
	"..\..\Common\tif_msrc.h"\
	
NODEP_CPP_PLTIFFE=\
	"..\..\common\tiffio.h"\
	
# End Source File
# Begin Source File

SOURCE=..\..\Common\tif_msrc.c
DEP_CPP_TIF_M=\
	"..\..\Common\tif_msrc.h"\
	
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Group "Filter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\Filter\pl2PassScale.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plcontribdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilter.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterContrast.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterCrop.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterFill.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterFillRect.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterFlip.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterGetAlpha.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterGrayscale.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterIntensity.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterLightness.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterMirror.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plfilterpixel.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterQuantize.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResize.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeBilinear.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeBox.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeGaussian.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterResizeHamming.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterThreshold.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\PLFilterVideoInvert.h
# End Source File
# Begin Source File

SOURCE=..\..\common\Filter\plhsvconvert.h
# End Source File
# End Group
# Begin Group "Win"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\plddhelper.h
# End Source File
# Begin Source File

SOURCE=.\plddrawbmp.h
# End Source File
# Begin Source File

SOURCE=.\pldibsect.h
# End Source File
# Begin Source File

SOURCE=.\plressrc.h
# End Source File
# Begin Source File

SOURCE=.\plStreamSink.h
# End Source File
# Begin Source File

SOURCE=.\plwemfdec.h
# End Source File
# Begin Source File

SOURCE=.\plwinbmp.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Common\config.h
# End Source File
# Begin Source File

SOURCE=..\..\common\jmemdest.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\jmemsrc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\planybmp.h
# End Source File
# Begin Source File

SOURCE=..\..\common\planydec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plbitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpenc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plbmpinfo.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pldatasink.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pldatasrc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pldebug.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plexcept.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plfilesink.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plfilesrc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plgifdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pljpegdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pljpegenc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plmemsink.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plmemsrc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\PLObject.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpaintlibdefs.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpcx.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpcxdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpgm.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpgmdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpicdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpicenc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpictdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpixel24.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpixel32.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpixel8.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpixeldefs.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpngdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpngenc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpoint.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plppm.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plppmdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plprognot.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plpsddec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plrect.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plstdpch.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pltga.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pltgadec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pltiffdec.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pltiffenc.h
# End Source File
# Begin Source File

SOURCE=..\..\common\pltiffencex.h
# End Source File
# Begin Source File

SOURCE=..\..\common\plwindefs.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\tif_msrc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\common\config.vc

!IF  "$(CFG)" == "paintlib - Win32 Release"

# Begin Custom Build
InputPath=..\..\common\config.vc

"..\..\common\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\config.vc ..\..\common\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# Begin Custom Build - Copying config.vc
InputPath=..\..\common\config.vc

"..\..\common\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\config.vc ..\..\common\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "paintlib - Win32 MFC Debug"

# Begin Custom Build - Copying config.vc
InputPath=..\..\common\config.vc

"..\..\common\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\config.vc ..\..\common\config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "paintlib - Win32 MFC Release"

# Begin Custom Build
InputPath=..\..\common\config.vc

"..\..\common\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\config.vc ..\..\common\config.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\common\plpixeldefs.vc

!IF  "$(CFG)" == "paintlib - Win32 Release"

# Begin Custom Build
InputPath=..\..\common\plpixeldefs.vc

"..\..\common\plpixeldefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\plpixeldefs.vc ..\..\common\plpixeldefs.h

# End Custom Build

!ELSEIF  "$(CFG)" == "paintlib - Win32 Debug"

# Begin Custom Build - Copying plpixeldefs.h
InputPath=..\..\common\plpixeldefs.vc

"..\..\common\plpixeldefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\plpixeldefs.vc ..\..\common\plpixeldefs.h

# End Custom Build

!ELSEIF  "$(CFG)" == "paintlib - Win32 MFC Debug"

# Begin Custom Build - Copying plpixeldefs.h
InputPath=..\..\common\plpixeldefs.vc

"..\..\common\plpixeldefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\plpixeldefs.vc ..\..\common\plpixeldefs.h

# End Custom Build

!ELSEIF  "$(CFG)" == "paintlib - Win32 MFC Release"

# Begin Custom Build
InputPath=..\..\common\plpixeldefs.vc

"..\..\common\plpixeldefs.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy ..\..\common\plpixeldefs.vc ..\..\common\plpixeldefs.h

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
