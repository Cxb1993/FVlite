#!/bin/bash

datafile="data/results.dat"
#datafile="steady_state.dat"
outfile="results.png"
#outfile="ScatteringPECsteadystate.png"

gnuplot -e "datafile='${datafile}'"   \
        -e "outfile='${outfile}'"     \
        FVTD_HEAT.plt
