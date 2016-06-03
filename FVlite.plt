# gnuplot file for FVlite
# 'Heatmap' of H field

#set terminal postscript eps enhanced color
set terminal png size 800,800 font "Times Roman" 12
#set terminal epslatex color size 4.5in, 4.5in
set output outfile

unset key

# colorbar
set cbrange [0:2]
#set cblabel "Hz  (A/m)"
#set palette defined (0 "blue", 1 "white", 2 "violet")
#set palette defined (0 "green", 1 "red")
set palette defined (0 "white", 1 "black")

# axes
set xlabel 'x (m)'
set ylabel 'y (m)'
set xrange [0:2]
set yrange [0:2]
#set zrange [-2:2]
set size square
set xyplane 0.1
#set view 90, 0 


# circles
#set object circle at 0.5,0.5 size 0.125 front fillcolor rgb "#B0C4DE" fillstyle solid
#set object circle at 0.5,0.5 size 0.125 front fillcolor rgb "#black" fillstyle empty


set view map
#MAXWELL
#plot datafile using 1:2:8 with image
#EULER
plot datafile using 1:2:3 with image
#splot datafile using 1:2:3  w l
#plot datafile using 1:3 with l
