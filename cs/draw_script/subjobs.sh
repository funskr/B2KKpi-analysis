#!/bin/bash

current=`pwd`
script=${current}/script

mkdir -p ${script}
rm -rf ${script}/*

name=`ls *_tmplate.C | cut -d "_" -f 1`


cat variables | grep -v "#" | while IFS=: read variable legend
do
    scriptname=${script}/${name}_${variable}.C
    rm -rf $scriptname
    touch $scriptname
    cat ./*_tmplate.C \
    | sed "s#VAR#${variable}#g" \
    | sed "s#LEGEND#${legend}#g" >> $scriptname
done

cd ${script}
rm -rf tmp_list
ls ${name}_* > tmp_list
while read line
do
root -l -q ${line}
done < tmp_list

echo "Done!!!"
