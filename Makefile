all: build

build: movies.c
	@gcc -Wall -g -o movies movies.c

clean:
	rm movies
