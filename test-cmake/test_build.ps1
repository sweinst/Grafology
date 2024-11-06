$wd = $PWD
try {
    # remove any left over
    Remove-Item -force -recurse ${PSScriptRoot}/tmp -ErrorAction Ignore
    # run cmake
    $nd = mkdir ${PSScriptRoot}/tmp
    Set-Location $nd
    cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug  "-DCMAKE_TOOLCHAIN_FILE=${env:VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" -S .. -B .
    # cleaning
    Set-location $wd
    Remove-Item -force -recurse ${PSScriptRoot}/tmp -ErrorAction Ignore
}
catch {
    Write-Host $_.Excption.Message
    Set-location $wd
    Remove-Item -force -recurse ${PSScriptRoot}/tmp -ErrorAction Ignore
    exit 1
}