#!/bin/bash

#config variables
nwords=$1
sleeptime=2
testsize=10000
testdir="../tests/data" 
imp="-hashtable"

outfile="results-search-$nwords"

if [ -e $outfile ]; then
    rm $outfile
fi

export TIME="%C,%E,%M"

#test each bias level
for zipf in "0" "0.5" "1" "1.5"
do
    echo "zipf s: $zipf"

    for size in "5000" "10000" "15000" "20000" 
    do 
        #run test and record to log file 
        for i in 1 2 3 4 5 6 7 8 9 10
        do
            echo run $i
            /usr/bin/time -a -o $outfile ./search $imp "$testdir/s$nwords-500000-z$zipf.txt" -size=$size > /dev/null
            sleep $sleeptime
        done 
    done
done
