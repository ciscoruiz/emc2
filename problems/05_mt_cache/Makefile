include ../../common/config.make 

SOLUTION=../../solutions/05_mt_cache

CPPFLAGS+=-I$(DEPS)/include -I$(SOLUTION)

all : clean_code

test_main.o : test_main.cpp
   
unit_test.o : unit_test.cpp $(SOLUTION)/TCache.h $(SOLUTION)/TEntry.h $(SOLUTION)/Status.h
   
clean_code : test_main.o unit_test.o 
	$(CXX) $(CPPFLAGS) -o clean_code  test_main.o unit_test.o $(LDFLAGS)

clean:
	rm -rf *.o clean_code
	

