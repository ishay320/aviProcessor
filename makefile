CFLAGS=-Wall -Werror -std=c++11 -pedantic -ggdb `pkg-config --cflags --libs opencv4`
LIB=-pthread

SOURCES=parser.cpp AviProccesor.cpp
OBJECTS=$(subst .cpp,.o,$(SOURCES))

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
endif

main: main.cpp $(OBJECTS)
	g++ -o $@ $^ $(CFLAGS) $(LIB)

test: test.cpp $(OBJECTS)
	g++ -o $@ $^ $(CFLAGS) $(LIB)

%.o: %.cpp $(HEADERS)
	g++ $(CFLAGS) --compile $< -o $@

clean:
	rm -f main test *.o
	
.PHONY: clean