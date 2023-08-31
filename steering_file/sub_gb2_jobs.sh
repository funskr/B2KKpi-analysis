#!/bin/bash

#test
#gbasf2 B2KKpi_analysis.py -p test3 -i /belle/MC/release-06-00-08/DB00002100/MC15ri_b/prod00024796/s00/e1003/4S/r00000/ssbar/mdst \
#       -f FBDT_cs_wight.root -s release-06-01-12

cat gb2_job.conf | grep -v "#" | while IFS=: read project list 
do
       #project=`echo ${list} | awk -F "." '{print $1}'`
       #project=${name}"_"${project}"_"$(date +%m%d%H%M)

       list="./list/"${list}
       echo $project $list
       
       gbasf2 B2KKpi_analysis.py -p ${project} --input_dslist ${list} -f FBDT_cs_weight.root -s release-05-02-20 --force
       #mc analized under boss version release-06-01-12

done