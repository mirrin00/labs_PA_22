H = ../Headers
S = ../Sources

generator: generate-matrices.cpp
	g++ generate-matrices.cpp -o ./generate-matrices.out

process: process.cpp
	g++ -lm -lpthread process.cpp -o ./process.out

thread: thread.cpp
	g++ -lm -lpthread thread.cpp -o ./thread.out
	
threads: threads.cpp
	g++ -lm -lpthread threads.cpp -o ./threads.out

clearmat:
	rm -rf *.txt

clearobj:
	rm -rf *.o

clear:
	rm -rf *.o *.out *.txt
