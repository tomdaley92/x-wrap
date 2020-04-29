# Author: Thomas Daley
# Date: September 16, 2016
# Purpose: Basic Build Template for C/C++ Windows Programs.
#
# Note: Microsoft Visual C/C++ Build Tools are assumed to be installed and 
# added to PATH.
#
# 1) vcvarsall x86
# 2) nmake

APPNAME = Xwrap
ARCH = x86

CONSOLE_APP = /SUBSYSTEM:CONSOLE
GUI_APP = /SUBSYSTEM:WINDOWS

CC = CL
CFLAGS = /MD /nologo /W3
LFLAGS = /link \
		 /LIBPATH:$(MAKEDIR)/frameworks/sdl/lib/$(ARCH) \
		 /ENTRY:mainCRTStartup

INCS = /I$(MAKEDIR)/frameworks/sdl/include/ \
	   /I$(MAKEDIR)/frameworks/imgui/
LIBS = SDL2.lib \
	   SDL2main.lib \
	   opengl32.lib \
       Shell32.lib \
       Advapi32.lib \
       Ole32.lib

# Default target
all: prepare debug release clean

prepare:
	@echo off
	IF  not exist debug ( MKDIR debug )
	IF  not exist release ( MKDIR release )

debug: objects resource
	@echo off
	$(CC) $(CFLAGS) /Zi /Fe:debug\$(APPNAME) $(INCS) *.obj *.res $(LIBS) $(LFLAGS) $(CONSOLE_APP)
	COPY frameworks\sdl\lib\$(ARCH)\SDL2.dll debug\SDL2.dll

release: objects resource
	@echo off
	$(CC) $(CFLAGS) /Fe:release\$(APPNAME) $(INCS) *.obj *.res $(LIBS) $(LFLAGS) $(GUI_APP)
	COPY frameworks\sdl\lib\$(ARCH)\SDL2.dll release\SDL2.dll

run_debug: 
	debug\$(APPNAME).exe

run_release: 
	release\$(APPNAME).exe

# Build object files
objects: src/*.c* frameworks/imgui/imgui*.cpp
	@echo off
	$(CC) $(CFLAGS) /c $(INCS) $(CFLAGS) $?	

# Build resource file
resource: src/$(APPNAME).rc resources/$(APPNAME).ico
	RC src\$(APPNAME).rc
	MOVE src\$(APPNAME).res $(APPNAME).res

# Remove leftover build artifacts
clean:
	@echo off
	IF exist *.obj (DEL *.obj)
	IF exist *.res (DEL *.res)

reset:
	@echo off
	IF  exist debug ( RD /S /Q debug )
	IF  exist release ( RD /S /Q release )
