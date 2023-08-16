#!/usr/bin/env python3

import basf2 as b2
import modularAnalysis as ma
#import sys
import stdV0s
#import stdCharged
#import stdPi0s
#import stdPhotons
import vertex
import flavorTagger as ft 
import variables.collections as vc
import variables.utils as vu
import sys
from variables.MCGenTopo import mc_gen_topo

#filenumber = sys.argv[1]

inputfile="/home/belle2/yuanmk/analysis/B2KKpi/sigMC/mdst/B2KKpi_signalMC_*.root"
outputfile=f"./ana_B2KKpi_signalMC_flavor_tag.root"

#create path
main = b2.Path()

#set analysis global tag (needed for flavor tagging)
#b2.conditions.prepend_globaltag(ma.getAnalysisGlobaltag())

ma.inputMdstList(
    filelist=[inputfile],
    #filelist=[b2.find_file(f"B2KKpi_signalMC_gen.root","examples")],
    path=main,
    environmentType='default'
)

# fill final state particle lists
ma.fillParticleList(
    "K+:pre",
    "kaonID>0.9",
    path=main
)

gamma_cut = "[clusterNHits > 1.5] and [0.2967 < clusterTheta < 2.6180] "
gamma_cut += "and [[clusterReg==1 and E>0.1] or [clusterReg==2 and E>0.06] or [clusterReg==3 and E>0.1]] "
gamma_cut += "and clusterE9E21>0.9"

ma.fillParticleList(
    "gamma:pi0",
    gamma_cut,
    path=main
)
# and [[clusterReg==1 and E>0.02] or [clusterReg==2 and E>0.02] or [clusterReg==3 and E>0.02]]
# and clusterE1E9 > 0.3

#ma.fillParticleList(
#    "gamma:good",
#    "",
#    path=main
#)
#stdCharged.stdK(path=main)
#stdPi0s.stdPi0s(listtype = 'eff30_May2020Fit', path=main)
#stdV0s.stdKshorts(path=main)

# combine final state particles to form composite particles
#pi0_cut = ""

ma.reconstructDecay(
    "pi0:loose -> gamma:pi0 gamma:pi0",
    cut="[0.111 < InvM < 0.153] and daughterAngle(0,1)<0.9",
    path=main
)

ma.reconstructDecay(
    "B0 -> K+:pre K-:pre pi0:loose",
    cut="[-0.30 < deltaE < 0.15] and [ 5.23 < Mbc < 5.289]",
    #candidate_limit=-1,
    path=main
)

# build the rest of the event
ma.buildRestOfEvent("B0", fillWithMostLikely=True, path=main)
track_based_cuts = "thetaInCDCAcceptance and pt > 0.075 and dr < 5 and abs(dz) < 10"
ecl_based_cuts = "thetaInCDCAcceptance and E > 0.05"
roe_mask = ("my_mask", track_based_cuts, ecl_based_cuts)
ma.appendROEMasks("B0", [roe_mask], path=main)

#vertex fit
vertex.treeFit('B0', ipConstraint=True, path=main)


# call flavor tag
ft.flavorTagger(["B0"], path=main)


#create list of variables to save into the output file
#B variables
b_vars = []
standard_vars = vc.kinematics + vc.mc_kinematics + vc.mc_truth

#flavor tag
b_vars += ft.flavor_tagging 

#add aliase
fs_vars = vc.pid + vc.track + vc.track_hits + standard_vars

kaon_vars = vc.pid + standard_vars
b_vars += vu.create_aliases_for_selected(
    kaon_vars,
    "B0 -> ^K+ ^K- [pi0 -> gamma gamma]",
    prefix=["Kp", "Km"],
)

pi0_vars = []
pi0_vars += ['InvM', 'daughterDiffOfPhi(0,1)', 'daughterAngle(0,1)']
pi0_vars += standard_vars
b_vars += vu.create_aliases_for_selected(
    pi0_vars,
    "B0 -> K+ K- [^pi0 -> gamma gamma]",
    prefix=["pi0"],
)

gamma_pi0_vars = []
gamma_pi0_vars += ['clusterTheta', 'clusterPhi']
gamma_pi0_vars += ['clusterE', 'clusterE1E9', 'clusterReg']
gamma_pi0_vars += ['clusterTiming' ,'clusterErrorTiming']
b_vars += vu.create_aliases_for_selected(
    gamma_pi0_vars,
    "B0 -> K+ K- [pi0 -> ^gamma ^gamma]",
    prefix=["gamma1", "gamma2"],
)

b_vars += vc.deltae_mbc
b_vars += standard_vars
b_vars += ['dr', 'dz']
#b_vars += ft.flavor_tagging

b_vars += mc_gen_topo(200)

# match reconstructed with MC particles
ma.matchMCTruth("B0", path=main)
ma.matchMCTruth("pi0:loose", path=main)

# save variables to an output file (ntuple)
ma.variablesToNtuple(
    "B0",
    variables= b_vars,
    filename=outputfile,
    treename="B0",
    path=main,
)

b2.process(main)

# print out the summary
print(b2.statistics)