# IMPORT-DATA stats stats.txt

#set terminal pdf size 13.33cm,10cm linewidth 2.0
set terminal pngcairo enhanced font 'Lato,10'
set output "speed.png"

set style line 11 lc rgb "#333333" lt 1
set border 3 back ls 11
set tics nomirror
# define grid
set style line 12 lc rgb "#333333" lt 0 lw 1
set grid back ls 12

set grid xtics ytics

set key top left

set yrange [0:4]

set xlabel 'Item Count [log_2(n)]'
set ylabel 'Run Time / n log_2n [Nanoseconds]'

#SQL DELETE FROM stats WHERE LOG(2, size) < 12

set output "plots/random.png"
set title 'Super Scalar Sample Sort Test: Random'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "random"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 0 title "algo=ssssort" with linespoints, \
    'speed-data.txt' index 1 title "algo=stdsort" with linespoints


set output "plots/80pcsorted.png"
set title 'Super Scalar Sample Sort Test: 80% Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "80pcsorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 2 title "algo=ssssort" with linespoints, \
    'speed-data.txt' index 3 title "algo=stdsort" with linespoints


set output "plots/90pcsorted.png"
set title 'Super Scalar Sample Sort Test: 90% Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "90pcsorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 4 title "algo=ssssort" with linespoints, \
    'speed-data.txt' index 5 title "algo=stdsort" with linespoints


set output "plots/99pcsorted.png"
set title 'Super Scalar Sample Sort Test: 99% Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "99pcsorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 6 title "algo=ssssort" with linespoints, \
    'speed-data.txt' index 7 title "algo=stdsort" with linespoints


set output "plots/99.9pcsorted.png"
set title 'Super Scalar Sample Sort Test: 99.9% Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "99.9pcsorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 8 title "algo=ssssort" with linespoints, \
    'speed-data.txt' index 9 title "algo=stdsort" with linespoints


set output "plots/90pctail.png"
set title 'Super Scalar Sample Sort Test: 90% Sorted + 10% Random Tail'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "tail90"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 10 title "algo=ssssort" with linespoints, \
    'speed-data.txt' index 11 title "algo=stdsort" with linespoints


set output "plots/99pctail.png"
set title 'Super Scalar Sample Sort Test: 99% Sorted + 1% Random Tail'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "tail99"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 12 title "algo=ssssort" with linespoints, \
    'speed-data.txt' index 13 title "algo=stdsort" with linespoints


set output "plots/sorted.png"
set title 'Super Scalar Sample Sort Test: Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "sorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 14 title "algo=ssssort" with linespoints, \
    'speed-data.txt' index 15 title "algo=stdsort" with linespoints



set output "plots/reverse.png"
set title 'Super Scalar Sample Sort Test: Reverse Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "reverse"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 16 title "algo=ssssort" with linespoints, \
    'speed-data.txt' index 17 title "algo=stdsort" with linespoints


set output "plots/ones.png"
set title 'Super Scalar Sample Sort Test: All Ones'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## MEDIAN(time / (size * log(2, size)) * 1e6) AS y,
## MULTIPLOT
## FROM stats WHERE name = "ones"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
