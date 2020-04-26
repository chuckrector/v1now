CC=g++

build/%.o: src/%.cpp
	$(CC) -c -o3 -o $@ -w $<

build/main: $(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp))
