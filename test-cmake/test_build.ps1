$wd = $PWD
$src_root=${PSScriptRoot}
$os = "windows"
$common_options = @(
    "-DVCPKG_APPLOCAL_DEPS=ON",
    "-DX_VCPKG_APPLOCAL_DEPS_INSTALL=ON",
    "-DVCPKG_TARGET_TRIPLET=x64-${os}",
    "-DVCPKG_MANIFEST_MODE=ON",
    "-DCMAKE_TOOLCHAIN_FILE=${env:VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
    "-S${src_root}"
)

try {
    # remove any left over
    Remove-Item -force -recurse ${PSScriptRoot}/tmp -ErrorAction Ignore
    # run cmake
    $nd = mkdir ${PSScriptRoot}/tmp
    Set-Location $nd
    & cmake -G 'Visual Studio 17 2022' -A x64 -DCMAKE_BUILD_TYPE=Debug -B . $common_options || exit 1
    cmake --build . || exit 1
    # cleaning
    Set-location $wd
    Remove-Item -force -recurse ${PSScriptRoot}/tmp -ErrorAction Ignore
    Write-Host "Done."
}
catch {
    Write-Host $_.Excption.Message
    exit 1
}
finally {
    Set-location $wd
    Remove-Item -force -recurse ${PSScriptRoot}/tmp -ErrorAction Ignore
}