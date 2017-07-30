# Author: Thomas Daley
# Date: September 16, 2016
# Purpose: Basic Build Template for C/C++ Windows Programs.
#
# Note: Microsoft Visual C/C++ Build Tools are assumed to be installed and 
# added to PATH.
#
# 1) vcvarsall
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
all: clean build_debug build_release
	DEL *.exe *.obj *.res *.pdb

build_debug: objects resource
	$(CC) $(CFLAGS) /Zi /Fe$(APPNAME) $(INCS) *.obj *.res $(LIBS) $(LFLAGS) $(CONSOLE_APP)
	MOVE $(APPNAME).exe debug/$(APPNAME).exe
	COPY frameworks\sdl\lib\$(ARCH)\SDL2.dll debug\SDL2.dll

build_release: objects resource
	$(CC) $(CFLAGS) /Fe$(APPNAME) $(INCS) *.obj *.res $(LIBS) $(LFLAGS) $(GUI_APP)
	MOVE $(APPNAME).exe release/$(APPNAME).exe
	COPY frameworks\sdl\lib\$(ARCH)\SDL2.dll release\SDL2.dll

test_debug: 
	debug\$(APPNAME).exe \

test_release: 
	release\$(APPNAME).exe \

# Build object files
objects: src/*.c* frameworks\imgui\imgui*.cpp
	$(CC) $(CFLAGS) /c $(INCS) $(CFLAGS) $?	

# Build resource file
resource: src\$(APPNAME).rc resources\$(APPNAME).ico
	RC src\$(APPNAME).rc
	MOVE src\$(APPNAME).res $(APPNAME).res

# Deletes any leftover executables or object files
clean:
	DEL $(APPNAME).exe *.obj *.res *.pdb *.ini
	cd debug & DEL $(APPNAME).exe *.dll *.ini
	cd release & DEL $(APPNAME).exe *.dll *.ini
