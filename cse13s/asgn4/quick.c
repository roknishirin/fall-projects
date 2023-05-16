# include <stdio.h>

# include "stats.h"
# include "quick.h"
# include "shell.h"

uint32_t SMALL=8; 
void quick_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
	
	// when there are less than 8 elements in the array,let shell sort do sorting	
	if (n_elements < SMALL) {
		shell_sort(stats, arr, n_elements);
		return;
	}

	uint32_t left=0, right=n_elements -1;	

	uint32_t pivot = (arr[left] + arr[n_elements/2] + arr[right])/3;

	while (left < right) {
		
		// moving the left pointer 
		while (arr[left] <= pivot && left < n_elements) {
			left += 1;
		}

		// if the left pointer reaches the end (sorted completely), terminate
		if (left == n_elements) {
			return;
		}
		
		// moving the right pointer 
		while (arr[right] > pivot && right > left) {
			right -= 1;
		}

		// swap
		if (left < right) {
			swap(stats, &arr[left], &arr[right]);
		}
	}
	// left
	quick_sort(stats, arr, left); 
	
	// right
	quick_sort(stats, arr + left, n_elements - left); 
}
