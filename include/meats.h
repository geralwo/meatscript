#ifndef MEATS_H
#define MEATS_H
#include <stddef.h>

typedef struct
{
	size_t Count;
	size_t capacity;
	size_t size_of_element;
	void *elements;
} MeatsArray;

MeatsArray *meats_array_new(size_t size_of_element);
MeatsArray *meats_array_create(size_t size_of_element, size_t initial_capacity);
void meats_array_free(MeatsArray *array);

void meats_array_add(MeatsArray *array, void *element);
void *meats_array_get(MeatsArray *array, size_t index);
void meats_array_remove(MeatsArray *array, void *element);
void meats_array_remove_at(MeatsArray *array, size_t index);
void meats_array_clear(MeatsArray *array);
void meats_array_foreach(MeatsArray *array, void (*func)(void *));

#endif // MEATS_H
