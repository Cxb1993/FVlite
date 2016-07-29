#!/bin/bash

datafile="data/MaxwellSlopeInit.dat"
outfile="plots/MaxwellSlopeInit.png"

gnuplot -e "datafile='${datafile}'"   \
        -e "outfile='${outfile}'"     \
        FVlite.plt
