# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror=return-type -g -std=c11 -pedantic -Iinclude

# Source and output
SRC = src/meats.c src/util.c src/asm_parser.c src/lexer.c src/stdlib/*.c src/vm/*.c src/vm/instructions/*.c
OUTPUT = build/meats

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

