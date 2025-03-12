#ifndef MEATLIB_LOGGER_H
#define MEATLIB_LOGGER_H
#include <stddef.h>
#include <stdarg.h>

typedef struct s_Logger
{
	size_t msg_count;
	char *msg_log;
} Logger;

extern Logger GlobalLogger;
void meat_logger_init(Logger *logger);
void meat_log(const char *format, ...);

#endif