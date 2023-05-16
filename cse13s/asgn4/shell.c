# include <stdio.h>

# include "stats.h"
# include "shell.h"

uint32_t next_gap(uint32_t gap) {
	if (gap <= 0) {
		return 0;
	}
	if (gap <= 2) {
		return (gap - 1);
	}
	return ((5 * gap) / 11);
}

uint32_t temp;

void shell_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
	for (uint32_t gap = next_gap(n_elements); gap > 0; gap = next_gap (gap)) {
		for (uint32_t i = gap; i < n_elements; i++) {
			uint32_t j = i;
			uint32_t temp = arr[i];
			move(stats, temp);
			while (j >= gap && (cmp(stats, temp, arr[j-gap]) == -1)) {
				arr[j] = arr[j-gap];
				move(stats, arr[j]);
				move(stats, arr[j-gap]);
				j -= gap;
			}
			arr[j] = temp;
			move(stats, arr[j]);
			move(stats, temp);
		}
	}
}
