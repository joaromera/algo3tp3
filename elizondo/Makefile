CXX=g++

STDFLAGS = -std=c++11 -O3 -Ofast
XTRAFLAGS = -Wall -Wextra

RM=rm -f

BINARIES = greedy_player static_player

all: $(BINARIES)

greedy_player:
	$(CXX) $(STDFLAGS) main.cpp -o greedy_player

static_player:
	$(CXX) $(STDFLAGS) main.cpp -o static_player

clean:
	$(RM) greedy_player
	$(RM) static_player
	$(RM) *.o *~
