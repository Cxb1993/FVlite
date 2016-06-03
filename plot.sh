#!/bin/bash

datafile="data/Default.dat"
outfile="plots/Default.png"

gnuplot -e "datafile='${datafile}'"   \
        -e "outfile='${outfile}'"     \
        FVTD.plt
