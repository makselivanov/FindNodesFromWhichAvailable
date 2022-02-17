all: main.cpp
	g++ -std=c++11 main.cpp -o main

run: main
	./main

clean: main
	rm main