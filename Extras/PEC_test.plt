# gnuplot file

#set terminal png size 1280,720
set terminal png size 1920,1080
set output 'PEC_test.png'
#set output 'ScatteringPECanalytic.png'
#set terminal epslatex color size 4in, 3in
#set output 'ScatteringPECanalytic.eps'

set xlabel 'theta'
set ylabel 'Hz' 

set xrange [0:360]
set xtics 60
set grid
set key box center bottom

plot 'PEC_test_exact.dat' u 1:2 w l lw 2 lc 0 title 'Exact',\
     'PEC_test_CutCells.dat' u 1:2 every 1 ls 4 lc 1 title 'FVTD CutCells',\
     'PEC_test_Staircase.dat' u 1:2 every 1 ls 8 lc 3 title "FVTD 'Staircased'",\
