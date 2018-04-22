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

Set-ExecutionPolicy -ExecutionPolicy RemoteSigned

if (!(Test-Path "HKLM:\Software\Dusk")) {
    New-ItemProperty -Path "HKLM:\Software\Dusk" -Name "Path" -Value "$PSScriptRoot"
}

Copy-Item "$PSScriptRoot\cmake\Project.cmake" "CmakeLists.txt"
