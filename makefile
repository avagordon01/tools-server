CXXFLAGS := -g -Og -Wno-write-strings -fpermissive
LDLIBS := -luv -lwebsockets

all: harness server
harness: harness.o tcp.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
server: server.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -f harness server *.o
