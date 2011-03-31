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

outfile="results-search-$nwords-cg"

if [ -e $outfile ]; then
    rm $outfile
fi

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

            valgrind --tool=cachegrind --branch-sim=yes --cachegrind-out-file=/dev/null ./search $imp $testdir/s$nwords-500000-z$zipf.txt $selfadjust -size=$nwords > /dev/null 2>> $outfile 

            sleep $sleeptime
        done
    done
done
