
# Install Path
INSTALL_PATH = /usr/local/

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
CFLAGS += -I. 
CFLAGS += -O3
CFLAGS += -lm
#CFLAGS += -fsanitize=address

MINGW_FLAGS += -IC:\MinGW\include\ 
MINGW_FLAGS += -LC:\MinGW\lib 
MINGW_FLAGS += -lmingw32

# SDL2 Library Flags
SDL2_FLAGS += -lSDL2main
SDL2_FLAGS += -lSDL2

# SR Library Targets
SR = sr.o
SR_LIB_DIR = $(INSTALL_PATH)lib/sr/
SR_HEADERS_DIR = $(INSTALL_PATH)include/sr.h
SR_LIB = libsr.so
SR_HEADER = sr.h

# Source Files
SR_SRC += state.c
SR_SRC += pipe.c
SR_SRC += obj.c
SR_SRC += tga.c
SR_SRC += clip.c
SR_SRC += rast.c
SR_SRC += shad.c
SR_SRC += mat.c

# Object Files
SR_OBJS = $(patsubst %.c, %.o, $(SR_SRC))

# Example Targets
EXAMPLES += examples/basic_triangle
EXAMPLES += examples/bunny
EXAMPLES += examples/texture

# Tests Targets
PIPE_TESTS += tests/check_render
PIPE_TESTS += tests/check_winding_order
PIPE_DEPS += clip.c 
PIPE_DEPS += rast.c 
PIPE_DEPS += mat.c

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
TESTS += tests/check_clip_test

all: $(SR) $(SR_LIB) examples tests

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(SR): $(SR_OBJS)
	ld -r $(SR_OBJS) -o $@

# Compile SR Library
$(SR_LIB): $(SR)
	$(CC) -shared $(SR) -o $(SR_LIB)

# Example Rules
$(EXAMPLES): %: %.c $(SR)
	$(CC) $(CFLAGS) examples/driver.c $(SR) $< -o $@ $(SDL2_FLAGS)

# Test Rules
$(PIPE_TESTS): %: %.c
	$(CC) $(CFLAGS) -Isrc -Iunity $< $(PIPE_DEPS) unity/unity.c -o $@

$(TESTS): %: %.c
	$(CC) $(CFLAGS) -Isrc -Iunity $< unity/unity.c -o $@

# Local
sr: $(SR_LIB)

examples: $(EXAMPLES)

tests: $(PIPE_TESTS) $(TESTS)

check-all: $(PIPE_TESTS) $(TESTS)
	for t in $(PIPE_TESTS); do $$t; done
	for t in $(TESTS); do $$t; done

# Install
install: $(SR_LIB) | $(SR_LIB_DIR) $(SR_HEADERS_DIR)
	cp $(SR_LIB) $(SR_LIB_DIR)$(SR_LIB)
	cp include/sr.h $(SR_HEADERS_DIR)$(SR_HEADER)

uninstall:
	rm -rf $(SR_LIB_DIR)
	rm -rf $(SR_HEADERS_DIR)

$(SR_LIB_DIR):
	mkdir -p $@

$(SR_HEADERS_DIR):
	mkdir -p $@

# Clean Up
clean-sr:
	for t in $(SR_OBJS); do rm $$t; done
	rm $(SR)
	rm $(SR_LIB)

clean-examples:
	for t in $(EXAMPLES); do rm $$t; done

clean-tests:
	for t in $(PIPE_TESTS); do rm $$t; done
	for t in $(TESTS); do rm $$t; done

clean: clean-sr clean-examples clean-tests
