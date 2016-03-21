# IMPORT-DATA stats stats.txt

set terminal pdf size 28cm,18cm linewidth 2.0
set output "speed.pdf"

set pointsize 0.7
set style line 6 lc rgb "#f0b000"
set style line 15 lc rgb "#f0b000"
set style line 24 lc rgb "#f0b000"
set style line 33 lc rgb "#f0b000"
set style line 42 lc rgb "#f0b000"
set style line 51 lc rgb "#f0b000"
set style line 60 lc rgb "#f0b000"
set style increment user

set grid xtics ytics

set key top left

set title 'Super Scalar Sample Sort Test'
set xlabel 'Item Count [log_2(n)]'
set ylabel 'Run Time per Item [Microseconds / Item]'

## MULTIPLOT(algo, name) SELECT LOG(2, size) AS x,
## MEDIAN(time / iterations / size * 1e6) AS y,
## MULTIPLOT
## FROM stats
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 0 title "algo=ssssort,name=random" with linespoints, \
    'speed-data.txt' index 1 title "algo=ssssort,name=reverse" with linespoints, \
    'speed-data.txt' index 2 title "algo=ssssort,name=sorted" with linespoints, \
    'speed-data.txt' index 3 title "algo=stdsort,name=random" with linespoints, \
    'speed-data.txt' index 4 title "algo=stdsort,name=reverse" with linespoints, \
    'speed-data.txt' index 5 title "algo=stdsort,name=sorted" with linespoints
