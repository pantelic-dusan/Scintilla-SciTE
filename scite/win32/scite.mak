# Make file for SciTE on Windows Visual C++ version
# Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile is for using Visual C++ with nmake.
# Usage for Microsoft:
#     nmake -f scite.mak
# For debug versions define DEBUG on the command line.
# For a build without Lua, define NO_LUA on the command line.
# The main makefile uses mingw32 gcc and may be more current than this file.

.SUFFIXES: .cxx .properties .dll

DIR_BIN=..\bin
DIR_SCINTILLA=..\..\scintilla
DIR_SCINTILLA_BIN=$(DIR_SCINTILLA)\bin

PROG=$(DIR_BIN)\SciTE.exe
PROGSTATIC=$(DIR_BIN)\Sc1.exe
DLLS=$(DIR_BIN)\Scintilla.dll $(DIR_BIN)\SciLexer.dll

WIDEFLAGS=-DUNICODE -D_UNICODE

LD=link

!IFDEF SUPPORT_XP
XP_DEFINE=-D_USING_V110_SDK71_
XP_LINK=-SUBSYSTEM:WINDOWS,5.01
!ENDIF

CXXFLAGS=-Zi -TP -MP -W4 -EHsc -Zc:forScope -Zc:wchar_t -std:c++latest -D_CRT_SECURE_NO_DEPRECATE=1 -D_CRT_NONSTDC_NO_DEPRECATE $(WIDEFLAGS) $(XP_DEFINE)
CCFLAGS=-TC -MP -W3 -wd4244 -D_CRT_SECURE_NO_DEPRECATE=1 -DLUA_USER_H=\"scite_lua_win.h\" $(XP_DEFINE)

CXXDEBUG=-Od -MTd -DDEBUG
# Don't use "-MD", even with "-D_STATIC_CPPLIB" because it links to MSVCR71.DLL
CXXNDEBUG=-O1 -Oi -MT -DNDEBUG -GL
NAME=-Fo
LDFLAGS=-OPT:REF -LTCG -DEBUG $(XP_LINK)
LDDEBUG=
LIBS=KERNEL32.lib USER32.lib GDI32.lib MSIMG32.lib COMDLG32.lib COMCTL32.lib ADVAPI32.lib IMM32.lib SHELL32.LIB OLE32.LIB OLEAUT32.LIB UXTHEME.LIB
NOLOGO=-nologo

!IFDEF QUIET
CXX=@$(CXX)
CXXFLAGS=$(CXXFLAGS) $(NOLOGO)
CCFLAGS=$(CCFLAGS) $(NOLOGO)
LDFLAGS=$(LDFLAGS) $(NOLOGO)
!ENDIF

!IFDEF DEBUG
CXXFLAGS=$(CXXFLAGS) $(CXXDEBUG)
CCFLAGS=$(CCFLAGS) $(CXXDEBUG)
LDFLAGS=$(LDDEBUG) $(LDFLAGS)
!ELSE
CXXFLAGS=$(CXXFLAGS) $(CXXNDEBUG)
CCFLAGS=$(CCFLAGS) $(CXXNDEBUG)
!ENDIF

INCLUDEDIRS=-I../../scintilla/include -I../src

SHAREDOBJS=\
	Cookie.obj \
	Credits.obj \
	DirectorExtension.obj \
	EditorConfig.obj \
	ExportHTML.obj \
	ExportPDF.obj \
	ExportRTF.obj \
	ExportTEX.obj \
	ExportXML.obj \
	FilePath.obj \
	FileWorker.obj \
	GUIWin.obj \
	IFaceTable.obj \
	JobQueue.obj \
	MatchMarker.obj \
	MultiplexExtension.obj \
	PropSetFile.obj \
	ScintillaWindow.obj \
	SciTEBase.obj \
	SciTEBuffers.obj \
	SciTEIO.obj \
	SciTEProps.obj \
	SciTEWinBar.obj \
	SciTEWinDlg.obj \
	StringHelpers.obj \
	StringList.obj \
	Strips.obj \
	StyleDefinition.obj \
	StyleWriter.obj \
	UniqueInstance.obj \
	Utf8_16.obj \
	WinMutex.obj

OBJS=\
	$(SHAREDOBJS) \
	SciTEWin.obj

LIBSCI=$(DIR_SCINTILLA_BIN)\libscintilla.lib

OBJSSTATIC=$(SHAREDOBJS) Sc1.obj

