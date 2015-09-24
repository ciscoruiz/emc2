include ../../common/config.make

CPPFLAGS+=-D_GLIBCXX_USE_NANOSLEEP -I.

HEADERS=ResourceBroker.hpp  ResourceContainer.hpp  Resource.hpp  ResourceRecover.hpp

all : dirty 

dirty : dirty.o
	$(CXX) $(CPPFLAGS) -o dirty  dirty.o $(LDFLAGS)

dirty.o : dirty.cpp ../../common/Trace.h $(HEADERS)

clean:
	rm -rf *.o dirty 
	

