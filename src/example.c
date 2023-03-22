#include <stdio.h>

#include <stdlib.h>

#define DYNARRAY_IMPLEMENTATION
#include "dynarray.h"

int main(void) {
	
	size_t *arr = dynarray_new(size_t, 10);
	if (!arr) return 0;

	printf("ALLOCATED:   %zu\n", DYNARRAY_ALLOCATED(arr));
	printf("ITEM SIZE: %zu\n", DYNARRAY_ITEM_SIZE(arr));
	printf("arr[0]:    %zu\n", arr[0]); // Uninitialized.
	arr[0] = 69;
	printf("arr[0]:    %zu\n", arr[0]);
	printf("ALLOCATED:   %zu\n", DYNARRAY_ALLOCATED(arr));
	printf("ITEM SIZE: %zu\n", DYNARRAY_ITEM_SIZE(arr));

	dynarray_free(arr);
	return 0;
}