#++Autogenerated -- run ../scripts/RegenerateSource.py to regenerate
#**LEXPROPS=\\\n\($(DIR_BIN)\\\* \)
LEXPROPS=\
$(DIR_BIN)\abaqus.properties $(DIR_BIN)\ada.properties \
$(DIR_BIN)\asl.properties $(DIR_BIN)\asm.properties $(DIR_BIN)\asn1.properties \
$(DIR_BIN)\au3.properties $(DIR_BIN)\ave.properties $(DIR_BIN)\avs.properties \
$(DIR_BIN)\baan.properties $(DIR_BIN)\blitzbasic.properties \
$(DIR_BIN)\bullant.properties $(DIR_BIN)\caml.properties \
$(DIR_BIN)\cmake.properties $(DIR_BIN)\cobol.properties \
$(DIR_BIN)\coffeescript.properties $(DIR_BIN)\conf.properties \
$(DIR_BIN)\cpp.properties $(DIR_BIN)\csound.properties \
$(DIR_BIN)\css.properties $(DIR_BIN)\d.properties $(DIR_BIN)\ecl.properties \
$(DIR_BIN)\eiffel.properties $(DIR_BIN)\erlang.properties \
$(DIR_BIN)\escript.properties $(DIR_BIN)\flagship.properties \
$(DIR_BIN)\forth.properties $(DIR_BIN)\fortran.properties \
$(DIR_BIN)\freebasic.properties $(DIR_BIN)\gap.properties \
$(DIR_BIN)\haskell.properties $(DIR_BIN)\hex.properties \
$(DIR_BIN)\html.properties $(DIR_BIN)\inno.properties \
$(DIR_BIN)\json.properties $(DIR_BIN)\kix.properties \
$(DIR_BIN)\latex.properties $(DIR_BIN)\lisp.properties \
$(DIR_BIN)\lot.properties $(DIR_BIN)\lout.properties $(DIR_BIN)\lua.properties \
$(DIR_BIN)\markdown.properties $(DIR_BIN)\matlab.properties \
$(DIR_BIN)\maxima.properties $(DIR_BIN)\mbox.properties \
$(DIR_BIN)\metapost.properties $(DIR_BIN)\mmixal.properties \
$(DIR_BIN)\modula3.properties $(DIR_BIN)\nimrod.properties \
$(DIR_BIN)\nncrontab.properties $(DIR_BIN)\nsis.properties \
$(DIR_BIN)\opal.properties $(DIR_BIN)\oscript.properties \
$(DIR_BIN)\others.properties $(DIR_BIN)\pascal.properties \
$(DIR_BIN)\perl.properties $(DIR_BIN)\pov.properties \
$(DIR_BIN)\powerpro.properties $(DIR_BIN)\powershell.properties \
$(DIR_BIN)\ps.properties $(DIR_BIN)\purebasic.properties \
$(DIR_BIN)\python.properties $(DIR_BIN)\r.properties \
$(DIR_BIN)\rebol.properties $(DIR_BIN)\registry.properties \
$(DIR_BIN)\ruby.properties $(DIR_BIN)\rust.properties \
$(DIR_BIN)\scriptol.properties $(DIR_BIN)\smalltalk.properties \
$(DIR_BIN)\sorcins.properties $(DIR_BIN)\specman.properties \
$(DIR_BIN)\spice.properties $(DIR_BIN)\sql.properties \
$(DIR_BIN)\tacl.properties $(DIR_BIN)\tal.properties $(DIR_BIN)\tcl.properties \
$(DIR_BIN)\tex.properties $(DIR_BIN)\txt2tags.properties \
$(DIR_BIN)\vb.properties $(DIR_BIN)\verilog.properties \
$(DIR_BIN)\vhdl.properties $(DIR_BIN)\visualprolog.properties \
$(DIR_BIN)\yaml.properties
#--Autogenerated -- end of automatically generated section

PROPS=$(DIR_BIN)\SciTEGlobal.properties $(DIR_BIN)\abbrev.properties $(LEXPROPS)

!IFNDEF NO_LUA
LUA_CORE_OBJS = lapi.obj lcode.obj lctype.obj ldebug.obj ldo.obj ldump.obj lfunc.obj lgc.obj llex.obj \
                lmem.obj lobject.obj lopcodes.obj lparser.obj lstate.obj lstring.obj \
                ltable.obj ltm.obj lundump.obj lvm.obj lzio.obj

