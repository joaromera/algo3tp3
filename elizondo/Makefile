CXX=g++

STDFLAGS = -std=c++11
XTRAFLAGS = -Wall -Wextra

RM=rm -f

BINARIES = greedy_player static_player gs_player

all: $(BINARIES)

greedy_player:
	$(CXX) $(STDFLAGS) main_greedy.cpp -o greedy_player

static_player:
	$(CXX) $(STDFLAGS) main_static.cpp -o static_player

gs_player:
	$(CXX) $(STDFLAGS) main_gs.cpp -o gs_player

clean:
	$(RM) greedy_player
	$(RM) static_player
	$(RM) gs_player
	$(RM) *.o *~
