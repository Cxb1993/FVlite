#!/bin/bash

datafile="data/results.dat"
outfile="results.png"

gnuplot -e "datafile='${datafile}'"   \
        -e "outfile='${outfile}'"     \
        FVTD.plt
