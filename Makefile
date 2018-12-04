all: SXFMap

SXFMap: main.o Map.o
	g++ main.o Map.o -o SXFMap

main.o: main.cpp
	g++ -c main.cpp

Map.o: Map.cpp
	g++ -c Map.cpp

clean: 
	rm -rf *.o SXFMap
