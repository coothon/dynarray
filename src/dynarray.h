/*
** An STB-style header: define DYNARRAY_IMPLEMENTATION in one translation unit,
** and include this where needed.
**     A small dynamic array library that supports arrays of any type and size.
** The size of the array and the size of each element is stored before the
** returned pointer. I consider this an improvement over using a struct or
** separately tracking sizes because it is used just like a static array. Also
** includes some convenience functions.
**     Use a lowercase macro over the function if there is one; they are the
** intended interface.
**
** MUST be freed with dynarray_free()!
*/

#ifndef DYNARRAY_H_
#define DYNARRAY_H_

#include <stdlib.h>
#include <string.h>

enum dynarray_offsets {
	DYNARRAY_ALLOCATED_OFFSET = 1,
	DYNARRAY_ITEM_SIZE_OFFSET,
	DYNARRAY_TRUE_START_OFFSET = DYNARRAY_ITEM_SIZE_OFFSET,
};

#define dynarray_new(type, amount) ((type *)_dynarray_new(sizeof(type), amount))
#define dynarray_free(dynarray) free((void *)(((size_t *)dynarray) - DYNARRAY_TRUE_START_OFFSET))
#define dynarray_zero(dynarray) dynarray_set(dynarray, 0);

#define DYNARRAY_ALLOCATED(dynarray) (*(((size_t *)dynarray) - DYNARRAY_ALLOCATED_OFFSET))
#define DYNARRAY_ITEM_SIZE(dynarray) (*(((size_t *)dynarray) - DYNARRAY_ITEM_SIZE_OFFSET))

#define DYNARRAY_TRUE_START(dynarray) (((size_t *)dynarray) - DYNARRAY_TRUE_START_OFFSET)

// Helper function. Use the macro.
void *_dynarray_new(size_t item_size, size_t min_alloc);

// Provide the array and amount of items needed. Returns NULL on failure.
void *dynarray_resize(void *dynarray, size_t new_size);

// Sets all elements to c. Only supports ints, unfortunately.
void dynarray_set(void *dynarray, int c);

#endif // DYNARRAY_H_

// IMPLEMENTATION

#ifdef DYNARRAY_IMPLEMENTATION

void *_dynarray_new(size_t item_size, size_t min_alloc) {
	// Store the size of each item and the amount of them allocated both before dynarray[0].
	size_t total_size = (DYNARRAY_TRUE_START_OFFSET * sizeof(size_t)) + (item_size * min_alloc);

	void *dynarray = malloc(total_size);
	if (!dynarray)
		return NULL;

	// We only want to act on the start of the data, so set the pointer there.
	dynarray = (void *)(((size_t *)dynarray) + DYNARRAY_TRUE_START_OFFSET);

	// Amount of items allocated.
	DYNARRAY_ALLOCATED(dynarray) = min_alloc;

	// Item size.
	DYNARRAY_ITEM_SIZE(dynarray) = item_size;

	return dynarray;
}

void *dynarray_resize(void *dynarray, size_t new_amount) {
	if (!dynarray)
		return NULL;

	void *new_address = realloc(DYNARRAY_TRUE_START(dynarray), (DYNARRAY_TRUE_START_OFFSET * sizeof(size_t)) +
																   (DYNARRAY_ITEM_SIZE(dynarray) * new_amount));
	if (!new_address)
		return NULL;

	// Set back to user's data.
	new_address = (void *)(((size_t *)new_address) + DYNARRAY_TRUE_START_OFFSET);

	DYNARRAY_ALLOCATED(new_address) = new_amount;

	return new_address;
}

void dynarray_set(void *dynarray, int c) {
	if (!dynarray)
		return;

	size_t amount = DYNARRAY_ALLOCATED(dynarray);
	size_t size = DYNARRAY_ITEM_SIZE(dynarray);

	memset(dynarray, c, amount * size);
}

#undef DYNARRAY_IMPLEMENTATION
#endif // DYNARRAY_IMPLEMENTATION
