#!/bin/bash

datafile="data/SlopeToroInit.dat"
outfile="plots/SlopeToroInit.png"

gnuplot -e "datafile='${datafile}'"   \
        -e "outfile='${outfile}'"     \
        FVlite.plt
