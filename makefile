

main: main.cpp
	g++ -o $@ $^ `pkg-config --cflags --libs opencv4`

clean:
	rm -f main
	
.PHONY: clean