## Makefile
## author:  Holger Arndt
## version: 21.11.2017
# Edited by Raihan for Ex22

CPP = cpp
MAKEDEP = $(CPP) -MM
CXX = g++
CXXFLAGS = -Wall -std=c++17

CCSRC = matrix.cc mattst.cc
CCOBJ := $(patsubst %.cc,%.o,$(CCSRC))
CCOBJDBG := $(patsubst %.cc,%_dbg.o,$(CCSRC))
CCDEP := $(patsubst %.cc,%.dep,$(CCSRC))
CCDEPDBG := $(patsubst %.cc,%_dbg.dep,$(CCSRC))

all: mattst mattst_dbg

mattst: $(CCOBJ)
	$(CXX) $(CCOBJ) -o $@

mattst_dbg: $(CCOBJDBG)
	$(CXX) $(CCOBJDBG) -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

%_dbg.o: %.cc
	$(CXX) -DDEBUG $(CXXFLAGS) -c $< -o $@

%.dep: %.cc
	$(MAKEDEP) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

%_dbg.dep: %.cc
	$(MAKEDEP) $< | sed 's,\($*\)\_dbg.o[ :]*,\1_dbg.o $@ : ,g' > $@

clean:
	rm -f $(CCOBJ) $(CCOBJDBG) $(CCDEP) $(CCDEPDBG) mattst mattst_dbg

-include $(CCDEP) $(CCDEPDBG)
