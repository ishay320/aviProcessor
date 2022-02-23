CFLAGS=-Wall -Werror -std=c++11 -pedantic -ggdb `pkg-config --cflags --libs opencv4`

main: main.cpp
	g++ -o $@ $^ $(CFLAGS)

clean:
	rm -f main
	
.PHONY: clean