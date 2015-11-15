CFLAGS = -std=c++11 -Wall -g -c
LFLAGS = -g -lm
OBJ = Channel.o AudioFile.o FormatCS229.o

imaudio.a: $(OBJ)
	[ -d ../bin ] || mkdir ../lib
	ar rvs ../lib/libimaudio.a $(OBJ)

Channel.o: Channel.cpp Channel.h
	g++ $(CFLAGS) Channel.cpp

AudioFile.o: AudioFile.cpp AudioFile.h Channel.h
	g++ $(CFLAGS) AudioFile.cpp

FormatCS229.o: FormatCS229.cpp FormatCS229.h iFileReader.h iFileWriter.h
	g++ $(CFLAGS) FormatCS229.cpp

clean:
	rm -rf *.o
	rm -rf ../bin/libimaudio.a 
