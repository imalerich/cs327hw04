CFLAGS = -std=c++11 -Wall -g -c
LFLAGS = -g -lm
OBJ = Channel.o AudioFile.o CS229Reader.o CS229Writer.o

imaudio.a: $(OBJ)
	[ -d ../bin ] || mkdir ../lib
	ar rvs ../lib/libimaudio.a $(OBJ)

Channel.o: Channel.cpp Channel.h
	g++ $(CFLAGS) Channel.cpp

AudioFile.o: AudioFile.cpp AudioFile.h Channel.h
	g++ $(CFLAGS) AudioFile.cpp

CS229Reader.o: CS229Reader.cpp CS229Reader.h iFileReader.h
	g++ $(CFLAGS) CS229Reader.cpp

CS229Writer.o: CS229Writer.cpp CS229Writer.h iFileWriter.h
	g++ $(CFLAGS) CS229Writer.cpp

clean:
	rm -rf *.o
	rm -rf ../bin/libimaudio.a 
