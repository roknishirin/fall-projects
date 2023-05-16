# Assignment 3 - I Have a Little Dreidel

The purpose of this program is to simulate a single game of dreidel. The simulation of dreidel is played with up to 8 players where each player, in alphabetical order, spin the dreidel and take one of the four actions: Nun (nothing), Gimel (the player takes the entire pot), Hayh (the player takes half of the pot, rounding down) and Shin (the player puts a coin into the pot). If a player is required to put a coin into the pot and they have no coins, they are out of the game and can no longer participate, meaning that they are eliminated from the game. After the game ends, the program prints out the name of the winning player, how many people that were playing, the amount of coins they all started with, how many rounds it took to complete the game, and the random number seed. 

## File Descriptions 

dreidel.h : This file contains the definitions for functions defined in dreidel.c and used by other code.

dreidel.c : This file contains the dreidel game logic.

play-dreidel.c : This file parses command line options to call functions in dreidel.c to play the dreidel game. It prints the name of the winning player as well as other metrics.

mtrand.c : This file is a 64-bit version of Mersenne Twister pseudorandom number generator.

mtrand.h : This file is the header file for mtrand.c and contains several function definitions that will be shared between several other files.

Makefile : This file is responsible for doing the compilation process for the play-dreidel.c file.

README.md : This file contains instructions on how to build and run play-dreidel.

DESIGN.pdf : This file describes the purpose of the program and communicates the overall design of the program.

WRITEUP.pdf : This file contains a discussion of the results for several questions regarding the play game command. It analyzes data derived from the command and uses graphical representations.

## Building

Run ```make``` to build the play-dreidel.c program.  
Run ```make clean``` to remove any compiler-generated files.  

## Running

```./play-dreidel [-flag]```  

note: a flag is not required in order to run the command  
  
flag options:  
	```-p n_players``` to input a value for how many players should start the game (a value between 2 and 8)  
	```-c n_coins``` to input a value for how many coins each player should start the game with (a value between 1 and 20)  
	```-s seed``` to input a random number seed (a nonzero value less no more than 10 decimal digits long)  
	```-v``` to print a message when a player is eliminated  
