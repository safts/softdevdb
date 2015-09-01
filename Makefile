all: build run

build:
	g++ -O2 *.cc  -o main

run:
	./main

clean:
	rm -f main
