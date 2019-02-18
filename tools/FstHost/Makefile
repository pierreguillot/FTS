CPPFLAGS=-I../../FST
CXXFLAGS=-g -O0
LIBS=-ldl -lrt

default: FstHost

all: FstHost FstHost.32 FstHost.64

FstHost.32: ARCHFLAGS=-m32
FstHost.64: ARCHFLAGS=-m64

FstHost FstHost.32 FstHost.64: FstHost.cpp ../../FST/fst.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(ARCHFLAGS) $< -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -f FstHost FstHost.32 FstHost.64
