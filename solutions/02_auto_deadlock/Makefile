include ../../common/config.make 

all : clean_code_01 clean_code_02

clean_code_01 : clean_code_01.o
	$(CXX) $(CPPFLAGS) -o clean_code_01  clean_code_01.o $(LDFLAGS)

clean_code_02.o : clean_code_02.cpp ../../common/Trace.h

clean_code_02 : clean_code_02.o
	$(CXX) $(CPPFLAGS) -o clean_code_02  clean_code_02.o $(LDFLAGS)

clean_code_02.o : clean_code_02.cpp ../../common/Trace.h

clean:
	rm -rf *.o clean_code_0?
	

