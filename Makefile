TARGET=fbf
RELEASEDIR=./release/
DEBUGDIR=./debug/
OBJS=aviviewer.o fbf.o
CXX=g++
CXXFLAGS= -Wall
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: release
$(TARGET): release

.PHONY: release
release: CXXFLAGS += -O2
release: $(OBJS)
	$(CXX) -o $(RELEASEDIR)$(TARGET) $(OBJS) $(OPENCVLIB)

.PHONY: debug
debug: CXXFLAGS += -g
debug: $(OBJS)
	$(CXX) -o $(DEBUGDIR)$(TARGET) $(OBJS) $(OPENCVLIB)

.cpp.o:
	$(CXX) -c -o $@ $< $(OPENCVINC) $(CXXFLAGS)
clean:
	rm *.o