# Assignment 2 -  A Small Numerical Library

This program creates and uses a small numerical library and a corresponding test harness. The test harness will determine which implemented functions to run through the use of commandline options that the user will enter.

## File Descriptions 

mathlib.h : Acts as a library where all of the math functions (sin, cos, arcsin, arccos, arctan, log) from mathlib.c are initialized and organized.

mathlib.c : This file contains the math function implementations, as prototyped in mathlib.h. 

mathlib-test.c : This file contains the main() program and acts as a test harness for the math library. It also contains the code to parse through command-line options and prints out the results of testing the math functions.

Makefile : This file is responsible for doing the compilation process for mathlib-test program. 

DESIGN.pdf : This file describes the purpose of the program and communicates the overall design of the program.

WRITEUP.pdf : This file contains a discussion of the results for the tests. It analyzes the differences in the output of my implementation versus those in the library. It includes possible reasons for the differences between my implementation and the standard library’s. Also includes graphical representations. 

## Building

Run ```make``` to build the mathlib-test program.  
Run ```make clean``` to remove any compiler-generated files.  

## Running

```./mathlib-test -flag```  
  
flag options:  
	-a to run all functions  
	-s to run the sin function  
	-c to run the cosine function  
	-S to run the arcsin function  
	-C to run the arccos function  
	-T to run the arctan function  
	-l to run the log function   
