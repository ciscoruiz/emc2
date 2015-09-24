include ../../common/config.make 

all : dirty 

dirty : dirty.o
	$(CXX) $(CPPFLAGS) -o dirty  dirty.o $(LDFLAGS)

dirty.o : dirty.cpp ../../common/Trace.h

clean:
	rm -rf *.o dirty 
	

