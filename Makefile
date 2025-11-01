.PHONY: all clean

CXX = g++
CXXFLAGS = -g `pkg-config --cflags raylib` -Wall -std=c++17
LDFLAGS = `pkg-config --libs raylib` -lpthread -L/opt/homebrew/lib -lglfw

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)

all: wizard

debug: CXXFLAGS += -DDEBUG -g
debug: wizard

wizard: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o $@

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

collisionCreator: CollisionLayoutCreator.o
	$(CXX) CollisionLayoutCreator.o -o bin/$@

CollisionLayoutCreator.o: utils/CollisionLayoutCreator.cpp
	$(CXX) -c $< -o $@

clean:
	rm -f *.o wizard collisionCreator
