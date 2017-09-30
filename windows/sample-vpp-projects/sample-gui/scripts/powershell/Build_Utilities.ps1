#      Powershell Utilities
#


Function Parse_CMake_Settings([string]$settings_path)
{
    #  Get the Contents of the Settings File
    $json_contents=Get-Content $settings_path | Out-String| ConvertFrom-Json
    
    #  Get the internal contents
    $cmake_args="cmake .. "

    #  Process the Generator
    $cmake_args += " -G `"$($json_contents.generator)`" "
    
    
    $cmake_vars = $json_contents.cmake_variables
#  Create actual command-line entries
    foreach( $pair in $cmake_vars )
    {
        $cmake_args += "-D$($pair.name)=`'$($pair.value)`' "
    }


    return $cmake_args
}