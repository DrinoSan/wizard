.PHONY: all clean debug

# ----- Compiler & flags -------------------------------------------------
CC      = clang                     # C compiler
CFLAGS  = -g -Wall -Wextra -std=c11 \
          $(shell pkg-config --cflags raylib)

# Linker flags – same libraries you used for C++ (Raylib pulls in glfw, etc.)
LDFLAGS = $(shell pkg-config --libs raylib) \
          -lpthread -L/opt/homebrew/lib -lglfw \
          -framework CoreFoundation -framework Cocoa \
          -framework IOKit -framework OpenGL

# ----- Source / object files -------------------------------------------
C_FILES   := $(wildcard *.c)                     # all .c files
C_FILES   += $(wildcard events/*.c)
OBJ_FILES := $(C_FILES:.c=.o)                    # corresponding .o files

# ----- Default target ---------------------------------------------------
all: wizard

# ----- Debug build (adds -DDEBUG and -g) --------------------------------
debug: CFLAGS += -DDEBUG -g
debug: wizard

# ----- Main executable --------------------------------------------------
wizard: $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $@

# ----- Object file rule -------------------------------------------------
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# ----- Optional helper: CollisionLayoutCreator (still C) ---------------

# ----- Clean ------------------------------------------------------------
clean:
	rm -f *.o wizard collisionCreator bin/collisionCreator
