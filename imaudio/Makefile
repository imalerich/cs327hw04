CFLAGS = -std=c++11 -Wall -g -c
LFLAGS = -lm
OBJ = Channel.o

imaudio.a: $(OBJ)
	[ -d ../bin ] || mkdir ../lib
	ar rvs ../lib/libimaudio.a $(OBJ)

Channel.o: Channel.cpp Channel.h
	g++ $(CFLAGS) Channel.cpp

clean:
	rm -rf *.o
	rm -rf ../bin/libimaudio.a 
