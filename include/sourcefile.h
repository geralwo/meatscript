#ifndef SOURECFILE_H
#define SOURECFILE_H

typedef struct
{
    char *file_name;
    char *source_code;
    size_t file_size;
} SourceFile;

SourceFile *read_file(const char *filepath);

#endif // SOURECFILE_H
