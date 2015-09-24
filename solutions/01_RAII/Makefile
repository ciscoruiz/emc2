include ../../common/config.make

all : clean_code

clean_code.o : clean_code.cpp ../../common/Trace.h

clean_code : clean_code.o
	$(CXX) $(CPPFLAGS) -o clean_code  clean_code.o $(LDFLAGS)

clean:
	rm -rf *.o clean_code
	

