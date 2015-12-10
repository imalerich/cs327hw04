CFLAGS = -std=c++11 -Wall -g -c
LFLAGS = -g -lm
OBJ = Channel.o AudioFile.o CS229Reader.o CS229Writer.o SinWave.o TriangleWave.o SawToothWave.o PulseWave.o AdsrEnvelope.o flags.o ABC229Reader.o WavWriter.o
FUNC = func/iWaveform.h func/iFunction.h
BASE = AudioFile.h Channel.h

imaudio.a: $(OBJ)
	[ -d ../bin ] || mkdir ../lib
	ar rvs ../lib/libimaudio.a $(OBJ)

Channel.o: Channel.cpp Channel.h
	g++ $(CFLAGS) Channel.cpp

AudioFile.o: AudioFile.cpp AudioFile.h Channel.h
	g++ $(CFLAGS) AudioFile.cpp

CS229Reader.o: CS229Reader.cpp CS229Reader.h iFileReader.h $(BASE)
	g++ $(CFLAGS) CS229Reader.cpp

CS229Writer.o: CS229Writer.cpp CS229Writer.h iFileWriter.h $(BASE)
	g++ $(CFLAGS) CS229Writer.cpp

WavWriter.o: WavWriter.cpp WavWriter.h iFileWriter.h $(BASe)
	g++ $(CFLAGS) WavWriter.cpp

ABC229Reader.o: ABC229Reader.cpp ABC229Reader.h iFileReader.h $(BASE)
	g++ $(CFLAGS) ABC229Reader.cpp

SinWave.o: func/SinWave.cpp func/SinWave.h $(FUNC)
	g++ $(CFLAGS) func/SinWave.cpp

TriangleWave.o: func/TriangleWave.cpp func/TriangleWave.h $(FUNC)
	g++ $(CFLAGS) func/TriangleWave.cpp

SawToothWave.o: func/SawToothWave.cpp func/SawToothWave.h $(FUNC)
	g++ $(CFLAGS) func/SawToothWave.cpp

PulseWave.o: func/PulseWave.cpp func/PulseWave.h $(FUNC)
	g++ $(CFLAGS) func/PulseWave.cpp

AdsrEnvelope.o: func/AdsrEnvelope.cpp func/AdsrEnvelope.h func/iFunction.h
	g++ $(CFLAGS) func/AdsrEnvelope.cpp

flags.o: flags.h flags.cpp
	g++ $(CFLAGS) flags.cpp

clean:
	rm -rf *.o
	rm -rf ../bin/libimaudio.a 
