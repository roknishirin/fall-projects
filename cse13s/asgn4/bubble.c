# include <stdio.h>
# include <stdbool.h>

# include "stats.h"
# include "bubble.h"


void bubble_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
	for (uint32_t i=0; i<n_elements -1; i++) {
		bool swapped = false;
		for (uint32_t j=n_elements - 1; j > i; j--) {
			if (cmp(stats, arr[j], arr[j-1]) == -1) {
				swap(stats, &arr[j], &arr[j-1]);
				swapped = true;
			} 
		}
		if (swapped == false) {
			break;
		}
	}
}
