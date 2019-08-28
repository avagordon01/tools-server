CXXFLAGS := -g -Og -Wno-write-strings -fpermissive
LDLIBS := -luv -lwebsockets

all: harness server
harness: harness-main.o harness.o tcp.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)
server: server-main.o server.o
	$(CXX) -o $@ $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -f harness server *.o
