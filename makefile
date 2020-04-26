CC=g++

build/%.o: src/%.cpp
	$(CC) -c -fmax-errors=5 -o3 -o $@ -w $<

build/main: $(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp))
