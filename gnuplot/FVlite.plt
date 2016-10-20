# gnuplot file for FVlite
# 'Heatmap' of H field

#set terminal postscript eps enhanced color
set terminal png size 1200,800 font "Times Roman" 12
#set terminal epslatex color size 4.5in, 4.5in
set output outfile

unset key


# axes
set xlabel 'x (m)'
set ylabel 'y (m)'
#set xrange [1.25:1.75]
#set yrange [0:0.5]
#set xrange[0:1]
#set yrange[0:1]

# colorbar
#set zrange [-2:2]
#set cbrange [-2:2]
#set cblabel "Hz"
set palette defined (0 "magenta", 1 "white", 2 "blue")
#set palette defined (0 "purple", 1 "magenta",  2 "white", 3 "blue", 4 "navy")
#set palette defined (0 "green", 1 "red")
#set palette defined (0 "white", 1 "black")
#load 'RdBu.plt'


#set size square
set size ratio 0.6
set xyplane 0.1
#set view 90, 0 


# circles
#set object circle at 1.5,0.25 size 0.125 front fillcolor rgb "#white" fillstyle solid
#set object circle at 1.5,0.25 size 0.125 front fillcolor rgb "#black" fillstyle empty


set view map

#MAXWELL
plot datafile using 1:2:8 with image

#MAXWELL ELECTROSTATIC
#plot datafile using 1:2:3 with image

#EULER
#plot datafile using 1:2:3 with image
#splot datafile using 1:2:3  w l
#plot datafile using 1:3 with l
