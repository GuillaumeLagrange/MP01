CXXFLAGS = -g -std=c++11 -O3

main : client.o message.h nickmess.h

.PHONY : clean

clean :
	rm -rf *.o
	rm -rf main
