#!/bin/bash

#check for command line
if [ -z "$1" ]; then
    echo "usage: $0 testsize"
    exit
fi

#config variables
nwords=$1
sleeptime=2
testdir="../tests/data" 

outfile="results-search-$nwords"

if [ -e $outfile ]; then
    rm $outfile
fi

export TIME="%C,%E,%M"

#test each bias level
for zipf in "0" "0.5" "1" "1.5"
do
    echo "zipf s: $zipf"

    for selfadjust in "" "-self-adjust"
    do 
        #test each implementation
        for imp in "-nop" "-map" "-treap" "-hashtable" "-skiplist" "-splaytree"
        do
            echo "testing: $imp"

            #run test and record to log file 
            for i in 1 2 3 4 5 6 7 8 9 10
            do
                echo run $i
                /usr/bin/time -a -o $outfile ./search $imp "$testdir/s$nwords-500000-z$zipf.txt" $selfadjust > /dev/null
                sleep $sleeptime
            done 
        done
    done
done
