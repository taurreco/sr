
# Compiler
CC = gcc

# Flags
CFLAGS += -ggdb3
CFLAGS += -g
CFLAGS += -Wall 
CFLAGS += -Wextra 
CFLAGS += -Wshadow 
CFLAGS += -std=gnu11 
CFLAGS += -Wno-unused-parameter
CFLAGS += -Iinclude 
CFLAGS += -O3
#CFLAGS += -fsanitize=address

MINGW_FLAGS += -IC:\MinGW\include\ 
MINGW_FLAGS += -LC:\MinGW\lib 
MINGW_FLAGS += -lmingw32 

SDL2_FLAGS += -lSDL2main
SDL2_FLAGS += -lSDL2

SR_SRC += src/sr_lib.c
SR_SRC += src/sr_pipe.c
SR_SRC += src/sr_obj.c
SR_SRC += src/sr_tga.c
SR_SRC += src/sr_clip.c
SR_SRC += src/sr_rast.c
SR_SRC += src/sr_shaders.c
SR_SRC += src/sr_math.c

# Example Targets
EXAMPLES += examples/basic_triangle
EXAMPLES += examples/bunny

# Tests Targets
PIPE_TESTS += tests/check_render
PIPE_TESTS += tests/check_winding_order
PIPE_DEPS += src/sr_clip.c 
PIPE_DEPS += src/sr_rast.c 
PIPE_DEPS += src/sr_math.c

# Raster Tests
TESTS += tests/check_draw_tr
TESTS += tests/check_draw_pt
TESTS += tests/check_edge_init
TESTS += tests/check_is_tl

# Clip Tests
TESTS += tests/check_clip_poly
TESTS += tests/check_clip_routine
TESTS += tests/check_lerp

# Math Tests
TESTS += tests/check_matmul
TESTS += tests/check_hash_table
TESTS += tests/check_clip_test

# Example Rules
$(EXAMPLES): %: %.c
	$(CC) $(CFLAGS) examples/driver.c $(SR_SRC) $< -o $@ $(SDL2_FLAGS) -Isrc -lm

# Test Rules
$(PIPE_TESTS): %: %.c
	$(CC) $(CFLAGS) -Isrc -Iunity $< $(PIPE_DEPS) unity/unity.c -o $@ -lm

$(TESTS): %: %.c
	$(CC) $(CFLAGS) -Isrc -Iunity $< unity/unity.c -o $@ -lm

examples: $(EXAMPLES)
tests: $(PIPE_TESTS) $(TESTS)

check-all: $(PIPE_TESTS) $(TESTS)
	for t in $(PIPE_TESTS); do $$t; done
	for t in $(TESTS); do $$t; done

# Clean Up
clean-tests:
	for t in $(PIPE_TESTS); do rm $$t; done
	for t in $(TESTS); do rm $$t; done

clean-examples:
	for t in $(EXAMPLES); do rm $$t; done

clean-all: clean-examples clean-tests