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

CC = CL
CFLAGS = /MD /nologo /W3
LFLAGS = /link \
	/LIBPATH:$(MAKEDIR)/lib/sdl/lib/$(ARCH) \
	/ENTRY:mainCRTStartup
INCS = /I $(MAKEDIR)/lib/sdl/include/ \
	/I $(MAKEDIR)/lib/imgui/
LIBS = SDL2.lib \
	SDL2main.lib \
	opengl32.lib \
	Shell32.lib \
	Advapi32.lib \
	Ole32.lib

# Build console based application for debugging/development
develop: objects resources
	IF  not exist bin\develop ( MKDIR bin\develop )
	$(CC) $(CFLAGS) /Zi /Fe:bin\develop\$(APPNAME) $(INCS) *.obj *.res $(LIBS) $(LFLAGS) /SUBSYSTEM:CONSOLE
	COPY lib\sdl\lib\$(ARCH)\SDL2.dll bin\develop\SDL2.dll
	IF exist *.obj (DEL *.obj)
	IF exist *.res (DEL *.res)

# Build gui based application for distribution
release: objects resources
	IF  not exist bin\release ( MKDIR bin\release )
	$(CC) $(CFLAGS) /Fe:bin\release\$(APPNAME) $(INCS) *.obj *.res $(LIBS) $(LFLAGS) /SUBSYSTEM:WINDOWS
	COPY lib\sdl\lib\$(ARCH)\SDL2.dll bin\release\SDL2.dll
	IF exist *.obj (DEL *.obj)
	IF exist *.res (DEL *.res)

# Build object files (.obj)
objects: src/*.c* lib/imgui/imgui*.cpp
	$(CC) $(CFLAGS) /c $(INCS) $(CFLAGS) $?	

# Build resource files (.res)
resources: src/$(APPNAME).rc res/$(APPNAME).ico
	RC src\$(APPNAME).rc
	MOVE src\$(APPNAME).res $(APPNAME).res

# Run console-based application
run:
	bin\develop\$(APPNAME).exe

# Remove leftover build artifacts and executables
clean:
	IF exist *.obj (DEL *.obj)
	IF exist *.res (DEL *.res)
	IF  exist bin ( RD /S /Q bin )
