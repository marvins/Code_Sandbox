#   Name  : Profile.ps1
#   Author: Marvin Smith
#   
#   Purpose:  Provides common utilities and scripts in a profile similar
#             to a bashrc.  Place in Env:UserProfile/My Documents/WindowsPowerShell/profile.ps1
#             Also ensure that script is signed or set execution policy to unrestricted


############################################
#  pgrep
#
#    purpose:  Use similar to Unix variant
Function pgrep( $argname ){
    return Get-Process $argname
}

#############################################
#   lsa
#
#     purpose:  show all files, including hidden in a directory
#   
Function lsa( $args ){
    return (Get-ChildItem $args -Force)
}

#############################################
#    Stop-Putty
#
#     purpose:  Stops all PuTTy Sessions immediately
#
Function Stop-Putty( $args ){

    #  Make sure putty is installed
    Get-Process putty | Stop-Process
}


