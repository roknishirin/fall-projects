#!/bin/bash

# Rebuild the collatz executable first.
make clean && make collatz

# initialization
 >/tmp/graph1.dat
 >/tmp/graph2.dat
 >/tmp/graph3.dat
 >/tmp/graph4.dat
 >/tmp/length.dat


# for loop which obtains the variables and uploads them to .dat files	

for i in {2..10000}; do 
	./collatz -n $i > coll.dat;  				 # parameter for collatz
	length=$(cat coll.dat | wc -l);				 # finding length of each line
	values=$(cat coll.dat | sort -n |tail -1);	         # finding highest value			 
	echo "$i $length" >> /tmp/graph1.dat;			 # adding (i,length) points
	echo "$i $values" >> /tmp/graph2.dat;			 # adding (i,value) points
	echo "$length" >> /tmp/length.dat;			 # making length into a file
	total=0				
	for j in $(< coll.dat); do				 # finding total value of each collatz sequence
	       	let total+=$j; 
	done	
	echo "$i $total" >> /tmp/graph4.dat;			 # adding (i, total) points
done 


sort -n /tmp/length.dat | uniq -c >> /tmp/graph3.dat  		 # sorting length file and finding repetition


# graph 1
gnuplot <<END
	set terminal pdf
	set output "graph1.pdf"
	set xlabel "n"
	set ylabel "length"
	set xtics 1000
	set ytics 50
	set xrange [2:10000]
	set yrange [0:300]
	set key off 
	set title "Collatz Sequence Lengths"
	plot "/tmp/graph1.dat" with points pt 0 
END

# graph 2
gnuplot <<END
	set terminal pdf
	set output "graph2.pdf"
	set xlabel "n"
	set ylabel "value"
	set xtics 1000
	set ytics 20000
	set xrange [2:10000]
	set yrange [0:100000]
	set key off
	set title "Maximum Collatz Sequence Value"
	plot "/tmp/graph2.dat" with points pt 0
END

# graph 3
gnuplot <<END 
	set terminal pdf 
	set output "graph3.pdf"
	set xlabel "length"
	set ylabel "frequency"
	set xtics 25
	set ytics 20
	set xrange [2:225]
	set yrange [0:200]
	set key off
	set title "Collatz Sequence Length Histogram"
	plot "/tmp/graph3.dat" using 2:1 with impulses
END

# graph 4
gnuplot <<END
	set terminal pdf
	set output "graph4.pdf"
	set xlabel "n"
	set ylabel "total sum"
	set xtics 1000
	set ytics 80000
	set xrange [2:10000]
	set yrange [0:800000]
	set key off 
	set title "Collatz Sequence Total Sum"
	plot "/tmp/graph4.dat" with points pt 0
END
		
