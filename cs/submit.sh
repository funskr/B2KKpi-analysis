#!/bin/bash

gbasf2 B2KKpi_cs.py -p B2KKpi_cs_uubar --input_dslist ./list/MC15ri_uubar_1abinv.list -s release-06-01-12
gbasf2 B2KKpi_cs.py -p B2KKpi_cs_ddbar --input_dslist ./list/MC15ri_ddbar_1abinv.list -s release-06-01-12
gbasf2 B2KKpi_cs.py -p B2KKpi_cs_ssbar --input_dslist ./list/MC15ri_ssbar_1abinv.list -s release-06-01-12
gbasf2 B2KKpi_cs.py -p B2KKpi_cs_ccbar --input_dslist ./list/MC15ri_ccbar_1abinv.list -s release-06-01-12