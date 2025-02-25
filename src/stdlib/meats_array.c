#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "meats.h"

MeatsArray *meats_array_new(size_t size_of_element)
{
	return meats_array_create(size_of_element, 16);
}
MeatsArray *meats_array_create(size_t size_of_element, size_t initial_capacity)
{
	// Allocate memory for the MeatsArray struct
	MeatsArray *array = (MeatsArray *)malloc(sizeof(MeatsArray));
	if (!array)
	{
		perror("Failed to allocate memory for MeatsArray");
		exit(EXIT_FAILURE);
	}

	// Allocate memory for the elements array
	array->elements = malloc(size_of_element * initial_capacity);
	if (!array->elements)
	{
		perror("Failed to allocate memory for MeatsArray elements");
		exit(EXIT_FAILURE);
	}

	// Initialize the array properties
	array->Count = 0;
	array->capacity = initial_capacity;
	array->size_of_element = size_of_element;
	return array;
}

void meats_array_add(MeatsArray *array, void *element)
{
	if (array->Count >= array->capacity)
	{
		array->capacity *= 2;
		array->elements = realloc(array->elements, array->size_of_element * array->capacity);

		if (!array->elements)
		{
			fprintf(stderr, "Memory reallocation failed\n");
			exit(1);
		}
	}

	// Copy the element into the array
	void *dest = (char *)array->elements + (array->Count * array->size_of_element);
	memcpy(dest, element, array->size_of_element);
	array->Count++;
}

void meats_array_remove(MeatsArray *array, void *element)
{
	TODO("implement remove of element by data or index");
	UNUSED(array);
	UNUSED(element);
}

void *meats_array_get(MeatsArray *array, size_t index)
{
	if (index >= array->Count)
	{
		return NULL; // Return NULL for out-of-bounds access
	}
	return (char *)array->elements + (index * array->size_of_element);
}

void meats_array_free(MeatsArray *array)
{

	if (!array)
		return; // Avoid freeing NULL pointer
	printf("MeatsArray :: freeing arr->elements %p\n", (void *)array->elements);
	free(array->elements);
	array->elements = NULL; // Prevent dangling pointer
	array->Count = 0;
	array->capacity = 0;
	array->size_of_element = 0;
	printf("MeatsArray :: freeing arr %p\n", (void *)array);
	free(array);
}

void meats_array_foreach(MeatsArray *array, void (*func)(void *))
{
	if (!array || !func)
		return;

	for (size_t i = 0; i < array->Count; i++)
	{
		void *element = (char *)array->elements + i * array->size_of_element;
		func(element);
	}
}
