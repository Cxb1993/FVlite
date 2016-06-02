#!/bin/bash

rm -f data/*
./FVTD.exe $1
#./plot.sh
./make_movie.sh
