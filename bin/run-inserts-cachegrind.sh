#!/bin/bash

#config variables
sleeptime=2
testdir="../tests/data"
outfile="results-insert-cg"

if [ -e $outfile ]; then
    rm $outfile
fi

#test each bias level
for selfadjust in "" "-self-adjust"
do 
    for nwords in "5000" "10000" "20000"
    do

        #test each implementation
        for imp in "-nop" "-map" "-treap" "-hashtable" "-skiplist" "-splaytree"
        do
            echo "testing: $imp" 
            valgrind --tool=cachegrind --branch-sim=yes --cachegrind-out-file=/dev/null ./search $imp $testdir/i$nwords.txt $selfadjust -size=$nwords 2>> $outfile 
            sleep $sleeptime
        done
    done
done
