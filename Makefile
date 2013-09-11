all: cpparse

#CFLAGS=-std=c++0x
CFLAGS=-g
LDFLAGS=-lboost_regex

cpparse.o: cpparse.cpp
	g++ -c -o cpparse.o cpparse.cpp $(CPPFLAGS) $(CFLAGS)

cpparse: cpparse.o
	g++ -o cpparse cpparse.o $(LDFLAGS)

clean:
	$(RM) *.o
	$(RM) cpparse

.PHONY: all clean
