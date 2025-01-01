<#
.SYNOPSIS
Set up the Visual Studio toolchainb.
.DESCRIPTION
Loads the Visual Studio PowerShell extension and set up the environment for a specific build architecture.
.PARAMETER Version
The Visual Studioo version: 2019 or 2022. Warning: once a version has been loaded, it cannot be changed.
.PARAMETER Bits
Use 32 (x86) or 64 (x64) version?
.PARAMETER Edition
Which edition of Visual Studio to use: Community, Professional, Enterprise or Preview?
#>

param(
    [ValidateSet(2019,2022)]
    [int] $Version = 2022,
    [ValidateSet(32, 64)]
    [int] $Bits = 64,
    [ValidateSet("Community", "Professional", "Enterprise", "Preview")]
    [string] $Edition = "Professional"
)

$cur_dir = $PWD
try {
    if ($Bits -eq 64) {
        $arch = "x64"
    }
    else {
        $arch = "x86"
    }
    if ($Version -eq 2019) {
        $vs_path = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2019\$Edition"
    }
    else {
        $vs_path = "$env:ProgramFiles\Microsoft Visual Studio\2022\$Edition"
    }
    Remove-Module Microsoft.VisualStudio.DevShell -ErrorAction Ignore
    Import-Module "${vs_path}\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
    Enter-VsDevShell -VsInstallPath "${vs_path}" -DevCmdArguments "-arch=$arch"
}
finally {
    set-location $cur_dir
}
