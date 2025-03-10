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

# Directories
TEST_DIR = tests
# Test all
test: $(OUTPUT)
	@echo "Running all tests..."
	@for dir in $(shell find $(TEST_DIR) -mindepth 1 -type d); do \
		echo "Running tests in $$dir..."; \
		for test_file in $$dir/*.meats; do \
			if [ -f "$$test_file" ]; then \
				echo "Testing: $$test_file"; \
				./$(OUTPUT) "$$test_file" || exit 1; \
			fi; \
		done; \
	done
	@echo "All tests completed."

# Test specific directory
test-%: $(OUTPUT)
	@echo "Running tests in $(TEST_DIR)/$*..."
	@if [ -d "$(TEST_DIR)/$*" ]; then \
		for test_file in $(TEST_DIR)/$*/*.meats; do \
			if [ -f "$$test_file" ]; then \
				echo "Testing: $$test_file"; \
				./$(OUTPUT) "$$test_file" || exit 1; \
			fi; \
		done; \
	else \
		echo "Error: Test directory '$(TEST_DIR)/$*' does not exist!"; \
		exit 1; \
	fi
	@echo "Tests in $(TEST_DIR)/$* completed."