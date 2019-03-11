FST_CPPFLAGS=-I../../FST -DFST_DONT_DEPRECATE_UNKNOWN=1
FST_CXXFLAGS=-g -O0 -fPIC

LIBS=

archs: FstProxy.32.so FstProxy.64.so

FstProxy.32.so: ARCHFLAGS=-m32
FstProxy.64.so: ARCHFLAGS=-m64

FstProxy.so FstProxy.32.so FstProxy.64.so: FstProxy.cpp ../../FST/fst.h ../../FST/fst_utils.h
	$(CXX) -shared $(FST_CPPFLAGS) $(CPPFLAGS) $(FST_CXXFLAGS) $(CXXFLAGS) $(ARCHFLAGS) $< -o $@ $(LIBS)

.PHONY: clean archs default
clean:
	rm -f *.so
