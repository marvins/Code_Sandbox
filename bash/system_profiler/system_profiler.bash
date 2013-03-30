#! /bin/bash -


NUM_ITERATIONS=3

LOGFILE=example_logfile.txt

COMMAND="demo/demo01"
EXEC="demo01"

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
        echo "RUN_START ${time01}" >> ${LOGFILE}

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

            sleep .1
            done

        time02=`date +%s`
        echo "RUN_END ${time02}" >> ${LOGFILE}
        
    done

# Remove all blank lines
sed -e '/^ *$/d' ${LOGFILE} > temp.txt
cat temp.txt > ${LOGFILE}

# Remove the temp file
if [ -f temp.txt ]; then
    rm temp.txt
fi

# Parse the script
python profile_reader.py ${LOGFILE}

