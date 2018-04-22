#
# In order to run this script, you must first run
#   Set-ExecutionPolicy -ExecutionPolicy RemoteSigned
#
# Move to your project folder, and run this script with the full path to the
# Dusk installation.
# ex.
#   . C:/Path/to/Dusk/NewProject.ps1 C:/Path/to/my/Project/
#

param(
   [string] $Directory
)

function Test-Administrator
{
    $user = [Security.Principal.WindowsIdentity]::GetCurrent();
    (New-Object Security.Principal.WindowsPrincipal $user).IsInRole([Security.Principal.WindowsBuiltinRole]::Administrator)
}

if (!(Test-Administrator))
{
    Write-Host "Run this script as an Administrator"
    Exit
}

if (!(Test-Path "HKLM:\Software\Dusk")) {
    New-ItemProperty -Path "HKLM:\Software\Dusk" -Name "Path" -Value "$PSScriptRoot"
}

Copy-Item "$PSScriptRoot\cmake\Project.cmake" "$Directory\CmakeLists.txt"
