# Compiler and flags
CC = gcc
# CFLAGS = -std=c17 -Wall -Wextra -Werror -g -pedantic -Iinclude -O0
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic -Iinclude -O2
# -Wstrict-prototypes -Wmissing-prototypes
# Source and output
SRC = src/main.c src/util.c src/parser.c src/asm_parser.c src/lexer.c src/stdlib/*.c src/vm/*.c src/vm/instructions/*.c src/vm/instructions/math/*.c
OUTPUT = build/meatsc

# Build target
all: $(OUTPUT)

# Ensure the build directory exists
build:
	mkdir -p build

# Linking and compilation
$(OUTPUT): build $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

# Run target
run: $(OUTPUT)
	./$(OUTPUT) tests/asm_test.meats

# Clean target
clean:
	rm build/*

