
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
EXAMPLES += examples/cube
EXAMPLES += examples/obj
EXAMPLES += examples/dragon

# Tests Targets
PIPE_TESTS += tests/check_draw_indexed
PIPE_TESTS += tests/check_winding_order
PIPE_DEPS += src/sr_clip.c 
PIPE_DEPS += src/sr_rast.c 
PIPE_DEPS += src/sr_math.c

RAST_TESTS += tests/check_draw_tr
RAST_TESTS += tests/check_draw_pt
RAST_TESTS += tests/check_edge_init
RAST_TESTS += tests/check_is_tl

OBJ_TESTS += tests/check_hash_table
OBJ_TESTS += tests/check_load_obj

TGA_TESTS += tests/check_load_tga

CLIP_TESTS += tests/check_clip_poly
CLIP_TESTS += tests/check_clip_routine
CLIP_TESTS += tests/check_clip_test
CLIP_TESTS += tests/check_lerp

MATH_TESTS += tests/check_matmul

ALL_TESTS += $(RASTER_TESTS) 
ALL_TESTS += $(OBJ_TESTS) 
ALL_TESTS += $(TGA_TESTS) 
ALL_TESTS += $(CLIP_TESTS) 
ALL_TESTS += $(MATH_TESTS)

# Example Rules
$(EXAMPLES): %: %.c
	$(CC) $(CFLAGS) $< $(SR_SRC) -o $@ $(SDL2_FLAGS) -Isrc -lm

# Test Rules
$(PIPE_TESTS): %: %.c
	$(CC) $(CFLAGS) -Isrc -Iunity $< $(PIPE_DEPS) unity/unity.c -o $@ -lm

$(ALL_TESTS): %: %.c
	$(CC) $(CFLAGS) -Isrc -Iunity $< unity/unity.c -o $@ -lm

examples: $(EXAMPLES)
tests: $(PIPE_TESTS) $(ALL_TESTS)

# Clean Up
clean-tests:
	for t in $(PIPE_TESTS); do rm $$t; done
	for t in $(ALL_TESTS); do rm $$t; done

clean-examples:
	for t in $(EXAMPLES); do rm $$t; done

clean-all: clean-examples clean-tests