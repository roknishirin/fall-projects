# Assignment 4: All Sorts of C Code

The purpose of this program is to implement Shell Sort, Bubble Sort, Quick Sort and Heapsort. Shell Sort first sorts pairs of elements which are far apart from each other and increments down the array. Bubble Sort works by examining adjacent pairs of items to sort each element one by one. Quick Sort is a divide-and-conquer algorithm which partitions arrays into sub-arrays and then sorting them. Heapsort uses a binary tree structure which compares parent nodes to the values of its children. This program also has a test harness which accepts various command-line options that can enable all of the sorting algorithms or a selection of them, can set the random seed to an inputted value, can set the number of elements in the array to a specific size, can print out the elements number from the array, and can print the program usage. 

## File Descriptions 

bubble.c : implements Bubble Sort 

bubble.h : specifies the interface to bubble.c

heap.c : implements Heap Sort

heap.h : specifies the interface to heap.c

shell.c : implements Shell Sort

shell.h : specifies the interface of shell.c

quick.c : implements Quick Sort

quick.h : specifies the interface of quick.c 

set.c : implements set ADT

set.h : specifies the interface for the set ADT

stats.c : implements the statistics module

stats.h : specifies the interface to the statistics module

mtrand.c : implements the Mersenne Twister module

mtrand.h : specifies the interface to the Mersenne Twister module

sorting.c : running the test harness and allows command line options to display the various sorting functions

README.md : This file contains instructions on how to build and run sorting.

DESIGN.pdf : This file describes the purpose of the program and communicates the overall design of the program.

WRITEUP.pdf : This file contains a discussion of the results for several questions regarding the sorting command. It analyzes data derived from the command and uses graphical representations.

## Building

Run ```make``` to build the sorting.c program.  
Run ```make clean``` to remove any compiler-generated files.  

## Running

``` ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]```

flag options:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  ``` -H```              Display program help and usage.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  ``` -a```              Enable all sorts.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  ``` -b```              Enable Bubble Sort.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  ``` -h```              Enable Heap Sort.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  ``` -q```              Enable Quick Sort.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  ``` -s```              Enable Shell Sort.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  ``` -n length```       Specify number of array elements (default: 100).  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  ``` -p elements```     Specify number of elements to print (default: 100).  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  ``` -r seed```         Specify random seed (default: 13371453).  


