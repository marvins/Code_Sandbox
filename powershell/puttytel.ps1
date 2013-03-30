####################################################
#     PuTTYTel
#    Author:    Marvin Smith
#
#    Date:       3/1/2013

#  Load the vb assembly
[void] [System.Reflection.Assembly]::LoadWithPartialName("'Microsoft.VisualBasic")


#  Path the Putty executable
$PUTTY='C:\Program Files (x86)\PuTTY\putty.exe'

#   Port values for each subsystem
$port_values=@( 7002, 7003, 7004, 7005, 7008, 7009, 7010, 7011, 7012)
$port_names=@( "Central", "Slice1", "Slice2", "Slice3", "Slice4","Slice5","Slice6","Slice7","Slice8")

#  Desired IP Address
$ip='sysmgr'



#########################################################
#      A function to check if a PID is still alive      #
#########################################################
Function check_PID( $pidval ){

    #  Get the process, but don't throw an exception if it fails
    $pidres = Get-Process -Id $pidval -ErrorAction SilentlyContinue

    if( $pidres -eq $null ){ return $false }
    else{
        return $true
    }
}


#########################
#    Enter username     #
#########################
Function enter_username( $PIDNUM, $portnum ){
    
    Write-Host "INFO: $($PIDNUM)"
    #  Check if the PID is still alive
    $pid_alive = check_PID $PIDNUM.Id

    #  If is is not alive, then ignore
    if( $pid_alive -ne $null ){
        echo "Found Port $portnum"

        #  Get the attention of the current window
        [Microsoft.VisualBasic.Interaction]::AppActivate($PIDNUM.ID)
        [void] [System.Reflection.Assembly]::LoadWithPartialName("'System.Windows.Forms")
        
        #  Send two Enter Keystrokes to check for user input
        [System.Windows.Forms.SendKeys]::SendWait("{ENTER}")
        [System.Windows.Forms.SendKeys]::SendWait("{ENTER}")

        #  Return control back to the console
        [Microsoft.VisualBasic.Interaction]::AppActivate($PID)
    
        $option=Read-Host "Does port $portnum require user login? (y/n - default) "
        if( $option -eq "Y" -or $option -eq "y" ){

            ${UNAME}=Read-Host 'Please enter username '
            ${PSSWD}=Read-Host -AsSecureString 'Please enter password '

            # Get the attention of the current system and enter the username
            [Microsoft.VisualBasic.Interaction]::AppActivate($PIDNUM.ID)
            [System.Windows.Forms.SendKeys]::SendWait("$UNAME{ENTER}")    
            
            #  Wait a few seconds and send password
            wait 2
            [System.Windows.Forms.SendKeys]::SendWait("$PSSWD{ENTER}")
            


        }

    } else {
        echo "Port $portnum is dead"
    }
}

#########################
#    Enter command      #
#########################
Function enter_command( $PIDNUM, $portnum, $COMMAND  ){

    #  Check if the PID is still alive
    $pid_alive = check_PID $PIDNUM.Id

    #  If is is not alive, then ignore
    if( $pid_alive -ne $null ){

        #  Get the attention of the current window
        [Microsoft.VisualBasic.Interaction]::AppActivate($PIDNUM.ID)
        [void] [System.Reflection.Assembly]::LoadWithPartialName("'System.Windows.Forms")
        
        #  Send two Enter Keystrokes to check for user input
        [System.Windows.Forms.SendKeys]::SendWait("$COMMAND{ENTER}")
        

        #  Return control back to the console
        [Microsoft.VisualBasic.Interaction]::AppActivate($PID)
    
    } else {
        echo "Port $portnum is dead"
    }
}

Function wait( $tm ) {
  Start-Sleep -seconds $tm
}


###########################################################
#                 Start of Main Program                   #
###########################################################
#  Open the required PuTTY Sessions
$pid_list=@()
for( $x=0; $x -lt $port_values.Length; $x++ ){
    $pid_list+=(Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet $ip -P  $($port_values[$x])")
}

#########################
#  Print PIDs
for( $x=0; $x -lt $port_values.Length; $x++ ){
    Write-Host "$($port_names[$x]) has PID $(($pid_list[$x]).Id)"

}


#################################################
#      Prompt Initial Instructions to User      #
$option=Read-Host 'Please wait for all slices to load. Now is a good time to position your windows. press Enter to continue'


#################################################
#             Log into each system              #
echo "Starting Log-In Process"
for( $x=0; $x -lt $port_values.Length; $x++ ){
    enter_username $($pid_list[$x])  $($port_values[$x])
}


#  Start infinite loop
$breakloop=$false
while ($breakloop -eq $false){


    $input=Read-Host 'Command '
    if( $input -eq 'help' ){
        Write-Host 'PuTTYTel Help'
        Write-Host '--------------'
        Write-Host ''
        Write-Host '  help: Display help'
        Write-Host '  quit: Quit PuttyTel'
        Write-Host '  port <NUMBER> command: <command> - Issue command to specific port'
    }
    elseif ( $input -eq 'quit' ){
        $breakloop=$true
    }

    #   Otherwise we have a command and need to enter it into each PuTTY Session
    else{
        for( $x=0; $x -lt $port_values.Length; $x++ ){
            enter_command $($pid_list[$x])  $($port_names[$x]) $input
        }
    }



}


#  Ask if you want to kill all windows
$option=Read-Host 'Do you wish to kill all current PuTTY Sessions? (y/n - default) '
if( $option -eq 'Y' -or $option -eq 'y' ){

    #####################################
    #  Check if the PID is still alive, and kill if necessary
    for( $x=0; $x -lt $port_values.Length; $x++ ){
        if( $(check_PID $(($pid_list[$x]).Id)) -eq $true ){
            Stop-Process $pid_list[$x]
        } 
    }
   
}

