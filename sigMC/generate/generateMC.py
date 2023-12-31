#!/usr/bin/env python3

import basf2 as b2
import generators as ge
import simulation as si
import reconstruction as re
import mdst
import sys

#the number of output mdst 
filenumber = sys.argv[1]

# Create the steering path
main = b2.Path()

# Define number of events and experiment number
main.add_module('EventInfoSetter', expList=1003, runList=0, evtNumList=10000)

# Generate B0B0bar events
ge.add_evtgen_generator(
    path=main,
    finalstate='signal',
    signaldecfile='./B2KKpi.dec'
)

# Simulate the detector response and the L1 trigger
si.add_simulation(path=main)

# Reconstruct the objects
re.add_reconstruction(path=main)

# Create the mDST output file
mdst.add_mdst_output(path=main, filename=f'/home/belle2/yuanmk/data/B2KKpi/sigMC/mdst_new/B2KKpi_signalMC_{filenumber}.root')
#{filenumber}
#add the progress module
#main.add_module('Progress', maxN=3)

# Process the steering path
b2.process(path=main)

# Finally, print out some statistics about the modules execution
print(b2.statistics)