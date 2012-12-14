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
            
            # Print the time
            echo `date +%s` >> ${LOGFILE}

            # Run MPSTAT
            echo "MPSTAT" >> ${LOGFILE}
            mpstat > temp.txt
            cat temp.txt | sed '1,2d' | sed 's/ * / /g' >> ${LOGFILE}

            # Run iostat
            echo "IOSTAT" >> ${LOGFILE}
            iostat -c > temp.txt
            cat temp.txt | sed '1,2d' | sed 's/avg-cpu://g' | sed 's/ * / /g' >> ${LOGFILE}
            
            # Run vmstat
            echo "VMSTAT" >> ${LOGFILE}
            vmstat > temp.txt
            cat temp.txt | sed '1d' | sed 's/ * / /g' >> ${LOGFILE}

            sleep 1
            done

        time02=`date +%s`
        echo ${time02} >> ${LOGFILE}
        
        # Remove all blank lines
        sed -e '/^ *$/d' ${LOGFILE} > temp.txt
        cat temp.txt > ${LOGFILE}
        
        # Remove the temp file
        if [ -f temp.txt ]; then
            rm temp.txt
        fi


    done


