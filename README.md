# Xwrap
**v0.0.1**

A simple and lightweight gamepad to 
keyboard+mouse mapping utility for 
Windows. A work in progress.

## Building on Windows
[Microsoft's C++ Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/) are assumed to be installed. Specifically, you will want these tools added to `PATH`:
  - vcvarsall
  - cl
  - nmake


Open `CMD` and set up your environment.

```bat
vcvarsall x86
```

Build from source using the `NMAKE` utility.

```bat
nmake
```

Other useful commands (see [makefile](/makefile)):

```bat
nmake test
nmake clean
nmake reset
```
