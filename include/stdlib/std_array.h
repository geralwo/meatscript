#ifndef MEATLIB_ARRAY_H
#define MEATLIB_ARRAY_H
#include <stddef.h>

typedef struct s_Array
{
	size_t count;
	size_t capacity;
	size_t size_of_element;
	void *data;
} Array;

void array_init(Array *arr, size_t size_of_element, size_t start_capacity);
void array_free(Array *arr);
void array_add(Array *arr, const void *element);
void *array_get(Array *arr, size_t index);
void array_remove_at(Array *arr, size_t index);
void array_clear(Array *arr);
#endif // MEATLIB_ARRAY_H