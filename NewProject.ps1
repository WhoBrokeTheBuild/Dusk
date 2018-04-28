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
   [string] $Directory = $pwd
)

Get-ChildItem "$PSScriptRoot\template" | Copy -Destination "$Directory" -Recurse
