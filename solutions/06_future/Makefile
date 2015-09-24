include ../../common/config.make 

#CPPFLAGS+=-I$(DEPS)/include -I.

all : future

future.o : future.cpp 
   
future : future.o 
	$(CXX) $(CPPFLAGS) -o future future.o $(LDFLAGS)

clean:
	rm -rf *.o future
	

