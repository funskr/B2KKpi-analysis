#!/bin/bash

current=`pwd`
JOB_NUM=$1
i=0
while [ $i -lt $JOB_NUM ]
do
    #echo $i
    cd ${current}
    bsub -q s "basf2 ./generateMC.py ${i}"
    let i=$i+1
done