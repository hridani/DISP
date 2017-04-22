# Microsoft Visual C++ generated build script - Do not modify

PROJ = TESTIN
DEBUG = 1
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\MSVC\DANI\TESTIN\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP = TEST.CPP    
RC = rc
CFLAGS_D_DEXE = /nologo /Gs /G2 /W3 /Zi /ALu /Gx- /Od /D "_DEBUG" /D "_DOS" /Fr /Fd"TESTIN.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G2 /W3 /AM /Ox /D "NDEBUG" /D "_DOS" /FR 
LFLAGS_D_DEXE = /NOLOGO /NOE /NOI /FARCALL /STACK:10000 /ONERROR:NOEXE /CO 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:7000 /ONERROR:NOEXE 
LIBS_D_DEXE = oldnames llibce 
LIBS_R_DEXE = oldnames mlibce 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = COMST.SBR \
		IRQ9NEWT.SBR \
		STRING.SBR \
		TEST.SBR \
		DISPNEWF.SBR \
		SCREENFN.SBR


COMST_DEP = c:\msvc\dani\include\comv.h \
	c:\msvc\dani\include\classth.h \
	c:\msvc\dani\include\memnew.h  \
	c:\msvc\dani\include\vfield.h \
	c:\msvc\dani\include\key.h \
	c:\msvc\dani\include\stfile.h \
	c:\msvc\dani\include\pia.h \
	c:\msvc\dani\include\table.h


IRQ9NEWT_DEP = c:\msvc\dani\testin\dispnew.h \
	c:\msvc\dani\include\classth.h \
	c:\msvc\dani\include\memnew.h  \
	c:\msvc\dani\include\comv.h \
	c:\msvc\dani\testin\irq9new.h \
	c:\msvc\dani\testin\comst.h \
	c:\msvc\dani\include\vfield.h \
	c:\msvc\dani\include\key.h \
	c:\msvc\dani\include\pia.h \
	c:\msvc\dani\include\table.h \
	c:\msvc\dani\include\usepia.h \
	c:\msvc\dani\include\svga.h \
	c:\msvc\dani\include\memnew.h


STRING_DEP = c:\msvc\dani\testin\comst.h \
	c:\msvc\dani\include\comv.h \
	c:\msvc\dani\include\classth.h \
	c:\msvc\dani\include\memnew.h  \
	c:\msvc\dani\include\vfield.h \
	c:\msvc\dani\include\key.h \
	c:\msvc\dani\include\pia.h \
	c:\msvc\dani\include\table.h


TEST_DEP = c:\msvc\dani\testin\comst.h \
	c:\msvc\dani\include\comv.h \
	c:\msvc\dani\include\classth.h \
	c:\msvc\dani\include\memnew.h  \
	c:\msvc\dani\include\vfield.h \
	c:\msvc\dani\include\key.h \
	c:\msvc\dani\include\pia.h \
	c:\msvc\dani\include\table.h \
	c:\msvc\dani\testin\dispnew.h \
	c:\msvc\dani\testin\irq9new.h \
	c:\msvc\dani\testin\screen.h \
	c:\msvc\dani\include\stfile.h \
	c:\msvc\dani\include\usepia.h \
	c:\msvc\dani\include\videoc.h \
	c:\msvc\dani\include\svga.h \
	c:\msvc\dani\include\memnew.h


DISPNEWF_DEP = c:\msvc\dani\testin\dispnew.h \
	c:\msvc\dani\include\classth.h \
	c:\msvc\dani\include\memnew.h  \
	c:\msvc\dani\include\comv.h \
	c:\msvc\dani\testin\irq9new.h \
	c:\msvc\dani\include\videoc.h \
	c:\msvc\dani\include\svga.h \
	c:\msvc\dani\include\memnew.h \
	c:\msvc\dani\testin\comst.h \
	c:\msvc\dani\include\vfield.h \
	c:\msvc\dani\include\key.h \
	c:\msvc\dani\include\pia.h \
	c:\msvc\dani\include\table.h


SCREENFN_DEP = c:\msvc\dani\testin\comst.h \
	c:\msvc\dani\include\comv.h \
	c:\msvc\dani\include\classth.h \
	c:\msvc\dani\include\memnew.h  \
	c:\msvc\dani\include\vfield.h \
	c:\msvc\dani\include\key.h \
	c:\msvc\dani\include\pia.h \
	c:\msvc\dani\include\table.h \
	c:\msvc\dani\testin\screen.h \
	c:\msvc\dani\include\stfile.h \
	c:\msvc\dani\include\svga.h \
	c:\msvc\dani\include\memnew.h \
	c:\msvc\dani\include\videoc.h \
	c:\msvc\dani\testin\irq9new.h \
	c:\msvc\dani\testin\dispnew.h


all:	$(PROJ).EXE $(PROJ).BSC

COMST.OBJ:	COMST.CPP $(COMST_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c COMST.CPP

IRQ9NEWT.OBJ:	IRQ9NEWT.CPP $(IRQ9NEWT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c IRQ9NEWT.CPP

STRING.OBJ:	STRING.CPP $(STRING_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c STRING.CPP

TEST.OBJ:	TEST.CPP $(TEST_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c TEST.CPP

DISPNEWF.OBJ:	DISPNEWF.CPP $(DISPNEWF_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DISPNEWF.CPP

SCREENFN.OBJ:	SCREENFN.CPP $(SCREENFN_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c SCREENFN.CPP

$(PROJ).EXE::	COMST.OBJ IRQ9NEWT.OBJ STRING.OBJ TEST.OBJ DISPNEWF.OBJ SCREENFN.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
COMST.OBJ +
IRQ9NEWT.OBJ +
STRING.OBJ +
TEST.OBJ +
DISPNEWF.OBJ +
SCREENFN.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
