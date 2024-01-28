## Makefile
## author:  Holger Arndt
## version: 21.11.2017
# Edited by Raihan for Angle(Ex20)

CPP = cpp
MAKEDEP = $(CPP) -MM
CXX = g++
CXXFLAGS = -Wall -std=c++17

CCSRC = Angle.cpp AngleTest.cpp
CCOBJ := $(patsubst %.cpp,%.o,$(CCSRC))
CCOBJDBG := $(patsubst %.cpp,%_dbg.o,$(CCSRC))
CCDEP := $(patsubst %.cpp,%.dep,$(CCSRC))
CCDEPDBG := $(patsubst %.cpp,%_dbg.dep,$(CCSRC))

all: AngTst AngTst_dbg

AngTst: $(CCOBJ)
	$(CXX) $(CCOBJ) -o $@

AngTst_dbg: $(CCOBJDBG)
	$(CXX) $(CCOBJDBG) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

%_dbg.o: %.cpp
	$(CXX) -DDEBUG $(CXXFLAGS) -c $< -o $@

%.dep: %.cpp
	$(MAKEDEP) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

%_dbg.dep: %.cpp
	$(MAKEDEP) $< | sed 's,\($*\)\_dbg.o[ :]*,\1_dbg.o $@ : ,g' > $@

clean:
	rm -f $(CCOBJ) $(CCOBJDBG) $(CCDEP) $(CCDEPDBG) AngTst AngTst_dbg

-include $(CCDEP) $(CCDEPDBG)
