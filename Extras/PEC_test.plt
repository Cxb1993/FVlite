# gnuplot file

#set terminal png size 1280,720
set terminal png size 1920,1080
set output 'PEC_test.png'

#set terminal epslatex color size 7in, 5in 
#set output 'freqdomain.eps'

set xlabel '$\theta$'
set ylabel '$H_z$' 
set yrange[0:2]

set xrange [0:360]
set xtics 60
set key center bottom spacing 1.5

plot 'PEC_test_exact.dat' u 1:2 w l lw 2 lc 0 title 'Exact',\
     'PEC_test_Staircase.dat' u 1:2 every 3 ls 8 lc 3 title 'FVTD standard',\
     'PEC_test_CutCells.dat' u 1:2 every 5 ls 4 lc 1 title 'FVTD Cut-Cell Method',\
