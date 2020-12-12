CXX=g++

STDFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

RM=rm -f

BINARIES = greedy_player static_player tournament

all: $(BINARIES)

greedy_player:
	$(CXX) $(STDFLAGS) main_greedy.cpp -o greedy_player

static_player:
	$(CXX) $(STDFLAGS) main_static.cpp -o static_player

tournament:
	$(CXX) $(STDFLAGS) main_tournament.cpp -o tournament

clean:
	$(RM) greedy_player
	$(RM) static_player
	$(RM) tournament
	$(RM) *.o *~
