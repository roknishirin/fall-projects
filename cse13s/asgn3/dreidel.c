# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>	 			// for boolean usage

# include "mtrand.h"
# include "dreidel.h"


char * players[] = {    			// putting players into string
        "Aharon",
        "Batsheva",
        "Chanah",
        "David",
        "Ephraim",
        "Faige",
        "Gamaliel",
        "Hannah",
};

bool elim=false;


// Returns one of the four letters NGHS (randomly)
char spin_dreidel(void) {
	int value = (mtrand_rand64() % 4); 	// passing in seed   
	switch (value) {
		case 0:
			return 'G';
			break;
		case 1:
			return 'H';
			break;
		case 2:
			return 'N';
			break;
		case 3:
			return 'S';
			break;
		default:
			return '0';

	}
}


// a function that counts the number of non-negative elements in the array
int player_count( int *player_coins, int player_length) {
	int counter = 0;
	for (int j=0; j < player_length; j++) {
		if (player_coins[j] > -1) {
			counter += 1;
		}
	}
	return counter; 
}


// a function that carries out the commands of each dreidel side 
void stop_dreidel(char spins, int *player_coins, int *pot, int i, int *n_rounds, int n_players) {
	switch (spins) {
		case 'G':
			*player_coins += *pot;
			*pot -= *pot;
			break;
		case 'H':
			*player_coins += (*pot / 2);
			*pot -= (*pot/2);
			break;
		case 'N':
			break;
		case 'S':
			if (*player_coins > 0) {
				*player_coins -= 1;
				*pot +=1; 
			}
			else {
				*player_coins -= 1;

				// printing out elimination statement when player has -1 coins 
				if (elim) {
					printf("%s: eliminated in round %d of a %d player game.\n", players[i], *n_rounds, n_players);
                                }	
			}
			break;
		}
	
}


// actually playing a single dreidel game and returning the number of the winning player 
int play_game (int n_players , int coins_per_player , int * n_rounds ) {
	
	int player_coins [n_players];
	
	// adding default coins to empty array
	for (int i=0; i<n_players; i++) {
		player_coins[i] = coins_per_player;
	}

	int pot = 0;
	
	// while loop as long as there is more than one person in counter
	while (player_count(player_coins, n_players) > 1) {
		
		//iterating through all of the players	
		for (int i=0; i<n_players; i++) {
			if (player_coins[i] >  -1) {				
				stop_dreidel(spin_dreidel(), player_coins+i, &pot, i, n_rounds, n_players);
			}
		}
		// counts the rounds
		*n_rounds += 1;
	}

	for (int i=0; i<n_players; i++) {
		if (player_coins[i] > 0) {
			// return number of players who won
			return i;
			
		}
	}
	return 0;
}
