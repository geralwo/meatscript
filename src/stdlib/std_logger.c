#include "stdlib/std_logger.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

Logger GlobalLogger = {0, NULL}; // Initialize the global logger with 0 messages and NULL log

// Function to initialize the logger
void meat_logger_init(Logger *logger)
{
	if (logger == NULL)
	{
		return;
	}

	logger->msg_count = 0;
	logger->msg_log = malloc(1); // Allocate memory for the log (initially empty)
	if (logger->msg_log == NULL)
	{
		// Handle memory allocation failure
		fprintf(stderr, "Failed to allocate memory for logger\n");
		exit(1);
	}
	logger->msg_log[0] = '\0'; // Set the initial log as an empty string
}

void meat_log(const char *format, ...)
{
	// First check if the GlobalLogger is initialized
	if (GlobalLogger.msg_log == NULL)
	{
		meat_logger_init(&GlobalLogger);
	}

	// Prepare the formatted string using va_list
	va_list args;
	va_start(args, format);

	// Calculate the length of the formatted message, including space for the newline
	size_t current_len = strlen(GlobalLogger.msg_log);
	size_t formatted_len = vsnprintf(NULL, 0, format, args); // Length of the formatted message
	size_t new_size = current_len + formatted_len + 2;	 // +2 for the newline and null terminator

	// Reallocate memory to fit the new log message
	char *new_log = realloc(GlobalLogger.msg_log, new_size);
	if (new_log == NULL)
	{
		// Handle memory allocation failure
		fprintf(stderr, "Failed to allocate memory for log expansion\n");
		exit(1);
	}

	// Format and append the new message to the log
	vsprintf(new_log + current_len, format, args);
	new_log[current_len + formatted_len] = '\n';	 // Add newline at the end of the message
	new_log[current_len + formatted_len + 1] = '\0'; // Null-terminate the string
	GlobalLogger.msg_log = new_log;
	GlobalLogger.msg_count++;

	// Clean up the va_list
	va_end(args);
}

// Optionally, to print the log (useful for debugging or in some use cases)
void print_log()
{
	if (GlobalLogger.msg_log != NULL && GlobalLogger.msg_count > 0)
	{
		printf("Log (%zu messages):\n%s\n", GlobalLogger.msg_count, GlobalLogger.msg_log);
	}
	else
	{
		printf("Log is empty or uninitialized.\n");
	}
}

// Optional: Cleanup function for releasing resources used by the logger
void logger_cleanup()
{
	if (GlobalLogger.msg_log != NULL)
	{
		free(GlobalLogger.msg_log);
		GlobalLogger.msg_log = NULL;
	}
}
