#!/bin/bash

basf2_mva_merge_mc  -s /home/belle2/yuanmk/data/B2KKpi/sample/cs/sigMC_sample.root \
                    -b /home/belle2/yuanmk/data/B2KKpi/sample/cs/uubar_sample.root \
                       /home/belle2/yuanmk/data/B2KKpi/sample/cs/ssbar_sample.root \
                       /home/belle2/yuanmk/data/B2KKpi/sample/cs/ddbar_sample.root \
                       /home/belle2/yuanmk/data/B2KKpi/sample/cs/ccbar_sample.root \
                    --cut "flag_candidate==1" \
                    --fsig 0.5 \
                    -o ./mix_sample.root -t B0 --ftest 0.3