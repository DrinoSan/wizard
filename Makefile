.PHONY: all clean

CXX = clang++
CXXFLAGS = -g `pkg-config --cflags raylib` -Wall -std=c++17
LDFLAGS = `pkg-config --libs raylib` -lpthread -L/opt/homebrew/lib -lglfw -framework CoreFoundation -framework Cocoa -framework IOKit -framework OpenGL

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)

all: wizard

wizard: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o $@

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o wizard
