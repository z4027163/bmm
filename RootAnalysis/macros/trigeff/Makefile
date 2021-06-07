ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs) -lRooFit -lRooFitCore -lMinuit

all: trigeff

trigeff: triggerefficiency.cc
	g++ -Wall -Wextra -O3 -o $@ triggerefficiency.cc $(ROOTFLAGS) $(ROOTLIBS)

clean:
	rm -f trigeff *~

