.PHONY: all clean run debug

CXX = clang++
CXXFLAGS = -g -Wall -std=c++20 \
           -Iexternal/imgui -Iexternal/imgui/extras \
           `pkg-config --cflags raylib`

# Raylib libs + macOS frameworks (pkg-config handles -lraylib, but add extras if needed)
LDFLAGS = `pkg-config --libs raylib` \
          -framework CoreVideo -framework IOKit -framework Cocoa \
          -framework OpenGL -framework GLUT -lpthread -L/opt/homebrew/lib \
          -framework CoreFoundation

# Project sources
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)

# ImGui + rlImGui sources (including demo)
IMGUI_FILES = external/imgui/rlImGui.cpp \
              external/imgui/imgui.cpp \
              external/imgui/imgui_draw.cpp \
              external/imgui/imgui_tables.cpp \
              external/imgui/imgui_widgets.cpp \
              external/imgui/imgui_demo.cpp

OBJ_FILES += $(IMGUI_FILES:.cpp=.o)

all: wizard

debug: CXXFLAGS += -DDEBUG -g
debug: wizard

wizard: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o $@

# Pattern rule for all .cpp -> .o (handles subdirs)
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

# Extra rules
collisionCreator: CollisionLayoutCreator.o
	$(CXX) CollisionLayoutCreator.o $(LDFLAGS) -o bin/$@

CollisionLayoutCreator.o: utils/CollisionLayoutCreator.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

run: wizard
	./wizard

clean:
	rm -f *.o external/imgui/*.o wizard bin/*
