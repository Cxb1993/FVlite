# gnuplot file for FVlite LevelSet

unset key

# axes
set xlabel 'x (m)'
set ylabel 'y (m)'
#set xrange [0:1]
#set yrange [0:1]
set hidden3d
set view 0,0
set view equal xy
set isosamples 6

#splot 'data/results.dat' using 1:2:9 with pm3d, 0
splot 'geometry.dat' using 1:2:3 w l, 0,\
      'vertices.dat' using 1:2:3 w l, 
#splot 'data/results.dat' using 1:2:9 w l, 0 # maxwell
pause -1
