c_opt = -std=c++11 -Wall -g -c
l_opt = -lm
obj = sample.o Channel.o

sample: $(obj)
	g++ $(l_opt) -o sample sample.o Channel.o

sample.o: sample.cpp
	g++ $(c_opt) sample.cpp

Channel.o: Channel.cpp Channel.h
	g++ $(c_opt) Channel.cpp

clean:
	rm -rf *.o
	rm -rf sample
