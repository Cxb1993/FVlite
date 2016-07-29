# gnuplot file for FVlite
# 'Heatmap' of H field

#set terminal epslatex color size 7in, 5in
#set output 'timedomain.eps' 
set terminal png size 1080,720
set output 'timedomain.png'

unset key

# axes
set xlabel '$x (m)$'
set ylabel '$y (m)$'
set xrange [1.25:1.75]
set yrange [0:0.5]

# colorbar
set zrange [-2:2]
set cbrange [-2:2]
set cblabel "$H_z$"
set palette defined (0 "magenta", 1 "white", 2 "blue")
#set palette defined (0 "purple", 1 "magenta",  2 "white", 3 "blue", 4 "navy")
#set palette defined (0 "green", 1 "red")
#set palette defined (0 "white", 1 "black")
#load 'RdBu.plt'


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
#EULER
#plot datafile using 1:2:3 with image
#splot datafile using 1:2:3  w l
#plot datafile using 1:3 with l
