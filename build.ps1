$common_options = @(
    "-DCMAKE_C_COMPILER=cl",
    "-DCMAKE_CXX_COMPILER=cl",
    "-DCMAKE_INSTALL_PREFIX=D:/src/Grafology/dist/Windows",
    "-DCMAKE_TOOLCHAIN_FILE=D:/src/vcpkg/scripts/buildsystems/vcpkg.cmake",
    "-DVCPKG_APPLOCAL_DEPS=ON",
    "-DX_VCPKG_APPLOCAL_DEPS_INSTALL=ON",
    "-DVCPKG_TARGET_TRIPLET=x64-windows",
    "-DVCPKG_MANIFEST_MODE=ON",
    "-SD:/src/Grafology",
    "-G Ninja"
)

$build_type = "Release"
& cmake "-DCMAKE_BUILD_TYPE=${build_type}" "-BD:/src/Grafology/bin/Windows/${build_type}" $common_options
