#!/bin/bash

datafile="data/Default.dat"
#datafile="steady_state.dat"
outfile="Default.png"
#outfile="ScatteringPECsteadystate.png"

gnuplot -e "datafile='${datafile}'"   \
        -e "outfile='${outfile}'"     \
        FVTD_HEAT.plt
