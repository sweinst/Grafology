<#
.SYNOPSIS
  Launch a build/install for one or more build types
.PARAMETER BuildTypes
  Which build types to generate: Debug, Release, RelWithDebInfo or a combination? Use 'All' to build all types.
.PARAMETER CC
  Which C compiler to use? Default is defied in the preset.
.PARAMETER CPP
  Which C++ compiler to use? Default is defied in the preset.
.PARAMETER RootDir
  The directory where the main CMakeFile.txt is located.
.PARAMETER Clean
  Remove build folder first?
#>

param(  
    [Parameter(Mandatory=$true, Position=0)]
    [ValidateNotNullOrEmpty()]
    [ValidateSet('', 'Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel', 'All')]
    [string[]] $BuildTypes,
    [string] $CC = $null,
    [string] $CPP = $null,
    [string] $RootDir = $PWD,
    [switch] $Clean
)

if ($BuildTypes -eq 'All') {
    $BuildTypes = @('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')
}
Write-Host "Build Types: $BuildTypes"

if ($IsWindows) {
  $os = "windows"
  $arch = "x64"
}
elseif ($IsLinux) {
  $os = "linux"
  $arch = "x64"
}
elseif ($IsMacOS) {
  $os = "osx"
  $arch = "arm64"
}
else {
  Write-Host "Unsupported OS. Exiting."
  exit 1
}

if (-not $env:VCPKG_ROOT) {
  if (-not $env:VCPKG_INSTALLATION_ROOT) {
    Write-Host "VCPKG_ROOT or VCPKG_INSTALLATION_ROOT are not set. Exiting."
    exit 1
  }
  $env:VCPKG_ROOT = $env:VCPKG_INSTALLATION_ROOT
}

$common_options = @(
    "-DCMAKE_INSTALL_PREFIX=${RootDir}/dist/${os}",
    "-DVCPKG_APPLOCAL_DEPS=ON",
    "-DX_VCPKG_APPLOCAL_DEPS_INSTALL=ON",
    "-DVCPKG_TARGET_TRIPLET=${arch}-${os}",
    "-DVCPKG_MANIFEST_MODE=ON",
    "-DCMAKE_TOOLCHAIN_FILE=${env:VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
    "-S${RootDir}",
    "-G Ninja"
)

if ($CC) {
    #${CC} = (Get-Command ${CC}).Source
    $common_options += "-DCMAKE_C_COMPILER=${CC}"
}

if ($CPP) {
  #${CPP} = (Get-Command ${CPP}).Source
  $common_options += "-DCMAKE_CXX_COMPILER=${CPP}"
}

set-location $RootDir

$BuildTypes | ForEach-Object {
    $build_type = $_
    if ($Clean -and (Test-Path "${RootDir}/bin/${os}/${build_type}")) {
        Write-Host "============> Cleaning $build_type"
        Remove-Item -Force -Recurse "${RootDir}/bin/${os}/${build_type}"
    }
    $preset="${os}-${arch}-${build_type}"
    Write-Host "============> Building $preset"
    # can't write "do_something || exit 1" in PowerShell due to a bug still not fixed in pwsh 7.4.1
    & cmake --preset ${preset} $common_options
    if ($LASTEXITCODE -ne 0) { exit 1 }
    & cmake --build "${RootDir}/bin/${os}/${build_type}"
    if ($LASTEXITCODE -ne 0) { exit 1 }
    & cmake --install "${RootDir}/bin/${os}/${build_type}"
    if ($LASTEXITCODE -ne 0) { exit 1 }
}
