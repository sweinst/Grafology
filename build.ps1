<#
.SYNOPSIS
  Launch a build/install for one or more build types
.PARAMETER BuildTypes
  Which build types to generate: Debug, Release, RelWithDebInfo or a combination? Use 'All' to build all types.
.PARAMETER Clean
  Remove build folder first?
#>

param(  
    [Parameter(Mandatory=$true, Position=0)]
    [ValidateNotNullOrEmpty()]
    [ValidateSet('', 'Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel', 'All')]
    [string[]] $BuildTypes,

    [switch] $Clean
)

if ($BuildTypes -eq 'All') {
    $BuildTypes = @('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')
}
Write-Host "Build Types: $BuildTypes"

$src_root=${PSScriptRoot}
$os = $IsWindows ? "windows" : ($IsLinux ? "linux" : "osx")

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

$BuildTypes | ForEach-Object {
    $build_type = $_
    if ($Clean) {
        Write-Host "============> Cleaning $build_type"
        Remove-Item -Force -Recurse "${src_root}/bin/${os}/${build_type}"
    }
    $preset="${os}-x64-${build_type}"
    Write-Host "============> Building $preset"
    & cmake --preset ${preset} $common_options || exit 1
    & cmake --build "${src_root}/bin/${os}/${build_type}" || exit 1
    & cmake --install "${src_root}/bin/${os}/${build_type}" || exit 1
}
