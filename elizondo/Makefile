CXX=g++

STDFLAGS = -std=c++11

RM=rm -f

BINARIES = greedy_player static_player tournament logical_board

all: $(BINARIES)

greedy_player:
	$(CXX) $(STDFLAGS) main_greedy.cpp -o greedy_player

static_player:
	$(CXX) $(STDFLAGS) main_static.cpp -o static_player

tournament:
	$(CXX) $(STDFLAGS) main_tournament.cpp -o tournament

logical_board:
	$(CXX) $(STDFLAGS) main_logical.cpp -o logical_board

clean:
	$(RM) greedy_player
	$(RM) static_player
	$(RM) tournament
	$(RM) logical_board
	$(RM) *.o *~
