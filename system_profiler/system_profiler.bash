#! /bin/bash -


NUM_ITERATIONS=1

LOGFILE=output.txt

EXEC="geo_convert"
ARGS="-i -I /home/ms6401/Dropbox/24*.ntf -O converted.jpg "
COMMAND="geo_convert -i -I /home/ms6401/Dropbox/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf -O converted.jpg"

if [ -f ${LOGFILE} ]; then
    rm ${LOGFILE}
fi

for (( i=0; i<${NUM_ITERATIONS}; i++ ))
    do
        
        # Start a process
        ${COMMAND} &
        pid=`pgrep ${EXEC}`
        
        # Grab the time
        time01=`date +%s`

        while kill -0 $pid >/dev/null 2>&1;
            do
            sleep 1
            done
        time02=`date +%s`

        echo "Start time: $time01"
        echo "Stop time: $time02"


    done


