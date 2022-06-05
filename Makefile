
# Compiler
CC = gcc

# Flags
CFLAGS = -ggdb3 -Wall -Wextra -Wshadow -std=gnu11 -Wno-unused-parameter
SDL2_FLAGS = -I/usr/local/include/SDL2 -I/usr/local/lib -lSDL2

# Directories
BIN_DIR = bin
SRC_DIR = src
TESTS_DIR = tests
EXAMPLES_DIR = examples
TEST_FRAMEWORK_DIR = test-framework

# Targets
# first grab all files in examples.  then sub suffix with nothing, sub prefix with bin/
EXAMPLES = $(patsubst $(EXAMPLES_DIR)/%, $(BIN_DIR)/%, \
$(patsubst %.c, %, $(wildcard $(EXAMPLES_DIR)/*.c)))
MATRIX_TESTS = $(patsubst $(TESTS_DIR)/%, $(BIN_DIR)/%, \
$(patsubst %.c, %, $(wildcard $(TESTS_DIR)/check_matrix_*.c)))
PIPELINE_TESTS = $(patsubst $(TESTS_DIR)/%, $(BIN_DIR)/%, \
$(patsubst %.c, %, $(wildcard $(TESTS_DIR)/check_pipeline_*.c)))

all: $(EXAMPLES) $(MATRIX_TESTS) $(PIPELINE_TESTS) \
$(BIN_DIR)/unity.o $(BIN_DIR)/matrix.o

# grab the pattern for the file
$(EXAMPLES): $(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c $(BIN_DIR)/matrix.o
	mkdir -p $(@D) 
	$(CC) $(CFLAGS) $^ -o $@ $(SDL2_FLAGS)

$(MATRIX_TESTS): $(BIN_DIR)/%: $(TESTS_DIR)/%.c \
$(BIN_DIR)/matrix.o $(BIN_DIR)/unity.o
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR)

$(PIPELINE_TESTS): $(BIN_DIR)/%: $(TESTS_DIR)/%.c \
$(BIN_DIR)/pipeline.o $(BIN_DIR)/unity.o
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR) -lm

$(BIN_DIR)/matrix.o: $(SRC_DIR)/matrix.c $(SRC_DIR)/matrix.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/pipeline.o: $(SRC_DIR)/pipeline.c $(SRC_DIR)/pipeline.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -lm

$(BIN_DIR)/unity.o: $(TEST_FRAMEWORK_DIR)/unity.c \
$(TEST_FRAMEWORK_DIR)/unity.h $(TEST_FRAMEWORK_DIR)/unity_internals.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

run-%:
	$$($(patsubst run-%, $(BIN_DIR)/%, $(@)))

check-matrix:
	for t in $(wildcard $(BIN_DIR)/check_matrix*); do $$t; done

check-all:
	for t in $(wildcard $(BIN_DIR)/check*); do $$t; done

memcheck-%:
	valgrind $(patsubst mem-check-%, $(BIN_DIR)/%, $(@))

memcheck-all:
	for t in $(wildcard $(BIN_DIR)/check*); do valgrind $$t; done
	
clean:
	rm -r $(BIN_DIR)
