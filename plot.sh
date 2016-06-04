#!/bin/bash

datafile="data/DefaultInit.dat"
outfile="plots/DefaultInit.png"

gnuplot -e "datafile='${datafile}'"   \
        -e "outfile='${outfile}'"     \
        FVlite.plt
