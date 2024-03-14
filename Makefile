CPP := ./src/main.cpp ./src/tsp_n_small.cpp ./src/tsp_pso.cpp
HEA := ./src/tsp_n_small.h ./src/tsp_pso.h

main: $(CPP) $(HEA)
	g++ $(CPP) -o main -Wall

