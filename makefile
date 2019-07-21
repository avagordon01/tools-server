CXXFLAGS := -g -Og -Wno-write-strings
LDLIBS := -luv

all: harness server
harness: harness.o tcp.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
server: server.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
