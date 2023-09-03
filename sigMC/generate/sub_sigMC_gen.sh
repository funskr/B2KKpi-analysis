#!/bin/bash

current=`pwd`
JOB_NUM=100
i=0
while [ $i -lt $JOB_NUM ]
do
    #echo $i
    cd ${current}
    bsub -q l "basf2 ./generateMC.py ${i}"
    let i=$i+1
done