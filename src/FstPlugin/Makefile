FST_CPPFLAGS=-I../../FST
FST_CXXFLAGS=-g -O0 -fPIC

LIBS=

default: FstPlugin.so

all: FstPlugin.so FstPlugin.32.so FstPlugin.64.so

FstPlugin.32.so: ARCHFLAGS=-m32
FstPlugin.64.so: ARCHFLAGS=-m64

FstPlugin.so FstPlugin.32.so FstPlugin.64.so: FstPlugin.cpp ../../FST/fst.h
	$(CXX) -shared $(FST_CPPFLAGS) $(CPPFLAGS) $(FST_CXXFLAGS) $(CXXFLAGS) $(ARCHFLAGS) $< -o $@ $(LIBS)

.PHONY: clean all default
clean:
	rm -f *.so
