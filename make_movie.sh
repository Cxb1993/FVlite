#!/bin/bash

runname=$1

rm -f plots/*

filenumber="`ls data -v | tail -3 | egrep -o [0-9]+`"

echo "Number of files: $filenumber"

i="1"
while [ $i -le $filenumber ]
do
    echo "Plotting $i..."
    gnuplot -e "datafile='data/$runname$i.dat';outfile='plots/$runname$i.png'" FVlite.plt
    i=$[$i+1]
done

#ffmpeg -i "./plots/plot%d.png" -t 20 -b 5000k -vcodec mpeg4 movie.mp4
#mv movie.mp4 movie.mpeg4
