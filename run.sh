#!/bin/bash

rm -f data/*
make
./FVTD.exe
./plot.sh
#./make_movie.sh
