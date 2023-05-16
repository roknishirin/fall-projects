# Assignment 6: The Great Firewall of Santa Cruz: Bloom Filters, Linked Lists and Hash Tables

This assignment creates a program called banhammer which is in charge of filtering language which is deemed invalid and giving the user feedback on what they should be using instead. In order to promote virtue and prevent vice, and to preserve social cohesion and discourage unrest, the program will filter through text so that the user cannot be corrupted through the usage of unfortunate, hurtful, offensive and far too descriptive language.  It will read in words from stdin, identify any badspeak or old speak and output an appropriate punishment message. The badspeak and oldspeak (with the newspeak translation) that caused the punishment will be punished after the message. It statistics are enabled punishment messages are suppressed and only statistics will be printed. 

## File Descriptions

banhammer.c : Contains main() and contains the implementation for the banhammer program. 

messages.h : Defines the mixspeak, badspeak and goodspeak messages that are used in banhammer.c.

city.h : Defines the interface for the hash function using CityHash.

city.c : Contains the implementation of the hash function using CityHash.

ht.h : Defines the interface for the hash table ADT.

ht.c : Contains the implementation of the hash table ADT.

ll.h : Defines the interface for the linked list ADT.

ll.c : Contains the implementation of the linked list ADT.

node.h : Defines the interface for the node ADT.

node.c : Contains the implementation of the node ADT.

bf.h : Defines the interface for the Bloom filter ADT.

bf.c : Contains the implementation of the Bloom filter ADT.

bv.h : Defines the implementation for the bit vector ADT.

bv.c : Contains the implementation of the bit vector ADT.

parser.h : Defines the interface for the parsing module.

parser.c : Contains the implementation of the parsing module. 

Makefile : This file is responsible for doing the compilation process for the banhammer program.

README.md : This file contains instructions on how to build and run banhammer.

DESIGN.pdf : This file describes the purpose of the program and communicates the overall design of the program.

WRITEUP.pdf : This file answers various given questions and provides supporting evidence and analysis using graphs and statistics from many experiments for the answers. 

## Building

Run ```make``` to build the banhammer.c program.  
Run ```make clean``` to remove all files that are compiler generated except the executable.  
Run ```make spotless``` to remove all files that are compiler generated as well as the executable.  

## Running banhammer 
```./banhammer [-t <ht_size>] [-f <bf_size>] [-s] [-m] [-h]```   

flag options:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-t <ht_size>: Hash table size set to <ht_size>. (default: 10000)```  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-f <bf_size>: Bloom filter size set to <bf_size>. (default 2^19)```  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-s : Enables the printing of statistics.```  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-m : Enables move-to-front rule.```  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ```-h : Display program synopsis and usage.```
