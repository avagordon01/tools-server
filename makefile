CXXFLAGS := -g -Og -Wno-write-strings
LDLIBS := -luv

all: harness muxer
harness: harness.o tcp.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
muxer: muxer.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
