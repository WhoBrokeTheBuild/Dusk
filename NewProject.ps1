
New-ItemProperty -Path "HKLM:\Software\Dusk" -Name "Path" -Value "$PSScriptRoot"

Copy-Item "$PSScriptRoot\cmake\Project.cmake" "CmakeLists.txt"
