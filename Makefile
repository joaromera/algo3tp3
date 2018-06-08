CXX=g++

STDFLAGS = -std=c++11

RM=rm -f

BINARIES = generator euclidiangraphs hiperconectados hiperauditados astarvsdpq

all: $(BINARIES)

# generator: generator.o
# 	$(CXX) $(STDFLAGS) $^ -o generator

# euclidiangraphs: euclidiangraphs.o
# 	$(CXX) $(STDFLAGS) $^ -o euclidiangraphs

# hiperconectados: hiperconectados.o prim.o kruskal.o kruskalpc.o
# 	$(CXX) $(STDFLAGS) $^ -o hiperconectados

# hiperauditados: hiperauditados.o dijkstraold.o dijkstra.o dijkstrapq.o bellmanford.o floydwarshall.o dantzig.o astar.o
# 	$(CXX) $(STDFLAGS) $^ -o hiperauditados

# astarvsdpq: astarvsdpq.o dijkstrapq.o astar.o
# 	$(CXX) $(STDFLAGS) $^ -o astarvsdpq

%.o: %.cpp
	$(CXX) $(STDFLAGS) -c $<

clean:
	$(RM) *.o *.txt
	$(RM) $(BINARIES)