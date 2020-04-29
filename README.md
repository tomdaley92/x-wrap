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


1. Open `CMD` and set up your environment.

```cmd
vcvarsall x86
```

2. Build from source using the `NMAKE` utility.

```cmd
nmake
```

3. Other useful build commands (see [makefile](/makefile)):

```cmd
nmake test
```

```cmd
nmake clean
```

```cmd
nmake reset
```
