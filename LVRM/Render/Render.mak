# Microsoft Developer Studio Generated NMAKE File, Based on Render.dsp
!IF "$(CFG)" == ""
CFG=Render - Win32 Debug
!MESSAGE �\�����w�肳��Ă��܂���B��̫�Ă� Render - Win32 Debug ��ݒ肵�܂��B
!ENDIF 

!IF "$(CFG)" != "Render - Win32 Release" && "$(CFG)" != "Render - Win32 Debug"
!MESSAGE �w�肳�ꂽ ����� Ӱ�� "$(CFG)" �͐���������܂���B
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "Render.mak" CFG="Render - Win32 Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "Render - Win32 Release" ("Win32 (x86) Static Library" �p)
!MESSAGE "Render - Win32 Debug" ("Win32 (x86) Static Library" �p)
!MESSAGE 
!ERROR �����ȍ\�����w�肳��Ă��܂��B
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Render - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Render.lib"


CLEAN :
	-@erase "$(INTDIR)\RDGlauxView.obj"
	-@erase "$(INTDIR)\RDGlutView.obj"
	-@erase "$(INTDIR)\RDView.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Render.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\Render.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Render.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Render.lib" 
LIB32_OBJS= \
	"$(INTDIR)\RDGlauxView.obj" \
	"$(INTDIR)\RDGlutView.obj" \
	"$(INTDIR)\RDView.obj"

"$(OUTDIR)\Render.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Render - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Render.lib"


CLEAN :
	-@erase "$(INTDIR)\RDGlauxView.obj"
	-@erase "$(INTDIR)\RDGlutView.obj"
	-@erase "$(INTDIR)\RDView.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Render.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\Render.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Render.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Render.lib" 
LIB32_OBJS= \
	"$(INTDIR)\RDGlauxView.obj" \
	"$(INTDIR)\RDGlutView.obj" \
	"$(INTDIR)\RDView.obj"

"$(OUTDIR)\Render.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Render.dep")
!INCLUDE "Render.dep"
!ELSE 
!MESSAGE Warning: cannot find "Render.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Render - Win32 Release" || "$(CFG)" == "Render - Win32 Debug"
SOURCE=.\glaux\RDGlauxView.cpp

"$(INTDIR)\RDGlauxView.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\glut\RDGlutView.cpp

"$(INTDIR)\RDGlutView.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\RDView.cpp

"$(INTDIR)\RDView.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

