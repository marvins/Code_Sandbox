#  Name: ACL File Utilities
#  Author: Marvin Smith
#  Purpose: Basic usage of ACL-modifying tools in Powershell
#
Function set-full-control( $filename, $username ){

    #  Grab the ACL
    icacls.exe $filename /grant "${username}:F"


}

Function set-security-permissions( $filename ){

    if( $(Test-Path $filename ) -eq $true ){
        takeown /F $filename /A 
        takeown /F $filename /A
    	icacls $filename  /grant  Administrators:F
        icacls $filename  /grant  SYSTEM:F
        icacls $filename  /remove 'Users'
        icacls $filename  /remove 'NT Service\TrustedInstaller'
     } 

}