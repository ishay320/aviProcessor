CFLAGS=-Wall -Werror -std=c++11 -pedantic -ggdb `pkg-config --cflags --libs opencv4`
LIB=-pthread

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
endif

main: main.cpp
	g++ -o $@ $^ $(CFLAGS) $(LIB)

clean:
	rm -f main
	
.PHONY: clean