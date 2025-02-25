#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define TODO(...)                                      \
        do                                             \
        {                                              \
                fprintf(stdout, "%s:%d: TODO (%s): ",  \
                        __FILE__, __LINE__, __func__); \
                fprintf(stdout, __VA_ARGS__);          \
                fprintf(stdout, "\n");                 \
        } while (0)

#define UNIMPLEMENTED(...)                                     \
        do                                                     \
        {                                                      \
                fprintf(stdout, "%s:%d: UNINIMPLEMENTED: %s ", \
                        __FILE__, __LINE__, __func__);         \
                fprintf(stdout, __VA_ARGS__);                  \
                fprintf(stdout, "\n");                         \
                abort();                                       \
        } while (0)

#define UNUSED(x) (void)(x)

uint8_t str_to_uint8(const char *str);
uint16_t str_to_uint16(const char *str);
uint32_t str_to_uint32(const char *str);
uint64_t str_to_uint64(const char *str);
void print_bits(uint64_t num);

char *read_line(char terminator);
char *copy_string(const char *str);
char *cut_string(const char *str, size_t position, size_t length);
int list_has_string(const char *str, char *list[]);
