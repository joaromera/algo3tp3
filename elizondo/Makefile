CXX=g++

STDFLAGS = -std=c++11

RM=rm -f

BINARIES = greedy_player static_player gs_player logical_board

all: $(BINARIES)

greedy_player:
	$(CXX) $(STDFLAGS) main_greedy.cpp -o greedy_player

static_player:
	$(CXX) $(STDFLAGS) main_static.cpp -o static_player

gs_player:
	$(CXX) $(STDFLAGS) main_gs.cpp -o gs_player

logical_board:
	$(CXX) $(STDFLAGS) main_logical.cpp -o logical_board

clean:
	$(RM) greedy_player
	$(RM) static_player
	$(RM) gs_player
	$(RM) logical_board
	$(RM) *.o *~
