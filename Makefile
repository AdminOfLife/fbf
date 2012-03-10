TARGET=fbf
RELEASEDIR=./release/
OBJS=aviviewer.o fbf.o
CXX=g++
CXXFLAGS= -Wall -O2
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

$(TARGET):$(OBJS)
	$(CXX) -o $(RELEASEDIR)$@ $(OBJS) $(OPENCVLIB)
.cpp.o:
	$(CXX) -c -o $@ $< $(OPENCVINC) $(CXXFLAGS)
clean:
	rm *.o