LUA_LIB_OBJS =	lauxlib.obj lbaselib.obj lbitlib.obj lcorolib.obj ldblib.obj liolib.obj lmathlib.obj ltablib.obj \
                lstrlib.obj loadlib.obj loslib.obj linit.obj lutf8lib.obj

LUA_OBJS = LuaExtension.obj $(LUA_CORE_OBJS) $(LUA_LIB_OBJS)

OBJS = $(OBJS) $(LUA_OBJS)
OBJSSTATIC = $(OBJSSTATIC) $(LUA_OBJS)
INCLUDEDIRS = $(INCLUDEDIRS) -I../lua/src
!ELSE
CXXFLAGS=$(CXXFLAGS) -DNO_LUA
!ENDIF

CXXFLAGS=$(CXXFLAGS) $(INCLUDEDIRS)
CCFLAGS=$(CCFLAGS) $(INCLUDEDIRS)


ALL: $(PROG) $(PROGSTATIC) $(DLLS) $(PROPS)

clean:
	del /q $(DIR_BIN)\*.exe *.o *.obj $(DIR_BIN)\*.dll *.res *.map $(DIR_BIN)\*.exp $(DIR_BIN)\*.lib $(DIR_BIN)\*.pdb

{$(DIR_SCINTILLA_BIN)}.dll{$(DIR_BIN)}.dll:
	copy $< $@

{..\src}.properties{$(DIR_BIN)}.properties:
	copy $< $@

# A custom rule for .obj files built by scintilla:
..\..\scintilla\win32\PlatWin.obj: 	..\..\scintilla\win32\PlatWin.cxx
	@echo You must run the Scintilla makefile to build $*.obj
	@exit 255

SciTERes.res: SciTERes.rc ..\src\SciTE.h SciTE.exe.manifest
	$(RC) $(INCLUDEDIRS) -fo$@ SciTERes.rc

Sc1Res.res: SciTERes.rc ..\src\SciTE.h SciTE.exe.manifest
	$(RC) $(INCLUDEDIRS) -dSTATIC_BUILD -fo$@ SciTERes.rc

$(PROG): $(OBJS) SciTERes.res
	$(LD) $(LDFLAGS) -OUT:$@ $** $(LIBS)

$(PROGSTATIC): $(OBJSSTATIC) $(LIBSCI) Sc1Res.res
	$(LD) $(LDFLAGS) -OUT:$@ $** $(LIBS)

# Define how to build all the objects and what they depend on
# Some source files are compiled into more than one object because of different conditional compilation

{..\src}.cxx.obj::
	$(CXX) $(CXXFLAGS) -c $<
{.}.cxx.obj::
	$(CXX) $(CXXFLAGS) -c $<

{..\lua\src}.c.obj::
	$(CXX) $(CCFLAGS) -c $<

Sc1.obj: SciTEWin.cxx
	$(CXX) $(CXXFLAGS) -DSTATIC_BUILD -c $(NAME)$@ SciTEWin.cxx

# Dependencies
DirectorExtension.obj: \
	DirectorExtension.cxx \
	../../scintilla/include/ILexer.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h \
	DirectorExtension.h
GUIWin.obj: \
	GUIWin.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h
SciTEWin.obj: \
	SciTEWin.cxx \
	SciTEWin.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/ILoader.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h \
	../src/SciTEKeys.h \
	UniqueInstance.h \
	../src/StripDefinition.h \
	Strips.h \
	../src/MultiplexExtension.h \
	../src/Extender.h \
	DirectorExtension.h \
	../src/LuaExtension.h
Sc1.obj: \
	SciTEWin.cxx \
	SciTEWin.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/ILoader.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h \
	../src/SciTEKeys.h \
	UniqueInstance.h \
	../src/StripDefinition.h \
	Strips.h \
	../src/MultiplexExtension.h \
	../src/Extender.h \
	DirectorExtension.h \
	../src/LuaExtension.h
SciTEWinBar.obj: \
	SciTEWinBar.cxx \
	SciTEWin.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/ILoader.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h \
	../src/SciTEKeys.h \
	UniqueInstance.h \
	../src/StripDefinition.h \
	Strips.h
SciTEWinDlg.obj: \
	SciTEWinDlg.cxx \
	SciTEWin.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/ILoader.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h \
	../src/SciTEKeys.h \
	UniqueInstance.h \
	../src/StripDefinition.h \
	Strips.h
Strips.obj: \
	Strips.cxx \
	SciTEWin.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/ILoader.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h \
	../src/SciTEKeys.h \
	UniqueInstance.h \
	../src/StripDefinition.h \
	Strips.h
