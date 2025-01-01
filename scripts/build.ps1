<#
.SYNOPSIS
  Launch a build/install for one or more build types
.PARAMETER BuildTypes
  Which build types to generate: Debug, Release, RelWithDebInfo or a combination? Use 'All' to build all types.
.PARAMETER CC
  Which C compiler to use? Default is defied in the preset.
.PARAMETER CPP
  Which C++ compiler to use? Default is defied in the preset.
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
    [switch] $Clean
)

if ($BuildTypes -eq 'All') {
    $BuildTypes = @('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')
}
Write-Host "Build Types: $BuildTypes"

$src_root=${PSScriptRoot}
$os = $IsWindows ? "windows" : ($IsLinux ? "linux" : "osx")

if (-not $env:VCPKG_ROOT) {
  if (-not $env:VCPKG_INSTALLATION_ROOT) {
    Write-Host "VCPKG_ROOT or VCPKG_INSTALLATION_ROOT are not set. Exiting."
    exit 1
  }
  $env:VCPKG_ROOT = $env:VCPKG_INSTALLATION_ROOT
}

$common_options = @(
    "-DCMAKE_INSTALL_PREFIX=${src_root}/dist/${os}",
    "-DVCPKG_APPLOCAL_DEPS=ON",
    "-DX_VCPKG_APPLOCAL_DEPS_INSTALL=ON",
    "-DVCPKG_TARGET_TRIPLET=x64-${os}",
    "-DVCPKG_MANIFEST_MODE=ON",
    "-DCMAKE_TOOLCHAIN_FILE=${env:VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
    "-S${src_root}",
    "-G Ninja"
)

if ($CC) {
    $common_options += "-DCMAKE_C_COMPILER=${CC}"
}

if ($CPP) {
    $common_options += "-DCMAKE_CXX_COMPILER=${CPP}"
}

$BuildTypes | ForEach-Object {
    $build_type = $_
    if ($Clean -and (Test-Path "${src_root}/bin/${os}/${build_type}")) {
        Write-Host "============> Cleaning $build_type"
        Remove-Item -Force -Recurse "${src_root}/bin/${os}/${build_type}"
    }
    $preset="${os}-x64-${build_type}"
    Write-Host "============> Building $preset"
    # can't write "do_something || exit 1" in PowerShell due to a bug still not fixed in pwsh 7.4.1
    & cmake --preset ${preset} $common_options
    if ($LASTEXITCODE -ne 0) { exit 1 }
    & cmake --build "${src_root}/bin/${os}/${build_type}"
    if ($LASTEXITCODE -ne 0) { exit 1 }
    & cmake --install "${src_root}/bin/${os}/${build_type}"
    if ($LASTEXITCODE -ne 0) { exit 1 }
}
