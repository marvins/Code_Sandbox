#  File:    runner.ps1
#  Author:  Marvin Smith
#  Date:    9/30/2017
#
#  Purpose:  Builds and manages app in Windows
#
Param(
    [switch]$Build   = $false,
    [switch]$Clean   = $false,
    [switch]$Help    = $false,
    [switch]$Release = $false,
    [switch]$Debug   = $false
)

#  Import Helper Subscripts
. .\scripts\powershell\Build_Utilities.ps1

#  Misc Information
$Build_Info = @{"Release" = "release"; "Debug" = "debug"}
$offset_str = '    '

#----------------------------------------------------#
#-       Function to Print Usage Information        -#
#----------------------------------------------------#
Function Print_Usage()
{
    Write-Output "usage: $(Split-Path -Leaf $MyInvocation.ScriptName)"
    Write-Output "-----------------"

    Write-Output '    -Help  : Print these usage instructions.'
    Write-Output '    -Build : Build project'
    Write-Output '    -Clean : Clean all existing builds.'

}


#----------------------------#
#-      Clean Project       -#
#----------------------------#
Function Clean_Project()
{
    Write-Output "->  Cleaning Previous Builds"
    
    #  Check Release Builds
    if( $Release -eq $true -and $(Test-Path $Build_Info["Release"]) -eq $true ){
        Write-Output "$offset_str-> Found Release Build: $($Build_Info["Release"])"
        Remove-Item -Recurse -Path $Build_Info["Release"]
    }
    elseif( $Release -eq $true -and $(Test-Path $Build_Info["Release"]) -eq $false ){
        Write-Output "$offset_str-> No Debug Release Found: $($Build_Info["Release"])"
    }

    #  Check Debug Builds
    if( $Debug -eq $true -and $(Test-Path $Build_Info["Debug"]) -eq $true ){
        Write-Output "$offset_str-> Found Debug Build: $($Build_Info["Debug"])"
        Remove-Item -Recurse -Path $Build_Info["Debug"]
    }
    elseif( $Debug -eq $true -and $(Test-Path $Build_Info["Debug"]) -eq $false ){
        Write-Output "$offset_str-> No Debug Build Found: $($Build_Info["Debug"])"
    }

}

#----------------------------#
#-      Build Project       -#
#----------------------------#
Function Build_Project( $Build_Type )
{
    #  Log Entry
    Write-Output "$offset_str->Building Project.  Type: $Build_Type"

    #   Create the release directory
    $build_dir=$Build_Info[$Build_Type]
    Write-Output "$offset_str   -> Build-Directory: $build_dir"

    #  Check if already exists
    if( $(Test-Path $build_dir) -eq $false )
    {
        Write-Output "$offset_str   -> Creating Build Directory"
        New-Item -ItemType Directory -Path $build_dir
    }

    #  Get the CMake Settings Path
    $CMakeSettingsPath=''
    if( $(Test-Path .\build-settings.json) -eq $true )
    {
        $CMakeSettingsPath = $(Join-Path $(Split-Path -Resolve .\build-settings.json) build-settings.json)
    }
    else{
        Write-Error 'Cannot find build-settings.json file. Aborting.'
        return 1
    }    

    #  Get CMake Configuration Settings from JSON
    Write-Output "$offset_str-> Parsing CMake Settings: $CMakeSettingsPath"
    $Build_Settings = Parse_CMake_Settings $CMakeSettingsPath
    Write-Output "Output: $($Build_Settings)"


    #  Enter Directory
    $original_dir=Get-Location
    Set-Location $build_dir

    #  Run CMake
    Write-Host "$offset_str   -> Running CMake."
    Write-Host "$Build_Settings"
    Invoke-Expression $Build_Settings

    #  Exit Directory
    Set-Location $original_dir

}

#----------------------------------#
#-        Check Parameters        -#
#----------------------------------#
if( $Help -eq $true -or $($Build -or $Clean) -eq $false )
{
    Print_Usage
}

#  Make sure if nobody set, then at least release is used
if( $Debug -eq $false -and $Release -eq $false )
{
    $Release=$true
}

#----------------------------------------------#
#-          Check if we need to clean         -#
#----------------------------------------------#
if( $Clean -eq $true )
{
	Clean_Project
}


#----------------------------------------------#
#-          Check if we need to build         -#
#----------------------------------------------#
if( $Build -eq $true )
{
    #   Check Release
    if( $Release -eq $true ){
        Build_Project 'Release'
    }
    if( $Debug -eq $true ){
        Build_Project 'Debug'
    }
}
