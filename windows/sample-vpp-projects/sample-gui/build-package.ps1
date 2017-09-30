#    File:    build-package.ps1
#    Author:  Marvin Smith
#    Date:    9/29/2017
#


$qt_home='C:\Qt\5.9.1\msvc2017_64'

$qt_packages="$qt_home\plugins\platforms"

$Build_Type='Release'

#  Build Package
if( $(Test-Path 'Releases\Sample-GUI') -eq $false )
{
    New-Item -ItemType Directory 'Releases\Sample-GUI' -Force
}

#  Copy Contents of Packages to Destination
Copy-Item "$qt_packages\\*.dll" 'Releases\Sample-GUI\'
Copy-Item "$qt_home\\bin\\*.dll" 'Releases\Sample-GUI\'

#  Copy GUI
Copy-Item ".\release\bin\$Build_Type\sample-gui.exe" '.\Releases\Sample-GUI\'