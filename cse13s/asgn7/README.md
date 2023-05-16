# Assignment 7: A (Huffman Coding) Tree Grows in Santa Cruz

This assignment creates two programs which are the huffman encoder and decoders. The encoder will read in an input file, finding the Huffman encoding of its contents, and use the encoding to compress the file. The decoder will read in a compressed input file and decompress it, expanding it back to its original, uncompressed size. 

scan build passed with no errors

# File Descriptions

encode.c : This file contains an implementation of the Huffman encoder. 

decode.c : This file contains an implementation of the Huffman decoder. 

defines.h : This file contains the macro definitions used throughout the files.

header.h : This file contains the struct definition for a file header.

node.h : This file contains the node ADT interface.

node.c : This file contains the implementation of the node ADT.

pq.h : This file contains the priority queue ADT interface.

pq.c : This file contains the implementation of the priority queue ADT. 

code.h : This file contains the code ADT interface.

code.c :  This file contains the implementation of the code ADT. 

io.h : This file contains the I/O module interface.

io.c : This file contains the implementation of the I/O module. 

stack.h : This file contains the stack ADT interface. 

stack.c : This file contains the implementation of the stack ADT.  

huffman.h : This file contains the Huffman conding module interface. 

huffman.c : This file contains the implementation of the Huffman coding module. 

Makefile: This file is responsible for doing the compilation process for the encode and decode program. 

README.md : This file contains instructions on how to build and run encode and decode. 

DESIGN.pdf : This file describes the purpose of the program and communicates the overall design of the program.

## Building

Run ```make encode``` to build the encode.c program.  
Run ```make decode``` to build the decode.c program.   
Run ```make``` to run the encode.c and decode.c programs.  
Run ```make clean``` to remove all files that are compiler generated except the executable.  
Run ```make spotless``` to remove all files that are compiler generated as well as the executable.  

## Running encode 
```./encode [-h] [-i infile] [-o outfile]```  
   
flag options:    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-h             Program usage and help.```   
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-v             Print compression statistics.```   
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-i infile      Input file to compress.```   
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-o outfile     Output of compressed data.```   

## Running decode  
```./decode [-h] [-i infile] [-o outfile]```   
   
flag options:   
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-h             Program usage and help.```   
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-v             Print compression statistics.```   
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-i infile      Input file to decompress.```    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-o outfile     Output of decompressed data.```   
