LIBS=-ldl -lrt

FST_CPPFLAGS=-I../../FST
FST_CXXFLAGS=-g -O0

default: FstHost

archs: FstHost.32 FstHost.64

FstHost.32: ARCHFLAGS=-m32
FstHost.64: ARCHFLAGS=-m64

FstHost FstHost.32 FstHost.64: FstHost.cpp ../../FST/fst.h
	$(CXX) $(FST_CPPFLAGS) $(CPPFLAGS) $(FST_CXXFLAGS) $(CXXFLAGS) $(ARCHFLAGS) $< -o $@ $(LIBS)

.PHONY: clean archs default
clean:
	rm -f FstHost FstHost.32 FstHost.64
