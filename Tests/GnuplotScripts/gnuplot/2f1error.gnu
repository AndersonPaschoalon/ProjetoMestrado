#!/usr/bin/gnuplot
# Usage:
# $ gnuplot -e "variable='value'" script.gnu
# $ gnuplot -e "title='kkk'" -e "sizeratio='0.5'" function.gnu

reset

# vars
if (!exists("filename")) filename='./plots/default'
if (!exists("datafile")) datafile='./data/2f1error.dat'
if (!exists("font")) font= 'Helvetica,10'
if (!exists("xlabel")) xlabel='xlabel'
if (!exists("ylabel")) ylabel='ylabel'
if (!exists("title")) title='title'
if (!exists("legend1")) legend1='legend1'
if (!exists("legend2")) legend2='legend2'
if (!exists("sizeratio")) sizeratio= '0'
if (!exists("linestyle1")) linestyle1 = '1'
if (!exists("linestyle2")) linestyle2 = '12'
if (!exists("type")) type = 'lines'

#styles
set style line 1 linecolor rgb "red" linetype 1 pointtype 1 #r-+
set style line 2 linecolor rgb "red" linetype 1 pointtype 2 #r-x
set style line 3 linecolor rgb "red" linetype 1 pointtype 3 #r-*
set style line 4 linecolor rgb "red" linetype 1 pointtype 6 #r-o
set style line 11 linecolor rgb "blue" linetype 1 pointtype 1 #b-+
set style line 12 linecolor rgb "blue" linetype 1 pointtype 2 #b-x
set style line 13 linecolor rgb "blue" linetype 10  pointtype 3 #b-*
set style line 14 linecolor rgb "blue" linetype 1 pointtype 6 #b-o
set style line 21 linecolor rgb "green" linetype 1 pointtype 1 #g-+
set style line 22 linecolor rgb "green" linetype 1 pointtype 2 #g-x
set style line 23 linecolor rgb "green" linetype 1 pointtype 3 #g-*
set style line 24 linecolor rgb "green" linetype 1 pointtype 6 #g-o
set style line 31 linecolor rgb "black" linetype 1 pointtype 1 #k-+
set style line 32 linecolor rgb "black" linetype 1 pointtype 2 #k-x
set style line 33 linecolor rgb "black" linetype 1 pointtype 3 #k-*
set style line 34 linecolor rgb "black" linetype 1 pointtype 6 #k-o
set style line 41 linecolor rgb "violet" linetype 1 pointtype 1 #v-+
set style line 42 linecolor rgb "violet" linetype 1 pointtype 2 #v-x
set style line 43 linecolor rgb "violet" linetype 1 pointtype 3 #v-*
set style line 44 linecolor rgb "violet" linetype 1 pointtype 6 #v-o
set style line 51 linecolor rgb "orange" linetype 1 pointtype 1 #v-+
set style line 52 linecolor rgb "orange" linetype 1 pointtype 2 #v-x
set style line 53 linecolor rgb "orange" linetype 1 pointtype 3 #v-*
set style line 54 linecolor rgb "orange" linetype 1 pointtype 6 #v-o

# plot options
set grid
set key box opaque right bottom
set title title
set ylabel ylabel
set xlabel xlabel
if( sizeratio == '0'){
	set size 1,1
}
else{
	set size ratio sizeratio
} 
set xtics font font
set ytics font font
set label font font
set xlabel font font
set ylabel font font
set title font font
set key font font
set tics  font font 
set datafile separator ","

#plot png
set terminal pngcairo
set out filename.'.png'
if( type eq 'points') \
	plot datafile using 1:2 with points ls linestyle1 title legend1, \
	datafile using 1:3 with points ls linestyle2, \
	datafile using 1:3:4 with errorbar ls linestyle2 title legend2
if( type eq 'linespoints') \
	plot datafile using 1:2 with points ls linestyle1 title legend1, \
	datafile using 1:2 with lines ls linestyle1, \
	datafile using 1:3 with lines ls linestyle2, \
	datafile using 1:3:4 with errorbar ls linestyle2  title legend2
if( type eq 'lines') \
	plot datafile using 1:2 with lines ls linestyle1  title legend1, \
	datafile using 1:3 with lines ls linestyle2, \
	datafile using 1:3:4 with errorbar ls linestyle2  title legend2

#plot pdf
set terminal pdf
set out filename.'.pdf'
replot

reset


