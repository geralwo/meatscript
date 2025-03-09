#include "stdlib/std_print.h"
#include <stdarg.h>

void meats_print(const char *format, ...)
{
	va_list args;
	va_start(args, format);

#ifdef _WIN32
	// Windows may need specific handling, but generally works with vprintf
	vprintf(format, args);
#else
	// On Linux/macOS, just use vprintf directly
	vprintf(format, args);
#endif

	va_end(args);
}