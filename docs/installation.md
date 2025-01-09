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

Add the following lines to your "CMakeLists.txt":
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

Where *DIST_LOCATION* is the location of the *"dist"* folder created by the cmake installation.

If you are working with only one OS (for example WIndows), just add:
```cmake
list (APPEND CMAKE_PREFIX_PATH "${DIST_LOCATION}/../dist/Windows/cmake")
```

Then, you will able to find the library with cmake:
```cmake
find_package(grafology CONFIG REQUIRED)
```
