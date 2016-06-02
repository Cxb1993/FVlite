# set terminal wxt
set terminal png size 480, 480
set output outfile

unset key

set ylabel rotate by 0

set hidden3d

set xlabel 'x'
set ylabel 'y'
set zrange [-1:1]

# For flat, 90,#. To look down y axis, 90,0. x axis, 90,90
set view 60,30

splot datafile u 1:2:8 w l
