# Microsoft Visual C++ generated build script - Do not modify

PROJ = CLASSTH
DEBUG = 1
PROGTYPE = 4
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\MSVC\DANI\LIB\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP = CLASSTH.CPP 
RC = rc
CFLAGS_D_LIB = /nologo /Gs /G2 /W3 /Z7 /ALu /Gx- /Od /D "_DEBUG" 
CFLAGS_R_LIB = /nologo /Gs /G2 /W3 /ALu /Gx- /Od /D "NDEBUG" 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_LIB)
LFLAGS = 
LIBS = 
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_LIB)
LFLAGS = 
LIBS = 
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = CLASSTH.SBR


CLASSTH_DEP = c:\msvc\dani\include\classth.h \
	c:\msvc\dani\include\memnew.h  \
	c:\msvc\dani\include\comv.h \
	c:\msvc\dani\include\svga.h \
	c:\msvc\dani\include\memnew.h


all:	$(PROJ).LIB

CLASSTH.OBJ:	..\SOURCE\CLASSTH.CPP $(CLASSTH_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c ..\SOURCE\CLASSTH.CPP

$(PROJ).LIB::	CLASSTH.OBJ $(OBJS_EXT)
	echo >NUL @<<$(PROJ).CRF
$@ /PAGESIZE:64
y
+CLASSTH.OBJ &
;
<<
	if exist $@ del $@
	lib @$(PROJ).CRF

$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
