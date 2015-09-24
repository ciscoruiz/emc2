include ../../common/config.make 

all : clean_code easy

clean_code : clean_code.o
	$(CXX) $(CPPFLAGS) -o clean_code  clean_code.o $(LDFLAGS)

clean_code.o : clean_code.cpp ../../common/Trace.h

easy : easy.o
	$(CXX) $(CPPFLAGS) -o easy  easy.o $(LDFLAGS)

easy.o : easy.cpp 

clean:
	rm -rf *.o clean_code easy
	

