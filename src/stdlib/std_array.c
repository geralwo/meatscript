#include "stdlib/std_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void array_init(Array *arr, size_t size_of_element, size_t start_capacity)
{
	arr->count = 0;
	arr->size_of_element = size_of_element;
	arr->capacity = start_capacity;
	arr->data = malloc(size_of_element * start_capacity);
	memset(arr->data, 0, (size_of_element * start_capacity));
}
void array_free(Array *arr)
{
	if (arr && arr->data)
	{
		free(arr->data);
		arr->data = NULL;
	}
	arr->count = 0;
	arr->size_of_element = 0;
	arr->capacity = 0;
}
void array_add(Array *arr, const void *element)
{
	if (arr->capacity == 0)
		return;
	// Check if we need to grow the array
	if (arr->count >= arr->capacity)
	{
		arr->capacity = arr->capacity * 2;
		arr->data = realloc(arr->data, arr->capacity * arr->size_of_element);
		if (!arr->data)
		{
			perror("Failed to reallocate memory");
			exit(1);
		}
	}

	// Copy the new element into the array
	memcpy((char *)arr->data + (arr->count * arr->size_of_element), element, arr->size_of_element);

	// Increment count
	arr->count++;
}
void *array_get(Array *arr, size_t index)
{
	if (index >= arr->count)
	{
		printf("ERROR: Index out of bounds Violation!\n");
		exit(1);
	}
	return (char *)arr->data + (index * arr->size_of_element);
}

void array_clear(Array *arr)
{
	memset(arr->data, 0, (arr->count * arr->size_of_element));
	arr->count = 0;
}