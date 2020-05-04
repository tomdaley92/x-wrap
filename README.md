# Xwrap
**v0.0.1**

A simple and lightweight tool to map
Xinput to keyboard & mouse strokes.

## Requirements
- Windows 10
- [XInput 1.4](https://docs.microsoft.com/en-us/windows/win32/xinput/xinput-versions)

## Building Natively on Windows
[Microsoft's C++ Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/) are assumed to be installed. Specifically, you will want `vcvarsall`, `cl`, and `nmake` accessible from the command line.

Open `cmd` and set up your environment.

```bat
vcvarsall x86
```

Build from source using the `nmake` utility. 

```bat
nmake develop
```

Run the application.
```bat
nmake run
```

Other useful commands (see [makefile](/makefile)):

```bat
nmake release
nmake clean
```
