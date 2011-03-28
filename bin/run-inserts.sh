#!/bin/bash

#config variables
sleeptime=2
testdir="../tests/data"
outfile="results-insert"

if [ -e $outfile ]; then
    rm $outfile
fi

export TIME="%C,%E,%M"

#test each bias level
for selfadjust in "" "-self-adjust"
do 
    for nwords in "5000" "10000" "20000"
    do

        #test each implementation
        for imp in "-nop" "-map" "-treap" "-hashtable" "-skiplist" "-splaytree"
        do
            echo "testing: $imp"

            #run test and record to log file 
            for i in 1 2 3 4 5 6 7 8 9 10
            do
                echo run $i
                /usr/bin/time -a -o $outfile ./search $imp $testdir/i$nwords.txt $selfadjust -size=$nwords > /dev/null
                sleep $sleeptime
            done 
        done
    done
done
