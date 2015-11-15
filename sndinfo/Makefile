CFLAGS = -std=c++11 -Wall -g -c -I ../imaudio/
LFLAGS = -lm -g -L ../lib/
OBJ = main.o
LIB = -limaudio

sndinfo: $(OBJ)
	[ -d ../bin ] || mkdir ../bin
	g++ -o ../bin/sndinfo $(LFLAGS) $(OBJ) $(LIB)

main.o: main.cpp
	g++ $(CFLAGS) main.cpp

clean:
	rm -rf *.o
	rm -rf ../bin/sndinfo
