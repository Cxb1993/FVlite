set terminal wxt
# set terminal png size 1080, 1080
#set output outfile

unset key

set ylabel rotate by 0

set hidden3d

set xlabel 'x'
set ylabel 'y'

set view 75,75

splot datafile u 1:2:8 w l
pause -1
