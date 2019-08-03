CXXFLAGS := -g -Og -Wno-write-strings -fpermissive
LDLIBS := -luv -lwebsockets

all: harness server ws-server
harness: harness.o tcp.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
server: server.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
ws-server: ws-server.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
