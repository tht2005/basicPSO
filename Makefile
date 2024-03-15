EXEC := main
CPP := $(wildcard ./src/*.cpp)
HEA := $(wildcard ./src/*.hpp)
CXXFLAGS := -std=c++17 -Wall

main: $(CPP) $(HEA)
	g++ $(CXXFLAGS) $(CPP) -o $(EXEC)

