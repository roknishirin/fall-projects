# include <stdio.h>
# include <stdbool.h> // for boolean
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

# include "dreidel.h" 
# include "mtrand.h" 

# define OPTIONS "vp:c:s:"


extern char * players[];
extern bool elim;

int main (int argc, char **argv) {
	int p = 4, c = 3, n_rounds=0;
	int opt = 0;
	uint64_t s=613;						// seed value 
	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		switch (opt) {
		case 'p': 					// if user types -p #
			p = atoi(optarg);			// make p = user input
			if (p < 2 || p > 8) {			// unless not in [2,8] range 
				return 1;
			} 
		       break;	
		case 'c':					// if user types -c #
		       c = atoi(optarg);			// make c = user inputer
			if (c < 1 || c > 20) {			// unless not in [1, 20] range
				return 1;
			}
			break;
		case 's':					// if user types -s #
			s = strtoul(optarg, NULL, 10);		// make s = user input
			if (s < 1 || s > 9999999999) {		// unless not nonzero or vry big
			return 1;
			} 
			break;
		case 'v':					// if user types -v
			elim = true;				// allows elimination statement 
			break;
		default:
			return 1;
		}
	}
	mtrand_seed(s);						// seeding

	// gets the number of the winner 
	int val = play_game(p, c, (&n_rounds)); 

	// print player name, players, coins, rounds, and seed
	printf("%s %d %d %d %lu\n", players[val], p, c, n_rounds, s);
	
	return 0;
}
