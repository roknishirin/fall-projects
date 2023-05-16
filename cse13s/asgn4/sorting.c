# include <unistd.h>
# include <stdio.h>
# include <inttypes.h> 
# include <stdint.h>
# include <stdlib.h>

# include "bubble.h"
# include "heap.h"
# include "mtrand.h"
# include "quick.h"
# include "set.h"
# include "shell.h"
# include "stats.h"

# define OPTIONS "asbqhr:n:p:H"



void prints (uint32_t printing, uint32_t *space) {
	for (uint32_t i = 0; i < printing; i ++) {
		printf("%13" PRIu32, space[i]);

	       	// print extra line if not at the end of the code and five columns have been created 	
		if ((i + 1) % 5 == 0 && i != printing -1) {
			printf("\n");
		}
	}
	// if p isn't 0, print an extra line character 
	if (printing != 0) {
		printf("\n");
	}
}


void seeding (uint32_t seed, uint32_t *space, uint32_t size) {
	// seeding 
	mtrand_seed(seed);
	for (uint32_t i=0; i<size; i++) {
		
		// bit masking
		space[i] = (uint32_t) (mtrand_rand64() & (uint32_t) ((1 <<30) -1));
	}
}


int main (int argc, char  **argv) {
	Stats* arr = (Stats *) calloc(1, sizeof(Stats)); 
	char *ptr;
	int opt;
	uint32_t seed = 13371453, size = 100, elements = 100, *space = NULL, printing;
	Set x = set_empty();
	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
	switch (opt) {
		case 'a':
			x = set_insert(x, 1);
			x = set_insert(x, 2);
			x = set_insert(x, 3);
			x = set_insert(x, 4);
			break;
		case 'b':
			x = set_insert(x, 1);
			break;
		case 'h':
			x = set_insert(x, 2);
			break; 
		case 'q':
			x = set_insert(x, 3);
			break;
		case 's':
			x = set_insert(x, 4);
			break;
		case 'r':
			seed = (uint32_t) strtoul(optarg, &ptr, 10);
			break;
		case 'n':
			size = (uint32_t) strtoul(optarg, &ptr, 10);
			if (size < 1 || size > 250000000) {
				return 1;
			}
			break;
		case 'p':
			elements = (uint32_t) strtoul(optarg, &ptr, 10);
			break;
		case 'H':
			printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\n");
                	printf("USAGE\n   ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]");
                	printf(
                        "\n\nOPTIONS\n"
                        "   -H              Display program help and usage.\n"
                        "   -a              Enable all sorts.\n"
                        "   -b              Enable Bubble Sort.\n"
                        "   -h              Enable Heap Sort.\n"
                        "   -q              Enable Quick Sort.\n"
                        "   -s              Enable Shell Sort.\n"
                        "   -n length       Specify number of array elements (default: 100).\n"
                        "   -p elements     Specify number of elements to print (default: 100).\n"
                        "   -r seed         Specify random seed (default: 13371453).\n");
			free (space);
			return 0;
		
		default:
			// if user inputs incorrect flag, return user statement
			printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\n");
               		printf("USAGE\n   ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]");
			printf(
                        "\n\nOPTIONS\n"
                        "   -H              Display program help and usage.\n"
                        "   -a              Enable all sorts.\n"
                        "   -b              Enable Bubble Sort.\n"
                        "   -h              Enable Heap Sort.\n"
                        "   -q              Enable Quick Sort.\n"
                        "   -s              Enable Shell Sort.\n"
                        "   -n length       Specify number of array elements (default: 100).\n"
                        "   -p elements     Specify number of elements to print (default: 100).\n"
                        "   -r seed         Specify random seed (default: 13371453).\n");
			return 1;
		}
	}
	space = malloc(size * sizeof(uint32_t));
	if (space == NULL) {
		return 1;
	}
	
	// determining value of printing
	if (size < elements) {
		printing = size;
	} else {
		printing = elements;
	}

	if (set_member(x, 1)) {
		seeding(seed, space, size);		// seeding 
		reset(arr);				// resetting array
		bubble_sort(arr, space, size);		// call bubble sort 
		printf("Bubble Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares\n", size, arr->moves, arr->compares);
		prints(printing, space);		// printing sorted array elements
	}
	if (set_member(x, 2)) {
		reset(arr);				// seeding
		seeding(seed, space, size);		// resetting array
		heap_sort(arr, space, size);		// call bubble sort
		printf("Heap Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares\n", size, arr->moves, arr->compares);
		prints(printing, space);		// printing sorted array elements
	}
	if (set_member(x, 3)) {
		reset(arr);				// seeding
		seeding(seed, space, size);		// resetting array
		quick_sort(arr, space, size);		// call bubble sort
		printf("Quick Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares\n", size, arr->moves, arr->compares);
		prints(printing, space);		// printing sorted array elements
	}
	if (set_member(x, 4)) {
		reset(arr);				// seeding
		seeding(seed, space, size);		// resetting array
		shell_sort(arr, space, size);		// call bubble sort
		printf("Shell Sort, %" PRIu32 " elements, %" PRIu64 " moves, %" PRIu64 " compares\n", size, arr->moves, arr->compares);
		prints(printing, space);		// printing sorted array elements
	}

	// freeing allocated memory 
	free (space);
	return 0;
}


