#!/bin/bash

rm -f plots/*

filenumber="`ls data -v | tail -1 | egrep -o [0-9]+`"

echo "Number of files: $filenumber"

i="0"
while [ $i -le $filenumber ]
do
    echo "Plotting $i..."
    gnuplot -e "datafile='data/results$i.dat';outfile='plots/plot$i.png'" FVTD_HEAT.plt
#    gnuplot -e "datafile='data/results$i.dat';outfile='plots/plot$i.png'" FVTD.plt
    i=$[$i+1]
done

ffmpeg -i "./plots/plot%d.png" -t 20 -b 5000k -vcodec mpeg4 movie.mp4
mv movie.mp4 movie.mpeg4
