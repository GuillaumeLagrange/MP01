CXXFLAGS = -g -std=c++11 -O3

main : client.o message.o nickmess.h

.PHONY : clean

clean :
	rm -rf *.o
	rm -rf main
