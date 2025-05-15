set terminal pngcairo enhanced font "Arial,18" size 1080,720

set xlabel 'File size (MB)'
set title "Write bandwidth with Lustre striping"

# Grid at major/minor ticks
set grid xtics ytics mxtics mytics lw 0.3 lt 1 lc rgb "gray"

#set xrange [100 : 12800]
set yrange[0.0 : *]

set border lw 2   # Thicker border

set key autotitle columnhead

set ylabel 'Average bandwidth (MB/s)'
set output 'avg.png'
plot \
    "stripe-1/results.txt" using 1:6 with linespoints title "Stripe count 1", \
    "stripe-4/results.txt" using 1:6 with linespoints title "Stripe count 4", \
    "stripe-8/results.txt" using 1:6 with linespoints title "Stripe count 8", \
    "stripe-24/results.txt" using 1:6 with linespoints title "Stripe count 24", \


set ylabel 'Min bandwidth (MB/s)'
set output 'min.png'
plot \
    "stripe-1/results.txt" using 1:7 with linespoints title "Stripe count 1", \
    "stripe-4/results.txt" using 1:7 with linespoints title "Stripe count 4", \
    "stripe-8/results.txt" using 1:7 with linespoints title "Stripe count 8", \
    "stripe-24/results.txt" using 1:7 with linespoints title "Stripe count 24", \

set ylabel 'Max bandwidth (MB/s)'
set output 'max.png'
plot \
    "stripe-1/results.txt" using 1:8 with linespoints title "Stripe count 1", \
    "stripe-4/results.txt" using 1:8 with linespoints title "Stripe count 4", \
    "stripe-8/results.txt" using 1:8 with linespoints title "Stripe count 8", \
    "stripe-24/results.txt" using 1:8 with linespoints title "Stripe count 24", \

set ylabel 'Median bandwidth (MB/s)'
set output 'median.png'
plot \
    "stripe-1/results.txt" using 1:9 with linespoints title "Stripe count 1", \
    "stripe-4/results.txt" using 1:9 with linespoints title "Stripe count 4", \
    "stripe-8/results.txt" using 1:9 with linespoints title "Stripe count 8", \
    "stripe-24/results.txt" using 1:9 with linespoints title "Stripe count 24", \

