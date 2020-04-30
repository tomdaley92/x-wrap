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

# Default
all: prepare debug release clean

# Ensure necessary directories are built
prepare:
	IF  not exist debug ( MKDIR debug )
	IF  not exist release ( MKDIR release )

# Build console based executable for debugging/development
debug: prepare objects resources
	$(CC) $(CFLAGS) /Zi /Fe:debug\$(APPNAME) $(INCS) *.obj *.res $(LIBS) $(LFLAGS) /SUBSYSTEM:CONSOLE
	COPY lib\sdl\lib\$(ARCH)\SDL2.dll debug\SDL2.dll

# Build gui based executable for distribution
release: prepare objects resources
	$(CC) $(CFLAGS) /Fe:release\$(APPNAME) $(INCS) *.obj *.res $(LIBS) $(LFLAGS) /SUBSYSTEM:WINDOWS
	COPY lib\sdl\lib\$(ARCH)\SDL2.dll release\SDL2.dll

# Build object files (.obj)
objects: src/*.c* lib/imgui/imgui*.cpp
	$(CC) $(CFLAGS) /c $(INCS) $(CFLAGS) $?	

# Build resource files (.res)
resources: src/$(APPNAME).rc res/$(APPNAME).ico
	RC src\$(APPNAME).rc
	MOVE src\$(APPNAME).res $(APPNAME).res

# Run console-based application
test:
	debug\$(APPNAME).exe

# Remove leftover build artifacts 
clean:
	IF exist *.obj (DEL *.obj)
	IF exist *.res (DEL *.res)

# Remove all build artifacts and executables
reset: clean
	IF  exist debug ( RD /S /Q debug )
	IF  exist release ( RD /S /Q release )
