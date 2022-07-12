
# Compiler
CC = gcc

# Flags
CFLAGS = -ggdb3 -Wall -Wextra -Wshadow -std=gnu11 -Wno-unused-parameter -Iinclude
SDL2_FLAGS = -I/usr/local/include/SDL2 -I/usr/local/lib -lSDL2

# Directories
BIN_DIR = bin
INCLUDE_DIR = include
SRC_DIR = src
TESTS_DIR = tests
EXAMPLES_DIR = examples
TEST_FRAMEWORK_DIR = unity

# Test directories
MATRIX_TESTS_DIR = $(TESTS_DIR)/matrix
RASTER_TESTS_DIR = $(TESTS_DIR)/raster
CLIP_TESTS_DIR = $(TESTS_DIR)/clip


# Targets
# first grab all files in examples.  then sub suffix with nothing, sub prefix with bin/
EXAMPLES = $(patsubst $(EXAMPLES_DIR)/%, $(BIN_DIR)/%, \
$(patsubst %.c, %, $(wildcard $(EXAMPLES_DIR)/*.c)))

MATRIX_TESTS = $(patsubst $(MATRIX_TESTS_DIR)/%, $(BIN_DIR)/%, \
$(patsubst %.c, %, $(wildcard $(MATRIX_TESTS_DIR)/*.c)))
RASTER_TESTS  = $(patsubst $(RASTER_TESTS_DIR)/%, $(BIN_DIR)/%, \
$(patsubst %.c, %, $(wildcard $(RASTER_TESTS_DIR)/*.c)))
CLIP_TESTS  = $(patsubst $(CLIP_TESTS_DIR)/%, $(BIN_DIR)/%, \
$(patsubst %.c, %, $(wildcard $(CLIP_TESTS_DIR)/*.c)))

TESTS = $(MATRIX_TESTS) $(RASTER_TESTS) $(CLIP_TESTS)

all: $(TESTS)

# grab the pattern for the file
$(EXAMPLES): $(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c $(BIN_DIR)/sr_matrix.o
	mkdir -p $(@D) 
	$(CC) $(CFLAGS) $^ -o $@ $(SDL2_FLAGS)


$(MATRIX_TESTS): $(BIN_DIR)/%: $(MATRIX_TESTS_DIR)/%.c \
$(BIN_DIR)/sr_matrix.o $(BIN_DIR)/unity.o
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR)

$(RASTER_TESTS): $(BIN_DIR)/%: $(RASTER_TESTS_DIR)/%.c \
$(BIN_DIR)/sr_raster.o $(BIN_DIR)/unity.o
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR) -lm

$(CLIP_TESTS): $(BIN_DIR)/%: $(CLIP_TESTS_DIR)/%.c \
$(BIN_DIR)/sr_clip.o $(BIN_DIR)/unity.o
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR)


$(BIN_DIR)/sr_matrix.o: $(SRC_DIR)/sr_matrix.c $(SRC_DIR)/sr_matrix.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# requires -lm flag for linux systems to direct gcc to where linux stores math.h
$(BIN_DIR)/sr_raster.o: $(SRC_DIR)/sr_raster.c $(SRC_DIR)/sr_raster.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -lm 

$(BIN_DIR)/sr_clip.o: $(SRC_DIR)/sr_clip.c $(SRC_DIR)/sr_clip.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


# test framework
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
