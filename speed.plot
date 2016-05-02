# IMPORT-DATA stats stats.txt

set terminal pdf size 13.33cm,10cm linewidth 2.0
set output "speed.pdf"

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

set style line 1 lt 1 lw .2
set style line 2 lt 1 lw 1

set style line 3 lt 2 lw .2
set style line 4 lt 2 lw 1

set style line 5 lt 3 lw .2
set style line 6 lt 3 lw 1


#SQL DELETE FROM stats WHERE LOG(2, size) < 12

# most cross-DB way of doing what "SELECT ..., a FROM ..., (VALUES(-1), (0),
# (1)) as dev (a)" does in postgres
# SQL CREATE TABLE IF NOT EXISTS temp_add (a INTEGER)
# SQL DELETE FROM temp_add
# SQL INSERT INTO temp_add VALUES (-1), (0), (1)

set title 'Super Scalar Sample Sort Test: Random'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = 'random'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, size
plot \
    'speed-data.txt' index 0 notitle ls 3 with lines, \
    'speed-data.txt' index 1 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 2 notitle ls 3 with lines, \
    'speed-data.txt' index 3 notitle ls 5 with lines, \
    'speed-data.txt' index 4 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 5 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: 80% Sorted'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = '80pcsorted'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, size
plot \
    'speed-data.txt' index 6 notitle ls 3 with lines, \
    'speed-data.txt' index 7 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 8 notitle ls 3 with lines, \
    'speed-data.txt' index 9 notitle ls 5 with lines, \
    'speed-data.txt' index 10 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 11 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: 90% Sorted'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = '90pcsorted'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 12 notitle ls 3 with lines, \
    'speed-data.txt' index 13 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 14 notitle ls 3 with lines, \
    'speed-data.txt' index 15 notitle ls 5 with lines, \
    'speed-data.txt' index 16 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 17 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: 99% Sorted'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = '99pcsorted'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 18 notitle ls 3 with lines, \
    'speed-data.txt' index 19 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 20 notitle ls 3 with lines, \
    'speed-data.txt' index 21 notitle ls 5 with lines, \
    'speed-data.txt' index 22 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 23 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: 99.9% Sorted'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = '99.9pcsorted'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 24 notitle ls 3 with lines, \
    'speed-data.txt' index 25 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 26 notitle ls 3 with lines, \
    'speed-data.txt' index 27 notitle ls 5 with lines, \
    'speed-data.txt' index 28 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 29 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: 90% Sorted + 10% Random Tail'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = 'tail90'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 30 notitle ls 3 with lines, \
    'speed-data.txt' index 31 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 32 notitle ls 3 with lines, \
    'speed-data.txt' index 33 notitle ls 5 with lines, \
    'speed-data.txt' index 34 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 35 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: 99% Sorted + 1% Random Tail'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = 'tail99'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 36 notitle ls 3 with lines, \
    'speed-data.txt' index 37 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 38 notitle ls 3 with lines, \
    'speed-data.txt' index 39 notitle ls 5 with lines, \
    'speed-data.txt' index 40 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 41 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: Sorted'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = 'sorted'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 42 notitle ls 3 with lines, \
    'speed-data.txt' index 43 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 44 notitle ls 3 with lines, \
    'speed-data.txt' index 45 notitle ls 5 with lines, \
    'speed-data.txt' index 46 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 47 notitle ls 5 with lines



set title 'Super Scalar Sample Sort Test: Reverse Sorted'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = 'reverse'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 48 notitle ls 3 with lines, \
    'speed-data.txt' index 49 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 50 notitle ls 3 with lines, \
    'speed-data.txt' index 51 notitle ls 5 with lines, \
    'speed-data.txt' index 52 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 53 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: Many duplicates (A[i]=i^{16} mod floor(log_2 n)'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = 'many-dupes'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 54 notitle ls 3 with lines, \
    'speed-data.txt' index 55 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 56 notitle ls 3 with lines, \
    'speed-data.txt' index 57 notitle ls 5 with lines, \
    'speed-data.txt' index 58 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 59 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: Few spikes, lots of noise'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = 'few-spikes-with-noise'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 60 notitle ls 3 with lines, \
    'speed-data.txt' index 61 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 62 notitle ls 3 with lines, \
    'speed-data.txt' index 63 notitle ls 5 with lines, \
    'speed-data.txt' index 64 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 65 notitle ls 5 with lines


set title 'Super Scalar Sample Sort Test: All Ones'
## MULTIPLOT(algo, a) SELECT LOG(2, size) AS x,
## AVG(time + a * stddev) / (size * log(2, size)) * 1e6 AS y,
## MULTIPLOT
## FROM stats, temp_add WHERE name = 'ones'
## GROUP BY MULTIPLOT, size ORDER BY MULTIPLOT, x
plot \
    'speed-data.txt' index 66 notitle ls 3 with lines, \
    'speed-data.txt' index 67 title "ssssort" ls 4 with linespoints, \
    'speed-data.txt' index 68 notitle ls 3 with lines, \
    'speed-data.txt' index 69 notitle ls 5 with lines, \
    'speed-data.txt' index 70 title "std::sort" ls 6 with linespoints, \
    'speed-data.txt' index 71 notitle ls 5 with lines


# SQL DROP TABLE temp_add
