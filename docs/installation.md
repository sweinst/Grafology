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

# VCPKG

An installation with *vcpkg* will be provided soon.

This will be the recommended way for installing the library/

## Manual installation

### Without cmake support

At the moment, the library is a headers only library. So adding "src/include" to your compiling options should be enough.

## With cmake support

If you want a *"manual"* cmake compatible installation, you need to build the library and install it.

The installation will create a "dist" folder which can be used with cmake. Cmake will need to know where to look for the configuration files.