UniqueInstance.obj: \
	UniqueInstance.cxx \
	SciTEWin.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/ILoader.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h \
	../src/SciTEKeys.h \
	UniqueInstance.h \
	../src/StripDefinition.h \
	Strips.h
WinMutex.obj: \
	WinMutex.cxx \
	../src/Mutex.h
Cookie.obj: \
	../src/Cookie.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/StringHelpers.h \
	../src/Cookie.h
Credits.obj: \
	../src/Credits.cxx \
	../../scintilla/include/ILexer.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h
EditorConfig.obj: \
	../src/EditorConfig.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/EditorConfig.h
ExportHTML.obj: \
	../src/ExportHTML.cxx \
	../../scintilla/include/ILexer.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h
ExportPDF.obj: \
	../src/ExportPDF.cxx \
	../../scintilla/include/ILexer.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h
ExportRTF.obj: \
	../src/ExportRTF.cxx \
	../../scintilla/include/ILexer.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h
ExportTEX.obj: \
	../src/ExportTEX.cxx \
	../../scintilla/include/ILexer.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h
ExportXML.obj: \
	../src/ExportXML.cxx \
	../../scintilla/include/ILexer.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h
FilePath.obj: \
	../src/FilePath.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/FilePath.h
FileWorker.obj: \
	../src/FileWorker.cxx \
	../../scintilla/include/ILoader.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/FilePath.h \
	../src/Mutex.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/Utf8_16.h
JobQueue.obj: \
	../src/JobQueue.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/PropSetFile.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h
MatchMarker.obj: \
	../src/MatchMarker.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/MatchMarker.h
MultiplexExtension.obj: \
	../src/MultiplexExtension.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/MultiplexExtension.h \
	../src/Extender.h
PropSetFile.obj: \
	../src/PropSetFile.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/PropSetFile.h
ScintillaWindow.obj: \
	../src/ScintillaWindow.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/ScintillaWindow.h
SciTEBase.obj: \
	../src/SciTEBase.cxx \
	../../scintilla/include/ILoader.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/SciLexer.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/MatchMarker.h \
	../src/EditorConfig.h \
	../src/SciTEBase.h
SciTEBuffers.obj: \
	../src/SciTEBuffers.cxx \
	../../scintilla/include/ILoader.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/SciLexer.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h
SciTEIO.obj: \
	../src/SciTEIO.cxx \
	../../scintilla/include/ILoader.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/FileWorker.h \
	../src/MatchMarker.h \
	../src/SciTEBase.h \
	../src/Utf8_16.h
SciTEProps.obj: \
	../src/SciTEProps.cxx \
	../../scintilla/include/ILexer.h \
	../../scintilla/include/Sci_Position.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/SciLexer.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringList.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleDefinition.h \
	../src/PropSetFile.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/SciTE.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/Cookie.h \
	../src/Worker.h \
	../src/MatchMarker.h \
	../src/EditorConfig.h \
	../src/SciTEBase.h \
	../src/IFaceTable.h
StringHelpers.obj: \
	../src/StringHelpers.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/StringHelpers.h
StringList.obj: \
	../src/StringList.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/StringList.h \
	../src/StringHelpers.h
StyleDefinition.obj: \
	../src/StyleDefinition.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/StringHelpers.h \
	../src/StyleDefinition.h
StyleWriter.obj: \
	../src/StyleWriter.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StyleWriter.h
Utf8_16.obj: \
	../src/Utf8_16.cxx \
	../src/Utf8_16.h

!IFNDEF NO_LUA
LuaExtension.obj: \
	../src/LuaExtension.cxx \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Sci_Position.h \
	../src/GUI.h \
	../src/ScintillaWindow.h \
	../src/StringHelpers.h \
	../src/FilePath.h \
	../src/StyleWriter.h \
	../src/Extender.h \
	../src/IFaceTable.h \
	../src/SciTEKeys.h \
	../src/LuaExtension.h \
	../lua/src/lua.h \
	../lua/src/luaconf.h \
	../src/scite_lua_win.h \
	../lua/src/lualib.h \
	../lua/src/lua.h \
	../lua/src/lauxlib.h
IFaceTable.obj: \
	../src/IFaceTable.cxx \
	../src/IFaceTable.h

# Lua core dependencies are omitted; if the Lua source code
# is modified, a make clean may be necessary.
!ENDIF
