CXXFLAGS := -Wno-write-strings

all: harness muxer
harness: harness.o tcp.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
muxer: muxer.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
