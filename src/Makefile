CXX=g++

STDFLAGS = -std=c++11

RM=rm -f

BINARIES = main

all: $(BINARIES)

main: main.o
	$(CXX) $(STDFLAGS) $^ -o main

%.o: %.cpp
	$(CXX) $(STDFLAGS) -c $<

clean:
	$(RM) *.o *.txt
	$(RM) $(BINARIES)