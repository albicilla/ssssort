# IMPORT-DATA stats ../stats.txt

set terminal pngcairo enhanced font 'Lato,10'

set style line 11 lc rgb "#333333" lt 1
set border 3 back ls 11
set tics nomirror
# define grid
set style line 12 lc rgb "#333333" lt 0 lw 1
set grid back ls 12

set grid xtics ytics

set key top left

set yrange [0:3.5]

set xlabel 'Item Count [log_2(n)]'
set ylabel 'Run Time / n log_2n [Nanoseconds]'

#SQL DELETE FROM stats WHERE LOG(2, size) < 12

set output "random.png"
set title 'Super Scalar Sample Sort Test: Random'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "random"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 0 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 1 title "std::sort" with linespoints


set output "80pcsorted.png"
set title 'Super Scalar Sample Sort Test: 80% Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "80pcsorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 2 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 3 title "std::sort" with linespoints


set output "90pcsorted.png"
set title 'Super Scalar Sample Sort Test: 90% Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "90pcsorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 4 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 5 title "std::sort" with linespoints


set output "99pcsorted.png"
set title 'Super Scalar Sample Sort Test: 99% Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "99pcsorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 6 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 7 title "std::sort" with linespoints


set output "99.9pcsorted.png"
set title 'Super Scalar Sample Sort Test: 99.9% Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "99.9pcsorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 8 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 9 title "std::sort" with linespoints


set output "90pctail.png"
set title 'Super Scalar Sample Sort Test: 90% Sorted + 10% Random Tail'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "tail90"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 10 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 11 title "std::sort" with linespoints


set output "99pctail.png"
set title 'Super Scalar Sample Sort Test: 99% Sorted + 1% Random Tail'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "tail99"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 12 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 13 title "std::sort" with linespoints


set output "sorted.png"
set title 'Super Scalar Sample Sort Test: Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "sorted"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 14 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 15 title "std::sort" with linespoints



set output "reverse.png"
set title 'Super Scalar Sample Sort Test: Reverse Sorted'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "reverse"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 16 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 17 title "std::sort" with linespoints

set output "many-dupes.png"
set title 'Super Scalar Sample Sort Test: Many duplicates (A[i]=i^{16} mod floor(log_2 n)'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "many-dupes"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 18 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 19 title "std::sort" with linespoints


set output "few-spikes-with-noise.png"
set title 'Super Scalar Sample Sort Test: Few spikes, lots of noise'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "few-spikes-with-noise"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 20 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 21 title "std::sort" with linespoints


set output "ones.png"
set title 'Super Scalar Sample Sort Test: All Ones'
## MULTIPLOT(algo) SELECT LOG(2, size) AS x,
## AVG(time) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats WHERE name = "ones"
## GROUP BY MULTIPLOT, x ORDER BY MULTIPLOT, x
plot \
    'speed_png-data.txt' index 22 title "ssssort" with linespoints, \
    'speed_png-data.txt' index 23 title "std::sort" with linespoints
