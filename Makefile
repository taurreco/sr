
# Compiler
CC = gcc

# Flags
CFLAGS +=-ggdb3 
CFLAGS += -Wall 
CFLAGS += -Wextra 
CFLAGS += -Wshadow 
CFLAGS += -std=gnu11 
CFLAGS += -Wno-unused-parameter 
CFLAGS += -Iinclude

SDL2_FLAGS += -I/usr/local/include/SDL2 
SDL2_FLAGS += -I/usr/local/lib 
SDL2_FLAGS += -lSDL2

# Directories
BIN_DIR = bin
INCLUDE_DIR = include
SRC_DIR = src
TESTS_DIR = tests
EXAMPLES_DIR = examples
TEST_FRAMEWORK_DIR = unity

# Test directories
MATRIX_TESTS_DIR = $(TESTS_DIR)/matrix
RENDER_TESTS_DIR = $(TESTS_DIR)/render
RASTER_TESTS_DIR = $(TESTS_DIR)/raster
CLIP_TESTS_DIR = $(TESTS_DIR)/clip
OBJ_TESTS_DIR = $(TESTS_DIR)/obj

# Targets
EXAMPLES = $(patsubst $(EXAMPLES_DIR)/%, $(BIN_DIR)/%, \
$(patsubst %.c, %, $(wildcard $(EXAMPLES_DIR)/*.c)))

MATRIX_TESTS = $(patsubst %.c, %, $(wildcard $(MATRIX_TESTS_DIR)/*.c))
RASTER_TESTS  = $(patsubst %.c, %, $(wildcard $(RASTER_TESTS_DIR)/*.c))
RENDER_TESTS  = $(patsubst %.c, %, $(wildcard $(RENDER_TESTS_DIR)/*.c))
CLIP_TESTS  = $(patsubst %.c, %, $(wildcard $(CLIP_TESTS_DIR)/*.c))
OBJ_TESTS = $(patsubst %.c, %, $(wildcard $(OBJ_TESTS_DIR)/*.c))

TESTS += $(MATRIX_TESTS) 
TESTS += $(RASTER_TESTS) 
TESTS += $(RENDER_TESTS) 
TESTS += $(CLIP_TESTS) 
TESTS += $(OBJ_TESTS)

all: $(TESTS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# test framework
$(BIN_DIR)/unity.o: $(TEST_FRAMEWORK_DIR)/unity.c \
$(TEST_FRAMEWORK_DIR)/unity.h $(TEST_FRAMEWORK_DIR)/unity_internals.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# grab the pattern for the file
$(EXAMPLES): %: %.c $(BIN_DIR)/sr_matrix.o
	$(CC) $(CFLAGS) $^ -o $@ $(SDL2_FLAGS)

$(MATRIX_TESTS): %: %.c $(SRC_DIR)/sr_matrix.h $(BIN_DIR)/unity.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR) -lm

$(RASTER_TESTS): %: %.c $(BIN_DIR)/sr_raster.o $(BIN_DIR)/unity.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR) -lm

$(RENDER_TESTS): %: %.c $(BIN_DIR)/sr_render.o $(BIN_DIR)/sr_raster.o \
$(BIN_DIR)/sr_clip.o $(BIN_DIR)/unity.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR) -lm

$(CLIP_TESTS): %: %.c $(BIN_DIR)/sr_clip.o $(BIN_DIR)/unity.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR)

$(OBJ_TESTS): %: %.c $(BIN_DIR)/unity.o
	$(CC) $(CFLAGS) $^ -o $@ -I$(TEST_FRAMEWORK_DIR) -I$(SRC_DIR) -lm

check-raster: $(RASTER_TESTS)
	for t in $(RASTER_TESTS); do $$t; done

check-clip: $(CLIP_TESTS)
	for t in $(CLIP_TESTS); do $$t; done

check-obj: $(OBJ_TESTS)
	for t in $(OBJ_TESTS); do $$t; done

check-render: $(RENDER_TESTS)
	for t in $(RENDER_TESTS); do $$t; done

check-matrix: $(MATRIX_TESTS)
	for t in $(MATRIX_TESTS); do $$t; done

check-all: check-raster check-clip check-obj check-matrix
	
clean:
	rm -r $(BIN_DIR)
	for t in $(TESTS); do rm $$t; done
