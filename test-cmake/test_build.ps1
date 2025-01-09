$src_root=${PSScriptRoot}
$tmp_dir="${src_root}/bin"
$os = $IsWindows ? "Windows" : ( $IsLinux ? "Linux" : ( $IsMacOS ? "MacOS" : "Unknown" ) )


Push-Location .
try {
    Set-location ${src_root}
    # remove any left over
    Remove-Item -force -recurse ${tmp_dir} -ErrorAction Ignore
    # run cmake
    cmake --preset ${os} || exit 1
    cmake --build bin/Debug || exit 1
    # cleaning
    Remove-Item -force -recurse ${PSScriptRoot}/tmp -ErrorAction Ignore
    Write-Host "Done."
}
catch {
    Write-Host $_.Exception.Message
    exit 1
}
finally {
    Pop-Location
    Remove-Item -force -recurse ${PSScriptRoot}/tmp -ErrorAction Ignore
}