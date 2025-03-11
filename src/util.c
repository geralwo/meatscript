#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <inttypes.h> // For PRIu64 format specifier
#include <string.h>
#include "sourcefile.h"
#include "util.h"

// Converts string to uint8_t
uint8_t str_to_uint8(const char *str)
{
	char *endptr;
	errno = 0;
	unsigned long value = strtoul(str, &endptr, 10);

	if (errno == ERANGE || value > UINT8_MAX)
	{
		fprintf(stderr, "Error: Value '%s' out of range for uint8_t\n", str);
		exit(1);
	}
	if (*endptr != '\0')
	{
		fprintf(stderr, "Error: Invalid characters in input string '%s'\n", str);
		exit(1);
	}
	return (uint8_t)value;
}

// Converts string to uint16_t
uint16_t str_to_uint16(const char *str)
{
	char *endptr;
	errno = 0;
	unsigned long value = strtoul(str, &endptr, 10);

	if (errno == ERANGE || value > UINT16_MAX)
	{
		fprintf(stderr, "Error: Value '%s' out of range for uint16_t\n", str);
		exit(1);
	}
	if (*endptr != '\0')
	{
		fprintf(stderr, "Error: Invalid characters in input string '%s'\n", str);
		exit(1);
	}
	return (uint16_t)value;
}

// Converts string to uint32_t
uint32_t str_to_uint32(const char *str)
{
	char *endptr;
	errno = 0;
	unsigned long value = strtoul(str, &endptr, 10);

	if (errno == ERANGE || value > UINT32_MAX)
	{
		fprintf(stderr, "Error: Value '%s' out of range for uint32_t\n", str);
		exit(1);
	}
	if (*endptr != '\0')
	{
		fprintf(stderr, "Error: Invalid characters in input string '%s'\n", str);
		exit(1);
	}
	return (uint32_t)value;
}

// Converts string to uint64_t
uint64_t str_to_uint64(const char *str)
{
	char *endptr;
	errno = 0;
	unsigned long value = strtoul(str, &endptr, 10);

	if (errno == ERANGE || value > UINT64_MAX)
	{
		fprintf(stderr, "Error: Value '%s' out of range for uint64_t\n", str);
		exit(1);
	}
	if (*endptr != '\0')
	{
		fprintf(stderr, "Error: Invalid characters in input string '%s'\n", str);
		exit(1);
	}
	return (uint64_t)value;
}

size_t str_to_size_t(const char *str)
{
	char *endptr;
	errno = 0;
	unsigned long value = strtoul(str, &endptr, 10);

	if (errno == ERANGE || value > SIZE_MAX)
	{
		fprintf(stderr, "Error: Value '%s' out of range for uint64_t\n", str);
		exit(1);
	}
	if (*endptr != '\0')
	{
		fprintf(stderr, "Error: Invalid characters in input string '%s'\n", str);
		exit(1);
	}
	return (size_t)value;
}

void print_bits(uint64_t num)
{
	for (uint64_t bit = 0; bit < (sizeof(uint64_t) * 8); bit++)
	{
		printf("%ld", num & 0x01);
		if ((bit + 1) % 8 == 0)
			printf(" ");
		if ((bit + 1) % 16 == 0)
			printf(" ");
		num = num >> 1;
	}
}

SourceFile *read_file(const char *filepath)
{
	FILE *file = fopen(filepath, "r");
	if (!file)
	{
		perror("Error opening file");
		return NULL;
	}

	// Seek to the end to determine the file size
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	rewind(file); // Go back to the start of the file

	// Allocate memory for the SourceFile struct
	SourceFile *source_file = (SourceFile *)malloc(sizeof(SourceFile));
	if (!source_file)
	{
		perror("Memory allocation failed");
		fclose(file);
		return NULL;
	}

	// Allocate memory for the file contents + null terminator
	source_file->source_code = (char *)malloc(file_size + 1);
	if (!source_file->source_code)
	{
		perror("Memory allocation failed");
		free(source_file);
		fclose(file);
		return NULL;
	}

	// Read the file contents into the buffer
	size_t bytes_read = fread(source_file->source_code, 1, file_size, file);
	source_file->source_code[bytes_read] = '\0'; // Null-terminate the string

	source_file->file_name = (char *)filepath;
	source_file->file_size = file_size;

	fclose(file);
	return source_file;
}

char *copy_string(const char *str)
{
	if (!str)
		return NULL;
	size_t len = strlen(str) + 1; // Include space for the null terminator
	char *copy = malloc(len);
	if (copy)
	{
		memset(copy, 0, len);
		memcpy(copy, str, len);
	}
	// printf("copied string %p to %p\n", (void *)str, (void *)copy);
	return copy;
}

char *cut_string(const char *str, size_t position, size_t length)
{
	size_t str_length = strlen(str);

	// Ensure the position is within bounds
	if (position >= str_length)
	{
		return ""; // Return empty string if position is out of bounds
	}

	// Adjust length if it exceeds the remaining characters
	if (position + length > str_length)
	{
		length = str_length - position;
	}

	// Allocate memory for new substring (+1 for null terminator)
	char *new_str = (char *)malloc(length + 1);
	if (!new_str)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	strncpy(new_str, str + position, length);
	new_str[length] = '\0'; // Null-terminate the new string

	return new_str;
}

int list_has_string(const char *str, char *list[])
{
	for (size_t i = 0; list[i] != NULL; i++)
	{
		if (strcmp(str, list[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}

char *read_line(char terminator)
{
	size_t capacity = 8;
	size_t length = 0;
	char *line = malloc(capacity);
	if (!line)
		return NULL;

	int ch;
	while ((ch = getchar()) != terminator && ch != EOF)
	{
		if (length + 1 >= capacity)
		{ // Expand buffer if needed
			capacity *= 2;
			char *new_line = realloc(line, capacity);
			if (!new_line)
			{
				free(line);
				return NULL;
			}
			line = new_line;
		}
		line[length++] = (char)ch;
	}
	line[length] = '\0'; // Null-terminate the string

	if (length == 0 && ch == EOF)
	{ // Handle EOF before input
		free(line);
		return NULL;
	}

	return line;
}

void print_bytes(uint8_t *data, size_t size)
{
	if (size > 0)
	{
		// printf("Bytecode bytes:\n");
		for (size_t i = 0; i < size; i++)
		{
			printf(" %02x ", data[i]);
			if ((i + 1) % 16 == 0)
			{
				printf("\n");
			}
		}
		printf("\n");
	}
}