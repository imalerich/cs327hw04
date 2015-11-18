CFLAGS = -std=c++11 -Wall -g -c -I ../imaudio/
LFLAGS = -lm -L ../lib/
OBJ = main.o
LIB = -limaudio

sndcat: $(OBJ)
	[ -d ../bin ] || mkdir ../bin
	g++ -o ../bin/sndcat $(LFLAGS) $(OBJ) $(LIB)

main.o: main.cpp
	g++ $(CFLAGS) main.cpp

clean:
	rm -rf *.o
	rm -rf ../bin/sndcat
