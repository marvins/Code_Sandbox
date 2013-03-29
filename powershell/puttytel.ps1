####################################################
#     PuTTYTel-GS2
#    Author:    Marvin Smith
#                     Sierra Nevada Corporation
#    Date:       3/1/2013

#  Load the vb assembly
[void] [System.Reflection.Assembly]::LoadWithPartialName("'Microsoft.VisualBasic")



#  Temp variables for username and password
${global:UNAME}=""
${global:PSSWD}=""

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

            ${global:UNAME}=Read-Host 'Please enter username '
            ${global:PSSWD}=Read-Host -AsSecureString 'Please enter password '

            # Get the attention of the current system and enter the username
            [Microsoft.VisualBasic.Interaction]::AppActivate($PIDNUM.ID)
            [System.Windows.Forms.SendKeys]::SendWait("${global:UNAME}{ENTER}")    
            
            #  Wait a few seconds and send password
            wait 2
            [System.Windows.Forms.SendKeys]::SendWait("${global:UNAME}{ENTER}")


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

$PUTTY='C:\Program Files (x86)\PuTTY\putty.exe'

#   Port values for each subsystem
$central_port=7002
$slice1_port=7003
$slice2_port=7004
$slice3_port=7005
$slice4_port=7008
$slice5_port=7009
$slice6_port=7010
$slice7_port=7011
$slice8_port=7012


#  Desired IP Address
$ip='sysmgr-11'


#  Open the required PuTTY Sessions
$putty_pid1=Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet ${ip} -P ${central_port}"
$putty_pid2=Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet ${ip} -P ${slice1_port}"
$putty_pid3=Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet ${ip} -P ${slice2_port}"
$putty_pid4=Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet ${ip} -P ${slice3_port}"
$putty_pid5=Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet ${ip} -P ${slice4_port}"
$putty_pid6=Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet ${ip} -P ${slice5_port}"
$putty_pid7=Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet ${ip} -P ${slice6_port}"
$putty_pid8=Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet ${ip} -P ${slice7_port}"
$putty_pid9=Start-Process -FilePath $PUTTY -PassThru -ArgumentList "-telnet ${ip} -P ${slice8_port}"

#  Print PIDs
echo "Central has PID $(($putty_pid1).Id)"
echo "Slice 1 has PID $(($putty_pid2).Id)"
echo "Slice 2 has PID $(($putty_pid2).Id)"
echo "Slice 3 has PID $(($putty_pid2).Id)"
echo "Slice 4 has PID $(($putty_pid2).Id)"
echo "Slice 5 has PID $(($putty_pid2).Id)"
echo "Slice 6 has PID $(($putty_pid2).Id)"
echo "Slice 7 has PID $(($putty_pid2).Id)"
echo "Slice 8 has PID $(($putty_pid2).Id)"

#   Start loop
$option=Read-Host 'Please wait for all slices to load. Now is a good time to position your windows. press Enter to continue'


#  Enter username
wait 2
echo "Starting Log-In Process"
enter_username $putty_pid1 $central_port
enter_username $putty_pid2 $slice1_port
enter_username $putty_pid3 $slice2_port
enter_username $putty_pid4 $slice3_port
enter_username $putty_pid5 $slice4_port
enter_username $putty_pid6 $slice5_port
enter_username $putty_pid7 $slice6_port
enter_username $putty_pid8 $slice7_port
enter_username $putty_pid9 $slice8_port

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
        enter_command $putty_pid1 $central_port $input
        enter_command $putty_pid2 $slice1_port  $input
        enter_command $putty_pid3 $slice2_port  $input
        enter_command $putty_pid4 $slice3_port  $input
        enter_command $putty_pid5 $slice4_port  $input
        enter_command $putty_pid6 $slice5_port  $input
        enter_command $putty_pid7 $slice6_port  $input
        enter_command $putty_pid8 $slice7_port  $input
        enter_command $putty_pid9 $slice8_port  $input
    }



}


#  Ask if you want to kill all windows
$option=Read-Host 'Do you wish to kill all current PuTTY Sessions? (y/n - default) '
if( $option -eq 'Y' -or $option -eq 'y' ){

    #####################################
    #  Check if the PID is still alive, and kill if necessary
    
    #  Central
    $pid_alive = check_PID $((putty_pid1).Id)
    if( $pid_alive -eq $true ){ Stop-Process $putty_pid1 }

    #  Slice 1
    $pid_alive = check_PID $((putty_pid2).Id)
    if( $pid_alive -eq $true ){ Stop-Process $putty_pid2 }
    
    #  Slice 2
    $pid_alive = check_PID $((putty_pid3).Id)
    if( $pid_alive -eq $true ){ Stop-Process $putty_pid3 }
    
    #  Slice 3
    $pid_alive = check_PID $((putty_pid4).Id)
    if( $pid_alive -eq $true ){ Stop-Process $putty_pid4 }
    
    #  Slice 4
    $pid_alive = check_PID $((putty_pid5).Id)
    if( $pid_alive -eq $true ){ Stop-Process $putty_pid5 }
    
    #  Slice 5
    $pid_alive = check_PID $((putty_pid6).Id)
    if( $pid_alive -eq $true ){ Stop-Process $putty_pid6 }
    
    #  Slice 6
    $pid_alive = check_PID $((putty_pid7).Id)
    if( $pid_alive -eq $true ){ Stop-Process $putty_pid7 }
    
    #  Slice 7
    $pid_alive = check_PID $((putty_pid8).Id)
    if( $pid_alive -eq $true ){ Stop-Process $putty_pid8 }
    
    #  Slice 8
    $pid_alive = check_PID $((putty_pid9).Id)
    if( $pid_alive -eq $true ){ Stop-Process $putty_pid9 }
}
