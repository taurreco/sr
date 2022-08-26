
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
SR_SRC += src/sr_render.c
SR_SRC += src/sr_load_obj.c
SR_SRC += src/sr_clip.c
SR_SRC += src/sr_raster.c
SR_SRC += src/sr_math.c

# Example Targets
EXAMPLES += examples/cube

# Tests Targets
RENDER_TESTS += tests/check_draw_indexed
RENDER_TESTS += tests/check_winding_order
RENDER_DEPS += src/sr_clip.c 
RENDER_DEPS += src/sr_raster.c 
RENDER_DEPS += src/sr_math.c

RASTER_TESTS += tests/check_draw_tr
RASTER_TESTS += tests/check_draw_pt
RASTER_TESTS += tests/check_edge_init
RASTER_TESTS += tests/check_is_tl

OBJ_TESTS += tests/check_hash_table
OBJ_TESTS += tests/check_load_obj

CLIP_TESTS += tests/check_clip_poly
CLIP_TESTS += tests/check_clip_routine
CLIP_TESTS += tests/check_clip_test
CLIP_TESTS += tests/check_lerp

MATH_TESTS += tests/check_matmul

ALL_TESTS += $(RASTER_TESTS) 
ALL_TESTS += $(OBJ_TESTS) 
ALL_TESTS += $(CLIP_TESTS) 
ALL_TESTS += $(MATH_TESTS)

# Example Rules
$(EXAMPLES): %: %.c
	$(CC) $(CFLAGS) $< $(SR_SRC) -o $@ $(SDL2_FLAGS) -lm

# Test Rules
$(RENDER_TESTS): %: %.c
	$(CC) $(CFLAGS) -Isrc -Iunity $< $(RENDER_DEPS) unity/unity.c -o $@ -lm

$(ALL_TESTS): %: %.c
	$(CC) $(CFLAGS) -Isrc -Iunity $< unity/unity.c -o $@ -lm

examples: $(EXAMPLES)
tests: $(RENDER_TESTS) $(ALL_TESTS)

# Clean Up
clean-tests:
	for t in $(RENDER_TESTS); do rm $$t; done
	for t in $(ALL_TESTS); do rm $$t; done

clean-examples:
	for t in $(EXAMPLES); do rm $$t; done

clean-all: clean-examples clean-tests