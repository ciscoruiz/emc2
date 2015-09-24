include ../../common/config.make 

CPPFLAGS += -I../../bad_code/03_nested_resources

all : clean_code_01 clean_code_02

clean_code_01 : clean_code_01.o
	$(CXX) $(CPPFLAGS) -o clean_code_01  clean_code_01.o $(LDFLAGS)

clean_code_02.o : clean_code_02.cpp ../../common/Trace.h ../../bad_code/03_nested_resources/*.hpp

clean_code_02 : clean_code_02.o
	$(CXX) $(CPPFLAGS) -o clean_code_02  clean_code_02.o $(LDFLAGS)

clean_code_02.o : clean_code_02.cpp ../../common/Trace.h ../../bad_code/03_nested_resources/*.hpp

clean:
	rm -rf *.o clean_code_0?
	

