# gnuplot file

#set terminal png size 1280,720
#set terminal png size 1920,1080
#set output 'PEC_test.png'

set terminal epslatex color size 7.5in, 8in 
set output 'electromagnetic.eps'

set multiplot layout 2,1

###############################################
# TIMEDOMAIN
##############################################

# axes
set xlabel '$x (m)$'
set ylabel '$y (m)$'
set xrange [1.25:1.75]
set yrange [0:0.5]

# colorbar
set zrange [-2:2]
set cbrange [-2:2]
set cblabel " "
set palette defined (0 "magenta", 1 "white", 2 "blue")
#set palette defined (0 "purple", 1 "magenta",  2 "white", 3 "blue", 4 "navy")
#set palette defined (0 "green", 1 "red")
#set palette defined (0 "white", 1 "black")
#load 'RdBu.plt'

unset key

set size square
#set size ratio 0.25
set xyplane 0.1
#set view 90, 0 


# circles
set object circle at 1.5,0.25 size 0.125 front fillcolor rgb "#white" fillstyle solid
set object circle at 1.5,0.25 size 0.125 front fillcolor rgb "#black" fillstyle empty


set view map
#MAXWELL
plot 'MaxwellTimeDomain.dat' using 1:2:8 with image

#################################################
# FREQDOMAIN
#################################################

set xlabel '$\theta$'
set ylabel '$H_z$' rotate by 90 

set yrange [0:2]

set xrange [0:360]
set xtics 60
set key center bottom

plot 'PEC_test_exact.dat' u 1:2 w l lw 2 lc 0 title 'Exact',\
     'PEC_test_Staircase.dat' u 1:2 every 5 ls 8 lc 3 title 'Standard Method',\
     'PEC_test_CutCells.dat' u 1:2 every 7 ls 4 lc 1 title 'Cut-Cell Method',\

unset multiplot
