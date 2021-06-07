ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs) -lRooFit -lRooFitCore -lMinuit

all: trigeff trigcal

trigeff: triggerefficiency.cc
	g++ -Wall -Wextra -O3 -o $@ triggerefficiency.cc $(ROOTFLAGS) $(ROOTLIBS)

trigcal: triggeroverlay.cc
	g++ -Wall -Wextra -O3 -o $@ triggeroverlay.cc $(ROOTFLAGS) $(ROOTLIBS)

clean:
	rm -f trigeff trigcal *~

