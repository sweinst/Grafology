<div align="center">
    <img 
        src="imgs/logo.png" 
        alt="Grafology logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Requirements
The only requirement is a **C++23** compiler.

The code has been tested with the following configurations:

| OS           | Compiler           | Remarks                        |
| ---          | ---                | ---                            |
| Windows 11   | Visual Studio 2022 |                                |
| Ubuntu 24.04 | g++-14             |                                |
| Ubuntu 24.04 | clang++-19         | installed from LLVM repository |
| MacOS 15     | g++-14             | installed with homebrew        |


# Installation

## VCPKG

An installation with *vcpkg* will be provided soon.

**This will be the recommended way for installing the library.**

## Manual installation

### Without cmake support

At the moment, the library is a headers only library. So adding "src/include" to your compiling options should be enough.

## With cmake support

If you want a *"manual"* cmake compatible installation, you need to build the library and install it.

The installation will create a "dist" folder which can be used with cmake. Cmake will need to know where to look for the configuration files.

If you are working with only one OS (for example WIndows), just add:
```cmake
list (APPEND CMAKE_PREFIX_PATH "${DIST_LOCATION}/../dist/Windows/cmake")
```

Where *DIST_LOCATION* is the location of the *"dist"* folder created by the cmake installation.

If you are working with multiple OSes, add the following lines to your "CMakeLists.txt":
```cmake
if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    set(DIST_OS "windows")
elseif ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
    set(DIST_OS "linux")
elseif ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Darwin")
    set(DIST_OS "osx")
endif()
list (APPEND CMAKE_PREFIX_PATH "${DIST_LOCATION}/../dist/${DIST_OS}/cmake")
```

Then, you will able to find the library with cmake:
```cmake
find_package(grafology CONFIG REQUIRED)
```

# Build

This is only required if you want to modify the library or generate a distribution

## Requirements

### Git & cmake

These tools come nowadays with any C++ development framework.

They can be found installed with:
- Windows: Visual Studio 2022, CLion, chocolatey, scoop, ...
- Ubuntu/Linux: Linux repositories
- Mac: XCode, Homebrew, ...


### Ninja

Ninja is the build system used on all platforms. It can be downloaded from the [github releases](https://github.com/ninja-build/ninja/releases).

Alternatively, it can be installed from different sources:
- Windows: Visual Studio 2022, Chocolatey, scoop, ....
- Ubuntu: Ubuntu repositories (ninja-build)
- Mac: Homebrew

### PowerShell Core

All the scripts use *Posh*. PowerSHell Core (alias PowerShell 7) is now available on all major platforms. It can be downloaded from the [github releases](https://github.com/PowerShell/PowerShell/releases).

Alternatively, it can be installed from different sources:
- Windows: Microsoft Store, Chocolatey, scoop, ....
- Ubuntu/Linux: Ubuntu snap, Microsoft repositories
- Mac: Homebrew


### VCPKG

I use [vcpkg](https://vcpkg.io/en/) as the C++ package manager. The only external library is [Catch2](https://github.com/catchorg/Catch2) used for the unit tests.

Follow the [instructions](https://learn.microsoft.com/en-gb/vcpkg/get_started/get-started?pivots=shell-powershell#1---set-up-vcpkg) on their website for the installation.

## Build

You need first to launch a terminal running PoweShell:
```powershell
> pwsh
```

**On Windows**, you need first to setup the path to the Visual Studio 2022 compiler and other tools. 
```powershell
> ./scripts/vs-setup.ps1  -Version 2022 -Edition Community
```
*Edition* can be "Community" or "Enterprise" depending on your Visual Studio installation

Run with PowerShell the build.ps1 script:
```powershell
> ./scripts/build.ps1 All
```

This will configure, build and install the library

You can get the full script help with
```powershell
> help ./scripts/build.ps1
```
