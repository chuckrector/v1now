CC=g++
MAKEFLAGS=-j

.PHONY: rebuild

rebuild:
	$(MAKE) clean
	$(MAKE) build/main

clean:
	rm -rf build
	mkdir build

build/%.o: src/%.cpp
	$(CC) -c -o3 -o $@ -w $<

build/main: $(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp))
